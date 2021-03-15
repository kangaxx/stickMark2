/*
  ==============================================================================

    SocketMarker.cpp
    Created: 16 Aug 2016 5:02:43pm
    Author:  fang.c.h

  ==============================================================================
*/

#include "SocketMarker.h"

#include <Winsock2.h>
#include "iphlpapi.h" 
//#include "windows.h"
#include "PCI1230.h"
#include "clog.h"
#pragma comment(lib,"Iphlpapi.lib")
#pragma comment(lib,"Ws2_32.lib")

typedef HANDLE (WINAPI* ICMPCREATEFILE)(VOID);   
typedef BOOL (WINAPI* ICMPCLOSEHANDLE)(HANDLE);   
typedef DWORD (WINAPI* ICMPSENDECHO)(HANDLE, DWORD, LPVOID, WORD,PIP_OPTION_INFORMATION, LPVOID, DWORD, DWORD); 

#define Ping_Result_Success		1	// Ping成功(1)
#define Ping_Result_Failure		0	// Ping失败(0)
#define Ping_Result_IP_Err		-1	// Ping的目标机地址错误(-1)
#define Ping_Result_Func_Err	-2	// 库函数解析失败(-2)

//#define IP_SUCCESS				0		// 状态是成功。
//#define IP_BUF_TOO_SMALL			11001 	// 答复缓冲区太小。
//#define IP_DEST_NET_UNREACHABLE	11002 	// 目标网络不可达。
//#define IP_DEST_HOST_UNREACHABLE	11003 	// 目标主机不可达。
//#define IP_DEST_PROT_UNREACHABLE	11004 	// 目的地的协议是遥不可及。
//#define IP_DEST_PORT_UNREACHABLE	11005 	// 目标端口不可达。
//#define IP_NO_RESOURCES			11006 	// IP资源不足是可用的。
//#define IP_BAD_OPTION				11007 	// 指定了错误的IP选项。
//#define IP_HW_ERROR				11008 	// 一个硬件错误。
//#define IP_PACKET_TOO_BIG			11009 	// 包太大。
//#define IP_REQ_TIMED_OUT			11010 	// 请求超时。
//#define IP_BAD_REQ				11011 	// 一个坏的请求。
//#define IP_BAD_ROUTE				11012 	// 一个糟糕的路线。
//#define IP_TTL_EXPIRED_TRANSIT	11013 	// 在传输过程中的生存时间（TTL）的过期。
//#define IP_TTL_EXPIRED_REASSEM	11014 	// 在碎片重组过程中的生存时间过期。
//#define IP_PARAM_PROBLEM			11015 	// 一个参数的问题。
//#define IP_SOURCE_QUENCH			11016 	// 数据报到达太快，处理和数据报可能被丢弃。
//#define IP_OPTION_TOO_BIG			11017 	// 一个IP选项是太大了。
//#define IP_BAD_DESTINATION		11018 	// 一个坏的目的地。
//#define IP_GENERAL_FAILURE		11050 	// 一般故障。这个错误可以返回一些畸形的ICMP数据包

// 函数功能：	判断是否能ping通IP
// 函数参数：	IP地址或域名
// 返回值：		库函数解析失败(-2)，Ping的目标机地址错误(-1)，Ping失败(0)，Ping成功(1).
int SocketMarker::ICMPPing(const char* host)
{
	// 初始化ICMP
	// 定义三个指针函数   
	ICMPCREATEFILE	pIcmpCreateFile	= NULL;   
	ICMPCLOSEHANDLE pIcmpCloseHandle= NULL;   
	ICMPSENDECHO	pIcmpSendEcho	= NULL;

	HINSTANCE hIcmp = LoadLibrary("ICMP.DLL");   
	if(hIcmp != NULL)
	{
		pIcmpCreateFile	= (ICMPCREATEFILE)GetProcAddress(hIcmp,"IcmpCreateFile");   
		pIcmpCloseHandle= (ICMPCLOSEHANDLE)GetProcAddress(hIcmp,"IcmpCloseHandle");   
		pIcmpSendEcho	= (ICMPSENDECHO)GetProcAddress(hIcmp,"IcmpSendEcho");
	}

	if (pIcmpCreateFile == NULL || pIcmpCloseHandle == NULL || pIcmpSendEcho == NULL )
		return Ping_Result_Func_Err;	// 库函数解析失败(-2)

	// 开始准备进行Ping
	DWORD timeOut	= 100;											// 设置超时   
	ULONG hAddr		= inet_addr(host);								// 如果是IP地址就直接转换   
	if(hAddr==INADDR_NONE)   
	{   
		hostent* hp=gethostbyname(host);							// 如果是域名就用DNS解析出IP地址   
		if(hp)   
			memcpy(&hAddr,hp->h_addr_list,hp->h_length);			// IP地址   
		else
			return Ping_Result_IP_Err;	// Ping的目标机地址错误(-1)
	}

	HANDLE hIp=pIcmpCreateFile();   
	IP_OPTION_INFORMATION ipoi;   
	memset(&ipoi,0,sizeof(IP_OPTION_INFORMATION));   
	ipoi.Ttl = 128;                  //Time-To-Live   

	unsigned char pSend[36];                                                                   //发送包   
	memset(pSend,'E',32);   

	int repSize=sizeof(ICMP_ECHO_REPLY)+32;   
	unsigned char pReply[100];                                                                 //接收包   
	ICMP_ECHO_REPLY* pEchoReply=(ICMP_ECHO_REPLY*)pReply;   

	DWORD nPackets=pIcmpSendEcho(hIp,hAddr,pSend,32,&ipoi,pReply,repSize,timeOut);             //发送ICMP数据报文   

	if(pEchoReply->Status!=0)                                                                  //超时，可能是主机禁用了ICMP 或者目标主机不存在  
	{   
		SocketMarker::WriteNetWorkLog(pEchoReply->Status);
		pIcmpCloseHandle(hIp);   
		return Ping_Result_Failure;   // Ping失败(0)
	}   

	pIcmpCloseHandle(hIp);   
	return Ping_Result_Success;  // Ping成功(1)
}

