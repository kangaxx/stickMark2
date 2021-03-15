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

class StickMarker : public juce::Component,
                    public juce::Button::Listener,
                    public juce::Timer,
                    public juce::Thread,
					public juce::AsyncUpdater
{

private:
    int m_edition; //版本,0表示先导,1表示海目星
public:
	void StopMachine(const juce::String& info);
	//int getStickResData(int sticked[4], int rets[4], int response[4]);
	//int getSensorData(int response[4]);
	virtual void handleAsyncUpdate();
	
	int sticked[4];
	int response[4];
	int sensorRes[4];

	bool m_bReconnect;
    //TransferFins* transferfins;
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

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
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
    std::unique_ptr<juce::GroupComponent> juce__groupComponent;
    std::unique_ptr<juce::TextButton> juce__textButton;
    std::unique_ptr<juce::TextButton> juce__textButton2;
    std::unique_ptr<juce::Label> juce__label;
    std::unique_ptr<juce::Label> juce__label2;
    std::unique_ptr<juce::Label> juce__label3;
    std::unique_ptr<juce::Label> juce__label4;
    std::unique_ptr<juce::Label> juce__label5;
    std::unique_ptr<juce::Label> juce__label6;
    std::unique_ptr<juce::Label> juce__label7;
    std::unique_ptr<juce::Label> juce__label8;
    std::unique_ptr<juce::Label> juce__label9;
    std::unique_ptr<juce::Label> juce__label10;
    std::unique_ptr<juce::Label> juce__label11;
    std::unique_ptr<juce::Label> juce__label12;
    std::unique_ptr<juce::ToggleButton> juce__toggleButton;
    std::unique_ptr<juce::Label> juce__label13;
    std::unique_ptr<juce::TextButton> juce__textButton3;
    std::unique_ptr<juce::TextButton> juce__textButton4;
    std::unique_ptr<juce::Label> juce__label14;
    std::unique_ptr<juce::Label> juce__label15;
    std::unique_ptr<juce::TextButton> juce__textButton5;
    std::unique_ptr<juce::TextButton> juce__textButton6;
    std::unique_ptr<juce::Label> juce__label16;
    std::unique_ptr<juce::Label> juce__label17;
    std::unique_ptr<juce::TextButton> juce__textButton7;
    std::unique_ptr<juce::TextButton> juce__textButton8;
    std::unique_ptr<juce::Label> juce__label18;
    std::unique_ptr<juce::Label> juce__label19;
    std::unique_ptr<juce::TextButton> juce__textButton9;
    std::unique_ptr<juce::TextButton> juce__textButton10;
    std::unique_ptr<juce::Label> juce__label20;
    std::unique_ptr<juce::Label> juce__label21;
    std::unique_ptr<juce::Label> juce__label22;
    std::unique_ptr<juce::Label> juce__label23;
    std::unique_ptr<juce::Label> juce__label24;
    std::unique_ptr<juce::Label> juce__label25;
    std::unique_ptr<juce::Label> juce__label26;
    std::unique_ptr<juce::Label> juce__label27;
    std::unique_ptr<juce::Label> juce__label28;
    std::unique_ptr<juce::Label> juce__label29;
    std::unique_ptr<juce::Label> juce__label30;
    std::unique_ptr<juce::Label> juce__label31;
    std::unique_ptr<juce::GroupComponent> juce__groupComponent2;
    std::unique_ptr<juce::TextButton> juce__textButton11;
    std::unique_ptr<juce::TextButton> juce__textButton12;
    std::unique_ptr<juce::TextButton> juce__textButton13;
    std::unique_ptr<juce::TextButton> juce__textButton14;
    std::unique_ptr<juce::Label> juce__label32;
    std::unique_ptr<juce::Label> juce__label33;
    std::unique_ptr<juce::Label> juce__label34;
    std::unique_ptr<juce::Label> juce__label35;
    std::unique_ptr<juce::Label> juce__label36;
    std::unique_ptr<juce::Label> juce__label37;
    std::unique_ptr<juce::Label> juce__label38;
    std::unique_ptr<juce::Label> juce__label39;
    std::unique_ptr<juce::Label> juce__label40;
    std::unique_ptr<juce::Label> juce__label41;
    std::unique_ptr<juce::Label> juce__label42;
    std::unique_ptr<juce::Label> juce__label43;
    std::unique_ptr<juce::Label> juce__label44;
    std::unique_ptr<juce::Label> juce__label45;
    std::unique_ptr<juce::Label> juce__label46;
    std::unique_ptr<juce::Label> juce__label47;
    std::unique_ptr<juce::Label> juce__label48;
    std::unique_ptr<juce::Label> juce__label49;
    std::unique_ptr<juce::Label> juce__label50;
    std::unique_ptr<juce::Label> juce__label51;
    std::unique_ptr<juce::Label> juce__label52;
    std::unique_ptr<juce::Label> juce__label53;
    std::unique_ptr<juce::Label> juce__label54;
    std::unique_ptr<juce::Label> juce__label55;
    std::unique_ptr<juce::Label> juce__label56;
    std::unique_ptr<juce::Label> juce__label57;
    std::unique_ptr<juce::GroupComponent> juce__groupComponent3;
    std::unique_ptr<juce::Label> juce__label58;
    std::unique_ptr<juce::Label> juce__label59;
    std::unique_ptr<juce::Label> juce__label60;
    std::unique_ptr<juce::Label> juce__label61;
    std::unique_ptr<juce::Label> juce__label62;
    std::unique_ptr<juce::Label> juce__label63;
    std::unique_ptr<juce::Label> juce__label64;
    std::unique_ptr<juce::Label> juce__label65;
    std::unique_ptr<juce::Label> juce__label66;
    std::unique_ptr<juce::TextButton> juce__textButton15;
    std::unique_ptr<juce::TextButton> juce__textButton16;
    std::unique_ptr<juce::TextButton> juce__textButton17;
public:
    std::unique_ptr<juce::TextEditor> juce__textEditor;
    //int getDword(int, int&);
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

