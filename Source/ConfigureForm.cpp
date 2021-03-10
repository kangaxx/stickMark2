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

    btnResetPlc->setBounds (32, 120, 150, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
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


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
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
              virtualName="" explicitFocusOrder="0" pos="32 120 150 24" buttonText="&#37325;&#36830;PLC"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