void SocketMarker::WriteNetWorkLog(int nStatus)
{
	switch (nStatus)
	{
	case IP_BUF_TOO_SMALL:			LOGWT("---------------------------IP_BUF_TOO_SMALL------------------------"); break;
	case IP_DEST_NET_UNREACHABLE:	LOGWT("---------------------------IP_DEST_NET_UNREACHABLE-----------------"); break;
	case IP_DEST_HOST_UNREACHABLE:	LOGWT("---------------------------IP_DEST_HOST_UNREACHABLE----------------"); break;
	case IP_DEST_PROT_UNREACHABLE:	LOGWT("---------------------------IP_DEST_PROT_UNREACHABLE----------------"); break;
	case IP_DEST_PORT_UNREACHABLE:	LOGWT("---------------------------IP_DEST_PORT_UNREACHABLE----------------"); break;
	case IP_NO_RESOURCES:			LOGWT("---------------------------IP_NO_RESOURCES-------------------------"); break;
	case IP_BAD_OPTION:				LOGWT("---------------------------IP_BAD_OPTION---------------------------"); break;
	case IP_HW_ERROR:				LOGWT("---------------------------IP_HW_ERROR-----------------------------"); break;
	case IP_PACKET_TOO_BIG:			LOGWT("---------------------------IP_PACKET_TOO_BIG-----------------------"); break;
	case IP_REQ_TIMED_OUT:			LOGWT("---------------------------IP_REQ_TIMED_OUT------------------------"); break;
	case IP_BAD_REQ:				LOGWT("---------------------------IP_BAD_REQ------------------------------"); break;
	case IP_BAD_ROUTE:				LOGWT("---------------------------IP_BAD_ROUTE----------------------------"); break;
	case IP_TTL_EXPIRED_TRANSIT:	LOGWT("---------------------------IP_TTL_EXPIRED_TRANSIT------------------"); break;
	case IP_TTL_EXPIRED_REASSEM:	LOGWT("---------------------------IP_TTL_EXPIRED_REASSEM------------------"); break;
	case IP_PARAM_PROBLEM:			LOGWT("---------------------------IP_PARAM_PROBLEM------------------------"); break;
	case IP_SOURCE_QUENCH:			LOGWT("---------------------------IP_SOURCE_QUENCH------------------------"); break;
	case IP_OPTION_TOO_BIG:			LOGWT("---------------------------IP_OPTION_TOO_BIG-----------------------"); break;
	case IP_BAD_DESTINATION:		LOGWT("---------------------------IP_BAD_DESTINATION----------------------"); break;
	case IP_GENERAL_FAILURE:		LOGWT("---------------------------IP_GENERAL_FAILURE----------------------"); break;
	default:
		break;
	}
}

bool SocketMarker::PingIP(String strIP)
{
	int nErr = ICMPPing(strIP.toStdString().c_str());

	if(nErr == Ping_Result_Success)
		return true;
	else
		return false;
}

