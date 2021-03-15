#include "WarningClient.h"
#include "MainComponent.h"
#include "clog.h"
extern bool UnicodeToMB1(char*& pmb, const wchar_t* pun, int uLen);

WarningClient::WarningClient() : _mainComponent(0)
{
	m_speed = 15.;//m/min
	m_recTime = 0;
}

void WarningClient::connectionMade()
{

}

void WarningClient::connectionLost()
{

}

double WarningClient::GetMachineSpeed()
{
	return m_speed;
}

void WarningClient::messageReceived(const juce::MemoryBlock& message)
{
	//接受Communication发送过来的数据,主要是打标速度
	//m_speed = msg.speed;
	if (message.getSize() < sizeof(INTER_MESSAGE))
		return;

	INTER_MESSAGE msg;
	memcpy(&msg, message.getData(), sizeof(INTER_MESSAGE));
	/*LOGWT("%s", "收到通讯服务器消息");
	LOGWT("msg.flag:%s", String(msg._flag).toStdString().c_str());*/

	if (msg._flag == -65)//communication通讯 获取模切机速度
	{
		m_speed = msg._data/10.; //m/min
		if (m_speed <= 2.)
			m_speed = 2.;
		m_recTime = juce::Time::getCurrentTime().toMilliseconds();
	}
	//else
	//{
	//	LOGWT("收到通讯服务器消息Flag=%d", msg._flag);
	//}
	
	//带路1： -75, 带路2： -76 带路3： -77 带路4： -78
	//先导:-175 -176  -177 -178
	if ((msg._flag >= -78 && msg._flag <= -75)|| (msg._flag >= -178 && msg._flag <= -175))//communication通讯 获取四联标信号
	{
		((MainComponent*)_mainComponent)->HandleMessage(msg._flag,m_speed);
		
		m_speed = msg._data/10.;
		if (m_speed <= 2.)
			m_speed = 2.;
		LOGWT("收到4连标指令:%d，当前速度 %f", msg._flag, m_speed);
	}
}

int	WarningClient::SendData(INTER_MESSAGE& data)
{
	juce::MemoryBlock msg(&data, sizeof(INTER_MESSAGE));
	if (sendMessage(msg))
		return 0;

	return 1;
}


MainFormClient::MainFormClient() : _mainComponent(0)
{

}


void MainFormClient::connectionMade()
{


}
void MainFormClient::connectionLost()
{

}


void MainFormClient::messageReceived(const juce::MemoryBlock& message)
{

	if (message.getSize() < sizeof(WS_INFO))
		return;

	WS_INFO head;
	memcpy(&head, message.getData(), sizeof(WS_INFO));

	// 解析数据包
	if (head._type == 6) //膜卷号
	{
		if (message.getSize() < sizeof(WS_INFO) + sizeof(ROLL_INFO))
			return;

		// 解析标准尺寸包
		ROLL_INFO stdSize;
		memset(&stdSize, 0, sizeof(ROLL_INFO));
		char* p = (char*)message.getData();
		p += sizeof(WS_INFO); // 跳过数据包头
		memcpy(&stdSize, p, sizeof(ROLL_INFO));

		((MainComponent*)_mainComponent)->ResetAndNewRoll(stdSize._rollName);
	}
	// 解析数据包
	if (head._type == 88) //CCD 重新连接
	{
		((MainComponent*)_mainComponent)->ReconnectAll();
	}
}

int	MainFormClient::SendData(StickMarkInfo& data)
{

	WS_INFO header;
	header._type = 8;
	header._version = 1;
	header._size = sizeof(StickMarkInfo);
	header._workStation = 1;

	int dataSize = sizeof(StickMarkInfo);
	header._size = dataSize;
	unsigned char* p = new unsigned char[sizeof(WS_INFO) + dataSize];
	memcpy(p, &header, sizeof(WS_INFO));
	memcpy(p + sizeof(WS_INFO), &data, sizeof(StickMarkInfo));
	juce::MemoryBlock msg(p, sizeof(WS_INFO) + dataSize);

	if (sendMessage(msg))
		return 0;

	return 1;
}
int  MainFormClient::SendData(WS_INFO& header, WARNING_MSG& data)
{
	int dataSize = sizeof(data);
	unsigned char* p = new unsigned char[sizeof(WS_INFO) + dataSize];
	memcpy(p, &header, sizeof(WS_INFO));
	memcpy(p + sizeof(WS_INFO), &data, sizeof(WARNING_MSG));

	juce::MemoryBlock msg(p, sizeof(WS_INFO) + dataSize);

	if (sendMessage(msg))
	{
		delete[] p;
		return 1;
	}
	delete[] p;
	return 0;
}
int	MainFormClient::SendMsgData(bool bNewUI, juce::String data)
{
	if (!isConnected())
		return 0;

	WARNING_MES msg;
	strcpy(msg.cTime, getCurrentSystemTime().c_str());
	msg.ip = 1;
	msg.isSend2Plc = 1;
	msg.type = 2;
	juce::String moduleName = L"打标机报警停机";
	char* pmbType;
	UnicodeToMB1(pmbType, moduleName.toUTF16(), moduleName.length());
	memset(msg.workStation, 0, 512);
	strcpy(msg.workStation, pmbType);
	delete[] pmbType;
		
	UnicodeToMB1(pmbType, data.toUTF16(), data.length());
	strcpy(msg.mes, pmbType);
	delete[] pmbType;

	if (!bNewUI)
	{
		MemoryBlock msg(&msg, sizeof(WARNING_MES));
		if (!sendMessage(msg))
		{
			LOGWT("发送报警or停机信息到主界面失败!");
			return false;
		}
	}
	else
	{
		WS_INFO header;
		header._type = 13;
		header._time = juce::Time::currentTimeMillis();
		WARNING_MSG smsg;
		memset(smsg.ip, 0, 20);
		strcpy(smsg.ip, std::to_string(msg.ip).c_str());
		smsg.type = 2;
		memset(smsg.mes, 0, 512);
		char* pmbType;
		UnicodeToMB1(pmbType, data.toUTF16(), data.length());
		memset(smsg.mes, 0, 512);
		strcpy(smsg.mes, pmbType);
		smsg.isSend2Plc = 1;
		delete[] pmbType;

		smsg.isSend2Plc = 2;
		return  SendData(header, smsg);
	}
	return 0;
}

int MainFormClient::SendData(NumCount& numcount)
{
	WS_INFO header;
	header._type = 66;
	header._time = juce::Time::currentTimeMillis();
	juce::MemoryBlock memb;
	memb.append(&header, sizeof(WS_INFO));
	memb.append(&numcount,sizeof(NumCount));
	return sendMessage(memb);
}
