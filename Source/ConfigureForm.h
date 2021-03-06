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

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>
#include "../hds/FastDelegate.h"
#include "../hds/commonfunction_c.h"

using namespace fastdelegate;
using namespace commonfunction_c;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class ConfigureForm  : public juce::Component,
                       public juce::Button::Listener
{
public:
    //==============================================================================
    ConfigureForm ();
    ~ConfigureForm() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	typedef FastDelegate2<std::string, std::string> SIGNAL_SS;
	void setFunctionSS(SIGNAL_SS somefunc) { m_HiddenDelegateSS = somefunc; }
	typedef FastDelegate1<int> SIGNAL_I;
	void setFunctionI(SIGNAL_I somefunc) { m_HiddenDelegateI = somefunc; }
	void updateMarkCount(int idx, int count);
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	SIGNAL_SS m_HiddenDelegateSS;
	SIGNAL_I m_HiddenDelegateI;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Label> lblPrint_1;
    std::unique_ptr<juce::Label> lblPrintValue_1;
    std::unique_ptr<juce::Label> lblPrint_2;
    std::unique_ptr<juce::Label> lblPrintValue_2;
    std::unique_ptr<juce::TextButton> btnResetPlc;
    std::unique_ptr<juce::Label> lblPrint_3;
    std::unique_ptr<juce::Label> lblPrintValue_3;
    std::unique_ptr<juce::Label> lblPrint_4;
    std::unique_ptr<juce::Label> lblPrintValue_4;
    std::unique_ptr<juce::TextButton> btnResetCcd;
    std::unique_ptr<juce::TextButton> btnNewScroll;
    std::unique_ptr<juce::Label> lblSendSignal;
    std::unique_ptr<juce::TextEditor> txtSignal;
    std::unique_ptr<juce::TextButton> btnSendSignal;
    std::unique_ptr<juce::TextButton> btnSetEa;
    std::unique_ptr<juce::Label> lblSendSignal2;
    std::unique_ptr<juce::TextEditor> txtSetEa;
    std::unique_ptr<juce::TextButton> btnReconfig;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConfigureForm)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