//////////////////////////////////////////////// SocketMarker 【Start】////////////////////////////////////////////////////////////////////////////////////////////////////
SocketMarker::SocketMarker(void* pParent,String strServer,int nPort)
{
	m_strServerIP	= strServer;
	m_nPort			= nPort;
	m_bConnected	= false;
	m_pParent		= pParent;

	m_bMarkerEnable = false;

	m_nThreadSend	= SendMode_Direct;
	m_pMarkThread	= nullptr;
	if (Pci1230Open(15) != PCI1230Success)
		AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon, "", L"打开IO控制卡失败");
	startTimer(3000);

	//String strFile = File::getCurrentWorkingDirectory().getFullPathName() + "\\pulseLast\\";
	//if (!File(strFile).exists())
	//	File(strFile).createDirectory();
	//strFile += "pulselast.xml";
	//File xmlFile(strFile);
	//if (!xmlFile.existsAsFile())
	//{
	//	FileOutputStream xmlFileStream(xmlFile);
	//	xmlFileStream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << '\n';
	//	xmlFileStream << "<pulseLast duration=\"30\"></pulseLast>" << '\n';
	//	xmlFileStream.flush();
	//	m_pulseLast = 30;
	//}

	//else
	//{
	//	std::unique_ptr<XmlElement> xmlele_uptr;
	//	xmlele_uptr = XmlDocument(xmlFile).getDocumentElement();
	//	m_pulseLast = xmlele_uptr->getAttributeValue(0).getIntValue();
	//}
	m_pulseLast = 50;
}

SocketMarker::~SocketMarker()
{
	stopTimer();

	if (m_pMarkThread != nullptr)
	{
		m_pMarkThread->stopThread(2000);

		delete m_pMarkThread;
		m_pMarkThread = nullptr;
	}

	if(isConnected())
		disconnect();
	Pci1230Close(15);
	m_pParent = nullptr;
}

void SocketMarker::SetServerIP(String strServer, int nPort)
{
	m_strServerIP	= strServer;
	m_nPort			= nPort;

	if (isConnected())
		disconnect();
}

void SocketMarker::connectionMade()
{

	m_bConnected = true;
}

void SocketMarker::connectionLost()
{
	m_bConnected = false;

}

void SocketMarker::messageReceived (const MemoryBlock& message)
{
}

bool SocketMarker::connectTo(String strServer,int nPort)
{
	if(m_bConnected)
		return false;

	m_strServerIP = strServer;
	m_nPort = nPort;

	if(m_strServerIP.isEmpty() || m_strServerIP == "")
		return false;

	if(m_nPort <= 0)
		return false;

	return connectTo();
}

bool SocketMarker::connectTo()
{
	bool bRet = false;
	
	bRet = connectToSocket(m_strServerIP,m_nPort,500);

	if(bRet)
		m_bConnected = true;
	else
		m_bConnected = false;

	return bRet;
}

bool SocketMarker::disconnect()
{
	bool bRet = false;

	if(m_bConnected)
	{
		InterConnection::disconnect();
		m_bConnected = false;
		bRet = true;
	}

	return bRet;
}

bool SocketMarker::isConnected()
{
	return m_bConnected;
}

void SocketMarker::timerCallback()
{
	// 主要涉及的是 网络连接、消息发送线程

	// IP地址、Port发生变化时

	// 喷码消息发送模式变化

	// 喷码服务的使能发生变化时

	if (m_bMarkerEnable)
	{
		if (!isConnected())
			connectTo();

		if (m_nThreadSend == SendMode_Thread)
		{
			if (m_pMarkThread != nullptr)
			{
				if (!m_pMarkThread->isThreadRunning())
					m_pMarkThread->startThread();
			}

			else
			{
				m_pMarkThread = new MarkerThread(this);

				if (m_pMarkThread != nullptr)
					m_pMarkThread->startThread();
			}
		}

		else if (m_nThreadSend == SendMode_Direct)
		{
			if (m_pMarkThread != nullptr)
			{
				if (m_pMarkThread->isThreadRunning())
					m_pMarkThread->stopThread(1000);

				delete m_pMarkThread;
				m_pMarkThread = nullptr;
			}
		}
	}
	else
	{
		if (isConnected())
			disconnect();

		if (m_nThreadSend == SendMode_Direct && m_pMarkThread != nullptr)
		{
			m_pMarkThread->stopThread(1000);

			delete m_pMarkThread;
			m_pMarkThread = nullptr;
		}
	}
}

void SocketMarker::SetMarkerEnable(bool bEnable)
{
	if (m_bMarkerEnable == bEnable)
		return;

	m_bMarkerEnable = bEnable; 
}

