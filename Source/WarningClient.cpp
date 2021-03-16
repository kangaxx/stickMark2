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
	//½ÓÊÜCommunication·¢ËÍ¹ýÀ´µÄÊý¾Ý,Ö÷ÒªÊÇ´ò±êËÙ¶È
	//m_speed = msg.speed;
	if (message.getSize() < sizeof(INTER_MESSAGE))
		return;

	INTER_MESSAGE msg;
	memcpy(&msg, message.getData(), sizeof(INTER_MESSAGE));
	/*LOGWT("%s", "ÊÕµ½Í¨Ñ¶·þÎñÆ÷ÏûÏ¢");
	LOGWT("msg.flag:%s", String(msg._flag).toStdString().c_str());*/

	if (msg._flag == -65)//communicationÍ¨Ñ¶ »ñÈ¡Ä£ÇÐ»úËÙ¶È
	{
		m_speed = msg._data/10.; //m/min
		if (m_speed <= 2.)
			m_speed = 2.;
		m_recTime = juce::Time::getCurrentTime().toMilliseconds();
	}
	//else
	//{
	//	LOGWT("ÊÕµ½Í¨Ñ¶·þÎñÆ÷ÏûÏ¢Flag=%d", msg._flag);
	//}
	
	//´øÂ·1£º -75, ´øÂ·2£º -76 ´øÂ·3£º -77 ´øÂ·4£º -78
	//ÏÈµ¼:-175 -176  -177 -178
	if ((msg._flag >= -78 && msg._flag <= -75)|| (msg._flag >= -178 && msg._flag <= -175))//communicationÍ¨Ñ¶ »ñÈ¡ËÄÁª±êÐÅºÅ
	{
		((MainComponent*)_mainComponent)->HandleMessage(msg._flag,m_speed);
		
		m_speed = msg._data/10.;
		if (m_speed <= 2.)
			m_speed = 2.;
		LOGWT("ÊÕµ½4Á¬±êÖ¸Áî:%d£¬µ±Ç°ËÙ¶È %f", msg._flag, m_speed);
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

	// ½âÎöÊý¾Ý°ü
	if (head._type == 6) //Ä¤¾íºÅ
	{
		if (message.getSize() < sizeof(WS_INFO) + sizeof(ROLL_INFO))
			return;

		// ½âÎö±ê×¼³ß´ç°ü
		ROLL_INFO stdSize;
		memset(&stdSize, 0, sizeof(ROLL_INFO));
		char* p = (char*)message.getData();
		p += sizeof(WS_INFO); // Ìø¹ýÊý¾Ý°üÍ·
		memcpy(&stdSize, p, sizeof(ROLL_INFO));

		((MainComponent*)_mainComponent)->ResetAndNewRoll(stdSize._rollName);
	}
	// ½âÎöÊý¾Ý°ü
	if (head._type == 88) //CCD ÖØÐÂÁ¬½Ó
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
	juce::String moduleName = L"´ò±ê»ú±¨¾¯Í£»ú";
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
			LOGWT("·¢ËÍ±¨¾¯orÍ£»úÐÅÏ¢µ½Ö÷½çÃæÊ§°Ü!");
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
