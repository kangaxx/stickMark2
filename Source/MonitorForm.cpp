/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.7

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include <windows.h>

//[/Headers]

#include "MonitorForm.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
WriteCommData* g_pWriteThread_codeTrans = nullptr;
double _curRunningLenPos_codeTrans[4] = { -1.,-1.,-1.,-1. };
double _preStickLenPos_codeTrans[4];
extern bool _threadShouldExit;
bool UnicodeToMB_CodeTrans(char*& pmb, const wchar_t* pun, int uLen)
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

String ProduceDefectString_CodeTrans(int code)
{
	String str = "";
	if (code & 0x200)
		str += String(juce::CharPointer_UTF8("\xe6\xbc\x8f\xe9\x87\x91\xe5\xb1\x9e"));
	if (code & 0x400)
		str += String(juce::CharPointer_UTF8("\xe6\x9d\xa1\xe7\x97\x95"));
	if (code & 0x800)
		str += String(juce::CharPointer_UTF8("\xe8\x84\xb1\xe7\xa2\xb3"));
	if (code & 0x1000)
		str += String(juce::CharPointer_UTF8("\xe6\xb0\x94\xe6\xb3\xa1"));
	if (code & 0x2000)
		str += String(juce::CharPointer_UTF8("\xe6\xb0\x94\xe6\xb3\xa1"));
	if (code & 0x4000)
		str += String(juce::CharPointer_UTF8("\xe7\x99\xbd\xe7\x82\xb9"));
	if (code & 0x8000)
		str += String(juce::CharPointer_UTF8("\xe9\xbb\x91\xe7\x82\xb9"));
	if (code & 0x400000)
		str += String(juce::CharPointer_UTF8("\xe6\x9e\x81\xe7\x89\x87\xe7\xa0\xb4\xe6\x8d\x9f"));
	if (code & 0x800000)
		str += String(juce::CharPointer_UTF8("AT9\xe6\xbc\x8f\xe9\x87\x91\xe5\xb1\x9e"));
	if (code & 0x1000000)
		str += String(juce::CharPointer_UTF8("\xe6\x9c\xaa\xe8\xbe\x8a\xe5\x8e\x8b"));
	if (code & 0x2000000)
		str += String(juce::CharPointer_UTF8("\xe8\xbe\xb9\xe7\xbc\x98\xe6\xbc\x8f\xe9\x87\x91\xe5\xb1\x9e"));
	if (code & 0x4000000)
		str += String(juce::CharPointer_UTF8("\xe6\x9e\x81\xe8\x80\xb3\xe7\xa0\xb4\xe6\x8d\x9f"));
	if (code & 0x4000000)
		str += String(juce::CharPointer_UTF8("\xe6\x9e\x81\xe8\x80\xb3\xe7\xa0\xb4\xe6\x8d\x9f"));
	if (code & 0x8000000)
		str += String(juce::CharPointer_UTF8("\xe6\x9e\x81\xe8\x80\xb3\xe4\xbd\x99\xe6\x96\x99"));
	if (code & 0x10000000)
		str += String(juce::CharPointer_UTF8("\xe6\x9a\x97\xe7\x97\x95"));
	if (code & 0x20000000)
		str += String(juce::CharPointer_UTF8("\xe6\x9c\xaa\xe6\xb6\x82\xe8\x86\x9c"));
	if (code & 0x40000000)
		str += String(juce::CharPointer_UTF8("\xe7\x9b\xb4\xe8\xbe\xb9\xe4\xbd\x99\xe6\x96\x99"));
	if (code & 0x100)
		str += String(juce::CharPointer_UTF8("\xe5\x88\x86\xe6\x9d\xa1\xe5\xae\xbd\xe5\xba\xa6"));
	if (code & 0x4 || code & 0x8 || code & 0x20000 || code & 0x40000 || code & 0x100000 || code & 0x200000) // 101   102
		str += String(juce::CharPointer_UTF8("\xe6\x9e\x81\xe8\x80\xb3\xe5\xae\x8c\xe6\x95\xb4\xe5\xba\xa6"));
	//if (code & 0x20000 || code & 0x40000) // 109	 110
	//	str += String("极耳高度";
	//if (code & 0x100000 || code & 0x200000) // 112
	//	str += String("极耳缺失NG";
	if (code & 0x10 || code & 0x20) // 103	  104
		str += String(juce::CharPointer_UTF8("\xe6\x9e\x81\xe8\x80\xb3\xe9\x97\xb4\xe8\xb7\x9d"));
	if (code & 0x40 || code & 0x80 || code & 0x10000) // 105	 106
		str += String(juce::CharPointer_UTF8("AT9\xe5\xae\xbd\xe5\xba\xa6"));
	//if (code & 0x10000) // 108
	//	str += String("AT9宽度差NG";
	if (code & 0x80000) // 111
		str += String(juce::CharPointer_UTF8("\xe6\x80\xbb\xe8\x86\x9c\xe9\x95\xbf"));

	if (str.isEmpty())
		str << code;

	return str;
}
//[/MiscUserDefs]

