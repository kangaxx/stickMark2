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
#include "ConfigureForm.h"
#include "../hds/FastDelegate.h"
#include "tzGrid.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
	An auto-generated component, created by the Projucer.

	Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MonitorForm  : public juce::Component,
                     public juce::Button::Listener
{
public:
    //==============================================================================
    MonitorForm ();
    ~MonitorForm() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void resetPlc(std::string window, std::string message)
	{
		AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, window, message);
	}
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	ConfigureForm *configureForm;
	std::unique_ptr<tzGrid> gridMain;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::TextButton> btnConfigure;
    std::unique_ptr<juce::TextButton> btnWarning;
    std::unique_ptr<juce::TextButton> btnClearNum;
    std::unique_ptr<juce::TextButton> btnReset;
    std::unique_ptr<juce::TextButton> btnHelp;
    std::unique_ptr<juce::Label> lblPlc;
    std::unique_ptr<juce::ImageButton> btnIp;
    std::unique_ptr<juce::Label> lblIp;
    std::unique_ptr<juce::ImageButton> btnPort;
    std::unique_ptr<juce::Label> lblPort;
    std::unique_ptr<juce::Label> lblWarnNum;
    std::unique_ptr<juce::TextEditor> txtWarnNum;
    std::unique_ptr<juce::TextButton> btnSave;
    std::unique_ptr<juce::Label> lblPlc2;
    std::unique_ptr<juce::TextEditor> txtWarnNum2;
    std::unique_ptr<juce::TextEditor> txtPlcStatus;
    std::unique_ptr<juce::TextButton> btnPlcConnect;
    std::unique_ptr<juce::GroupComponent> groupCommand;
	StringArray row_data;
	char rowData[28][2000];
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MonitorForm)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

