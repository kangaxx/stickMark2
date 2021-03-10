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

    btnConfigure->setBounds (8, 8, 150, 24);

    btnWarning.reset (new juce::TextButton ("btnWarning"));
    addAndMakeVisible (btnWarning.get());
    btnWarning->setButtonText (juce::CharPointer_UTF8 ("\xe6\x8a\xa5\xe8\xad\xa6\xe9\x87\x8d\xe7\xbd\xae"));
    btnWarning->addListener (this);

    btnWarning->setBounds (160, 8, 150, 24);

    btnClearNum.reset (new juce::TextButton ("btnClearNum"));
    addAndMakeVisible (btnClearNum.get());
    btnClearNum->setButtonText (juce::CharPointer_UTF8 ("\xe8\xae\xa1\xe6\x95\xb0\xe6\xb8\x85\xe9\x9b\xb6"));
    btnClearNum->addListener (this);

    btnClearNum->setBounds (312, 8, 150, 24);

    btnReset.reset (new juce::TextButton ("btnReset"));
    addAndMakeVisible (btnReset.get());
    btnReset->setButtonText (juce::CharPointer_UTF8 ("\xe4\xb8\x80\xe9\x94\xae\xe5\xa4\x8d\xe4\xbd\x8d"));
    btnReset->addListener (this);

    btnReset->setBounds (464, 8, 150, 24);

    btnHelp.reset (new juce::TextButton ("btnHelp"));
    addAndMakeVisible (btnHelp.get());
    btnHelp->setButtonText (juce::CharPointer_UTF8 ("\xe5\xb8\xae\xe5\x8a\xa9"));
    btnHelp->addListener (this);

    btnHelp->setBounds (616, 8, 150, 24);

    lblPlc.reset (new juce::Label ("lblPlc",
                                   juce::CharPointer_UTF8 ("PLC\xe8\xbf\x9e\xe6\x8e\xa5\xe7\x8a\xb6\xe6\x80\x81")));
    addAndMakeVisible (lblPlc.get());
    lblPlc->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblPlc->setJustificationType (juce::Justification::centredLeft);
    lblPlc->setEditable (false, false, false);
    lblPlc->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblPlc->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblPlc->setBounds (16, 48, 104, 24);

    btnIp.reset (new juce::ImageButton ("btnIp"));
    addAndMakeVisible (btnIp.get());
    btnIp->setButtonText (juce::String());
    btnIp->addListener (this);

    btnIp->setImages (false, true, true,
                      juce::Image(), 1.000f, juce::Colour (0x00000000),
                      juce::Image(), 1.000f, juce::Colour (0x00000000),
                      juce::Image(), 1.000f, juce::Colour (0x00000000));
    btnIp->setBounds (200, 48, 32, 24);

    lblIp.reset (new juce::Label ("lblIp",
                                  TRANS("IP")));
    addAndMakeVisible (lblIp.get());
    lblIp->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblIp->setJustificationType (juce::Justification::centredLeft);
    lblIp->setEditable (false, false, false);
    lblIp->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblIp->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblIp->setBounds (152, 48, 32, 24);

    btnPort.reset (new juce::ImageButton ("btnPort"));
    addAndMakeVisible (btnPort.get());
    btnPort->setButtonText (juce::String());
    btnPort->addListener (this);

    btnPort->setImages (false, true, true,
                        juce::Image(), 1.000f, juce::Colour (0x00000000),
                        juce::Image(), 1.000f, juce::Colour (0x00000000),
                        juce::Image(), 1.000f, juce::Colour (0x00000000));
    btnPort->setBounds (312, 48, 32, 24);

    lblPort.reset (new juce::Label ("lblPort",
                                    TRANS("PORT")));
    addAndMakeVisible (lblPort.get());
    lblPort->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblPort->setJustificationType (juce::Justification::centredLeft);
    lblPort->setEditable (false, false, false);
    lblPort->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblPort->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblPort->setBounds (248, 48, 32, 24);

    lblWarnNum.reset (new juce::Label ("lblWarnNum",
                                       juce::CharPointer_UTF8 ("\xe8\xb4\xb4\xe6\xa0\x87\xe6\x8a\xa5\xe8\xad\xa6\xe6\x95\xb0\xe7\x9b\xae")));
    addAndMakeVisible (lblWarnNum.get());
    lblWarnNum->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblWarnNum->setJustificationType (juce::Justification::centredLeft);
    lblWarnNum->setEditable (false, false, false);
    lblWarnNum->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblWarnNum->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblWarnNum->setBounds (368, 48, 112, 24);

    txtWarnNum.reset (new juce::TextEditor ("txtWarnNum"));
    addAndMakeVisible (txtWarnNum.get());
    txtWarnNum->setMultiLine (false);
    txtWarnNum->setReturnKeyStartsNewLine (false);
    txtWarnNum->setReadOnly (false);
    txtWarnNum->setScrollbarsShown (true);
    txtWarnNum->setCaretVisible (true);
    txtWarnNum->setPopupMenuEnabled (true);
    txtWarnNum->setText (juce::String());

    txtWarnNum->setBounds (488, 48, 150, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    configureForm = new ConfigureForm();
	configureForm->setFunctionSS(MakeDelegate(this, &MonitorForm::resetPlc));
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


    //[Destructor]. You can add your own custom destruction code here..
    delete configureForm;
    configureForm = nullptr;
    //[/Destructor]
}

