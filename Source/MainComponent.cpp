#include "MainComponent.h"
#include"PCI1230.h"
//#include<xlnt/xlnt.hpp>
//#include"BasicExcel.hpp"
//#include"ExcelFormat.h"
#include <windows.h>
#include "clog.h"
//juce::FileLogger* g_log = nullptr;
WriteCommData* g_pWriteThread = nullptr;
WarningClient* g_client = nullptr;
MainComponent* g_MainComponent = nullptr;
CXMLConfig m_configFile;

juce::int64 _preCheckTime[4];
juce::int64 _handmStick[4];
juce::int64 _preStickTime[4];
double _preStickLenPos[4];
double _curRunningLenPos[4] = { -1.,-1.,-1.,-1. };
double  g_halEA = 2000;
extern int isMarkerOnUse[4];
std::vector<NumCount> g_stickNumCount;
//std::mutex mute;
juce::CriticalSection mute;
extern bool _threadShouldExit;
void threadOfTimer()
{
	while (true)
	{
		if (_threadShouldExit)
		{
			break;			// �˳��߳��ź�
		}
		g_MainComponent->timerCallback();

		juce::Thread::sleep(3000);
	}
}
//Unicode����תΪ���ֽڱ��� 
bool UnicodeToMB1(char*& pmb, const wchar_t* pun, int uLen)
{
	pmb = nullptr;

	// convert an widechar string to Multibyte   
	int MBLen = WideCharToMultiByte(CP_ACP, 0, pun, -1, NULL, 0, NULL, NULL);  //CP_ACP OK
	if (MBLen <= 0)
	{
		return false;
	}
	pmb = new char[MBLen + 1];
	memset(pmb, 0, MBLen + 1);

	int nRtn = WideCharToMultiByte(CP_ACP, 0, pun, -1, pmb, MBLen, NULL, NULL);

	if (nRtn != MBLen)
	{
		delete[] pmb;
		pmb = nullptr;
		return false;
	}
	return true;
}

//==============================================================================
String  num2str(int n) {
    String str;
    switch (n) {
    case 7:
        str = L"©����  ";
        break;
    case 8:
        str = L"��Ƭ����";
        break;
    case 9:
        str = L"��������";
        break;
    case 10:
        str = L"��������";
        break;
    case 11:
        str = L"����    ";
        break;
    case 12:
        str = L"��̼    ";
        break;
    case 13:
        str = L"����    ";
        break;
    case 19:
        str = L"�ڰ�    ";
        break;
    case 21:
        str = L"�װ�    ";
        break;
    default:
        str = L"";
        break;
    }
    return str;
}
String  opt2str(TRANSER_OUT_LOGIC tol) {
    String ret;
    //ret += "\n";
    //�������𡢼�������Ϊ8��9
    for (int i = 7; i < tol.size; i++) {
		if (i >= 20)
			break;

        String mj = "", smj = "";
        if (tol.flag[i][0] >= 2) {
            String str3((double)tol.flag[i][6] / 100.0, 2);
            //char str2[10];
            //sprintf(str2, "%.2f", (float)tol.flag[i][6] / 100.0);
            //str3 = str2;
            mj = L"�����>=" + str3;
        }
        if (tol.flag[i][0] >= 3) {
            String str3((double)tol.flag[i][8] / 100.0, 2);
            //char str2[10];
            //sprintf(str2, "%.2f", (float)tol.flag[i][8] / 100.0);
            //str3 = str2;
            smj = L"�򵥸����>=" + str3;
        }
        ret += num2str(tol.id[i]);
        ret += mj;
        ret = ret + L"�Ҹ���>=" + String(std::to_string(tol.num[i])) + smj + L"\n";
    }
    return ret;
}


//[/MiscUserDefs]
//void ReceivePlcData(Component* pwnd, String ip,const DataMessage& data)
//{
//	PLCComponent* plcComp = (PLCComponent*)pwnd;
//
//	plcComp->HandleAsysMessage(ip,data.addr, data._plcRegsData);
//}
int ProduceDefectCode(int code)
{

	if (code & 0x200) return 1;
	if (code & 0x400) return 5;
	if (code & 0x800) return 3;
	if (code & 0x1000) return 4;
	if (code & 0x2000) return 2;
	if (code & 0x4000) return 9;
	if (code & 0x8000) return 8;
	if (code & 0x400000) return 6;
	if (code & 0x800000) return 1;
	if (code & 0x1000000) return 6;
	if (code & 0x2000000) return 1;
	if (code & 0x4000000)  return 6;
	if (code & 0x8000000)  return 6;
	if (code & 0x10000000)  return 6;
	if (code & 0x20000000)  return 6;
	if (code & 0x40000000)  return 6;
	if (code & 0x100)  return 10;
	if (code & 0x80000)  return 10;
	if (code & 4)  return 10;
	if (code & 8) return 10;
	if (code & 131072) return 10;
	if (code & 262144)return 10;
	if (code & 16) return 10;
	if (code & 32) return 10;
	if (code & 64)return 10;
	if (code & 128) return 10;

	return 10;
}
//Unicode����תΪ���ֽڱ��� 
bool UnicodeToMB(char*& pmb, const wchar_t* pun, int uLen)
{
	pmb = nullptr;

    // convert an widechar string to Multibyte   
    int MBLen = WideCharToMultiByte(CP_ACP, 0, pun, -1, NULL, 0, NULL, NULL);  //CP_ACP OK
    if (MBLen <= 0)
    {
        return false;
    }
    pmb = new char[MBLen + 1];
    memset(pmb, 0, MBLen + 1);

    int nRtn = WideCharToMultiByte(CP_ACP, 0, pun, -1, pmb, MBLen, NULL, NULL);

    if (nRtn != MBLen)
    {
        delete[] pmb;
		pmb = nullptr;
        return false;
    }
    return true;
}

String ProduceDefectString(int code)
{
    String str = L"";
    if (code & 0x200)
        str += L"©����";
    if (code & 0x400)
        str += L"����";
    if (code & 0x800)
        str += L"��̼";
    if (code & 0x1000)
        str += L"����";
    if (code & 0x2000)
        str += L"�Ӵ� ";
    if (code & 0x4000)
        str += L"�׵� ";
    if (code & 0x8000)
        str += "�ڵ�";
    if (code & 0x400000)
        str += L"��Ƭ���� ";
    if (code & 0x800000)
        str += L"AT9©���� ";
    if (code & 0x1000000)
        str += "δ��ѹ,";
    if (code & 0x2000000)
        str += L"��Ե©����";
    if (code & 0x4000000)
        str += L"��������";
    if (code & 0x4000000)
        str += L"�������� ";
    if (code & 0x8000000)
        str += L"�������� ";
    if (code & 0x10000000)
        str += L"����";
    if (code & 0x20000000)
        str += L"δͿĤ";
    if (code & 0x40000000)
        str += L"ֱ������";
    if (code & 0x100)
        str += L"�������";
	if (code & 0x4 || code & 0x8 || code & 0x20000 || code & 0x40000 || code & 0x100000 || code & 0x200000) // 101   102	
		str += L"����������";
	//if (code & 0x20000 || code & 0x40000) // 109	 110
	//	str += L"�����߶�";
	//if (code & 0x100000 || code & 0x200000) // 112	
	//	str += L"����ȱʧNG";
	if (code & 0x10 || code & 0x20) // 103	  104
		str += L"�������";	
	if (code & 0x40 || code & 0x80|| code & 0x10000) // 105	 106
		str += L"AT9���";
	//if (code & 0x10000) // 108
	//	str += L"AT9��Ȳ�NG";
	if (code & 0x80000) // 111
		str += L"��Ĥ��";

	if (str.isEmpty())
		str << code;

    return str;
}

void MainComponent::ReconnectAll()
{
	bool ret = true;
	if(!client.isConnected())
		ret = client.connectToSocket(_strCommuIP, 600, 1000);
	if (!ret)
	{
		juce::int64 curT = juce::Time::currentTimeMillis();
		if (curT - _preReportTime > 10000)
		{
			g_MainComponent->m_mfClient.SendMsgData(m_bNewUI, L"����ͨѶ������ʧ�ܣ�ȷ������ͨѶ�Ƿ�����");
			_preReportTime = curT;
		}
	}
}

