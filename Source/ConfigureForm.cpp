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

#include "ConfigureForm.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
ConfigureForm::ConfigureForm ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    lblPrint_1.reset (new juce::Label ("lbl_print_1",
                                       juce::CharPointer_UTF8 ("\xe6\x89\x93\xe5\x8d\xb0\xe6\x9c\xba 1")));
    addAndMakeVisible (lblPrint_1.get());
    lblPrint_1->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblPrint_1->setJustificationType (juce::Justification::centredLeft);
    lblPrint_1->setEditable (false, false, false);
    lblPrint_1->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblPrint_1->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblPrint_1->setBounds (48, 40, 150, 24);

    lblPrintValue_1.reset (new juce::Label ("lbl_print_1_value",
                                            juce::CharPointer_UTF8 ("\xe8\xae\xa1\xe6\x95\xb0")));
    addAndMakeVisible (lblPrintValue_1.get());
    lblPrintValue_1->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblPrintValue_1->setJustificationType (juce::Justification::centredLeft);
    lblPrintValue_1->setEditable (false, false, false);
    lblPrintValue_1->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblPrintValue_1->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblPrintValue_1->setBounds (216, 40, 150, 24);

    lblPrint_2.reset (new juce::Label ("lbl_print_2",
                                       juce::CharPointer_UTF8 ("\xe6\x89\x93\xe5\x8d\xb0\xe6\x9c\xba 2")));
    addAndMakeVisible (lblPrint_2.get());
    lblPrint_2->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblPrint_2->setJustificationType (juce::Justification::centredLeft);
    lblPrint_2->setEditable (false, false, false);
    lblPrint_2->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblPrint_2->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblPrint_2->setBounds (408, 40, 150, 24);

    lblPrintValue_2.reset (new juce::Label ("lbl_print_2_value",
                                            juce::CharPointer_UTF8 ("\xe8\xae\xa1\xe6\x95\xb0")));
    addAndMakeVisible (lblPrintValue_2.get());
    lblPrintValue_2->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblPrintValue_2->setJustificationType (juce::Justification::centredLeft);
    lblPrintValue_2->setEditable (false, false, false);
    lblPrintValue_2->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblPrintValue_2->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblPrintValue_2->setBounds (608, 40, 150, 24);

    btnResetPlc.reset (new juce::TextButton ("btnResetPlc"));
    addAndMakeVisible (btnResetPlc.get());
    btnResetPlc->setButtonText (juce::CharPointer_UTF8 ("\xe9\x87\x8d\xe8\xbf\x9ePLC"));
    btnResetPlc->addListener (this);

    btnResetPlc->setBounds (360, 144, 150, 24);

    lblPrint_3.reset (new juce::Label ("lblPrint_3",
                                       juce::CharPointer_UTF8 ("\xe6\x89\x93\xe5\x8d\xb0\xe6\x9c\xba 3")));
    addAndMakeVisible (lblPrint_3.get());
    lblPrint_3->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblPrint_3->setJustificationType (juce::Justification::centredLeft);
    lblPrint_3->setEditable (false, false, false);
    lblPrint_3->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblPrint_3->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblPrint_3->setBounds (48, 88, 150, 24);

    lblPrintValue_3.reset (new juce::Label ("lbl_print_1_value",
                                            juce::CharPointer_UTF8 ("\xe8\xae\xa1\xe6\x95\xb0")));
    addAndMakeVisible (lblPrintValue_3.get());
    lblPrintValue_3->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblPrintValue_3->setJustificationType (juce::Justification::centredLeft);
    lblPrintValue_3->setEditable (false, false, false);
    lblPrintValue_3->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblPrintValue_3->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblPrintValue_3->setBounds (216, 88, 150, 24);

    lblPrint_4.reset (new juce::Label ("lblPrint_4",
                                       juce::CharPointer_UTF8 ("\xe6\x89\x93\xe5\x8d\xb0\xe6\x9c\xba 4")));
    addAndMakeVisible (lblPrint_4.get());
    lblPrint_4->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblPrint_4->setJustificationType (juce::Justification::centredLeft);
    lblPrint_4->setEditable (false, false, false);
    lblPrint_4->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblPrint_4->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblPrint_4->setBounds (408, 88, 150, 24);

    lblPrintValue_4.reset (new juce::Label ("lbl_print_1_value",
                                            juce::CharPointer_UTF8 ("\xe8\xae\xa1\xe6\x95\xb0")));
    addAndMakeVisible (lblPrintValue_4.get());
    lblPrintValue_4->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblPrintValue_4->setJustificationType (juce::Justification::centredLeft);
    lblPrintValue_4->setEditable (false, false, false);
    lblPrintValue_4->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblPrintValue_4->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblPrintValue_4->setBounds (608, 88, 150, 24);

    btnResetCcd.reset (new juce::TextButton ("btnResetCcd"));
    addAndMakeVisible (btnResetCcd.get());
    btnResetCcd->setButtonText (juce::CharPointer_UTF8 ("ccd\xe5\xa4\x8d\xe4\xbd\x8d"));
    btnResetCcd->addListener (this);

    btnResetCcd->setBounds (200, 144, 150, 24);

    btnNewScroll.reset (new juce::TextButton ("btnNewScroll"));
    addAndMakeVisible (btnNewScroll.get());
    btnNewScroll->setButtonText (juce::CharPointer_UTF8 ("\xe6\x96\xb0\xe5\x8d\xb7"));
    btnNewScroll->addListener (this);

    btnNewScroll->setBounds (32, 144, 160, 24);

    lblSendSignal.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("\xe4\xbf\xa1\xe5\x8f\xb7\xe7\xab\xaf\xe5\x8f\xa3(0 - 31)")));
    addAndMakeVisible (lblSendSignal.get());
    lblSendSignal->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblSendSignal->setJustificationType (juce::Justification::centredLeft);
    lblSendSignal->setEditable (false, false, false);
    lblSendSignal->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblSendSignal->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblSendSignal->setBounds (48, 200, 150, 24);

    txtSignal.reset (new juce::TextEditor ("new text editor"));
    addAndMakeVisible (txtSignal.get());
    txtSignal->setMultiLine (false);
    txtSignal->setReturnKeyStartsNewLine (false);
    txtSignal->setReadOnly (false);
    txtSignal->setScrollbarsShown (true);
    txtSignal->setCaretVisible (true);
    txtSignal->setPopupMenuEnabled (true);
    txtSignal->setText (TRANS("0"));

    txtSignal->setBounds (184, 200, 120, 24);

    btnSendSignal.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (btnSendSignal.get());
    btnSendSignal->setButtonText (juce::CharPointer_UTF8 ("\xe5\x8f\x91\xe9\x80\x81"));
    btnSendSignal->addListener (this);

    btnSendSignal->setBounds (512, 200, 150, 24);

    btnSetEa.reset (new juce::TextButton ("btnSetEa"));
    addAndMakeVisible (btnSetEa.get());
    btnSetEa->setButtonText (juce::CharPointer_UTF8 ("\xe8\xae\xbe\xe7\xbd\xae"));
    btnSetEa->addListener (this);

    btnSetEa->setBounds (512, 256, 150, 24);

    lblSendSignal2.reset (new juce::Label ("new label",
                                           juce::CharPointer_UTF8 ("EA\xe6\x80\xbb\xe9\x95\xbf\xe5\xba\xa6")));
    addAndMakeVisible (lblSendSignal2.get());
    lblSendSignal2->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblSendSignal2->setJustificationType (juce::Justification::centredLeft);
    lblSendSignal2->setEditable (false, false, false);
    lblSendSignal2->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblSendSignal2->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblSendSignal2->setBounds (48, 256, 150, 24);

    txtSetEa.reset (new juce::TextEditor ("new text editor"));
    addAndMakeVisible (txtSetEa.get());
    txtSetEa->setMultiLine (false);
    txtSetEa->setReturnKeyStartsNewLine (false);
    txtSetEa->setReadOnly (false);
    txtSetEa->setScrollbarsShown (true);
    txtSetEa->setCaretVisible (true);
    txtSetEa->setPopupMenuEnabled (true);
    txtSetEa->setText (TRANS("4020"));

    txtSetEa->setBounds (184, 256, 120, 24);

    btnReconfig.reset (new juce::TextButton ("btnReconfig"));
    addAndMakeVisible (btnReconfig.get());
    btnReconfig->setButtonText (TRANS("reconfig"));
    btnReconfig->addListener (this);

    btnReconfig->setBounds (520, 144, 150, 24);


    //[UserPreSize]

    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
	setSize(700, 400);
    //[/Constructor]
}