bool SocketMarker::AddMarker(MarkerMessage markMsg)
{
	bool bRet = false;

	if (listMarker.size() < Marker_Message_Max)
	{
		listMarker.push_back(markMsg);
		bRet = true;
	}
	else
	{
		// Lost Marker 
	}

	return bRet;
}

void SocketMarker::ClearMarker()
{
	listMarker.clear();
}

void SocketMarker::SendMarkerMsg(MarkerMessage markMsg)
{
	if (isConnected())
	{
		String strMsg = markMsg.GetString();
		if (strMsg.length() <= 0)
			return;

		MemoryBlock messageData(strMsg.length(), true);
		messageData.copyFrom(strMsg.toStdString().c_str(),0, strMsg.length());

		sendMessage(messageData);
	}
}

bool SocketMarker::SendMarker(MarkerMessage markMsg)
{
	bool bRet = false;

	if (m_nThreadSend == SendMode_Direct)
	{
		SendMarkerMsg(markMsg);
	}

	else if (m_nThreadSend == SendMode_Thread)
	{
		AddMarker(markMsg);
	}

	return bRet;
}

void SocketMarker::SetSendMode(SendMode nMode)
{
	if (m_nThreadSend == nMode)
		return;

	m_nThreadSend = nMode;

	ClearMarker();

	if (m_nThreadSend == SendMode_Direct)
	{
		if (m_pMarkThread != nullptr)
		{
			m_pMarkThread->stopThread(2000);

			delete m_pMarkThread;
			m_pMarkThread = nullptr;
		}
	}

	else if (m_nThreadSend == SendMode_Thread)
	{
		if (m_pMarkThread != nullptr)
		{
			m_pMarkThread->startThread();
		}

		else
		{
			m_pMarkThread = new MarkerThread(this);
			if (m_pMarkThread != nullptr)
				m_pMarkThread->startThread();
		}
	}
}

void SocketMarker::SendIOSig(int ioNum)
{
	switch (ioNum)
	{
	case 1:
		Pci1230WriteDoBit(15, 0, 1);
		juce::Thread::sleep(10);
		Pci1230WriteDoBit(15, 0, 0);
		break;
	case 2:
		Pci1230WriteDoBit(15, 2, 1);
		juce::Thread::sleep(10);
		Pci1230WriteDoBit(15, 2, 0);
		break;
	case 3:
		Pci1230WriteDoBit(15, 4, 1);
		juce::Thread::sleep(10);
		Pci1230WriteDoBit(15, 4, 0);
		break;
	case 4:
		Pci1230WriteDoBit(15, 6, 1);
		juce::Thread::sleep(10);
		Pci1230WriteDoBit(15, 6, 0);
		break;
	default:
		break;
	}
}

bool SocketMarker::SendMarkerMsg2(MarkerMessage markMsg)
{
	String msgToSend = markMsg.GetString();
	int sendLength=writeData((void*)msgToSend.toStdString().c_str(), msgToSend.length());
	return sendLength == msgToSend.length();
}

void SocketMarker::SendIOSigAndMarkerMsg(int ioNum, MarkerMessage markMsg, int pulseLast = 20)
{
	String msgToSend = markMsg.GetString();
	switch (ioNum)
	{
	case 1:
		Pci1230WriteDoBit(15, 0, 1);
		writeData((void*)msgToSend.toStdString().c_str(), msgToSend.length());
		juce::Thread::sleep(pulseLast);
		Pci1230WriteDoBit(15, 0, 0);
		break;
	case 2:
		Pci1230WriteDoBit(15, 2, 1);
		writeData((void*)msgToSend.toStdString().c_str(), msgToSend.length());
		juce::Thread::sleep(pulseLast);
		Pci1230WriteDoBit(15, 2, 0);
		break;
	case 3:
		Pci1230WriteDoBit(15, 4, 1);
		writeData((void*)msgToSend.toStdString().c_str(), msgToSend.length());
		juce::Thread::sleep(pulseLast);
		Pci1230WriteDoBit(15, 4, 0);
		break;
	case 4:
		Pci1230WriteDoBit(15, 6, 1);
		writeData((void*)msgToSend.toStdString().c_str(), msgToSend.length());
		juce::Thread::sleep(pulseLast);
		Pci1230WriteDoBit(15, 6, 0);
		break;
	default:
		break;
	}
}
//////////////////////////////////////////////// SocketMarker 【 End 】////////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////// MarkerMessage 【Start】////////////////////////////////////////////////////////////////////////////////////////////////////
MarkerMessage::MarkerMessage(int nCamerID, float fCoatWidth, float	fCoatGap, int nTimeDelay,float fBase)
{
	_nCamerID	= nCamerID;		// 相机索引；
	_fCoatWidth = fCoatWidth;	// 涂布宽度
	_fCoatGap	= fCoatGap;		// 纵向间隙
	_nTimeDelay = nTimeDelay;	// 计算处理引起的延时时长
	_fBase		= fBase;		// 当前的基材位置

	vectNGPos.clear();
}