//==============================================================================
MonitorForm::MonitorForm ()
{
    //[Constructor_pre] You can add your own custom stuff here..
	CLabelDM[0] = new CLabelerDataManager();
	CLabelDM[1] = new CLabelerDataManager();
	g_pWriteThread_codeTrans = new WriteCommData();
	g_pWriteThread_codeTrans->startThread();
    //[/Constructor_pre]

    btnConfigure.reset (new juce::TextButton ("btnConfigure"));
    addAndMakeVisible (btnConfigure.get());
    btnConfigure->setButtonText (juce::CharPointer_UTF8 ("\xe5\x88\x9d\xe5\xa7\x8b\xe8\xae\xbe\xe7\xbd\xae"));
    btnConfigure->addListener (this);

    btnConfigure->setBounds (8, 8, 136, 24);

    btnWarning.reset (new juce::TextButton ("btnWarning"));
    addAndMakeVisible (btnWarning.get());
    btnWarning->setButtonText (juce::CharPointer_UTF8 ("\xe6\x8a\xa5\xe8\xad\xa6\xe9\x87\x8d\xe7\xbd\xae"));
    btnWarning->addListener (this);

    btnWarning->setBounds (144, 8, 136, 24);

    btnClearNum.reset (new juce::TextButton ("btnClearNum"));
    addAndMakeVisible (btnClearNum.get());
    btnClearNum->setButtonText (juce::CharPointer_UTF8 ("\xe8\xae\xa1\xe6\x95\xb0\xe6\xb8\x85\xe9\x9b\xb6"));
    btnClearNum->addListener (this);

    btnClearNum->setBounds (280, 8, 136, 24);

    btnReset.reset (new juce::TextButton ("btnReset"));
    addAndMakeVisible (btnReset.get());
    btnReset->setButtonText (juce::CharPointer_UTF8 ("\xe4\xb8\x80\xe9\x94\xae\xe5\xa4\x8d\xe4\xbd\x8d"));
    btnReset->addListener (this);

    btnReset->setBounds (416, 8, 136, 24);

    btnHelp.reset (new juce::TextButton ("btnHelp"));
    addAndMakeVisible (btnHelp.get());
    btnHelp->setButtonText (juce::CharPointer_UTF8 ("\xe5\xb8\xae\xe5\x8a\xa9"));
    btnHelp->addListener (this);

    btnHelp->setBounds (552, 8, 136, 24);

    lblPlc.reset (new juce::Label ("lblPlc",
                                   juce::CharPointer_UTF8 ("PLC\xe8\xbf\x9e\xe6\x8e\xa5\xe7\x8a\xb6\xe6\x80\x81")));
    addAndMakeVisible (lblPlc.get());
    lblPlc->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblPlc->setJustificationType (juce::Justification::centredLeft);
    lblPlc->setEditable (false, false, false);
    lblPlc->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblPlc->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblPlc->setBounds (296, 48, 104, 24);

    btnIp.reset (new juce::ImageButton ("btnIp"));
    addAndMakeVisible (btnIp.get());
    btnIp->setButtonText (juce::String());
    btnIp->addListener (this);

    btnIp->setImages (false, true, true,
                      juce::Image(), 1.000f, juce::Colour (0x00000000),
                      juce::Image(), 1.000f, juce::Colour (0x00000000),
                      juce::Image(), 1.000f, juce::Colour (0x00000000));
    btnIp->setBounds (536, 48, 32, 24);

    lblIp.reset (new juce::Label ("lblIp",
                                  TRANS("IP")));
    addAndMakeVisible (lblIp.get());
    lblIp->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblIp->setJustificationType (juce::Justification::centredLeft);
    lblIp->setEditable (false, false, false);
    lblIp->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblIp->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblIp->setBounds (496, 48, 32, 24);

    btnPort.reset (new juce::ImageButton ("btnPort"));
    addAndMakeVisible (btnPort.get());
    btnPort->setButtonText (juce::String());
    btnPort->addListener (this);

    btnPort->setImages (false, true, true,
                        juce::Image(), 1.000f, juce::Colour (0x00000000),
                        juce::Image(), 1.000f, juce::Colour (0x00000000),
                        juce::Image(), 1.000f, juce::Colour (0x00000000));
    btnPort->setBounds (608, 48, 32, 24);

    lblPort.reset (new juce::Label ("lblPort",
                                    TRANS("PORT")));
    addAndMakeVisible (lblPort.get());
    lblPort->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblPort->setJustificationType (juce::Justification::centredLeft);
    lblPort->setEditable (false, false, false);
    lblPort->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblPort->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblPort->setBounds (568, 48, 32, 24);

    lblWarnNum.reset (new juce::Label ("lblWarnNum",
                                       juce::CharPointer_UTF8 ("\xe8\xb4\xb4\xe6\xa0\x87\xe6\x8a\xa5\xe8\xad\xa6\xe6\x95\xb0\xe7\x9b\xae")));
    addAndMakeVisible (lblWarnNum.get());
    lblWarnNum->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblWarnNum->setJustificationType (juce::Justification::centredLeft);
    lblWarnNum->setEditable (false, false, false);
    lblWarnNum->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblWarnNum->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblWarnNum->setBounds (640, 48, 112, 24);

    txtWarnNum.reset (new juce::TextEditor ("txtWarnNum"));
    addAndMakeVisible (txtWarnNum.get());
    txtWarnNum->setMultiLine (false);
    txtWarnNum->setReturnKeyStartsNewLine (false);
    txtWarnNum->setReadOnly (false);
    txtWarnNum->setScrollbarsShown (true);
    txtWarnNum->setCaretVisible (true);
    txtWarnNum->setPopupMenuEnabled (true);
    txtWarnNum->setText (TRANS("0"));

    btnSave.reset (new juce::TextButton ("btnSave"));
    addAndMakeVisible (btnSave.get());
    btnSave->setButtonText (juce::CharPointer_UTF8 ("\xe4\xbf\x9d\xe5\xad\x98"));
    btnSave->addListener (this);

    lblPlc2.reset (new juce::Label ("lblPlc",
                                    juce::CharPointer_UTF8 ("\xe8\xb4\xb4\xe6\xa0\x87\xe4\xbf\xa1\xe6\x81\xaf")));
    addAndMakeVisible (lblPlc2.get());
    lblPlc2->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblPlc2->setJustificationType (juce::Justification::centredLeft);
    lblPlc2->setEditable (false, false, false);
    lblPlc2->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblPlc2->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblPlc2->setBounds (8, 48, 80, 24);

    txtWarnNum2.reset (new juce::TextEditor ("txtWarnNum"));
    addAndMakeVisible (txtWarnNum2.get());
    txtWarnNum2->setMultiLine (false);
    txtWarnNum2->setReturnKeyStartsNewLine (false);
    txtWarnNum2->setReadOnly (false);
    txtWarnNum2->setScrollbarsShown (true);
    txtWarnNum2->setCaretVisible (true);
    txtWarnNum2->setPopupMenuEnabled (true);
    txtWarnNum2->setText (TRANS("0"));

    txtWarnNum2->setBounds (80, 48, 88, 24);

    txtPlcStatus.reset (new juce::TextEditor ("new text editor"));
    addAndMakeVisible (txtPlcStatus.get());
    txtPlcStatus->setMultiLine (false);
    txtPlcStatus->setReturnKeyStartsNewLine (false);
    txtPlcStatus->setReadOnly (false);
    txtPlcStatus->setScrollbarsShown (true);
    txtPlcStatus->setCaretVisible (true);
    txtPlcStatus->setPopupMenuEnabled (true);
    txtPlcStatus->setColour (juce::TextEditor::shadowColourId, juce::Colours::yellow);
    txtPlcStatus->setText (juce::CharPointer_UTF8 ("\xe6\x9c\xaa\xe8\xbf\x9e\xe6\x8e\xa5"));

    txtPlcStatus->setBounds (400, 48, 95, 24);

    btnPlcConnect.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (btnPlcConnect.get());
    btnPlcConnect->setButtonText (juce::CharPointer_UTF8 ("\xe8\xbf\x9e\xe6\x8e\xa5PLC"));
    btnPlcConnect->addListener (this);
    btnPlcConnect->setColour (juce::TextButton::buttonColourId, juce::Colours::red);

    btnPlcConnect->setBounds (192, 48, 95, 24);


    //[UserPreSize]



	gridMain.reset(new tzGrid());
	gridMain->setBackgroudColour(juce::Colour(0xff323e44));
	gridMain->setRowHeight(80.0);
	gridMain->addColoumn(juce::CharPointer_UTF8("\xe5\x90\x8d\xe7\xa7\xb0"), 120.0);
	gridMain->addColoumn(juce::CharPointer_UTF8("\xe5\xbc\x80\xe5\x85\xb3"), 80.0, tzGrid::columnComponentType::switchBox, tzGrid::columnDataType::nullColumn);
	gridMain->addColoumn(juce::CharPointer_UTF8("\xe7\x8a\xb6\xe6\x80\x81"), 160.0);
	gridMain->addColoumn(juce::CharPointer_UTF8("\xe8\xad\xa6\xe5\x91\x8a\xe4\xbf\xa1\xe6\x81\xaf"), 280.0);
	gridMain->addColoumn(juce::CharPointer_UTF8("\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba\xe4\xb8\xaa\xe6\x95\xb0"), 100.0);
	gridMain->addColoumn(juce::CharPointer_UTF8("\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba\xe4\xbf\xa1\xe5\x8f\xb7\xe6\x95\xb0"), 100.0);
	gridMain->addColoumn(juce::CharPointer_UTF8("\xe6\x84\x9f\xe5\xba\x94\xe6\xa0\x87\xe7\xad\xbe\xe6\x95\xb0"), 100.0);
	gridMain->setRowNum(4);
	//添加行数据

	dataModels = new DuLink<gridDataInfo*>();
	for (int i = 0; i < MAX_MARKER_NUM; ++i) {
		gridDataInfo* tmpData = new gridDataInfo();
		dataModels->insertToLast(tmpData);
	}

	//第一行数据
	((gridDataInfo*)(*dataModels)[0])->setName(juce::CharPointer_UTF8("\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba 01"));
	((gridDataInfo*)(*dataModels)[0])->setCommand("");
	((gridDataInfo*)(*dataModels)[0])->setStatus(juce::CharPointer_UTF8("\xe5\x85\xb3\xe6\x9c\xba"));
	((gridDataInfo*)(*dataModels)[0])->setWarning(juce::CharPointer_UTF8("\xe6\x97\xa0"));
	((gridDataInfo*)(*dataModels)[0])->setMarkNum(0);
	((gridDataInfo*)(*dataModels)[0])->setMarkSignalNum(0);
	((gridDataInfo*)(*dataModels)[0])->setSensorNum(0);

	//第二行数据
	((gridDataInfo*)(*dataModels)[1])->setName(juce::CharPointer_UTF8("\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba 02"));
	((gridDataInfo*)(*dataModels)[1])->setCommand("");
	((gridDataInfo*)(*dataModels)[1])->setStatus(juce::CharPointer_UTF8("\xe5\x85\xb3\xe6\x9c\xba"));
	((gridDataInfo*)(*dataModels)[1])->setWarning(juce::CharPointer_UTF8("\xe6\x97\xa0"));
	((gridDataInfo*)(*dataModels)[1])->setMarkNum(0);
	((gridDataInfo*)(*dataModels)[1])->setMarkSignalNum(0);
	((gridDataInfo*)(*dataModels)[1])->setSensorNum(0);

	//第三行数据
	((gridDataInfo*)(*dataModels)[2])->setName(juce::CharPointer_UTF8("\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba 03"));
	((gridDataInfo*)(*dataModels)[2])->setCommand("");
	((gridDataInfo*)(*dataModels)[2])->setStatus(juce::CharPointer_UTF8("\xe5\x85\xb3\xe6\x9c\xba"));
	((gridDataInfo*)(*dataModels)[2])->setWarning(juce::CharPointer_UTF8("\xe6\x97\xa0"));
	((gridDataInfo*)(*dataModels)[2])->setMarkNum(0);
	((gridDataInfo*)(*dataModels)[2])->setMarkSignalNum(0);
	((gridDataInfo*)(*dataModels)[2])->setSensorNum(0);

	//第四行数据
	((gridDataInfo*)(*dataModels)[3])->setName(juce::CharPointer_UTF8("\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba 04"));
	((gridDataInfo*)(*dataModels)[3])->setCommand("");
	((gridDataInfo*)(*dataModels)[3])->setStatus(juce::CharPointer_UTF8("\xe5\x85\xb3\xe6\x9c\xba"));
	((gridDataInfo*)(*dataModels)[3])->setWarning(juce::CharPointer_UTF8("\xe6\x97\xa0"));
	((gridDataInfo*)(*dataModels)[3])->setMarkNum(0);
	((gridDataInfo*)(*dataModels)[3])->setMarkSignalNum(0);
	((gridDataInfo*)(*dataModels)[3])->setSensorNum(0);

	for (int i = 0; i < dataModels->size(); i++) {
		rowData.add(((gridDataInfo*)(*dataModels)[i])->getName());
		rowData.add(((gridDataInfo*)(*dataModels)[i])->getCommand());
		rowData.add(((gridDataInfo*)(*dataModels)[i])->getStatus());
		rowData.add(((gridDataInfo*)(*dataModels)[i])->getWarning());
		rowData.add(((gridDataInfo*)(*dataModels)[i])->getMarkNum());
		rowData.add(((gridDataInfo*)(*dataModels)[i])->getMarkSignalNum());
		rowData.add(((gridDataInfo*)(*dataModels)[i])->getSensorNum());
	}

	gridMain->addRowData(rowData); //所有数据一起传输

	addAndMakeVisible(gridMain.get());

    //[/UserPreSize]

    setSize (830, 500);


    //[Constructor] You can add your own custom stuff here..

	configureForm = new ConfigureForm();
	configureForm->setFunctionSS(MakeDelegate(this, &MonitorForm::resetPlc));
	gridMain->setFunctionII(MakeDelegate(this, &MonitorForm::switchMarker));
	gridMain->setBounds(8, 100, 814 - 830 + getWidth(), 360);
	txtWarnNum->setBounds(760, 48, getWidth() - 770, 24);
	btnSave->setBounds(688, 8, getWidth() - 695, 24);

	String strCommuIP = "192.168.1.201";
	int ret2 = m_configFile.GetCommuServerAddr(strCommuIP);
	if (ret2 == 0)
	{
		AlertWindow::showMessageBox(AlertWindow::WarningIcon, juce::CharPointer_UTF8("\xe9\x85\x8d\xe7\xbd\xae\xe9\xa1\xb9\xe7\xbc\xba\xe9\x80\x9a\xe8\xae\xaf\xe6\x9c\x8d\xe5\x8a\xa1\xe5\x99\xa8IP"), L"");
	}
	_strCommuIP = strCommuIP;
	LOGWT("通讯服务器IP地址：%s", strCommuIP.toStdString().c_str());

	String strPLCIP = "192.168.1.80";
	ret2 = m_configFile.GetPLCServerAddr(strPLCIP);
	if (ret2 == 0)
	{
		AlertWindow::showMessageBox(AlertWindow::WarningIcon, juce::CharPointer_UTF8("\xe9\x85\x8d\xe7\xbd\xae\xe9\xa1\xb9\xe7\xbc\xba\xe6\x89\x93\xe6\xa0\x87\xe6\x9c\xbaIP"), L"");
	}
	LOGWT("打标机PLC IP地址：%s", strPLCIP.toStdString().c_str());

	int iSupportC5 = 0;
	m_configFile.GetContiC5Stop(iSupportC5);
	m_iSupportCM5 = iSupportC5;// (iSupportC5 >= 0) ? true : false;

	bool ret = client.connectToSocket(strCommuIP, 600, 1000);
	if (!ret)
		juce::AlertWindow::showMessageBox(juce::AlertWindow::WarningIcon, juce::CharPointer_UTF8("\xe8\xbf\x9e\xe6\x8e\xa5\xe9\x80\x9a\xe8\xae\xaf\xe6\x9c\x8d\xe5\x8a\xa1\xe5\x99\xa8\xe5\xa4\xb1\xe8\xb4\xa5\xef\xbc\x8c\xe7\xa1\xae\xe8\xae\xa4\xe7\xbd\x91\xe7\xbb\x9c\xe9\x80\x9a\xe8\xae\xaf\xe6\x98\xaf\xe5\x90\xa6\xe6\xad\xa3\xe5\xb8\xb8"), "");
    //[/Constructor]
}