MainComponent::MainComponent()
{
	LOGINIT();
	m_bNewUI = false;
	_preReportTime = 0;
	for (int i = 0; i < 4; i++)
	{
		m_roadsSearch[i] = 0;
		_handmStick[i] = 0;
		_preCheckTime[i] = Time::currentTimeMillis();
		_preStickTime[i] = -1;

		_preStickLenPos[i] = -1.;
		_curRunningLenPos[i] = -1.;
	}

    unsigned int _openresult = Pci1230Open(15);
    if (_openresult != PCI1230Success) {
		AlertWindow::showMessageBox(AlertWindow::WarningIcon, L"��IO���ƿ�ʧ��", L"");

    }
	m_bUseLocal = true;

    m_preSentPos[0][0] = -1.;
    m_preSentPos[0][1] = -1.;

    m_sentCount[0] = 0;
    m_preSentPos[1][0] = -1.;
    m_preSentPos[1][1] = -1.;

    m_sentCount[1] = 0;
    for (int i = 0; i < 4; ++i) {
        Num_Mark[i] = 0;
    }

    for (int i = 0; i < 4; ++i) {
        Label_MarkCount[i].reset(new juce::Label());
        Label_MarkCount[i]->setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);
        juce::String label_text = L"����" + juce::String(i + 1) + L"����  " + juce::String(Num_Mark[i]);
        Label_MarkCount[i]->setText(label_text,juce::NotificationType::dontSendNotification);
        addAndMakeVisible(Label_MarkCount[i].get());
    }

   
    NewRoll.reset(new juce::TextButton()); 
    juce::String button_text = L"�¾�";
    NewRoll->setButtonText(button_text);
    NewRoll->setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(200,200,200));
    addAndMakeVisible(NewRoll.get());
	NewRoll.get()->addListener(this);

    Reconnect = new juce::TextButton;
    Reconnect->setButtonText(L"����PLC");
    Reconnect->setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(200, 200, 200));
    addAndMakeVisible(Reconnect);
    Reconnect->addListener(this);
    
	m_EASumLen = 4000.;

	EAEditor.reset(new juce::TextEditor("EAEditor"));
	EAEditor->setText(String(m_EASumLen));
	EAEditor->setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(200, 200, 200));
	addAndMakeVisible(EAEditor.get());

	EALabel.reset(new juce::TextButton("EALabel"));
	EALabel->setButtonText(L"�趨EA�ܳ�");
	EALabel->setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(200, 200, 200));
	addAndMakeVisible(EALabel.get());
	EALabel.get()->addListener(this);


    MarkInfo = new juce::TextButton();
    addAndMakeVisible(MarkInfo);
    MarkInfo->setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(200, 200, 200));
    MarkInfo->setButtonText(L"������Ϣ");
    MarkInfo->addListener(this);


	rollEditor.reset(new juce::TextEditor());
	addAndMakeVisible(rollEditor.get());

    
    //DO��Ӧ��togglebutton
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 4; ++j) {
            DO_Array[i][j].reset(new juce::ToggleButton());
            juce::String text("DO" + juce::String(i * 4 + j));
            DO_Array[i][j]->setColour(juce::ToggleButton::ColourIds::textColourId, juce::Colours::black);
            DO_Array[i][j]->setButtonText(text);
            DO_Array[i][j]->setColour(juce::ToggleButton::ColourIds::tickColourId, juce::Colours::green);
            DO_Array[i][j]->setColour(juce::ToggleButton::ColourIds::tickDisabledColourId, juce::Colours::black);
            DO_Array[i][j]->setBoundsRelative(0.04f + 0.92/8 * i, 0.23 + 0.06 * j, 0.1, 0.06);
            DO_Array[i][j]->addListener(this);
            addAndMakeVisible(DO_Array[i][j].get());
        }
    }

    reloadConfig.reset(new juce::TextButton);
    addAndMakeVisible(reloadConfig.get());
    reloadConfig->setButtonText(L"reConfig");
    reloadConfig->setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(200, 200, 200));
    reloadConfig->addListener(this);
    
    ////DI��Ӧ��togglebutton
    //for (int i = 0; i < 8; ++i) {
    //    for (int j = 0; j < 4; ++j) {
    //        DI_Array[i][j].reset(new juce::ToggleButton());
    //        juce::String text("DI" + juce::String(i * 4 + j));
    //        DI_Array[i][j]->setColour(juce::ToggleButton::ColourIds::textColourId, juce::Colours::black);
    //        DI_Array[i][j]->setButtonText(text);
    //        DI_Array[i][j]->setColour(juce::ToggleButton::ColourIds::tickColourId, juce::Colours::green);
    //        DI_Array[i][j]->setColour(juce::ToggleButton::ColourIds::tickDisabledColourId, juce::Colours::black);
    //        DI_Array[i][j]->setEnabled(false);
    //        DI_Array[i][j]->setBoundsRelative(0.04f + 0.92 / 8 * i, 0.59 + 0.06 * j, 0.1, 0.06);
    //        addAndMakeVisible(DI_Array[i][j].get());
    //    }
    //}

    //CCD��λ
    addAndMakeVisible(textButtonReset = new TextButton("exchange"));
    textButtonReset->setButtonText(L"CCD��λ");
   // textButtonReset->setConnectedEdges(Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    textButtonReset->setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(200,200,200));
    textButtonReset->addListener(this);
    
    CLabelDM[0] = new CLabelerDataManager();
    CLabelDM[1] = new CLabelerDataManager();
 
	String strCommuIP = "192.168.1.201";
	int ret2 = m_configFile.GetCommuServerAddr(strCommuIP);
	if (ret2 == 0)
	{
		AlertWindow::showMessageBox(AlertWindow::WarningIcon, L"������ȱͨѶ������IP", L"");
	}
	_strCommuIP = strCommuIP;
	LOGWT("ͨѶ������IP��ַ��%s", strCommuIP.toStdString().c_str());

	String strPLCIP = "192.168.1.80";
	ret2 = m_configFile.GetPLCServerAddr(strPLCIP);
	if (ret2 == 0)
	{
		AlertWindow::showMessageBox(AlertWindow::WarningIcon, L"������ȱ����IP", L"");
	}
	LOGWT("����PLC IP��ַ��%s", strPLCIP.toStdString().c_str());

	int iSupportC5 = 0;
	m_configFile.GetContiC5Stop(iSupportC5);
	m_iSupportCM5 = iSupportC5;// (iSupportC5 >= 0) ? true : false;

    bool ret = client.connectToSocket(strCommuIP, 600, 1000);
    if (!ret)
       juce:: AlertWindow::showMessageBox(juce::AlertWindow::WarningIcon, L"����ͨѶ������ʧ�ܣ�ȷ������ͨѶ�Ƿ����� ", "");
	client.SetComponent(this);


	bool conret = m_mfClient.connectToSocket(strCommuIP, 9000, 1000);
	if (!conret)
		AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "", L"    ����������ʧ��");
	m_sent2ServerIndex = 0;
	LOGWT("������ IP��ַ��%s", strCommuIP.toStdString().c_str());
	m_mfClient.SetComponent(this);

    bool retc=sever.beginWaitingForSocket(5005);
    juce::String logFileSubDirectoryName = "LabelLog";
    juce::String logFileName = "LabelLog";
    logFileName << juce::Time::getCurrentTime().formatted("%Y-%m-%d_%H-%M-%S");
    juce::String welcomeMessage = "Welcom log message for label";
    juce::int64 maxInitialFileSizeBytes = 5 * 1024 * 1024;

    //g_log = new juce::FileLogger(juce::File::getCurrentWorkingDirectory().getChildFile(logFileSubDirectoryName)
    //    .getChildFile(logFileName),
    //    welcomeMessage, maxInitialFileSizeBytes);


	int m_iSpeedRatio;
    g_iUsePLCSpeed = 0; //Ĭ������������ٶ�
	m_configFile.LoadPara(m_nRollMarks[0][0], m_nRollMarks[0][1], m_nRollMarks[1][0], m_nRollMarks[1][1], m_iSpeedRatio, m_EASumLen);
	if (m_nRollMarks[0][0] < 10)
        m_nRollMarks[0][0] = 10;
    if (m_nRollMarks[0][1] < 10)
        m_nRollMarks[0][1] = 10;
    if (m_nRollMarks[1][0] < 10)
        m_nRollMarks[1][0] = 10;
    if (m_nRollMarks[1][1] < 10)
        m_nRollMarks[1][1] = 10;

	_strPLCIP = strPLCIP;
    stickmarker = new StickMarker(_strPLCIP);
	_edition = 1;
	m_configFile.GetEdition(_edition);
	stickmarker->setEdition(_edition);
	_bSupportMarkCode = false;

	String strIP;
	int port;
	int ioPort[4] = { -1,-1,-1,-1 };
	ret = m_configFile.GetMarkingServerAddr(strIP, port, ioPort, _bSupportMarkCode);
	//if (ret == 0 || ioPort[0] < 0)
	//{
	//	AlertWindow::showMessageBox(AlertWindow::WarningIcon, L"������ȱʧ����IP�Ͷ˿�", L"");
	//}
	LOGWT("����IP���˿����ã�%s %d", strIP, port);

	LOGWT("����IO���ã�%d %d %d %d", ioPort[0], ioPort[1], ioPort[2], ioPort[3]);

	//_markingEng = new SocketMarker(this, strIP, port);//��īͨѶ
	//_markingEng->SetMarkerEnable(false);
	//_markingEng->SetSendMode(SendMode_Direct);
	      
	_ioPorts[0] = 0;
	_ioPorts[1] = 2;
	_ioPorts[2] = 4;
	_ioPorts[3] = 6;
	_pulseLast = 50;
	ret = m_configFile.GetMarkingIOPorts(_ioPorts, _pulseLast);
	LOGWT("����IO���ã�%d %d %d %d", _ioPorts[0], _ioPorts[1], _ioPorts[2], _ioPorts[3]);


	m_bSupportStick_Remain = false;
	int stickRemain = 0;
	ret = m_configFile.GetRemainStick(stickRemain);
	m_bSupportStick_Remain = stickRemain;
	LOGWT("������꣺%d", m_bSupportStick_Remain);

	int styleUI = 0;
	if(m_configFile.GetUIMode(styleUI))
		m_bNewUI = styleUI;
	else
		AlertWindow::showMessageBox(AlertWindow::WarningIcon, L"������ȱʧ������ģʽ����", L"");
	LOGWT("UI��Ϣ��ʽm_bNewUI��%d", m_bNewUI);

	g_pWriteThread = new WriteCommData();
	g_pWriteThread->startThread();	   
    //��������
    //for (int i = 0; i < 100; i++)
    //{
    //    StickMarkInfo tm;
    //    tm.codeReason = 0x100;
    //    tm.lenPos = 100000;
    //    tm.ip = "127.0.0.1";		
    //    tm.roadUser = 4;//1

    //    tm.sendTime = Time::currentTimeMillis();
    //    tm.stickTime = Time::currentTimeMillis();
    //    tm.road = 4;
    //    tm.iSe = 1;
    //    tm.bSticked = true;
    //    tm.EA = 1;			   	
    //    m_stickInfos.push_back(tm);

    //}

	for (int i = 0; i < 4; i++)
		preCmdTime[i] = Time::currentTimeMillis();

	CLabelDM[0]->SetIOports(_ioPorts);
	CLabelDM[1]->SetIOports(_ioPorts);


	CLabelDM[0]->SetEALength(m_EASumLen);
	CLabelDM[1]->SetEALength(m_EASumLen);

	EAEditor.get()->setText(String(m_EASumLen));

    g_client = &client;

    g_MainComponent = this;
	setSize(650, 350);

	int bC4MarkLocal = 1;
	m_configFile.GetC4Mode(bC4MarkLocal);
	if (bC4MarkLocal >= 1)
		m_bUseLocal = true;
	else
		m_bUseLocal = 0;
	LOGWT("����4���꣨1.���� 0.����PLC��%d", m_bUseLocal);

	stickmarker->m_bLocalC4 = m_bUseLocal;

	//���Դ���
	//for (int i = 0; i < 100; ++i)
	//{
	//	NumCount numcount;
	//	numcount.id = i%4;
	//	numcount.num[0] = 3 * i;
	//	numcount.num[1] = 3 * i + 1;
	//	numcount.num[2] = 3 * i + 2;
	//	g_stickNumCount.push_back(numcount);
	//	StickMarkInfo sf;
	//	sf.EA = i;
	//	sf.roadUser = i % 4 + 1;
	//	sf.bSticked = true;
	//	sf.codeReason = 0x200;
	//	m_stickInfos.push_back(sf);
	//}

	//��ʱ���ᵼ�»��濨���������̴߳���
	//6003_X8 C1
    startTimer(5000);

	// �����ź��߳�
	//ֻ��6003_X9 C2 ���Թ�
	//std::thread th1(threadOfTimer);
	//th1.detach();
}

