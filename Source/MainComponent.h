#pragma once

#include <JuceHeader.h>
#include"StickServer.h"
#include"LabelDataManager.h"
#include"WarningClient.h"
#include"CameraDataModel.h"
#include".\WarningUI\StickMarker.h"
#include"XMLConfig.h"

//#include "SocketMarker.h"
using namespace juce;

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/

// ������дͨ�����ݵ��߳���
// ���̰߳�ȫ��
class WriteCommData : public Thread
{

	juce::FileLogger* p_log;
public:
	WriteCommData(int maxDataCapacity = 10000);
	~WriteCommData();

	virtual void run();
	
	// ������ݵ������У��ȴ�д�������ļ�
	// ������������������ʧ�ܣ�����false
	bool AddDebugData(const String& data);

	// �����δд�������
	void Clear();

protected:
	bool PickAData(String& data);

	juce::CriticalSection	_cs;
	juce::int64			_count;
	int						_maxCapacity;
	std::list<String>	_waitToWriteData;

};

class MainComponent : public juce::Component, public juce::Button::Listener,public juce::Timer
{

	bool _bSupportMarkCode;
	juce::int64 _globalPiece;
	double _EA_DefectCutPos[4];
	juce::int64 _preReportTime;
public:
	bool		m_bUseLocal; //�Լ�ʵ��4����
    juce::int64 m_sumEars;
	juce::int64 m_continue4StickEA[4];
	juce::int64 m_curEA[4];
	bool m_bSupportStick_Remain;//֧�ִ������
	int  m_iSupportCM5;
	int	 m_roadsSearch[4];
	double _EA_endPos[4];
    //==============================================================================
    MainComponent();
    ~MainComponent() override;
	void Save2MonthReport();
	String m_rollName;
	bool m_bNewUI;
    //==============================================================================
	void ResetAndNewRoll(String rollName);
	void SaveReport();
    void paint (juce::Graphics&) override;
    void resized() override;
    void buttonClicked(juce::Button*) override;
	void HandleMessage(int flag,double&); //��������Ϣ
    void HandleClientMessage(juce::String,HDataMessage* hmsg);
    void SetLabelStd(const TRANSER_OUT_LOGIC& msg);
    void CreateNewRoll();
    void timerCallback() override;
    int m_nRollMarks[2][2];
    std::vector<StickMarkInfo> m_stickInfos;
	std::vector<StickMarkInfo> m_stickInfoReserves; //�¾�ʱ���� 
    double m_preSentPos[2][2];
    juce::int64 m_sentCount[2];
    TRANSER_OUT_LOGIC m_labelStd;
    int g_iUsePLCSpeed;
	//SocketMarker*			_markingEng; //��īͨѶ
	juce::int64 preCmdTime[4];//4����ָ���յ�ʱ��
	int	  _pulseLast;
	int   _ioPorts[4]; //4��·��Ӧ��IO
	double m_EASumLen;
	String _strPLCIP;
	int		_edition;
public:
    std::unique_ptr<juce::Label>  Label_MarkCount[4];
    int Num_Mark[4];
	MainFormClient m_mfClient;
	juce::CriticalSection _finsLock;

	String _strCommuIP;
	void ReconnectAll();
private:
    //==============================================================================
    // Your private member variables go here...

	juce::int64 m_sent2ServerIndex;

    std::unique_ptr<juce::ToggleButton>  DO_Array[8][4];
    std::unique_ptr<juce::ToggleButton>  DI_Array[8][4];
    std::unique_ptr<juce::TextButton> NewRoll;
	std::unique_ptr<juce::TextEditor> rollEditor;
    std::unique_ptr<juce::TextButton> reloadConfig;

    CLabelerDataManager* CLabelDM[2];
    InterprocessStickServer sever;
    WarningClient client;
    juce::CriticalSection _objectLock;
    juce::TextButton* textButtonReset;
    juce::TextButton* MarkInfo;
    StickMarker* stickmarker;
    juce::TextButton* Reconnect;
    
	std::unique_ptr<juce::TextEditor> EAEditor;
	std::unique_ptr<juce::TextButton> EALabel;





    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};