MonitorForm::~MonitorForm()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	_threadShouldExit = true;
	juce::Thread::sleep(1000);
	if (CLabelDM[0] != NULL)
	{
		delete CLabelDM[0];
		CLabelDM[0] = nullptr;
	}

	if (CLabelDM[1] != NULL)
	{
		delete CLabelDM[1];
		CLabelDM[1] = nullptr;
	}

	if (g_pWriteThread_codeTrans != NULL)
	{
		g_pWriteThread_codeTrans->signalThreadShouldExit();
		g_pWriteThread_codeTrans->stopThread(1000);
		delete g_pWriteThread_codeTrans;
		g_pWriteThread_codeTrans = nullptr;
	}
    //[/Destructor_pre]

    btnConfigure = nullptr;
    btnWarning = nullptr;
    btnClearNum = nullptr;
    btnReset = nullptr;
    btnHelp = nullptr;
    lblPlc = nullptr;
    btnIp = nullptr;
    lblIp = nullptr;
    btnPort = nullptr;
    lblPort = nullptr;
    lblWarnNum = nullptr;
    txtWarnNum = nullptr;
    btnSave = nullptr;
    lblPlc2 = nullptr;
    txtWarnNum2 = nullptr;
    txtPlcStatus = nullptr;
    btnPlcConnect = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
	delete configureForm;
	configureForm = nullptr;
	if (dataModels != NULL) {
		delete dataModels;
	}
    //[/Destructor]
}