MainComponent::~MainComponent()
{
	_threadShouldExit = true;
	juce::Thread::sleep(1000);

	LOGWT("����˳�!");

	CreateNewRoll();

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 4; ++j) {
            DO_Array[i][j] = nullptr;
            //DI_Array[i][j] = nullptr;
        }
    }
    for (int i = 0; i < 4; ++i) {
       
        Label_MarkCount[i] = nullptr;
    }
	NewRoll = nullptr;
	stickmarker->signalThreadShouldExit();
	stickmarker->stopThread(1000);
	g_pWriteThread->signalThreadShouldExit();
	g_pWriteThread->stopThread(1000);
	delete g_pWriteThread;
    delete textButtonReset;
}

//==============================================================================
void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(240, 240, 240));

    float Width = getWidth();
    float Height = getHeight();

    g.setColour(juce::Colours::black);
    //DO��Ӧ�Ŀռ�
    g.drawHorizontalLine(0.2 * Height, 0.02 * Width, 0.98 * Width);
    g.drawHorizontalLine(0.5 * Height, 0.02 * Width, 0.98 * Width);
    g.drawVerticalLine(0.02 * Width, 0.2 * Height, 0.5 * Height);
    g.drawVerticalLine(0.98 * Width, 0.2 * Height, 0.5 * Height);
    g.drawText("DO", 0.02 * Width, 0.16 * Height, 0.08 * Width, 0.04 * Height,juce::Justification::centred);

    //DI��Ӧ�Ŀռ�
    //g.drawHorizontalLine(0.56 * Height, 0.02 * Width, 0.98 * Width);
    //g.drawHorizontalLine(0.86 * Height, 0.02 * Width, 0.98 * Width);
    //g.drawVerticalLine(0.02 * Width, 0.56 * Height, 0.86 * Height);
    //g.drawVerticalLine(0.98* Width, 0.56 * Height, 0.86 * Height);
    //g.drawText("DI", 0.02 * Width, 0.52 * Height, 0.08 * Width, 0.04 * Height, juce::Justification::centred);





   

}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

    for (int i = 0; i < 4; ++i) {
        Label_MarkCount[i]->setBoundsRelative(0.01 + i%2*0.2, 0.02 + i/2*0.05, 0.2, 0.05);
    }

   
	int i = 0;
    NewRoll->setBoundsRelative(0.5 + i % 2 * 0.2, 0.02+i / 2 * 0.06, 0.1, 0.05);

    textButtonReset->setBoundsRelative(0.5, 0.08, 0.1, 0.05);
    MarkInfo->setBoundsRelative(0.7, 0.08, 0.1, 0.05);
    Reconnect->setBoundsRelative(0.04, 0.9, 0.1, 0.05);


	EALabel->setBoundsRelative(0.24, 0.9, 0.1, 0.05);
	EAEditor->setBoundsRelative(0.35, 0.9, 0.1, 0.05);

	i = 1;
	rollEditor->setBoundsRelative(0.5 + i % 2 * 0.2, 0.02 + i / 2 * 0.06, 0.2, 0.05);


    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 4; ++j) {
            DO_Array[i][j]->setBoundsRelative(0.04f + 0.92/8 * i, 0.23 + 0.06 * j, 0.1, 0.05);
        }
    }

    reloadConfig->setBoundsRelative(0.7, 0.9, 0.1, 0.05);

    //for (int i = 0; i < 8; ++i) {
    //    for (int j = 0; j < 4; ++j) {
    //        DI_Array[i][j]->setBoundsRelative(0.04f + 0.92 / 8 * i, 0.59 + 0.06 * j, 0.1, 0.05);
    //    }
    //}


}
void MainComponent::ResetAndNewRoll(String rollName)
{
	LOGWT("�յ�Ĥ���:%s", rollName.toStdString().c_str());
	m_rollName = rollName;
	//��λ���
	bool ret2 = stickmarker->SendCmd(4204, 1);
	
	CreateNewRoll();

	CTLMARK_PG data;
	data.code = 10000;
	data.valid = false;
	sever.SendData(data);

	//�¾�����
	stickmarker->SendCmd(4204, 0);

}
void MainComponent::buttonClicked(juce::Button* buttonthatwasclicked) {
    for (int i = 0; i < 8; ++i)
	{
        for (int j = 0; j < 4; ++j)
		{
            if (buttonthatwasclicked == DO_Array[i][j].get())
			{
				int ioPort = i * 4 + j;
                if (DO_Array[i][j]->getToggleState())
				{
                    Pci1230WriteDoBit(15, ioPort, 1);
                    if (isMarkerOnUse[0] && ioPort == _ioPorts[0])
					{
						Num_Mark[0]++;
						_preStickTime[0] = juce::Time::currentTimeMillis();
						LOGWT("��·1�����ֶ�����");
                    }
					else if (isMarkerOnUse[1] &&  ioPort == _ioPorts[1])
					{
						Num_Mark[1]++;
						_preStickTime[1] = juce::Time::currentTimeMillis();
						LOGWT("��·2�����ֶ�����");

                    }
					else if (isMarkerOnUse[2] && ioPort == _ioPorts[2])
					{
						Num_Mark[2]++;
						_preStickTime[2] = juce::Time::currentTimeMillis();
						LOGWT("��·3�����ֶ�����");
					}
					else if (isMarkerOnUse[3] &&  ioPort == _ioPorts[3])
					{
						Num_Mark[3]++;
						_preStickTime[3] = juce::Time::currentTimeMillis();
						LOGWT("��·4�����ֶ�����");
					}
                }
				else
				{
					Pci1230WriteDoBit(15, ioPort, 0);

				}
            }
        }
    }
   
	if (buttonthatwasclicked == EALabel.get())
	{
		m_EASumLen =  EAEditor.get()->getText().getIntValue();
		CLabelDM[0]->SetEALength(m_EASumLen);
		CLabelDM[1]->SetEALength(m_EASumLen);
		int iSpeedRatio = 100;
		//д���ļ�
		m_configFile.SavePara(m_nRollMarks[0][0], m_nRollMarks[0][1], m_nRollMarks[1][0], m_nRollMarks[1][1], iSpeedRatio, m_EASumLen);
		//
		LOGWT("�趨EA�ܳ�:%d", m_EASumLen);
		//stickmarker->SendCmd(504, m_EASumLen);

	}
    if (buttonthatwasclicked == NewRoll.get())
    {
		m_rollName = rollEditor->getText();
        CreateNewRoll();
		bool ret = stickmarker->SendCmd(4204, 0);
		//ret = stickmarker->transferfins->SetDWordData(4204, 0);

        if (ret)
            stickmarker->Info << L"Ĥ�����д0�ɹ�"<<'\n';
        else
            stickmarker->Info << L"Ĥ�����д0ʧ��" << '\n';
        stickmarker->juce__textEditor->setText(stickmarker->Info, false);
         
		LOGWT("Ĥ�����:%s", m_rollName.toStdString().c_str());
    }
    
    if (buttonthatwasclicked == textButtonReset) 
    {
        int ret = AlertWindow::showYesNoCancelBox(AlertWindow::WarningIcon, L"��λCCD ", L"��ȷ��CCD�����ֹͣ״̬", L"ȷ��", L"��", L"ȡ��");
        if (ret != 1)
            return;
        CTLMARK_PG data;
        data.code = 10000;
        data.valid = false;
		bool ret2 = false;
		ret2 = stickmarker->SendCmd(4204, 1);
		//ret2= stickmarker->transferfins->SetDWordData(4204, 1);

       if (ret2)
           stickmarker->Info << L"Ĥ��ʼд1�ɹ�" << '\n';
       else
           stickmarker->Info << L"Ĥ��ʼд1ʧ��" << '\n';
       stickmarker->juce__textEditor->setText(stickmarker->Info, false);
	           
	   CreateNewRoll();

       if (sever.SendData(data) == 0)
        {
            AlertWindow::showMessageBox(AlertWindow::InfoIcon, L"���͸�λ����ɹ� ", "");
 
        }
        else
        {
            AlertWindow::showMessageBox(AlertWindow::InfoIcon, L"���͸�λ����ʧ��! ", "");

        }
	   LOGWT("���͸�λ����");

    }

    if (buttonthatwasclicked == MarkInfo)
    {
        juce::DialogWindow::showDialog("", stickmarker, this, juce::Colours::white, true);
    }
    
    else if (buttonthatwasclicked == Reconnect)
    {
		_finsLock.enter();
		stickmarker->Reconnect();
		_finsLock.exit();

		//stickmarker->transferfins->Disconnect();// UnInitialize();
		LOGWT("��������");
		if (stickmarker->IsConnected())
		{
			juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::InfoIcon, "", L"����PLC�ɹ�");
			return;
		}
        else
        {
			juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::InfoIcon, "", L"����PLCʧ��");

            //stickmarker->transferfins->Connect();
            //if (stickmarker->transferfins->IsConnected())
            //    juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::InfoIcon, "", L"����PLC�ɹ�");
           // else
           //     juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::InfoIcon, "", L"����PLCʧ��");
        }
    }

    else if (buttonthatwasclicked == reloadConfig.get())
    {
        int ret= m_configFile.GetMarkingIOPorts(_ioPorts, _pulseLast);
        CLabelDM[0]->SetIOports(_ioPorts);
        CLabelDM[1]->SetIOports(_ioPorts);
        if (ret)
            juce::AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, L"���¼������óɹ�", L"");
        else
            juce::AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, L"���¼�������ʧ��", L"");
    }

}