void MonitorForm::resetPlc(std::string window, std::string message)
{
	juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon, window, message);
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

<JUCER_COMPONENT documentType="Component" className="MonitorForm" componentName=""
                 parentClasses="public juce::Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TEXTBUTTON name="btnConfigure" id="dea7d10815db584d" memberName="btnConfigure"
              virtualName="" explicitFocusOrder="0" pos="8 8 150 24" buttonText="&#21021;&#22987;&#35774;&#32622;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="btnWarning" id="2672d195d9f55e8f" memberName="btnWarning"
              virtualName="" explicitFocusOrder="0" pos="160 8 150 24" buttonText="&#25253;&#35686;&#37325;&#32622;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="btnClearNum" id="e1d41ce7833739f3" memberName="btnClearNum"
              virtualName="" explicitFocusOrder="0" pos="312 8 150 24" buttonText="&#35745;&#25968;&#28165;&#38646;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="btnReset" id="1353b063f8d1529e" memberName="btnReset" virtualName=""
              explicitFocusOrder="0" pos="464 8 150 24" buttonText="&#19968;&#38190;&#22797;&#20301;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="btnHelp" id="e2be261bd0fa423e" memberName="btnHelp" virtualName=""
              explicitFocusOrder="0" pos="616 8 150 24" buttonText="&#24110;&#21161;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="lblPlc" id="b9d5aab725191297" memberName="lblPlc" virtualName=""
         explicitFocusOrder="0" pos="16 48 104 24" edTextCol="ff000000"
         edBkgCol="0" labelText="PLC&#36830;&#25509;&#29366;&#24577;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <IMAGEBUTTON name="btnIp" id="43b14e2d1802a28f" memberName="btnIp" virtualName=""
               explicitFocusOrder="0" pos="200 48 32 24" buttonText="" connectedEdges="0"
               needsCallback="1" radioGroupId="0" keepProportions="1" resourceNormal=""
               opacityNormal="1.0" colourNormal="0" resourceOver="" opacityOver="1.0"
               colourOver="0" resourceDown="" opacityDown="1.0" colourDown="0"/>
  <LABEL name="lblIp" id="c1c0cc739817ca48" memberName="lblIp" virtualName=""
         explicitFocusOrder="0" pos="152 48 32 24" edTextCol="ff000000"
         edBkgCol="0" labelText="IP" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <IMAGEBUTTON name="btnPort" id="b213410f39bb611d" memberName="btnPort" virtualName=""
               explicitFocusOrder="0" pos="312 48 32 24" buttonText="" connectedEdges="0"
               needsCallback="1" radioGroupId="0" keepProportions="1" resourceNormal=""
               opacityNormal="1.0" colourNormal="0" resourceOver="" opacityOver="1.0"
               colourOver="0" resourceDown="" opacityDown="1.0" colourDown="0"/>
  <LABEL name="lblPort" id="8da4ab8246d7947f" memberName="lblPort" virtualName=""
         explicitFocusOrder="0" pos="248 48 32 24" edTextCol="ff000000"
         edBkgCol="0" labelText="PORT" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="lblWarnNum" id="aa7944596d3f15c4" memberName="lblWarnNum"
         virtualName="" explicitFocusOrder="0" pos="368 48 112 24" edTextCol="ff000000"
         edBkgCol="0" labelText="&#36148;&#26631;&#25253;&#35686;&#25968;&#30446;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <TEXTEDITOR name="txtWarnNum" id="ea91343574eb17e6" memberName="txtWarnNum"
              virtualName="" explicitFocusOrder="0" pos="488 48 150 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