//==============================================================================
void MonitorForm::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MonitorForm::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    txtWarnNum->setBounds (760, 48, getWidth() - 911, 24);
    btnSave->setBounds (688, 8, getWidth() - 836, 24);
    //[UserResized] Add your own custom resize handling here..
	txtWarnNum->setBounds(760, 48, getWidth() - 770, 24);
	btnSave->setBounds(688, 8, getWidth() - 695, 24);
	gridMain->setBounds(8, 100, 814 - 830 + getWidth(), 360);
    //[/UserResized]
}

void MonitorForm::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnConfigure.get())
    {
        //[UserButtonCode_btnConfigure] -- add your button handler code here..
		juce::DialogWindow::showDialog("", configureForm, this, juce::Colours::white, true);
        //[/UserButtonCode_btnConfigure]
    }
    else if (buttonThatWasClicked == btnWarning.get())
    {
        //[UserButtonCode_btnWarning] -- add your button handler code here..
        //[/UserButtonCode_btnWarning]
    }
    else if (buttonThatWasClicked == btnClearNum.get())
    {
        //[UserButtonCode_btnClearNum] -- add your button handler code here..
        //[/UserButtonCode_btnClearNum]
    }
    else if (buttonThatWasClicked == btnReset.get())
    {
        //[UserButtonCode_btnReset] -- add your button handler code here..
        //[/UserButtonCode_btnReset]
    }
    else if (buttonThatWasClicked == btnHelp.get())
    {
        //[UserButtonCode_btnHelp] -- add your button handler code here..
        //[/UserButtonCode_btnHelp]
    }
    else if (buttonThatWasClicked == btnIp.get())
    {
        //[UserButtonCode_btnIp] -- add your button handler code here..
        //[/UserButtonCode_btnIp]
    }
    else if (buttonThatWasClicked == btnPort.get())
    {
        //[UserButtonCode_btnPort] -- add your button handler code here..
        //[/UserButtonCode_btnPort]
    }
    else if (buttonThatWasClicked == btnSave.get())
    {
        //[UserButtonCode_btnSave] -- add your button handler code here..
		//insertWarning(0, String("new warning;"));
		//redrawGrid();
        //[/UserButtonCode_btnSave]
    }
    else if (buttonThatWasClicked == btnPlcConnect.get())
    {
        //[UserButtonCode_btnPlcConnect] -- add your button handler code here..
        //[/UserButtonCode_btnPlcConnect]
    }

    //[UserbuttonClicked_Post]

    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void MonitorForm::ReconnectAll()
{
	bool ret = true;
	if (!client.isConnected())
		ret = client.connectToSocket(_strCommuIP, 600, 1000);
	if (!ret)
	{
		juce::int64 curT = juce::Time::currentTimeMillis();
		if (curT - _preReportTime > 10000)
		{
			m_mfClient.SendMsgData(m_bNewUI, L"连接通讯服务器失败，确认网络通讯是否正常");
			_preReportTime = curT;
		}
	}
}
void MonitorForm::insertWarning(int MarkerIndex, String warning)
{
	((gridDataInfo*)(*dataModels)[MarkerIndex])->insertWarning(warning);
}