//��Ϣ������õĺ���
void MainComponent::HandleMessage(int flag,double& speed) //��������Ϣ
{
	juce::int64 curTime = Time::currentTimeMillis();
	double dSpeed = speed * 1000. / 60000; //10m/min-->mm/ms
	int sleepTime = 30;
	if (dSpeed > 0.5)
		sleepTime = 50. / dSpeed+30;

	if (sleepTime < 300)
		sleepTime = 300;

	_finsLock.enter();

	if( (flag == -75 || flag == -175) && isMarkerOnUse[0]) //����·1���ʹ���ź�
	{
		if (!isMarkerOnUse[0])
		{
			LOGWT("��·1����,����4����ָ����Ч");
			_finsLock.exit();
			return;
		}
		//��������4����ָ��
		if ((curTime - preCmdTime[0]) < 100000) //100s���ڲ����ظ�4����
		{
			LOGWT("��·1����4����ָ��ʱ����ϴ�С��100s����Ч");
			_finsLock.exit();
			return;
		}
		preCmdTime[0] = curTime;
		LOGWT("��·1�յ�4����ָ��,�ٶ�=%f,EA=%d", speed, m_curEA[0]);
		//��4����ָ��	
		if (m_bUseLocal)
		{
			for (int i = 0; i < 4 && speed>0.5; i++)
			{
				Pci1230WriteDoBit(15, _ioPorts[0], 1);
				juce::Thread::sleep(50);
				Pci1230WriteDoBit(15, _ioPorts[0], 0);
				juce::Thread::sleep(sleepTime);
				Num_Mark[0] += 1;
				LOGWT("��·1����%d��4����", i + 1);
				_preStickTime[0] = Time::currentTimeMillis()+1000;
			}
		}

		if (!m_bUseLocal) //ʹ�ó�����
		{
			bool ret = stickmarker->SendCmd(4432, 1);
			_preStickTime[0] = Time::currentTimeMillis() + 2000;
			//Num_Mark[0] += 4;
		}
		stickmarker->m_bC4Mark[0] = true;

		LOGWT("��·1����4����");
		m_continue4StickEA[0] = m_curEA[0];
		

		//Num_Mark[0] += 4;
		//preCmdTime[0] = curTime;
		_preStickLenPos[0] = _curRunningLenPos[0];
		//��������4����ָ��󣬸�ͨѶ���������͹ر��ź�

		//
		if (client.isConnected())
		{
			INTER_MESSAGE data;
			data._spec = 201404;
			data._flag = flag;
			client.SendData(data);

		}
	}
	else if ((flag == -76 || flag == -176) && isMarkerOnUse[1]) //����·2���ʹ���ź�
	{
		if (!isMarkerOnUse[1])
		{
			LOGWT("��·2����,����4����ָ����Ч");
			_finsLock.exit();
			return;
		}
		if ((curTime - preCmdTime[1]) < 100000) //100s���ڲ����ظ�4����
		{
			LOGWT("��·2����4����ָ��ʱ����ϴ�С��100s����Ч");
			_finsLock.exit();
			return;
		}
		LOGWT("��·2�յ�4����ָ��,�ٶ�=%f,EA=%d", speed, m_curEA[1]);
		preCmdTime[1] = curTime;
		//��4����ָ��
		if (m_bUseLocal)
		{
			for (int i = 0; i < 4 && speed>0.5; i++)
			{
				Pci1230WriteDoBit(15, _ioPorts[1], 1);
				juce::Thread::sleep(50);
				Pci1230WriteDoBit(15, _ioPorts[1], 0);
				juce::Thread::sleep(sleepTime);
				Num_Mark[1] += 1;
				LOGWT("��·2����%d��4����", i + 1);
				_preStickTime[1] = Time::currentTimeMillis() + 1000;

			}
		}
		LOGWT("��·2����4����");
		m_continue4StickEA[1] = m_curEA[1];

		if (!m_bUseLocal)
		{
			bool ret = stickmarker->SendCmd(4436, 1);
			_preStickTime[1] = Time::currentTimeMillis() + 2000;
			//Num_Mark[1] += 4;
		}
		stickmarker->m_bC4Mark[1] = true;
		//Num_Mark[1] += 4;
		//preCmdTime[1] = curTime;
		_preStickLenPos[1] = _curRunningLenPos[1];
		//��������4����ָ��󣬸�ͨѶ���������͹ر��ź�
		if (client.isConnected())
		{
			INTER_MESSAGE data;
			data._spec = 201404;
			data._flag = flag;
			client.SendData(data);

		}
	}
	else if ( (flag == -77|| flag == -177) && isMarkerOnUse[2]) //����·3���ʹ���ź�
	{	
		if (!isMarkerOnUse[2])
		{
			LOGWT("��·3����,����4����ָ����Ч");
			_finsLock.exit();
			return;
		}
		if ((curTime - preCmdTime[2]) < 100000) //100s���ڲ����ظ�4����
		{
			LOGWT("��·3����4����ָ��ʱ����ϴ�С��100s����Ч");
			_finsLock.exit();
			return;
		}
		LOGWT("��·3�յ�4����ָ��,�ٶ�=%f,EA=%d", speed, m_curEA[2]);
		preCmdTime[2] = curTime;
		//��4����ָ��
		if (m_bUseLocal)
		{
			for (int i = 0; i < 4 && speed>0.5; i++)
			{
				Pci1230WriteDoBit(15, _ioPorts[2], 1);
				juce::Thread::sleep(50);
				Pci1230WriteDoBit(15, _ioPorts[2], 0);
				juce::Thread::sleep(sleepTime);
				Num_Mark[2] += 1;
				LOGWT("��·3����%d��4����", i + 1);
				_preStickTime[2] = Time::currentTimeMillis()+1000;
			}
		}
		LOGWT("��·3����4����");
		m_continue4StickEA[2] = m_curEA[2];
		if (!m_bUseLocal)
		{
			bool ret = stickmarker->SendCmd(4440, 1);
			_preStickTime[2] = Time::currentTimeMillis() + 2000;
			//Num_Mark[2] += 4;
		}
		stickmarker->m_bC4Mark[2] = true;
		//Num_Mark[2] += 4;
		//preCmdTime[2] = curTime;
		_preStickLenPos[2] = _curRunningLenPos[2];
		//��������4����ָ��󣬸�ͨѶ���������͹ر��ź�
		if (client.isConnected())
		{
			INTER_MESSAGE data;
			data._spec = 201404;
			data._flag = flag;
			client.SendData(data);
		}
	}
	else if ( (flag == -78|| flag == -178) && isMarkerOnUse[3]) //����·4���ʹ���ź�
	{
		if (!isMarkerOnUse[3])
		{
			LOGWT("��·4����,����4����ָ����Ч");
			_finsLock.exit();
			return;
		}

		if ((curTime - preCmdTime[3]) < 100000) //100s���ڲ����ظ�4����
		{
			LOGWT("��·4����4����ָ��ʱ����ϴ�С��100s����Ч");
			_finsLock.exit();
			return;
		}
		LOGWT("��·4�յ�4����ָ��,�ٶ�=%f,EA=%d", speed, m_curEA[3]);
		preCmdTime[3] = curTime;
		//��4����ָ��
		if (m_bUseLocal)
		{
			for (int i = 0; i < 4 && speed>0.5; i++)
			{
				Pci1230WriteDoBit(15, _ioPorts[3], 1);
				juce::Thread::sleep(50);
				Pci1230WriteDoBit(15, _ioPorts[3], 0);
				juce::Thread::sleep(sleepTime);
				Num_Mark[3] += 1;
				LOGWT("��·4����%d��4����", i + 1);
				_preStickTime[3] = Time::currentTimeMillis()+1000;

			}
		}
		LOGWT("��·4����4����");
		m_continue4StickEA[3] = m_curEA[3];
		
		if(!m_bUseLocal)
		{
			bool ret = stickmarker->SendCmd(4444, 1);
			_preStickTime[3] = Time::currentTimeMillis() + 2000;
			//Num_Mark[3] += 4;
		}
		stickmarker->m_bC4Mark[3] = true;
		//Num_Mark[3] += 4;
		//preCmdTime[3] = curTime;
		_preStickLenPos[3] = _curRunningLenPos[3];
		//��������4����ָ��󣬸�ͨѶ���������͹ر��ź�
		if (client.isConnected())
		{
			INTER_MESSAGE data;
			data._spec = 201404;
			data._flag = flag;
			client.SendData(data);
		}
	}
	_finsLock.exit();
}
void MainComponent::HandleClientMessage(juce::String host,HDataMessage* pMessage)
{
    int iRoad = pMessage->data.iRoad;
	StickMarkInfo tm;
	int ioPort = -1;
	if (pMessage->data.iRoad == 0 && pMessage->data.iSel == 1)
	{
		tm.roadUser = 1;
		ioPort = _ioPorts[0];
	}
	else if (pMessage->data.iRoad == 1 && pMessage->data.iSel == 1)
	{
		tm.roadUser = 2;
		ioPort = _ioPorts[1];

	}
	else if (pMessage->data.iRoad == 0 && pMessage->data.iSel == 0)
	{
		tm.roadUser = 3;
		ioPort = _ioPorts[2];

	}
	else if (pMessage->data.iRoad == 1 && pMessage->data.iSel == 0)
	{
		tm.roadUser = 4;
		ioPort = _ioPorts[3];

	}
	//�������
	/*if (pMessage->data.code < 0)
	{
		double destPos = pMessage->data.destPos * 0.1;
		MarkerMessage* msg = nullptr; (1, 128, -1, 0);
		for (int i = 0; i < m_stickInfos.size(); i++)
		{
			if (m_stickInfos[i].lenPos < destPos &&
				m_stickInfos[i].road == pMessage->data.iRoad &&
				m_stickInfos[i].iSe == pMessage->data.iSel &&
				(destPos - m_stickInfos[i].lenPos) < m_EASumLen)
			{
				int type = ProduceDefectCode(m_stickInfos[i].codeReason);
				NGPosition pos((Marker_Defect_Type)type);
				if (msg == nullptr)
				{
					msg = new MarkerMessage(type - 1, 128, -1, 0);
					msg->AddNGMark(pos);
					continue;
				}
				msg->AddNGMark(pos);

			}
		}
		if (msg)
		{
			_markingEng->SetMarkerEnable(true);
			_markingEng->SendIOSigAndMarkerMsg(ioPort, *msg, _pulseLast);
			delete msg;
		}
	}*/
	//

    if (iRoad >= 0 && iRoad < 2) //�ϴ�· ����
    {

        if (pMessage->data.valid)
        {
            pMessage->data.destPos /= 10.;

            tm.codeReason = pMessage->data.code;
            tm.lenPos = pMessage->data.destPos;
			strcpy(tm.ip, host.toStdString().c_str());

    
            tm.sendTime = Time::currentTimeMillis();
            tm.stickTime = Time::currentTimeMillis();
            tm.road = pMessage->data.iRoad;
            tm.iSe = pMessage->data.iSel;
            tm.bSticked = false;
            tm.EA = pMessage->data.EA;
            //1,0  0,0   0,1 1,1
            //AddLabelData�ڲ��Ѿ�����
            bool ret = CLabelDM[iRoad]->AddLabelData(pMessage->data.iRoad, pMessage->data.iSel, pMessage->data.srcPos, pMessage->data.destPos);



            _objectLock.enter();
			if (ret)
			{
				m_stickInfos.push_back(tm);
			}
            //��Ҫȷ������
            if (g_pWriteThread && ret)
            {         

                char szTm[256] = { 0 };
                juce::Time time = juce::Time::getCurrentTime();
                int index = sprintf(szTm, "%d_%d_%d_%d:%d:%d", time.getYear(), time.getMonth() + 1, time.getDayOfMonth(), time.getHours(), time.getMinutes(), time.getSeconds());
                sprintf(&szTm[index], " Succ add label from%s: srcPos=%f,iRoad=%d, iSel=%d, destPos=%f\n", host.toStdString().c_str(), pMessage->data.srcPos, iRoad, pMessage->data.iSel, pMessage->data.destPos);
				g_pWriteThread->AddDebugData(szTm);
            }
            else if (g_pWriteThread && !ret)
            {
                char szTm[256] = { 0 };
                juce::Time time = juce::Time::getCurrentTime();
                int index = sprintf(szTm, "%d_%d_%d_%d:%d:%d", time.getYear(), time.getMonth() + 1, time.getDayOfMonth(), time.getHours(), time.getMinutes(), time.getSeconds());
                sprintf(&szTm[index], "Fail add label from%s: iRoad=%d, iSel=%d, destPos=%f\n", host.toStdString().c_str(), iRoad, pMessage->data.iSel, pMessage->data.destPos);
				g_pWriteThread->AddDebugData(szTm);

            }
            _objectLock.exit();

        }
        else
        {
            pMessage->data.srcPos /= 10.;
            double dSpeed = client.GetMachineSpeed();
            double destPos = 0.;

			_objectLock.enter();

			// ����豸�������¾���
			if(m_bSupportStick_Remain && pMessage->data.srcPos < 600 && (_curRunningLenPos[tm.roadUser - 1]- pMessage->data.srcPos) >= 1000.)
			{
				for (int i = 0; i < m_stickInfos.size(); i++) //ͣ��������¾�ʼ������Ĵ�·δ����������
				{
					if (m_stickInfos[i].bSticked || m_stickInfos[i].roadUser <= 0)
						continue;
					bool bSkip = false;
					for (int j = i + 1; j < m_stickInfos.size(); j++) //ͣ��������¾�ʼ������Ĵ�·δ����������
					{
						//�������һ���Ѿ����꣬��Ĵ����Ϣ��Ӧ�ô���
						if (m_stickInfos[j].bSticked && m_stickInfos[i].road == m_stickInfos[j].road &&  m_stickInfos[i].iSe == m_stickInfos[j].iSe)
						{
							bSkip = true;
							break;
						}
						if (m_stickInfos[j].bSticked && (m_stickInfos[j].lenPos - m_stickInfos[i].lenPos) >= m_EASumLen*0.5) //4m�Ժ�����Ѿ�����ģ���ǰ���Ҳ��Ӧ�ó���
						{
							bSkip = true;
							break;
						}
					}
					if (bSkip)
						continue;

					if (_curRunningLenPos[m_stickInfos[i].roadUser - 1] <  m_stickInfos[i].lenPos) //��Ч
					{
						m_stickInfos[i].lenPos = m_stickInfos[i].lenPos- _curRunningLenPos[m_stickInfos[i].roadUser - 1];
						m_stickInfoReserves.push_back(m_stickInfos[i]);
						m_stickInfos.erase(m_stickInfos.begin() + i);
						i--;
					}
				}
			}

			_curRunningLenPos[tm.roadUser - 1] = pMessage->data.srcPos;			

            if (/*g_iUsePLCSpeed &&*/ dSpeed > 5.)
            {
                juce::int64 diffT = juce::Time::getCurrentTime().toMilliseconds() - client.m_recTime;
                if (diffT < 0) diffT = -diffT;

                if (diffT < 5000)
                    pMessage->data.speed = dSpeed;
            }


			//����
			if (false && g_pWriteThread)
			{
				char szTm[256] = { 0 };
				juce::Time time = juce::Time::getCurrentTime();
				int index = sprintf(szTm, "%d_%d_%d_%d:%d:%d", time.getYear(), time.getMonth() + 1, time.getDayOfMonth(), time.getHours(), time.getMinutes(), time.getSeconds());
				sprintf(&szTm[index], " ***position  from %s***: iRoad=%d,  iSel=%d, curPos=%f\n", host.toStdString().c_str(), iRoad, pMessage->data.iSel,pMessage->data.srcPos);
				g_pWriteThread->AddDebugData(szTm);
			}

			m_sumEars = (juce::int64)pMessage->data.destPos;
			m_curEA[tm.roadUser - 1] = m_sumEars;

			//����4�����覴ô��
			bool bValid = true;
			if (m_curEA[tm.roadUser - 1] >0 && m_curEA[tm.roadUser - 1] == m_continue4StickEA[tm.roadUser - 1])
			{
				bValid = false;
			}

            //RunningLength�ڲ��Ѿ�����
			bool bSticked = false;
			if(bValid)
				bSticked = CLabelDM[iRoad]->RunningLength(pMessage->data.iRoad, pMessage->data.iSel, pMessage->data.speed, pMessage->data.srcPos, destPos);
            if (bSticked)
			{             
               Num_Mark[tm.roadUser-1]++;   
            }


  

            m_preSentPos[iRoad][pMessage->data.iSel] = pMessage->data.srcPos;
            m_sentCount[iRoad]++;
            if (m_sentCount[iRoad] > 900000000)
            {
                m_sentCount[iRoad] = 0;
            }

            if (bSticked)
            {
                for (int i = 0; i < m_stickInfos.size(); i++)
                {
                    if (m_stickInfos[i].bSticked || m_stickInfos[i].road != pMessage->data.iRoad || m_stickInfos[i].iSe != pMessage->data.iSel)
                        continue;

                    if (fabs(m_stickInfos[i].lenPos - destPos) < 5.)
                    {
                        m_stickInfos[i].bSticked = true;
                        m_stickInfos[i].stickTime = Time::currentTimeMillis();

						//�ж��Ƿ�ĳ����·����5��EA���
						if(m_iSupportCM5 >= 2)
						{
							int roadConti[4] = { 0,0,0,0 };
							double eaOrders[4] = { -1,-1,-1,-1 };
							for (int j = 0; j < m_stickInfos.size(); j++)
							{
								int roadM = m_stickInfos[j].roadUser;
								if (roadM >= 1 && roadM <= 4)
								{
									if (j < m_roadsSearch[roadM - 1])
										continue;

									if (eaOrders[roadM - 1] < 0 || (abs(m_stickInfos[j].lenPos - eaOrders[roadM - 1]) <= m_EASumLen * 1.5))
									{
										roadConti[roadM - 1]++;
										eaOrders[roadM - 1] = m_stickInfos[j].lenPos;
									}
									else
									{
										roadConti[roadM - 1] = 1;
										eaOrders[roadM - 1] = m_stickInfos[j].lenPos;
									}
									if (roadConti[roadM - 1] >= m_iSupportCM5)
									{
										m_roadsSearch[roadM - 1] = j;
										String info = L"��·";
										info << roadM << L"��������ͣ��!";
										stickmarker->StopMachine(info);
										break;
									}
								}
							  }
						}

		
                        //��Ҫȷ������
                        if (g_pWriteThread)
                        {
                            char szTm[256] = { 0 };
                            juce::Time time = juce::Time::getCurrentTime();
                            int index = sprintf(szTm, "%d_%d_%d_%d:%d:%d", time.getYear(), time.getMonth() + 1, time.getDayOfMonth(), time.getHours(), time.getMinutes(), time.getSeconds());
                            sprintf(&szTm[index], " ***Succ label from %s ***: iRoad=%d, iSel=%d, destPos=%f\n", m_stickInfos[i].ip, iRoad, pMessage->data.iSel, destPos);
							g_pWriteThread->AddDebugData(szTm);
                        }
                        break;
                    }
                }
            }
            _objectLock.exit();

        }
    }

}


