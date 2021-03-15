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

#define Ping_Result_Success		1	// Ping�ɹ�(1)
#define Ping_Result_Failure		0	// Pingʧ��(0)
#define Ping_Result_IP_Err		-1	// Ping��Ŀ�����ַ����(-1)
#define Ping_Result_Func_Err	-2	// �⺯������ʧ��(-2)

//#define IP_SUCCESS				0		// ״̬�ǳɹ���
//#define IP_BUF_TOO_SMALL			11001 	// �𸴻�����̫С��
//#define IP_DEST_NET_UNREACHABLE	11002 	// Ŀ�����粻�ɴ
//#define IP_DEST_HOST_UNREACHABLE	11003 	// Ŀ���������ɴ
//#define IP_DEST_PROT_UNREACHABLE	11004 	// Ŀ�ĵص�Э����ң���ɼ���
//#define IP_DEST_PORT_UNREACHABLE	11005 	// Ŀ��˿ڲ��ɴ
//#define IP_NO_RESOURCES			11006 	// IP��Դ�����ǿ��õġ�
//#define IP_BAD_OPTION				11007 	// ָ���˴����IPѡ�
//#define IP_HW_ERROR				11008 	// һ��Ӳ������
//#define IP_PACKET_TOO_BIG			11009 	// ��̫��
//#define IP_REQ_TIMED_OUT			11010 	// ����ʱ��
//#define IP_BAD_REQ				11011 	// һ����������
//#define IP_BAD_ROUTE				11012 	// һ������·�ߡ�
//#define IP_TTL_EXPIRED_TRANSIT	11013 	// �ڴ�������е�����ʱ�䣨TTL���Ĺ��ڡ�
//#define IP_TTL_EXPIRED_REASSEM	11014 	// ����Ƭ��������е�����ʱ����ڡ�
//#define IP_PARAM_PROBLEM			11015 	// һ�����������⡣
//#define IP_SOURCE_QUENCH			11016 	// ���ݱ�����̫�죬��������ݱ����ܱ�������
//#define IP_OPTION_TOO_BIG			11017 	// һ��IPѡ����̫���ˡ�
//#define IP_BAD_DESTINATION		11018 	// һ������Ŀ�ĵء�
//#define IP_GENERAL_FAILURE		11050 	// һ����ϡ����������Է���һЩ���ε�ICMP���ݰ�

// �������ܣ�	�ж��Ƿ���pingͨIP
// ����������	IP��ַ������
// ����ֵ��		�⺯������ʧ��(-2)��Ping��Ŀ�����ַ����(-1)��Pingʧ��(0)��Ping�ɹ�(1).
int SocketMarker::ICMPPing(const char* host)
{
	// ��ʼ��ICMP
	// ��������ָ�뺯��   
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
		return Ping_Result_Func_Err;	// �⺯������ʧ��(-2)

	// ��ʼ׼������Ping
	DWORD timeOut	= 100;											// ���ó�ʱ   
	ULONG hAddr		= inet_addr(host);								// �����IP��ַ��ֱ��ת��   
	if(hAddr==INADDR_NONE)   
	{   
		hostent* hp=gethostbyname(host);							// �������������DNS������IP��ַ   
		if(hp)   
			memcpy(&hAddr,hp->h_addr_list,hp->h_length);			// IP��ַ   
		else
			return Ping_Result_IP_Err;	// Ping��Ŀ�����ַ����(-1)
	}

	HANDLE hIp=pIcmpCreateFile();   
	IP_OPTION_INFORMATION ipoi;   
	memset(&ipoi,0,sizeof(IP_OPTION_INFORMATION));   
	ipoi.Ttl = 128;                  //Time-To-Live   

	unsigned char pSend[36];                                                                   //���Ͱ�   
	memset(pSend,'E',32);   

	int repSize=sizeof(ICMP_ECHO_REPLY)+32;   
	unsigned char pReply[100];                                                                 //���հ�   
	ICMP_ECHO_REPLY* pEchoReply=(ICMP_ECHO_REPLY*)pReply;   

	DWORD nPackets=pIcmpSendEcho(hIp,hAddr,pSend,32,&ipoi,pReply,repSize,timeOut);             //����ICMP���ݱ���   

	if(pEchoReply->Status!=0)                                                                  //��ʱ������������������ICMP ����Ŀ������������  
	{   
		SocketMarker::WriteNetWorkLog(pEchoReply->Status);
		pIcmpCloseHandle(hIp);   
		return Ping_Result_Failure;   // Pingʧ��(0)
	}   

	pIcmpCloseHandle(hIp);   
	return Ping_Result_Success;  // Ping�ɹ�(1)
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

//////////////////////////////////////////////// SocketMarker ��Start��////////////////////////////////////////////////////////////////////////////////////////////////////
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
		AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon, "", L"��IO���ƿ�ʧ��");
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
	// ��Ҫ�漰���� �������ӡ���Ϣ�����߳�

	// IP��ַ��Port�����仯ʱ

	// ������Ϣ����ģʽ�仯

	// ��������ʹ�ܷ����仯ʱ

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
//////////////////////////////////////////////// SocketMarker �� End ��////////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////// MarkerMessage ��Start��////////////////////////////////////////////////////////////////////////////////////////////////////
MarkerMessage::MarkerMessage(int nCamerID, float fCoatWidth, float	fCoatGap, int nTimeDelay,float fBase)
{
	_nCamerID	= nCamerID;		// ���������
	_fCoatWidth = fCoatWidth;	// Ϳ�����
	_fCoatGap	= fCoatGap;		// �����϶
	_nTimeDelay = nTimeDelay;	// ���㴦���������ʱʱ��
	_fBase		= fBase;		// ��ǰ�Ļ���λ��

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
//////////////////////////////////////////////// MarkerMessage �� End ��////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////// MarkerThread ��Start��////////////////////////////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////// MarkerThread �� End ��////////////////////////////////////////////////////////////////////////////////////////////////////