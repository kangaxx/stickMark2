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
using namespace fastdelegate;
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
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	SIGNAL_SS m_HiddenDelegateSS;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Label> lblPrint_1;
    std::unique_ptr<juce::Label> lblPrintValue_1;
    std::unique_ptr<juce::Label> lblPrint_2;
    std::unique_ptr<juce::Label> lblPrintValue_2;
    std::unique_ptr<juce::TextButton> btnResetPlc;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConfigureForm)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