void MainComponent::Save2MonthReport()
{
	try
	{

		/* by gxx 20210315
		juce::Time tm = Time::getCurrentTime();
		String mon = String::formatted("%02d", tm.getMonth() + 1);
		
		String strFullPath = L"D:\\CCD_�������\\"; 
		strFullPath << L"\\" << tm.getMonth() + 1;
		File fFile(strFullPath);
		if(!fFile.exists())
		if (!fFile.createDirectory())
			return ;

		String xfileName = strFullPath;
		xfileName << L"\\";
		xfileName << L"���´��ͳ�ƻ���---"; 
				

		
		xfileName << tm.getYear() << mon << L".xlsx";
		juce::StringArray classDefects;
		

		classDefects.add(L"©����");
		classDefects.add(L"��̼");
		classDefects.add(L"����");
		classDefects.add(L"����");
		classDefects.add(L"����");
		classDefects.add(L"�ڵ�");
		classDefects.add(L"����������");
		classDefects.add(L"�������");
		classDefects.add(L"��Ĥ��");
		classDefects.add(L"AT9���");
		classDefects.add(L"�������");
		classDefects.add(L"�Ӵ�");


		juce::HashMap<String, int> statics[4];


		for (int i = 0; i < classDefects.size(); i++)
		{
			statics[0].set(classDefects[i], 0);
			statics[1].set(classDefects[i], 0);
			statics[2].set(classDefects[i], 0);
			statics[3].set(classDefects[i], 0);

		}

		int Nums[5] = { 0 };
		for (int i = 0; i < m_stickInfos.size(); i++)
		{
			Nums[m_stickInfos[i].roadUser]++;
		
		}


		juce::String str;
		for (int i = 0; i < m_stickInfos.size(); i++)
		{
			int code = m_stickInfos[i].codeReason;
			String reason = ProduceDefectString(code);
			for (int j = 0; j < classDefects.size(); j++)
			{
				if (classDefects[j].indexOfIgnoreCase(0, reason) >= 0)
				{
					str = classDefects[j];
					int cur = statics[m_stickInfos[i].roadUser-1][str];
					cur++;
					statics[m_stickInfos[i].roadUser - 1].set(str, cur);
					break;
				}
			}		
		}

		int rowsOfD = classDefects.size();
		juce::String title = L"N3119111623734";

		xlnt::workbook* wb = nullptr;

		juce::File d1(xfileName);
		if (!d1.exists())
		{
			wb = new xlnt::workbook();

		}
		else
		{
			 wb = new xlnt::workbook(xlnt::path(xfileName.toStdString().c_str()));

		}
		xlnt::worksheet ws = wb->active_sheet();
		xlnt::range range = ws.rows();
		xlnt::row_t lastRow =  range.back().back().row();


		ws.cell("A1").value(juce::String(L"���").toRawUTF8());
		ws.cell("B1").value(juce::String(L"��λ").toRawUTF8());
		ws.cell("C1").value(juce::String(L"��ʼʱ��").toRawUTF8());
		ws.cell("D1").value(juce::String(L"��ֹʱ��").toRawUTF8());
		ws.cell("E1").value(juce::String(L"����").toRawUTF8());
		ws.cell("F1").value(juce::String(L"��Ʒ��").toRawUTF8());
		ws.cell("G1").value(juce::String(L"������").toRawUTF8());

		for (int j = 0; j < classDefects.size(); j++)
		{
			char t1 = 'G' + j + 1;
			char ms[2] = { 0 };
			ms[0] = t1;
			int c = 1;
			String t2 = ms;
			t2 << c;
			ws.cell(t2.toRawUTF8()).value(classDefects[j].toRawUTF8());

		}
		if (lastRow == 1)
			lastRow++;


		for (int k = 0; k < 4; k++)
		{
			//Ĥ���
			String rowVal = "A";
			rowVal << (int)lastRow;
			ws.cell(rowVal.toStdString().c_str()).value(m_rollName.toRawUTF8());

			//��λ
			rowVal = "B";
			rowVal << (int)lastRow;
			String rValue = L"��λ";
			rValue << k+1;
			ws.cell(rowVal.toStdString().c_str()).value(rValue.toRawUTF8());

			//��ʼʱ��
			rowVal = "C";
			rowVal << (int)lastRow;
			rValue = L"88888888";
			ws.cell(rowVal.toStdString().c_str()).value(rValue.toRawUTF8());


			rValue = L"";
			juce::Time time2 = juce::Time::getCurrentTime();
			rValue << time2.getYear() << "-" << time2.getMonth() + 1 << "-" << time2.getDayOfMonth() << " " << time2.getHours() << ":" << time2.getMinutes() << ":"
				<< ":" << time2.getSeconds();
			//��ֹʱ��
			rowVal = "D";
			rowVal << (int)lastRow;
			ws.cell(rowVal.toStdString().c_str()).value(rValue.toRawUTF8());

			//����
			rValue = L"";
			rValue << m_sumEars;
			rowVal = "E";
			rowVal << (int)lastRow;
			ws.cell(rowVal.toStdString().c_str()).value(m_sumEars);

			//��Ʒ��
			rValue = L"";
			rValue << Nums[k+1];
			rowVal = "F";
			rowVal << (int)lastRow;
			ws.cell(rowVal.toStdString().c_str()).value(Nums[k + 1]);//rValue.toRawUTF8()


			if (m_sumEars < classDefects.size())
			{
				m_sumEars = -1;
			}
			double rate = Nums[k+1]/ double(m_sumEars * 4) * 100;

			//������
			rValue = L"";
			if (m_sumEars < 0)
				rValue << L"---";
			else
				rValue << rate<<L"%";
			rowVal = "G";
			rowVal << (int)lastRow;
			ws.cell(rowVal.toStdString().c_str()).value(rValue.toRawUTF8());


			for (int j = 0; j < classDefects.size(); j++)
			{	

				char t1 = 'G' + j + 1;
				char ms[2] = { 0 };
				ms[0] = t1;
				String t2 = ms;				

				t2 << (int)lastRow;
				str = classDefects[j];
				int cur = statics[k][str];

				ws.cell(t2.toRawUTF8()).value(cur);

			}
			lastRow++;
		}
		//ws.freeze_panes("B2");
		wb->save(xfileName.toWideCharPointer());

		*/
	}
	catch (...)
	{
		return;
	}

}

