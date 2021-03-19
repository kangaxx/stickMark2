#pragma once
#include <JuceHeader.h>
#include"FinsTcp\TransferFins.h"
#include"FinsTcp\FinsCLI_JQ.h"
#include<bitset>
#include"CameraDataModel.h"

struct COMMAND_WRITE_CT
{
	int regAddr; //D register address
	int flag; //0 write 1 read
};

class StickMarker_codeTrans : 
	public juce::Timer,
	public juce::Thread
{

private:
	int m_edition; //版本,0表示先导,1表示海目星
public:
	void StopMachine(const juce::String& info);

	int sticked[4];
	int response[4];
	int sensorRes[4];
	virtual void handleAsyncUpdate();
	bool m_bReconnect;
	//TransferFins* transferfins;
	int m_pulse;
	juce::String Info;
	juce::CriticalSection _cs;
	std::vector<COMMAND_WRITE_CT> _cmds;
	juce::String _strPLC;
	bool m_bLocalC4;
	bool m_bC4Mark[4];
	int _allowMore;
	int _allowLess;
public:
	StickMarker_codeTrans(juce::String plcIP);
	~StickMarker_codeTrans() override;
	bool SendCmd(int addr, int val); //新卷需要向4204地址写0(结束卷)
	void Reconnect();
	void setEdition(int e) //设定版本
	{
		m_edition = e;
	}

	virtual void run();
public:
	void timerCallback() override;
	NumCount m_prevstickNumCount[4];
	bool IsConnected() { return m_bIsConnected; }
	void ClearLessMore();
private:
	bool m_bIsConnected;
private:
	
public:
	std::unique_ptr<juce::TextEditor> juce__textEditor;
	bool m_linkState;
private:
	juce::String WarningInfo[50];

	std::bitset<32> m_bState;
	std::bitset<32> m_prebState;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StickMarker_codeTrans)
};