void MonitorForm::updateSwitchStatus(int MarkerIndex, String status)
{
	((gridDataInfo*)(*dataModels)[MarkerIndex])->setStatus(status);
}

void MonitorForm::redrawGrid()
{
	rowData.clear();
	for (int i = 0; i < dataModels->size(); i++) {
		rowData.add(((gridDataInfo*)(*dataModels)[i])->getName());
		rowData.add(((gridDataInfo*)(*dataModels)[i])->getCommand());
		rowData.add(((gridDataInfo*)(*dataModels)[i])->getStatus());
		rowData.add(((gridDataInfo*)(*dataModels)[i])->getWarning());
		rowData.add(((gridDataInfo*)(*dataModels)[i])->getMarkNum());
		rowData.add(((gridDataInfo*)(*dataModels)[i])->getMarkSignalNum());
		rowData.add(((gridDataInfo*)(*dataModels)[i])->getSensorNum());
	}

	gridMain->addRowData(rowData);
	gridMain->resized();
}

void MonitorForm::ResetAndNewRoll(String rollName)
{
	LOGWT("收到膜卷号:%s", rollName.toStdString().c_str());
	m_rollName = rollName;
	//复位打标
	bool ret2 = stickmarker->SendCmd(4204, 1);

	CreateNewRoll();

	CTLMARK_PG data;
	data.code = 10000;
	data.valid = false;
	sever.SendData(data);

	//新卷清零
	stickmarker->SendCmd(4204, 0);
}