void MainComponent::SaveReport()
{
	/* by gxx 20210315
    if (m_sumEars <= 0 || m_stickInfos.size() == 0)
        return;

    try
    {

        String xfileName = L"CCD������"; //.xlsx
        xfileName << Time::getCurrentTime().formatted("%Y-%m-%d_%H-%M-%S") << L".xlsx";
        juce::StringArray classDefects;
        classDefects.add(L"��������");
        classDefects.add(L"�׵�");
        classDefects.add(L"��̼");
        classDefects.add(L"©����");
        classDefects.add(L"�Ӵ�");
        classDefects.add(L"�ڵ�");
        classDefects.add(L"����");
        classDefects.add(L"����");
        classDefects.add(L"����");
        classDefects.add(L"Ĥ����");
        classDefects.add(L"��Ƭ����");
        classDefects.add(L"��������");
        classDefects.add(L"ֱ������");
        juce::HashMap<String, int> statics;
        for (int i = 0; i < classDefects.size(); i++)
        {
            statics.set(classDefects[i], 0);
        }

        juce::String str;
        for (int i = 0; i < m_stickInfos.size(); i++)
        {
            int code = m_stickInfos[i].codeReason;
            //
            if (code & 0x200)
            {
                str = L"©����";
                int cur = statics[str];
                cur++;
                statics.set(str, cur);
            }
            if (code & 0x400)
            {
                str = L"����";
                int cur = statics[str];
                cur++;
                statics.set(str, cur);
            }
            if (code & 0x800)
            {
                str = L"��̼";
                int cur = statics[str];
                cur++;
                statics.set(str, cur);

            }
            if (code & 0x1000)
            {
                str = L"����";
                int cur = statics[str];
                cur++;
                statics.set(str, cur);

            }
            if (code & 0x2000)
            {
                str = L"�Ӵ�";
                int cur = statics[str];
                cur++;
                statics.set(str, cur);

            }
            if (code & 0x4000)
            {
                str = L"�׵�";
                int cur = statics[str];
                cur++;
                statics.set(str, cur);

            }
            if (code & 0x8000)
            {
                str = L"�ڵ�";
                int cur = statics[str];
                cur++;
                statics.set(str, cur);

            }
            if (code & 0x400000)
            {
                str = L"��Ƭ����";
                int cur = statics[str];
                cur++;
                statics.set(str, cur);

            }
            //if (code & 0x800000)
            //{
            //	str = L"AT9©����";
            //	int cur = statics[str];
            //	cur++;
            //	statics.set(str, cur);
            //}

            //if (code & 0x1000000)
            //{
            //	str = L"δ��ѹ";
            //	int cur = statics[str];
            //	cur++;
            //	statics.set(str, cur);
            //}
            if (code & 0x2000000)
            {
                str = L"©����";
                int cur = statics[str];
                cur++;
                statics.set(str, cur);
            }
            //str += L"��Ե©����";
            if (code & 0x4000000)
            {
                str = L"��������";
                int cur = statics[str];
                cur++;
                statics.set(str, cur);
            }

            if (code & 0x8000000)
            {
                //str += L"�������� ";
                str = L"��������";
                int cur = statics[str];
                cur++;
                statics.set(str, cur);

            }
            if (code & 0x10000000)
            {
                str = L"����";
                int cur = statics[str];
                cur++;
                statics.set(str, cur);
            }
            //str += L"����";

            //	if (code & 0x20000000)
            //		str += L"δͿĤ";
            if (code & 0x40000000)
            {
                str = L"ֱ������";
                int cur = statics[str];
                cur++;
                statics.set(str, cur);
            }
            //	str += L"ֱ������ ";
            if (code & 0x100)
            {
                str = L"Ĥ����";
                int cur = statics[str];
                cur++;
                statics.set(str, cur);
                //str += L"Ĥ���� ";
            }
        }

        int rowsOfD = classDefects.size();
        juce::String title = m_rollName;

        xlnt::workbook wb;
        xlnt::worksheet ws = wb.active_sheet();
        ws.cell("B2").value(title.toRawUTF8());
        ws.merge_cells("B2:H3");
        ws.merge_cells("B4:E4");
        ws.merge_cells("F4:H4");
        ws.merge_cells("B5:H5");
        ws.merge_cells("B6:E7");
        ws.merge_cells("F6:H7");
        //�����
        ws.merge_cells("B8:H8");

        juce::String  detailCols[5];
        //��ϸ���
        String  strM1 = "B9:";
        strM1 << "H" << 9 + rowsOfD;
        ws.merge_cells(strM1.toStdString().c_str());//
        detailCols[0] = "B9";

        //��۲���ͳ��
        int n1 = 9 + rowsOfD + 1;
        String  strM2 = "B";
        strM2 << n1 << ":" << "H" << n1;
        ws.merge_cells(strM2.toStdString().c_str());


        //��ϸ����ͳ��
        int  detailCol = n1;
        n1++;
        detailCol = n1;
        String strM3 = "B";
        strM3 << n1 << ":" << "E" << n1 + rowsOfD;
        ws.merge_cells(strM3.toStdString().c_str());
        detailCols[1] = "B";
        detailCols[1] << n1;

        //����������
        String strM4 = "F";
        strM4 << n1 << ":" << "H" << n1 + rowsOfD;
        ws.merge_cells(strM4.toStdString().c_str());
        detailCols[2] = "F";
        detailCols[2] << n1;

        //������ϸ
        n1 = n1 + rowsOfD + 1;
        String strM5 = "B";
        strM5 << n1 << ":" << "H" << n1;
        ws.merge_cells(strM5.toStdString().c_str());
        detailCols[3] = "B";
        detailCols[3] << n1;

        n1++;
        xlnt::border border_;
        xlnt::border::border_property prop;
        prop.style(xlnt::border_style::thick);
        border_.side(xlnt::border_side::start, prop);
        border_.side(xlnt::border_side::end, prop);
        border_.side(xlnt::border_side::top, prop);
        border_.side(xlnt::border_side::bottom, prop);
        xlnt::alignment align;
        align.horizontal(xlnt::horizontal_alignment::center);
        align.vertical(xlnt::vertical_alignment::center);

        ws.range("B2:H3").border(border_);
        ws.range("B2:H3").alignment(align);

        align.horizontal(xlnt::horizontal_alignment::left);
        align.vertical(xlnt::vertical_alignment::top);
        align.wrap(true);

        juce::Time time2 = juce::Time::getCurrentTime();
        String opTime = L"ʱ��:";
        opTime << time2.getYear() << "-" << time2.getMonth() + 1 << "-" << time2.getDayOfMonth() << " " << time2.getHours() << ":" << time2.getMinutes() << ":"
            << ":" << time2.getSeconds();

        ws.range("B4:E4").border(border_);
        ws.range("B4:E4").alignment(align);
        ws.cell("B4").value(opTime.toRawUTF8());
        ws.cell("F4").value(juce::String(L"����Ա��Admin").toRawUTF8());

        ws.range("F4:H4").border(border_);
        ws.range("F4:H4").alignment(align);

        ws.range("B5:H5").border(border_);
        ws.range("B5:H5").alignment(align);


        ws.range("B6:E7").border(border_);
        ws.range("B6:E7").alignment(align);

        ws.range("F6:H7").border(border_);
        ws.range("F6:H7").alignment(align);

        ws.cell("B6").value(juce::String(L"Ĥ��ţ�N3119111632734\nƷ������9988H2").toRawUTF8());
        ws.cell("F6").value(juce::String(L"��������2\n����128.7").toRawUTF8());

        ws.range("B8:H8").border(border_);
        ws.range("B8:H8").alignment(align);
        ws.cell("B8").value(juce::String(L"�����").toRawUTF8());

        String rules = opt2str(m_labelStd);
        ws.cell(detailCols[0].toStdString().c_str()).value(rules.toRawUTF8());


        //

        ws.range(strM1.toStdString().c_str()).border(border_);
        ws.range(strM1.toStdString().c_str()).alignment(align);

        ws.range(strM2.toStdString().c_str()).border(border_);
        ws.range(strM2.toStdString().c_str()).alignment(align);

        ws.range(strM3.toStdString().c_str()).border(border_);
        ws.range(strM3.toStdString().c_str()).alignment(align);

        ws.range(strM4.toStdString().c_str()).border(border_);
        ws.range(strM4.toStdString().c_str()).alignment(align);

        ws.range(strM5.toStdString().c_str()).border(border_);
        ws.range(strM5.toStdString().c_str()).alignment(align);
        ws.cell(detailCols[3].toStdString().c_str()).value(juce::String(L"����������ϸ").toRawUTF8());


        int sumBadEars = 0;
        juce::String detailInfo;
        for (int i = 0; i < classDefects.size(); i++)
        {
            int num = statics[classDefects[i]];
            sumBadEars += num;
            detailInfo << classDefects[i] << L":" << num << L"\n";
        }
        ws.cell(detailCols[1].toStdString().c_str()).value(detailInfo.toRawUTF8());

        detailInfo = L"";
        detailInfo << L"����������(*4):" << m_sumEars * 4 << "\n";
        detailInfo << L"����������:" << sumBadEars << "\n";
        if (m_sumEars <= 0)
            m_sumEars = 1;
        double rate = (1-sumBadEars / double(m_sumEars * 4)) * 100;
        detailInfo << L"����:" <<  rate << "%" << "\n";

        ws.cell(detailCols[2].toStdString().c_str()).value(detailInfo.toRawUTF8());



        juce::String detail;
        detail << "B" << n1;
        ws.cell(detail.toStdString().c_str()).border(border_);
        ws.cell(detail.toStdString().c_str()).alignment(align);
        ws.cell(detail.toStdString().c_str()).value(juce::String(L"Ƭ��").toRawUTF8());
        juce::String  cellB = "CDEFGH";
        juce::StringArray tips;
        tips.add(L"��һ��");
        tips.add(L"�ڶ���");
        tips.add(L"������");
        tips.add(L"������");
        tips.add(L"������");
        tips.add(L"������");

        for (int i = 0; i < 6; i++)
        {
            detail = "";
            detail << cellB[i] << n1;
            ws.cell(detail.toStdString().c_str()).border(border_);
            ws.cell(detail.toStdString().c_str()).alignment(align);
            ws.cell(detail.toStdString().c_str()).value(tips[i].toRawUTF8());
        }
        n1++;
        for (int i = 0; i < m_stickInfos.size(); i++)
        {
            detail = "B";
            detail << n1;
            ws.cell(detail.toStdString().c_str()).border(border_);
            ws.cell(detail.toStdString().c_str()).alignment(align);

            String str = "";
            str << m_stickInfos[i].EA;
            ws.cell(detail.toStdString().c_str()).value(str.toRawUTF8());

            detail = "";
            detail << cellB[m_stickInfos[i].roadUser - 1];
            detail << n1;
            ws.cell(detail.toStdString().c_str()).border(border_);
            ws.cell(detail.toStdString().c_str()).alignment(align);

            ws.cell(detail.toStdString().c_str()).value("NG");

            for (int j = 0; j < 6; j++)
            {
                detail = "";
                detail << cellB[j];
                detail << n1;
                ws.cell(detail.toStdString().c_str()).border(border_);
                ws.cell(detail.toStdString().c_str()).alignment(align);
            }

            n1++;
        }

        //ws.freeze_panes("B2");
        wb.save(xfileName.toWideCharPointer());
    }
    catch (...)
    {
        return;
    }
	*/
}

