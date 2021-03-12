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
//[/Headers]

#include "MonitorForm.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MonitorForm::MonitorForm ()
{
    //[Constructor_pre] You can add your own custom stuff here..

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

    txtWarnNum->setBounds (760, 48, 56, 24);

    btnSave.reset (new juce::TextButton ("btnHelp"));
    addAndMakeVisible (btnSave.get());
    btnSave->setButtonText (juce::CharPointer_UTF8 ("\xe4\xbf\x9d\xe5\xad\x98"));
    btnSave->addListener (this);

    btnSave->setBounds (688, 8, 136, 24);

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

    groupCommand.reset (new juce::GroupComponent ("new group",
                                                  juce::String()));
    addAndMakeVisible (groupCommand.get());

    groupCommand->setBounds (0, -8, 832, 96);


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
		static gridDataInfo* tmpData = new gridDataInfo();
		dataModels->insertToLast(tmpData);
	}

	//第一行数据
	((gridDataInfo*)(*dataModels)[0])->setName(juce::CharPointer_UTF8("\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba 01"));
	((gridDataInfo*)(*dataModels)[0])->setStatus(juce::CharPointer_UTF8("\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba 01"));
	((gridDataInfo*)(*dataModels)[0])->setWarning(juce::CharPointer_UTF8("\xe6\x97\xa0"));
	((gridDataInfo*)(*dataModels)[0])->setMarkNum(0);
	((gridDataInfo*)(*dataModels)[0])->setMarkSignalNum(0);
	((gridDataInfo*)(*dataModels)[0])->setSensorNum(0);

	//第二行数据
	((gridDataInfo*)(*dataModels)[1])->setName(juce::CharPointer_UTF8("\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba 02"));
	((gridDataInfo*)(*dataModels)[1])->setStatus(juce::CharPointer_UTF8("\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba 02"));
	((gridDataInfo*)(*dataModels)[1])->setWarning(juce::CharPointer_UTF8("\xe6\x97\xa0"));
	((gridDataInfo*)(*dataModels)[1])->setMarkNum(0);
	((gridDataInfo*)(*dataModels)[1])->setMarkSignalNum(0);
	((gridDataInfo*)(*dataModels)[1])->setSensorNum(0);

	//第三行数据
	((gridDataInfo*)(*dataModels)[2])->setName(juce::CharPointer_UTF8("\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba 03"));
	((gridDataInfo*)(*dataModels)[2])->setStatus(juce::CharPointer_UTF8("\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba 03"));
	((gridDataInfo*)(*dataModels)[2])->setWarning(juce::CharPointer_UTF8("\xe6\x97\xa0"));
	((gridDataInfo*)(*dataModels)[2])->setMarkNum(0);
	((gridDataInfo*)(*dataModels)[2])->setMarkSignalNum(0);
	((gridDataInfo*)(*dataModels)[2])->setSensorNum(0);

	//第四行数据
	((gridDataInfo*)(*dataModels)[3])->setName(juce::CharPointer_UTF8("\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba 04"));
	((gridDataInfo*)(*dataModels)[3])->setStatus(juce::CharPointer_UTF8("\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba 04"));
	((gridDataInfo*)(*dataModels)[3])->setWarning(juce::CharPointer_UTF8("\xe6\x97\xa0"));
	((gridDataInfo*)(*dataModels)[3])->setMarkNum(0);
	((gridDataInfo*)(*dataModels)[3])->setMarkSignalNum(0);
	((gridDataInfo*)(*dataModels)[3])->setSensorNum(0);


	//第一行数据	
	rowData.add(juce::CharPointer_UTF8("\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba 01")); //贴标机 01
	rowData.add(""); //第二列数据没有text,但是也要留空
	rowData.add(juce::CharPointer_UTF8("\xe5\x85\xb3\xe6\x9c\xba")); //关机
	rowData.add(juce::CharPointer_UTF8("\xe6\x97\xa0")); //无
	rowData.add("0");
	rowData.add("0");
	rowData.add("0");

	//第二行数据
	rowData.add(juce::CharPointer_UTF8("\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba 02")); //贴标机 02
	rowData.add(""); //第二列数据没有text,但是也要留空
	rowData.add(juce::CharPointer_UTF8("\xe5\x85\xb3\xe6\x9c\xba")); //关机
	rowData.add(juce::CharPointer_UTF8("\xe6\x97\xa0")); //无
	rowData.add("0");
	rowData.add("0");
	rowData.add("0");

	//第三行数据
	rowData.add(juce::CharPointer_UTF8("\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba 03")); //贴标机 03
	rowData.add(""); //第二列数据没有text,但是也要留空
	rowData.add(juce::CharPointer_UTF8("\xe5\x85\xb3\xe6\x9c\xba")); //关机
	rowData.add(juce::CharPointer_UTF8("\xe6\x97\xa0")); //无
	rowData.add("0");
	rowData.add("0");
	rowData.add("0");

	//第四行数据
	rowData.add(juce::CharPointer_UTF8("\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba 04")); //贴标机 04
	rowData.add(""); //第二列数据没有text,但是也要留空
	rowData.add(juce::CharPointer_UTF8("\xe5\x85\xb3\xe6\x9c\xba")); //关机
	rowData.add(juce::CharPointer_UTF8("\xe6\xbc\x8f\xe6\xa0\x87\xe6\x8a\xa5\xe8\xad\xa6\xef\xbc\x9b\xe6\x95\xb4\xe5\xae\x9a\xe5\xa4\xb1\xe8\xb4\xa5\xe6\x8a\xa5\xe8\xad\xa6\xef\xbc\x9b\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba\xe7\xb4\xaf\xe5\x8a\xa0\xe6\x8a\xa5\xe8\xad\xa6\xef\xbc\x9b\xe6\x96\xad\xe5\xb8\xa6\xe6\x8a\xa5\xe8\xad\xa6\xef\xbc\x9b\xe8\xbf\x9e\xe7\xbb\xad\xe6\x89\x93\xe5\x8d\xb0\xe6\x8a\xa5\xe8\xad\xa6")); //无
	rowData.add("0");
	rowData.add("0");
	rowData.add("0");
	gridMain->addRowData(rowData); //所有数据一起传输

    addAndMakeVisible(gridMain.get());
    gridMain->setBounds(8, 100, 814, 360);
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    configureForm = new ConfigureForm();
	configureForm->setFunctionSS(MakeDelegate(this, &MonitorForm::resetPlc));
	gridMain->setFunctionII(MakeDelegate(this, &MonitorForm::switchMarker));
	setSize (830, 500);
    //[/Constructor]
}

MonitorForm::~MonitorForm()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
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
    groupCommand = nullptr;


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

    //[UserResized] Add your own custom resize handling here..
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
StringArray MonitorForm::transModelToString(std::list<gridDataInfo> models)
{
	return StringArray();
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
                 fixedSize="0" initialWidth="600" initialHeight="400">
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
              virtualName="" explicitFocusOrder="0" pos="760 48 56 24" initialText="0"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTBUTTON name="btnHelp" id="87f4b36d4d1f1540" memberName="btnSave" virtualName=""
              explicitFocusOrder="0" pos="688 8 136 24" buttonText="&#20445;&#23384;"
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
  <GROUPCOMPONENT name="new group" id="b7ba15b17c2913df" memberName="groupCommand"
                  virtualName="" explicitFocusOrder="0" pos="0 -8 832 96" title=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

