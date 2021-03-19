/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>
#include"..\FinsTcp\TransferFins.h"
#include"..\FinsTcp\FinsCLI_JQ.h"
#include<bitset>
#include"../CameraDataModel.h"

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
struct COMMAND_WRITE
{
	int regAddr; //D register address
	int flag; //0 write 1 read
};

class StickMarker : public juce::Timer,
                    public juce::Thread
{

private:
    int m_edition; //版本,0表示先导,1表示海目星
public:
	void StopMachine(const juce::String& info);
	
	int sticked[4];
	int response[4];
	int sensorRes[4];

	bool m_bReconnect;
    int m_pulse;
    juce::String Info;
	juce::CriticalSection _cs;
	std::vector<COMMAND_WRITE> _cmds;
	juce::String _strPLC;
	bool m_bLocalC4;
	bool m_bC4Mark[4];
	int _allowMore;
	int _allowLess;
public:
    //==============================================================================
    StickMarker (juce::String plcIP);
    ~StickMarker() override;
	bool SendCmd(int addr, int val); //新卷需要向4204地址写0(结束卷)
	void Reconnect();
    void setEdition(int e) //设定版本
    {
        m_edition = e;
    }
    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]
	virtual void run();
public:
    void timerCallback() override;
	NumCount m_prevstickNumCount[4];
	bool IsConnected(){return m_bIsConnected;}
	void ClearLessMore();
private:
    //bool isNumCountChange(NumCount& numcount);
    //void getStickNumCount(int sticked[4], int response[4], int sensors[4]);
	bool m_bIsConnected;
private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
public:
    std::unique_ptr<juce::TextEditor> juce__textEditor;
	bool m_linkState;
private:
    juce::String WarningInfo[50];
    
    std::bitset<32> m_bState;
	std::bitset<32> m_prebState;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StickMarker)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

