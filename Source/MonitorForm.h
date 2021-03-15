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
#include "../hds/commonfunction_c.h"
#define MAX_MARKER_NUM 4
#define INT_MARKER_SWITCH_OPEN 2
#define INT_MARKER_SWITCH_CLOSE 2
using namespace commonfunction_c;
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

	//num there four marker , num 0 - 3 and status 1 is close  , 2 is open
	void switchMarker(int num, int status)
	{
		//AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, "switch mark", "mark num : " + String(num) + " ,status : " + String(status));
		if (status == INT_MARKER_SWITCH_OPEN)
			updateSwitchStatus(num, juce::CharPointer_UTF8("\xe5\xbc\x80\xe6\x9c\xba"));
		else
			updateSwitchStatus(num, juce::CharPointer_UTF8("\xe5\x85\xb3\xe6\x9c\xba"));
		if (gridMain->isInitialed())
			redrawGrid();
	}
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	ConfigureForm *configureForm;

	//data Model
	std::unique_ptr<tzGrid> gridMain;
	struct gridDataInfo
	{
		String name;
		String command; //这一列没有文字展现但是也要占用一列
		String status;
		String warning;
		int markNum;
		int markSignalNum;
		int sensorNum;
		gridDataInfo(String n, String s, String w, int mn, int msn, int gsn) : name(n), status(s), warning(w), markNum(mn), markSignalNum(msn), sensorNum(gsn), command("")
		{

		}
		gridDataInfo() { command = ""; }

		void setName(String n) { name = n; }
		void setCommand(String c) { command = c; }
		void setStatus(String s) { status = s; }
		void setWarning(String w) { warning = w; }
		void setMarkNum(int mn) { markNum = mn; }
		void setMarkSignalNum(int msn) { markSignalNum = msn; }
		void setSensorNum(int sn) { sensorNum = sn; }
		void incMarkNum(int num) { markNum += num;  }
		void incMarkSignalNum(int num) { markSignalNum += num; }
		void incSendsorNum(int num) { sensorNum += num; }
		void addWarning(String w) { warning += w; }
		String getName() { return name; }
		String getCommand() { return command; }
		String getStatus() { return status; }
		String getWarning() { return warning; }
		String getMarkNum() { return String(markNum); }
		String getMarkSignalNum() { return String(markSignalNum); }
		String getSensorNum() { return String(sensorNum); }

		void insertWarning(String w) { warning = warning + w; }
	};
	DuLink<gridDataInfo*> *dataModels = NULL;
	StringArray rowData;
	void insertWarning(int MarkerIndex, String warning);
	void updateSwitchStatus(int MarkerIndex, String status);
	void redrawGrid();
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


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MonitorForm)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