bool MonitorForm::SendCmd(int addr, int val)
{
	_cs.enter();
	COMMAND_WRITE data;
	data.flag = val;
	data.regAddr = addr;
	_cmds.push_back(data);
	_cs.exit();
	return true;

}

void MonitorForm::HandleClientMessage(juce::String host, HDataMessage *pMessage)
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
	//喷码测试
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

	//上带路 打标机

	if (iRoad >= 0 && iRoad < 2)
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
			//AddLabelData内部已经加锁

			bool ret = CLabelDM[iRoad]->AddLabelData(pMessage->data.iRoad, pMessage->data.iSel, pMessage->data.srcPos, pMessage->data.destPos);



			_objectLock.enter();
			if (ret)
			{
				m_stickInfos.push_back(tm);
			}
			//需要确认问题

			if (g_pWriteThread_codeTrans != nullptr && ret)
			{
				char szTm[256] = { 0 };
				juce::Time time = juce::Time::getCurrentTime();
				int index = sprintf(szTm, "%d_%d_%d_%d:%d:%d", time.getYear(), time.getMonth() + 1, time.getDayOfMonth(), time.getHours(), time.getMinutes(), time.getSeconds());
				sprintf(&szTm[index], " Succ add label from%s: srcPos=%f,iRoad=%d, iSel=%d, destPos=%f\n", host.toStdString().c_str(), pMessage->data.srcPos, iRoad, pMessage->data.iSel, pMessage->data.destPos);
				g_pWriteThread_codeTrans->AddDebugData(szTm);
			}
			else if (g_pWriteThread_codeTrans != nullptr && !ret)
			{
				char szTm[256] = { 0 };
				juce::Time time = juce::Time::getCurrentTime();
				int index = sprintf(szTm, "%d_%d_%d_%d:%d:%d", time.getYear(), time.getMonth() + 1, time.getDayOfMonth(), time.getHours(), time.getMinutes(), time.getSeconds());
				sprintf(&szTm[index], "Fail add label from%s: iRoad=%d, iSel=%d, destPos=%f\n", host.toStdString().c_str(), iRoad, pMessage->data.iSel, pMessage->data.destPos);
				g_pWriteThread_codeTrans->AddDebugData(szTm);

			}
			_objectLock.exit();

		}
		else
		{
			pMessage->data.srcPos /= 10.;
			double dSpeed = client.GetMachineSpeed();
			double destPos = 0.;

			_objectLock.enter();

			// 如果设备运行中新卷了

			if (m_bSupportStick_Remain && pMessage->data.srcPos < 600 && (_curRunningLenPos_codeTrans[tm.roadUser - 1] - pMessage->data.srcPos) >= 1000.)
			{
				//停机情况下新卷开始，保存改带路未被打标的数据

				for (int i = 0; i < m_stickInfos.size(); i++)
				{
					if (m_stickInfos[i].bSticked || m_stickInfos[i].roadUser <= 0)
						continue;
					bool bSkip = false;
					//停机情况下新卷开始，保存改带路未被打标的数据

					for (int j = i + 1; j < m_stickInfos.size(); j++)
					{
						//如果后面一个已经出标，则改打标信息不应该存在

						if (m_stickInfos[j].bSticked && m_stickInfos[i].road == m_stickInfos[j].road &&  m_stickInfos[i].iSe == m_stickInfos[j].iSe)
						{
							bSkip = true;
							break;
						}
						if (m_stickInfos[j].bSticked && (m_stickInfos[j].lenPos - m_stickInfos[i].lenPos) >= m_EASumLen * 0.5) //4m以后出现已经出标的，则前面的也不应该出现
						{
							bSkip = true;
							break;
						}
					}
					if (bSkip)
						continue;

					if (_curRunningLenPos_codeTrans[m_stickInfos[i].roadUser - 1] < m_stickInfos[i].lenPos) //有效
					{
						m_stickInfos[i].lenPos = m_stickInfos[i].lenPos - _curRunningLenPos_codeTrans[m_stickInfos[i].roadUser - 1];
						m_stickInfoReserves.push_back(m_stickInfos[i]);
						m_stickInfos.erase(m_stickInfos.begin() + i);
						i--;
					}
				}
			}

			_curRunningLenPos_codeTrans[tm.roadUser - 1] = pMessage->data.srcPos;

			if (/*g_iUsePLCSpeed &&*/ dSpeed > 5.)
			{
				juce::int64 diffT = juce::Time::getCurrentTime().toMilliseconds() - client.m_recTime;
				if (diffT < 0) diffT = -diffT;

				if (diffT < 5000)
					pMessage->data.speed = dSpeed;
			}



			if (false && g_pWriteThread_codeTrans)
			{
				char szTm[256] = { 0 };
				juce::Time time = juce::Time::getCurrentTime();
				int index = sprintf(szTm, "%d_%d_%d_%d:%d:%d", time.getYear(), time.getMonth() + 1, time.getDayOfMonth(), time.getHours(), time.getMinutes(), time.getSeconds());
				sprintf(&szTm[index], " ***position  from %s***: iRoad=%d,  iSel=%d, curPos=%f\n", host.toStdString().c_str(), iRoad, pMessage->data.iSel, pMessage->data.srcPos);
				g_pWriteThread_codeTrans->AddDebugData(szTm);
			}

			m_sumEars = (juce::int64)pMessage->data.destPos;
			m_curEA[tm.roadUser - 1] = m_sumEars;


			bool bValid = true;
			if (m_curEA[tm.roadUser - 1] > 0 && m_curEA[tm.roadUser - 1] == m_continue4StickEA[tm.roadUser - 1])
			{
				bValid = false;
			}

			bool bSticked = false;
			if (bValid)
				bSticked = CLabelDM[iRoad]->RunningLength(pMessage->data.iRoad, pMessage->data.iSel, pMessage->data.speed, pMessage->data.srcPos, destPos);
			if (bSticked)
			{
				Num_Mark[tm.roadUser - 1]++;
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

						//判读是否某个带路连续5个EA打标

						if (m_iSupportCM5 >= 2)
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
										String info = L"带路";
										info << roadM << L"连续贴标停机!";
										stickmarker->StopMachine(info);
										break;
									}
								}
							}
						}


						//需要确认问题

						if (g_pWriteThread_codeTrans)
						{
							char szTm[256] = { 0 };
							juce::Time time = juce::Time::getCurrentTime();
							int index = sprintf(szTm, "%d_%d_%d_%d:%d:%d", time.getYear(), time.getMonth() + 1, time.getDayOfMonth(), time.getHours(), time.getMinutes(), time.getSeconds());
							sprintf(&szTm[index], " ***Succ label from %s ***: iRoad=%d, iSel=%d, destPos=%f\n", m_stickInfos[i].ip, iRoad, pMessage->data.iSel, destPos);
							g_pWriteThread_codeTrans->AddDebugData(szTm);
						}
						break;
					}
				}
			}
			_objectLock.exit();

		}
	}
}