ConfigureForm::~ConfigureForm()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    lblPrint_1 = nullptr;
    lblPrintValue_1 = nullptr;
    lblPrint_2 = nullptr;
    lblPrintValue_2 = nullptr;
    btnResetPlc = nullptr;
    lblPrint_3 = nullptr;
    lblPrintValue_3 = nullptr;
    lblPrint_4 = nullptr;
    lblPrintValue_4 = nullptr;
    btnResetCcd = nullptr;
    btnNewScroll = nullptr;
    lblSendSignal = nullptr;
    txtSignal = nullptr;
    btnSendSignal = nullptr;
    btnSetEa = nullptr;
    lblSendSignal2 = nullptr;
    txtSetEa = nullptr;
    btnReconfig = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

void ConfigureForm::updateMarkCount(int idx, int count)
{
	juce::String value = juce::String(count);
	switch (idx)
	{
	case 1:
		lblPrintValue_1->setText(value, juce::NotificationType::sendNotification);
		break;
	case 2:
		lblPrintValue_2->setText(value, juce::NotificationType::sendNotification);
		break;
	case 3:
		lblPrintValue_3->setText(value, juce::NotificationType::sendNotification);
		break;
	case 4:
		lblPrintValue_4->setText(value, juce::NotificationType::sendNotification);
		break;
	default:
		break;
	}
}