MarkerMessage::~MarkerMessage()
{
}

String MarkerMessage::GetString()
{
	// cam_a#w=480#l=1500;time#delay=203;a#x1=9#y1=329#x2=15#y2=12;b#x1=9#y1=329#x2=15#y2=12
	String strRet;

	if (_nCamerID != -1)
	{
		String strCameraID;

		switch (_nCamerID + 1)
		{
		case 1:	strCameraID = "a"; break;
		case 2:	strCameraID = "b"; break;
		case 3:	strCameraID = "c"; break;
		case 4:	strCameraID = "d"; break;
		case 5:	strCameraID = "e"; break;
		case 6:	strCameraID = "f"; break;
		case 7:	strCameraID = "g"; break;
		case 8:	strCameraID = "h"; break;
		case 9:	strCameraID = "i"; break;
		case 10:	strCameraID = "j"; break;

		case 11:	strCameraID = "k"; break;
		case 12:	strCameraID = "l"; break;
		case 13:	strCameraID = "m"; break;
		case 14:	strCameraID = "n"; break;
		case 15:	strCameraID = "o"; break;

		default:break;
		}

		if (strCameraID.isNotEmpty() && vectNGPos.size() > 0)
		{
			//String strTmp = String::formatted("#w=%d#l=%d;time#delay=%d;", _fCoatWidth, _fCoatGap, _nTimeDelay);
			String strTmp = String::formatted("#w=%.0f#l=%.0f;time#delay=%d;", _fCoatWidth, _fCoatGap, _nTimeDelay);
			//String strTmp = String::formatted("#w=%.0f#l=%.0f;time#delay=%d;#Len=%.2f;", _fCoatWidth, _fCoatGap, _nTimeDelay, _fBase);


			strRet = "cam_" + strCameraID + strTmp;

			for (int i = 0; i < vectNGPos.size(); i++)
			{
				String strNG;
				String strPre;

				NGPosition markerNG = vectNGPos[i];

				switch (markerNG._nType)
				{
				case MDT_1:	strPre = "a"; break;
				case MDT_2:	strPre = "b"; break;
				case MDT_3:	strPre = "c"; break;
				case MDT_4:	strPre = "d"; break;
				case MDT_5:	strPre = "e"; break;
				case MDT_6:	strPre = "f"; break;
				case MDT_7:	strPre = "g"; break;
				case MDT_8:	strPre = "h"; break;
				case MDT_9:	strPre = "i"; break;
				case MDT_10:strPre = "j"; break;

				default:strPre = ""; break;
				}

				if (strPre.isNotEmpty())
				{
					//strNG = String::formatted("#x1=%d#y1=%d#x2=%d#y2=%d;", markerNG._fLeft, markerNG._fTop, markerNG._fRight, markerNG._fBottom);
					strNG = String::formatted("#x1=%.0f#y1=%.0f#x2=%.0f#y2=%.0f;", markerNG._fLeft, markerNG._fTop, markerNG._fRight, markerNG._fBottom);
					strRet += strPre + strNG;
				}
			}
		}
	}

	return strRet;
}
//////////////////////////////////////////////// MarkerMessage 【 End 】////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////// MarkerThread 【Start】////////////////////////////////////////////////////////////////////////////////////////////////////
MarkerThread::MarkerThread(Component* pOwer) : Thread("Marker Thread"), m_pOwer(pOwer)
{
	m_bShouldExit = false;
}

MarkerThread::~MarkerThread()
{
	exitThread();
}

void MarkerThread::run()
{
	while (!m_bShouldExit)
	{
		SocketMarker* pSocketMarker = (SocketMarker*)m_pOwer;

		if (pSocketMarker->listMarker.size() > 0)
		{
			std::list<MarkerMessage>::iterator it = pSocketMarker->listMarker.begin();
			MarkerMessage& markerMsg = *it;
			pSocketMarker->SendMarkerMsg(markerMsg);
			pSocketMarker->listMarker.erase(it);
		}

		wait(1000);
	}
}

void MarkerThread::exitThread()
{
	m_bShouldExit = true;

	while (isThreadRunning())
		wait(100);
}
//////////////////////////////////////////////// MarkerThread 【 End 】////////////////////////////////////////////////////////////////////////////////////////////////////