void MonitorForm::CreateNewRoll()
{
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
			String szTitle = L"打标原因,长度位置,EA序号,带路,发送打标时间,实际打标时间,是否打标";
			char* pmbType;
			UnicodeToMB_CodeTrans(pmbType, szTitle.toUTF16(), szTitle.length());
			fprintf(fp, "%s\n", pmbType);
			if (pmbType != nullptr)
				delete[] pmbType;

			for (int i = 0; i < m_stickInfos.size(); i++)
			{

				String strReason = ProduceDefectString_CodeTrans(m_stickInfos[i].codeReason);
				UnicodeToMB_CodeTrans(pmbType, strReason.toUTF16(), strReason.length());
				fprintf(fp, "%s,", pmbType);
				fprintf(fp, "%f,", m_stickInfos[i].lenPos);
				fprintf(fp, "%d,", m_stickInfos[i].EA);
				fprintf(fp, "%d,", m_stickInfos[i].roadUser);
				if (pmbType != nullptr)
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
					stick = String(juce::CharPointer_UTF8("\xe5\xae\x8c\xe6\x88\x90"));
				else
					stick = String(juce::CharPointer_UTF8("\xe6\x9c\xaa\xe5\xae\x8c\xe6\x88\x90"));
				UnicodeToMB_CodeTrans(pmbType, stick.toUTF16(), stick.length());
				fprintf(fp, "%s\n", pmbType);
				if (pmbType != nullptr)
					delete[] pmbType;

			}
			fclose(fp);
		}
	}
	if (m_bSupportStick_Remain)
	{
		if (m_stickInfoReserves.size() == 0) //如果在运行过程中新卷，则m_stickInfoReserves会有
		{
			for (int i = 0; i < m_stickInfos.size(); i++) //停机情况下新卷开始，保存改带路未被打标的数据
			{
				if (m_stickInfos[i].bSticked || m_stickInfos[i].roadUser <= 0)
					continue;


				bool bSkip = false;
				for (int j = i + 1; j < m_stickInfos.size(); j++) //停机情况下新卷开始，保存改带路未被打标的数据
				{
					//如果后面一个已经出标，则改打标信息不应该存在
					if (m_stickInfos[j].bSticked && m_stickInfos[i].road == m_stickInfos[j].road &&  m_stickInfos[i].iSe == m_stickInfos[j].iSe)
					{
						bSkip = true;
						break;
					}
					if (m_stickInfos[j].bSticked && (m_stickInfos[j].lenPos - m_stickInfos[i].lenPos) > 4000) //4m以后出现已经出标的，则前面的也不应该出现
					{
						bSkip = true;
						break;
					}
				}
				if (bSkip)
					continue;

				if (_curRunningLenPos_codeTrans[m_stickInfos[i].roadUser - 1] < m_stickInfos[i].lenPos) //有效
				{
					m_stickInfos[i].lenPos = m_stickInfos[i].lenPos - _curRunningLenPos_codeTrans[m_stickInfos[i].roadUser - 1];
					m_stickInfoReserves.push_back(m_stickInfos[i]);
					m_stickInfos.erase(m_stickInfos.begin() + i);
					i--;

				}
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		_curRunningLenPos_codeTrans[i] = -1.;
		_preStickLenPos_codeTrans[i] = -1.;
	}

	m_stickInfos.clear();
	m_stickInfos.reserve(2000);
	m_sumEars = 0;//总数

	Num_Mark[0] = 0;
	Num_Mark[1] = 0;
	Num_Mark[2] = 0;
	Num_Mark[3] = 0;

	if (m_bSupportStick_Remain)
	{
		//未被打标的数据需要加入到对应控制器

		for (int i = 0; i < m_stickInfoReserves.size(); i++)
		{
			if (m_stickInfoReserves[i].bSticked)
				continue;

			bool bRet = CLabelDM[m_stickInfoReserves[i].road]->AddLabelData(m_stickInfoReserves[i].road, m_stickInfoReserves[i].iSe,
				m_stickInfoReserves[i].lenPos, m_stickInfoReserves[i].lenPos);

			if (bRet)
				m_stickInfos.push_back(m_stickInfoReserves[i]);
		}
		m_stickInfoReserves.clear();
		m_stickInfoReserves.reserve(100);

	}
	m_sent2ServerIndex = 0;
	_objectLock.exit();
}

void MonitorForm::Save2MonthReport()
{
	//todo reference MainComponent Save2MonthReport()
}

void MonitorForm::SaveReport()
{
	//todo reference MainComponent SaveReport()
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MonitorForm" componentName=""
                 parentClasses="public juce::Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="830" initialHeight="500">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TEXTBUTTON name="btnConfigure" id="dea7d10815db584d" memberName="btnConfigure"
              virtualName="" explicitFocusOrder="0" pos="8 8 136 24" buttonText="&#21021;&#22987;&#35774;&#32622;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="btnWarning" id="2672d195d9f55e8f" memberName="btnWarning"
              virtualName="" explicitFocusOrder="0" pos="144 8 136 24" buttonText="&#25253;&#35686;&#37325;&#32622;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="btnClearNum" id="e1d41ce7833739f3" memberName="btnClearNum"
              virtualName="" explicitFocusOrder="0" pos="280 8 136 24" buttonText="&#35745;&#25968;&#28165;&#38646;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="btnReset" id="1353b063f8d1529e" memberName="btnReset" virtualName=""
              explicitFocusOrder="0" pos="416 8 136 24" buttonText="&#19968;&#38190;&#22797;&#20301;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="btnHelp" id="e2be261bd0fa423e" memberName="btnHelp" virtualName=""
              explicitFocusOrder="0" pos="552 8 136 24" buttonText="&#24110;&#21161;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="lblPlc" id="b9d5aab725191297" memberName="lblPlc" virtualName=""
         explicitFocusOrder="0" pos="296 48 104 24" edTextCol="ff000000"
         edBkgCol="0" labelText="PLC&#36830;&#25509;&#29366;&#24577;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <IMAGEBUTTON name="btnIp" id="43b14e2d1802a28f" memberName="btnIp" virtualName=""
               explicitFocusOrder="0" pos="536 48 32 24" buttonText="" connectedEdges="0"
               needsCallback="1" radioGroupId="0" keepProportions="1" resourceNormal=""
               opacityNormal="1.0" colourNormal="0" resourceOver="" opacityOver="1.0"
               colourOver="0" resourceDown="" opacityDown="1.0" colourDown="0"/>
  <LABEL name="lblIp" id="c1c0cc739817ca48" memberName="lblIp" virtualName=""
         explicitFocusOrder="0" pos="496 48 32 24" edTextCol="ff000000"
         edBkgCol="0" labelText="IP" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <IMAGEBUTTON name="btnPort" id="b213410f39bb611d" memberName="btnPort" virtualName=""
               explicitFocusOrder="0" pos="608 48 32 24" buttonText="" connectedEdges="0"
               needsCallback="1" radioGroupId="0" keepProportions="1" resourceNormal=""
               opacityNormal="1.0" colourNormal="0" resourceOver="" opacityOver="1.0"
               colourOver="0" resourceDown="" opacityDown="1.0" colourDown="0"/>
  <LABEL name="lblPort" id="8da4ab8246d7947f" memberName="lblPort" virtualName=""
         explicitFocusOrder="0" pos="568 48 32 24" edTextCol="ff000000"
         edBkgCol="0" labelText="PORT" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="lblWarnNum" id="aa7944596d3f15c4" memberName="lblWarnNum"
         virtualName="" explicitFocusOrder="0" pos="640 48 112 24" edTextCol="ff000000"
         edBkgCol="0" labelText="&#36148;&#26631;&#25253;&#35686;&#25968;&#30446;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <TEXTEDITOR name="txtWarnNum" id="ea91343574eb17e6" memberName="txtWarnNum"
              virtualName="" explicitFocusOrder="0" pos="760 48 347M 24" initialText="0"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTBUTTON name="btnSave" id="87f4b36d4d1f1540" memberName="btnSave" virtualName=""
              explicitFocusOrder="0" pos="688 8 347M 24" buttonText="&#20445;&#23384;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="lblPlc" id="2ffc8ee9af202b5" memberName="lblPlc2" virtualName=""
         explicitFocusOrder="0" pos="8 48 80 24" edTextCol="ff000000"
         edBkgCol="0" labelText="&#36148;&#26631;&#20449;&#24687;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="txtWarnNum" id="47d226b845356873" memberName="txtWarnNum2"
              virtualName="" explicitFocusOrder="0" pos="80 48 88 24" initialText="0"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="d8ae457465e46dec" memberName="txtPlcStatus"
              virtualName="" explicitFocusOrder="0" pos="400 48 95 24" shadowcol="ffffff00"
              initialText="&#26410;&#36830;&#25509;" multiline="0" retKeyStartsLine="0"
              readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTBUTTON name="new button" id="5c020d9ff9fc23fa" memberName="btnPlcConnect"
              virtualName="" explicitFocusOrder="0" pos="192 48 95 24" bgColOff="ffff0000"
              buttonText="&#36830;&#25509;PLC" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