//==============================================================================
void ConfigureForm::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void ConfigureForm::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void ConfigureForm::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnResetPlc.get())
    {
        //[UserButtonCode_btnResetPlc] -- add your button handler code here..
		m_HiddenDelegateSS("window from configure", "yes Fastdelegate");
        //[/UserButtonCode_btnResetPlc]
    }
    else if (buttonThatWasClicked == btnResetCcd.get())
    {
        //[UserButtonCode_btnResetCcd] -- add your button handler code here..
        //[/UserButtonCode_btnResetCcd]
    }
    else if (buttonThatWasClicked == btnNewScroll.get())
    {
        //[UserButtonCode_btnNewScroll] -- add your button handler code here..
        //[/UserButtonCode_btnNewScroll]
    }
    else if (buttonThatWasClicked == btnSendSignal.get())
    {
        //[UserButtonCode_btnSendSignal] -- add your button handler code here..
		int port = BaseFunctions::Str2Int(txtSignal->getText().toStdString(), -1);
		if ( port < 0 || port > 31)
			juce::AlertWindow::showMessageBox(juce::AlertWindow::WarningIcon, juce::String(L"端口错误"), juce::String(L"请输入 0 - 31之前的数字"));
		else
			m_HiddenDelegateI(port);
        //[/UserButtonCode_btnSendSignal]
    }
    else if (buttonThatWasClicked == btnSetEa.get())
    {
        //[UserButtonCode_btnSetEa] -- add your button handler code here..
        //[/UserButtonCode_btnSetEa]
    }
    else if (buttonThatWasClicked == btnReconfig.get())
    {
        //[UserButtonCode_btnReconfig] -- add your button handler code here..
        //[/UserButtonCode_btnReconfig]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="ConfigureForm" componentName=""
                 parentClasses="public juce::Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <LABEL name="lbl_print_1" id="30a30619a0241cc4" memberName="lblPrint_1"
         virtualName="" explicitFocusOrder="0" pos="48 40 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="&#25171;&#21360;&#26426; 1" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="lbl_print_1_value" id="5acd692aa7c2f44b" memberName="lblPrintValue_1"
         virtualName="" explicitFocusOrder="0" pos="216 40 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="&#35745;&#25968;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="lbl_print_2" id="3cbba17df04b54e3" memberName="lblPrint_2"
         virtualName="" explicitFocusOrder="0" pos="408 40 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="&#25171;&#21360;&#26426; 2" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="lbl_print_2_value" id="973e35c7d15cbeb1" memberName="lblPrintValue_2"
         virtualName="" explicitFocusOrder="0" pos="608 40 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="&#35745;&#25968;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="btnResetPlc" id="3d2036c82e34c007" memberName="btnResetPlc"
              virtualName="" explicitFocusOrder="0" pos="360 144 150 24" buttonText="&#37325;&#36830;PLC"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="lblPrint_3" id="36eca028e3efba4d" memberName="lblPrint_3"
         virtualName="" explicitFocusOrder="0" pos="48 88 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="&#25171;&#21360;&#26426; 3" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="lbl_print_1_value" id="73b06405e0d164ba" memberName="lblPrintValue_3"
         virtualName="" explicitFocusOrder="0" pos="216 88 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="&#35745;&#25968;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="lblPrint_4" id="d026de63eb7b0d33" memberName="lblPrint_4"
         virtualName="" explicitFocusOrder="0" pos="408 88 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="&#25171;&#21360;&#26426; 4" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="lbl_print_1_value" id="976fa3385be7630d" memberName="lblPrintValue_4"
         virtualName="" explicitFocusOrder="0" pos="608 88 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="&#35745;&#25968;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="btnResetCcd" id="5f72a3a08ec102d5" memberName="btnResetCcd"
              virtualName="" explicitFocusOrder="0" pos="200 144 150 24" buttonText="ccd&#22797;&#20301;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="btnNewScroll" id="11769114367b6b70" memberName="btnNewScroll"
              virtualName="" explicitFocusOrder="0" pos="32 144 160 24" buttonText="&#26032;&#21367;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="6fb88adc46b34f0f" memberName="lblSendSignal"
         virtualName="" explicitFocusOrder="0" pos="48 200 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="&#20449;&#21495;&#31471;&#21475;(0 - 31)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <TEXTEDITOR name="new text editor" id="5aeb7e75eb7c1fb0" memberName="txtSignal"
              virtualName="" explicitFocusOrder="0" pos="184 200 120 24" initialText="0"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTBUTTON name="new button" id="1d4f8611b9b47ffc" memberName="btnSendSignal"
              virtualName="" explicitFocusOrder="0" pos="512 200 150 24" buttonText="&#21457;&#36865;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="btnSetEa" id="93dae559b4f81117" memberName="btnSetEa" virtualName=""
              explicitFocusOrder="0" pos="512 256 150 24" buttonText="&#35774;&#32622;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="b945ff7678e70452" memberName="lblSendSignal2"
         virtualName="" explicitFocusOrder="0" pos="48 256 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="EA&#24635;&#38271;&#24230;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="new text editor" id="dcc50853dae51717" memberName="txtSetEa"
              virtualName="" explicitFocusOrder="0" pos="184 256 120 24" initialText="4020"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTBUTTON name="btnReconfig" id="afc5b4297a3be9c8" memberName="btnReconfig"
              virtualName="" explicitFocusOrder="0" pos="520 144 150 24" buttonText="reconfig"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