void  MainComponent::SetLabelStd(const TRANSER_OUT_LOGIC& msg)
{
    m_labelStd = msg;

}

void MainComponent::CreateNewRoll()
{

	//��������
	//for (int i = 0; i < 100; i++)
	//{
		//StickMarkInfo tm;
		//tm.codeReason = 0x100;
		//tm.lenPos = 100000;
		//tm.ip = "127.0.0.1";		
		//tm.roadUser = 4;//1
	
		//tm.sendTime = Time::currentTimeMillis();
		//tm.stickTime = Time::currentTimeMillis();
		//tm.road = 4;
		//tm.iSe = 1;
		//tm.bSticked = true;
		//tm.EA = 1;			   	
		//m_stickInfos.push_back(tm);

	//}
	//m_sumEars = 1000;

	////////////////////////////////////////

    //[UserButtonCode_textButtonSpeed1] -- add your button handler code here..

	stickmarker->ClearLessMore();
  
    _objectLock.enter();

	for (int i = 0; i < 4; i++)
	{
		m_roadsSearch[i] = 0;
		_handmStick[i] = 0;
		_preCheckTime[i] = Time::currentTimeMillis();
		_preStickTime[i] = -1;
		//_curRunningLenPos[i] = -1.;
		//_preStickLenPos[i] = -1.;
		m_continue4StickEA[i] = -1;
		m_curEA[i] = -1;
	}

    m_preSentPos[0][0] = -1.;
    m_preSentPos[0][1] = -1.;

    m_sentCount[0] = 0;
    m_preSentPos[1][0] = -1.;
    m_preSentPos[1][1] = -1.;

    m_sentCount[1] = 0;

    //delete  g_log;
    //g_log = nullptr;

    String logFileSubDirectoryName = "LabelLog";
    String logFileName = "LabelLog";
    logFileName << Time::getCurrentTime().formatted("%Y-%m-%d_%H-%M-%S");
    String welcomeMessage = "Welcom log message for label";
    int64 maxInitialFileSizeBytes = 5 * 1024 * 1024;

    //g_log = new FileLogger(File::getCurrentWorkingDirectory().getChildFile(logFileSubDirectoryName)
    //    .getChildFile(logFileName),
    //    welcomeMessage, maxInitialFileSizeBytes);

	if (m_sumEars > 0)
	{
		Save2MonthReport();

		SaveReport();

	}
    CLabelDM[0]->NewRoll();
    CLabelDM[1]->NewRoll();


	if (m_stickInfos.size() > 0)
	{
		Time time = Time::getCurrentTime();
		char szFile[256] = { 0 };
		sprintf(szFile, "StickReport_%d_%d_%d_%d-%d-%d.csv", time.getYear(), time.getMonth() + 1, time.getDayOfMonth(), time.getHours(), time.getMinutes(), time.getSeconds());
		FILE* fp = fopen(szFile, "w");
		if (fp)
		{
			String szTitle = L"���ԭ��,����λ��,EA���,��·,���ʹ��ʱ��,ʵ�ʴ��ʱ��,�Ƿ���";
			char* pmbType;
			UnicodeToMB(pmbType, szTitle.toUTF16(), szTitle.length());
			fprintf(fp, "%s\n", pmbType);
			delete[] pmbType;

			for (int i = 0; i < m_stickInfos.size(); i++)
			{

				String strReason = ProduceDefectString(m_stickInfos[i].codeReason);
				UnicodeToMB(pmbType, strReason.toUTF16(), strReason.length());
				fprintf(fp, "%s,", pmbType);
				fprintf(fp, "%f,", m_stickInfos[i].lenPos);
				fprintf(fp, "%d,", m_stickInfos[i].EA);
				fprintf(fp, "%d,", m_stickInfos[i].roadUser);
				delete[] pmbType;

				char szTm[256] = { 0 };
				juce::Time time(m_stickInfos[i].sendTime);
				sprintf(szTm, "%d_%d_%d_%d:%d:%d", time.getYear(), time.getMonth() + 1, time.getDayOfMonth(), time.getHours(), time.getMinutes(), time.getSeconds());
				fprintf(fp, "%s,", szTm);

				memset(szTm, 0, 256);
				juce::Time time2(m_stickInfos[i].stickTime);
				sprintf(szTm, "%d_%d_%d_%d:%d:%d", time2.getYear(), time2.getMonth() + 1, time2.getDayOfMonth(), time2.getHours(), time2.getMinutes(), time2.getSeconds());
				fprintf(fp, "%s,", szTm);

				String stick;
				if (m_stickInfos[i].bSticked)
					stick = L"���";
				else
					stick = L"δ���";
				UnicodeToMB(pmbType, stick.toUTF16(), stick.length());
				fprintf(fp, "%s\n", pmbType);
				delete[] pmbType;

			}
			fclose(fp);
		}
	}
	if (m_bSupportStick_Remain)
	{
		if (m_stickInfoReserves.size() == 0) //��������й������¾���m_stickInfoReserves����
		{
			for (int i = 0; i < m_stickInfos.size(); i++) //ͣ��������¾�ʼ������Ĵ�·δ����������
			{
				if (m_stickInfos[i].bSticked || m_stickInfos[i].roadUser <= 0)
					continue;


				bool bSkip = false;
				for (int j = i + 1; j < m_stickInfos.size(); j++) //ͣ��������¾�ʼ������Ĵ�·δ����������
				{
					//�������һ���Ѿ����꣬��Ĵ����Ϣ��Ӧ�ô���
					if (m_stickInfos[j].bSticked && m_stickInfos[i].road == m_stickInfos[j].road &&  m_stickInfos[i].iSe == m_stickInfos[j].iSe)
					{
						bSkip = true;
						break;
					}
					if (m_stickInfos[j].bSticked && (m_stickInfos[j].lenPos - m_stickInfos[i].lenPos) > 4000) //4m�Ժ�����Ѿ�����ģ���ǰ���Ҳ��Ӧ�ó���
					{
						bSkip = true;
						break;
					}
				}
				if (bSkip)
					continue;

				if (_curRunningLenPos[m_stickInfos[i].roadUser - 1] < m_stickInfos[i].lenPos) //��Ч
				{
					m_stickInfos[i].lenPos = m_stickInfos[i].lenPos-_curRunningLenPos[m_stickInfos[i].roadUser - 1];
					m_stickInfoReserves.push_back(m_stickInfos[i]);
					m_stickInfos.erase(m_stickInfos.begin() + i);
					i--;

				}
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		_curRunningLenPos[i] = -1.;
		_preStickLenPos[i] = -1.;
	}

    m_stickInfos.clear();
    m_stickInfos.reserve(2000);
    m_sumEars = 0;//����

	Num_Mark[0] = 0;
	Num_Mark[1] = 0;
	Num_Mark[2] = 0;
	Num_Mark[3] = 0;

	if (m_bSupportStick_Remain)
	{
		for (int i = 0; i < m_stickInfoReserves.size(); i++) //δ������������Ҫ���뵽��Ӧ������
		{
			if (m_stickInfoReserves[i].bSticked)
				continue;

			bool bRet = CLabelDM[m_stickInfoReserves[i].road]->AddLabelData(m_stickInfoReserves[i].road, m_stickInfoReserves[i].iSe,
				m_stickInfoReserves[i].lenPos, m_stickInfoReserves[i].lenPos);
		
			if(bRet)
				m_stickInfos.push_back(m_stickInfoReserves[i]);
		}
		m_stickInfoReserves.clear();
		m_stickInfoReserves.reserve(100);

	}
	m_sent2ServerIndex = 0;
    _objectLock.exit();


}


//-------------------------------------------------------------------------------------------------------------------
// WriteCommData
WriteCommData::WriteCommData(int maxCapacity) : _maxCapacity(maxCapacity), Thread("WriteCommDataThread")
{
	_count = 0;
	juce::String logFileSubDirectoryName = "LabelLog";
	juce::String logFileName = "LabelLog";
	logFileName << juce::Time::getCurrentTime().formatted("%Y-%m-%d_%H-%M-%S");
	juce::String welcomeMessage = "Welcom log message for label";
	juce::int64 maxInitialFileSizeBytes = 5 * 1024 * 1024;


	p_log = new juce::FileLogger(juce::File::getCurrentWorkingDirectory().getChildFile(logFileSubDirectoryName)
		.getChildFile(logFileName),
		welcomeMessage, maxInitialFileSizeBytes);
}

WriteCommData::~WriteCommData()
{
	if (p_log != NULL)
	{
		delete p_log;
		p_log = nullptr;
	}

}

void WriteCommData::run()
{
	int n = 0;

	while (!threadShouldExit())
	{
		String data;
		bool ret = PickAData(data);
		if (!ret) // ������
		{
			sleep(500);
			continue;
		}
		if (p_log)
		{
			p_log->logMessage(data);

			if (_count > 50000)
			{
				delete p_log;

				String logFileSubDirectoryName = "LabelLog";
				String logFileName = "LabelLog";
				logFileName << Time::getCurrentTime().formatted("%Y-%m-%d_%H-%M-%S");
				String welcomeMessage = "Welcom log message for label";
				int64 maxInitialFileSizeBytes = 5 * 1024 * 1024;

				p_log = new FileLogger(File::getCurrentWorkingDirectory().getChildFile(logFileSubDirectoryName)
				    .getChildFile(logFileName),
				    welcomeMessage, maxInitialFileSizeBytes);

				_count = 0;
			}

		}

	}
}

void WriteCommData::Clear()
{
	juce::ScopedLock lock(_cs);
	_waitToWriteData.clear();
}

bool WriteCommData::PickAData(String& data)
{
	juce::ScopedLock lock(_cs);

	if (_waitToWriteData.size() < 1)
		return false;

	data = _waitToWriteData.front();
	_waitToWriteData.pop_front();
	return true;
}

bool WriteCommData::AddDebugData(const String& data)
{
	juce::ScopedLock lock(_cs);

	if (_waitToWriteData.size() == _maxCapacity)
		return false;

	_waitToWriteData.push_back(data);

	_count++;
	return true;
}


void MainComponent::timerCallback()
{
	ReconnectAll();

	juce::int64 curTime = Time::currentTimeMillis();
	for (int i = 0; i < 4; ++i)
	{
		juce::String label_text = L"����" + juce::String(i + 1) + L"����  " + juce::String(Num_Mark[i]);
		Label_MarkCount[i]->setText(label_text, juce::NotificationType::dontSendNotification);


		_preCheckTime[i] = Time::currentTimeMillis();
	}

	_objectLock.enter();
	if (m_mfClient.isConnected())
	{
		if (m_sent2ServerIndex < m_stickInfos.size())
		{
			StickMarkInfo sf = m_stickInfos[m_sent2ServerIndex];
			if (sf.bSticked)
			{
				m_mfClient.SendData(sf);
				m_sent2ServerIndex++;
			}
		}
	}
	_objectLock.exit();

	
	mute.enter();
	if (!g_stickNumCount.empty())
	{
		for (int i = 0; i < g_stickNumCount.size(); ++i)
		{
			m_mfClient.SendData(g_stickNumCount[i]);
		}
		g_stickNumCount.clear();
	}
	mute.exit();
	
}

//FeedBackSignal::FeedBackSignal() :Thread("")
//{
//    juce::String str = "";
//    fileptr = new juce::File(juce::File::getCurrentWorkingDirectory().getFullPathName() +"\\read feedback"+str+".txt");
//    stream = new juce::FileOutputStream(*fileptr);
//
//    num = 0;
//}
//
//FeedBackSignal::~FeedBackSignal()
//{
//
//}
//
//void FeedBackSignal::run()
//{
//    unsigned int data = 0;                     
//   /* *stream << juce::String("thread begin to run")<<"\n";
//    stream->flush();*/
//    while (!threadShouldExit())
//    {
//        bool ret=wait(-1);
//        if (ret) 
//        {
//            /**stream << juce::String("thread was waken up") << "\n";
//            stream->flush();*/
//            clock_t _interval = 0;
//            while (!data)
//            {
//                clock_t t1=clock();
//                Pci1230ReadDiBit(15, 8, &data);//��λ2
//                if(!data)
//                    Pci1230ReadDiBit(15, 10, &data);//��λ4
//                if(!data)
//                    Pci1230ReadDiBit(15, 12, &data);//��λ1
//                if(!data)
//                    Pci1230ReadDiBit(15, 14, &data);//��λ3
//                clock_t t2 = clock();
//                _interval += (t2 - t1);
//                if (_interval >= 2000)
//                    break;
//                
//            }
//
//            if (data)
//            {
//                num++;
//                juce::String str = juce::Time::getCurrentTime().formatted("%m/%d %H:%M:%S")+juce::String(L" ��ǰ�ѳɹ���ȡ:  ") + juce::String(num) + '\n';
//                *stream << str;
//                stream->flush();
//            }
//            else
//            {
//                Pci1230WriteDoBit(15, 1, 1);
//                sleep(100);
//                Pci1230WriteDoBit(15, 1, 0);
//                juce::String str = juce::Time::getCurrentTime().formatted("%m/%d %H:%M:%S")+ juce::String(L"  �����źŶ�ȡ��ʱ") + '\n';
//                *stream << str;
//                stream->flush();
//                juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::WarningIcon, "", L"  ���������쳣");
//            }
//        }
//    }
//}
//
//StickDataSend::StickDataSend()
//{
//    memset(&ws_info, 0, sizeof(WS_INFO));
//    ws_info._type = 8;
//    bool ret=connectToSocket("127.0.0.1",9000,2000);
//    if (!ret)
//       juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::WarningIcon, "", L"       ����������ʧ��");
//    filestream = new juce::FileOutputStream(juce::File::getCurrentWorkingDirectory().getFullPathName() + "\\communicatelog.txt");
//    send_index = 0;
//}
//
//StickDataSend::~StickDataSend()
//{
//
//}
//
//void StickDataSend::connectionMade()
//{
//
//}
//
//void StickDataSend::connectionLost()
//{
//
//}
//
//void StickDataSend::messageReceived(const MemoryBlock& message)
//{
//
//}
//
//
//bool StickDataSend::SendData(std::vector<StickMarkInfo>& v)
//{
//    if (!isConnected())
//    {
//        return false;
//    }
//    
//    if (send_index == v.size() - 1)
//        return false;
//    unsigned int numofmsg = 0;
//    int _vsize = v.size();
//    for (int i = send_index; i <_vsize; ++i)
//    {
//        MemoryBlock DataToSend(&ws_info, sizeof(WS_INFO));
//        _StickMarkInfo _stickmarkinfo;
//        _stickmarkinfo.bSticked = v[i].bSticked;
//        _stickmarkinfo.codeReason = v[i].codeReason;
//        _stickmarkinfo.EA = v[i].EA;
//        memcpy(_stickmarkinfo.ip, v[i].ip.toStdString().c_str(),v[i].ip.length());
//        _stickmarkinfo.iSe = v[i].iSe;
//        _stickmarkinfo.lenPos = v[i].lenPos;
//        _stickmarkinfo.road = v[i].road;
//        _stickmarkinfo.roadUser = v[i].roadUser;
//        _stickmarkinfo.sendTime = v[i].sendTime;
//        _stickmarkinfo.stickTime = v[i].stickTime;
//        DataToSend.append(&_stickmarkinfo, sizeof(_StickMarkInfo));
//        bool ret=sendMessage(DataToSend);
//        if (ret)
//            ++numofmsg;
//
//        send_index = i;
//    }
//
//    
//
//    *filestream << juce::Time::getCurrentTime().formatted("%Y/%m/%d %H:%M:%S")+L"���ʹ����Ϣ"<<juce::String(numofmsg)<< '\n';
//    filestream->flush();
//    return true;
//}
//
//void StickDataSend::clear_index()
//{
//    send_index = 0;
//}
//
//
//void FeedBackSignal::compare(int* NumMark)
//{
//    int totalmark = 0;
//    for (int i = 0; i < 4; ++i)
//        totalmark += NumMark[i];
//    *stream << "Signals Sent Num:  " << totalmark << '\n';
//    *stream << "Signals Received Num:  " << num << '\n';
//    stream->flush();
//    if (num == totalmark)
//    {
//        *stream << "SIGNALS SENT MATCH SIGNALS RECEIVED!" << '\n';
//        stream->flush();
//    }
//    else
//    {
//        *stream << "SIGNALS SNET DISMATCH SIGNAS RECEIVED!" << '\n';
//        stream->flush();
//    }
//    
//    num = 0;
//    *stream << '\n';
//    *stream << "-------                    New Roll                       -------" << '\n';
//    *stream << "*****************************************************************" << '\n';
//    stream->flush();
//}