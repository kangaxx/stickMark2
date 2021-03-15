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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "StickMarker.h"
#include"..\PCI1230.h"
#include"..\WarningClient.h"
#include"..\clog.h"
#include"..\MainComponent.h"
extern MainComponent* g_MainComponent;
extern WarningClient* g_client;
extern juce::int64 _preCheckTime[4];
extern juce::int64 _handmStick[4];
extern juce::int64 _preStickTime[4];

extern double _preStickLenPos[4];
extern double _curRunningLenPos[4];
extern double g_halEA;
extern CXMLConfig m_configFile;
extern int isMarkerOnUse[4];
extern std::vector<NumCount> g_stickNumCount;
//extern std::mutex mute;
extern juce::CriticalSection mute;
//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]
StickMarker* g_StickMarker = nullptr;
bool g_isHBThreadRunning = false;
bool _threadShouldExit = false;

#define OMRON_COAT_LIB

void threadOfHeartBeat()
{
/*
	if (g_isHBThreadRunning)
		return;
	g_isHBThreadRunning = true;

	juce::int64 passTime = 0;
	int iWrite = 0;
	int m_pulse = 1;
	int nReadTime = 0;
	int failWrite = 0;
	while (true)
	{
		if (_threadShouldExit)
		{
			break;			// 退出线程信号
		}
		if (g_StickMarker->transferfins->IsConnected())
		{
			if (iWrite % 2 == 0)
			{
				iWrite = 0;
				m_pulse = !m_pulse;
				juce::int64 curTime = Time::currentTimeMillis();
				g_StickMarker->m_linkState = g_StickMarker->transferfins->SetDWordData(4200, m_pulse);//与plc通信心跳
				if (nReadTime < 101)
				{
					passTime += (Time::currentTimeMillis() - curTime);
					nReadTime++;
				}
				if (!g_StickMarker->m_linkState)
				{
					failWrite++;
				}
				else
				{
					failWrite = 0;
				}
				if (failWrite > 4)
				{
					g_StickMarker->transferfins->Disconnect();
					juce::Thread::sleep(2000);
					g_StickMarker->transferfins->Connect();
					failWrite = 0;
				}
			}
			iWrite++;

			if (nReadTime == 100)
			{
				LOGWT("100次网络平均执行时间:%d ms", passTime);
			}
		}
		else
		{
			g_StickMarker->transferfins->Disconnect();
			juce::Thread::sleep(2000);
			g_StickMarker->transferfins->Connect();
			failWrite = 0;
			LOGWT("重新连接");
		}

		juce::Thread::sleep(80);
	}
*/
}
//==============================================================================
StickMarker::StickMarker(juce::String plcIP):juce::Thread("")
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]
	_strPLC = plcIP;
	m_bReconnect = false;

	m_bC4Mark[0] = false;
	m_bC4Mark[1] = false;
	m_bC4Mark[2] = false;
	m_bC4Mark[3] = false;
	m_bLocalC4 = true;
	_allowMore = 0;
	_allowLess = 0;
    juce__groupComponent.reset (new juce::GroupComponent (juce::String(),
                                                          juce::CharPointer_UTF8 ("\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xbaPLC\xe6\x8a\xa5\xe8\xad\xa6\xe7\x8a\xb6\xe6\x80\x81")));
    addAndMakeVisible (juce__groupComponent.get());
    juce__groupComponent->setColour (juce::GroupComponent::outlineColourId, juce::Colours::white);
    juce__groupComponent->setColour (juce::GroupComponent::textColourId, juce::Colours::black);

    juce__textButton.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (juce__textButton.get());
    juce__textButton->setButtonText (juce::CharPointer_UTF8 ("\xe6\x8a\xa5\xe8\xad\xa6\xe5\xa4\x8d\xe4\xbd\x8d"));
    juce__textButton->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnTop | juce::Button::ConnectedOnBottom);
    juce__textButton->addListener (this);
    juce__textButton->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff009cda));
    juce__textButton->setColour (juce::TextButton::textColourOnId, juce::Colours::white);

    juce__textButton2.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (juce__textButton2.get());
    juce__textButton2->setButtonText (juce::CharPointer_UTF8 ("\xe8\xae\xa1\xe6\x95\xb0\xe6\xb8\x85\xe9\x9b\xb6"));
    juce__textButton2->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnTop | juce::Button::ConnectedOnBottom);
    juce__textButton2->addListener (this);
    juce__textButton2->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff009cda));
    juce__textButton2->setColour (juce::TextButton::textColourOnId, juce::Colours::white);

    juce__label.reset (new juce::Label ("new label",
                                        juce::CharPointer_UTF8 ("1\xe5\x8f\xb7\xe6\xbc\x8f\xe6\xa0\x87\xe6\x8a\xa5\xe8\xad\xa6(1\xe5\x8f\xb7\xe8\xb4\xb4\xe6\xa0\x87\xe8\x89\xb2\xe6\xa0\x87\xe6\x84\x9f\xe5\xba\x94\xe5\x99\xa8\xe6\x9c\xaa\xe6\x84\x9f\xe5\xba\x94\xe5\x88\xb0)")));
    addAndMakeVisible (juce__label.get());
    juce__label->setFont (juce::Font (14.90f, juce::Font::plain).withTypefaceStyle ("Regular"));
    juce__label->setJustificationType (juce::Justification::centred);
    juce__label->setEditable (false, false, false);
    juce__label->setColour (juce::Label::backgroundColourId, juce::Colour (0x00009cda));
    juce__label->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label2.reset (new juce::Label ("new label",
                                         juce::CharPointer_UTF8 ("2\xe5\x8f\xb7\xe6\xbc\x8f\xe6\xa0\x87\xe6\x8a\xa5\xe8\xad\xa6(2\xe5\x8f\xb7\xe8\xb4\xb4\xe6\xa0\x87\xe8\x89\xb2\xe6\xa0\x87\xe6\x84\x9f\xe5\xba\x94\xe5\x99\xa8\xe6\x9c\xaa\xe6\x84\x9f\xe5\xba\x94\xe5\x88\xb0)")));
    addAndMakeVisible (juce__label2.get());
    juce__label2->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    juce__label2->setJustificationType (juce::Justification::centred);
    juce__label2->setEditable (false, false, false);
    juce__label2->setColour (juce::Label::backgroundColourId, juce::Colour (0x00009cda));
    juce__label2->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label2->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label2->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label3.reset (new juce::Label ("new label",
                                         juce::CharPointer_UTF8 ("3\xe5\x8f\xb7\xe6\xbc\x8f\xe6\xa0\x87\xe6\x8a\xa5\xe8\xad\xa6(3\xe5\x8f\xb7\xe8\xb4\xb4\xe6\xa0\x87\xe8\x89\xb2\xe6\xa0\x87\xe6\x84\x9f\xe5\xba\x94\xe5\x99\xa8\xe6\x9c\xaa\xe6\x84\x9f\xe5\xba\x94\xe5\x88\xb0)")));
    addAndMakeVisible (juce__label3.get());
    juce__label3->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    juce__label3->setJustificationType (juce::Justification::centred);
    juce__label3->setEditable (false, false, false);
    juce__label3->setColour (juce::Label::backgroundColourId, juce::Colour (0x00009cda));
    juce__label3->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label3->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label3->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label4.reset (new juce::Label ("new label",
                                         juce::CharPointer_UTF8 ("4\xe5\x8f\xb7\xe6\xbc\x8f\xe6\xa0\x87\xe6\x8a\xa5\xe8\xad\xa6(4\xe5\x8f\xb7\xe8\xb4\xb4\xe6\xa0\x87\xe8\x89\xb2\xe6\xa0\x87\xe6\x84\x9f\xe5\xba\x94\xe5\x99\xa8\xe6\x9c\xaa\xe6\x84\x9f\xe5\xba\x94\xe5\x88\xb0)")));
    addAndMakeVisible (juce__label4.get());
    juce__label4->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    juce__label4->setJustificationType (juce::Justification::centred);
    juce__label4->setEditable (false, false, false);
    juce__label4->setColour (juce::Label::backgroundColourId, juce::Colour (0x00009cda));
    juce__label4->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label4->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label4->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label5.reset (new juce::Label ("new label",
                                         juce::CharPointer_UTF8 ("1\xe5\x8f\xb7\xe6\x95\xb4\xe5\xae\x9a\xe5\xa4\xb1\xe8\xb4\xa5\xe6\x8a\xa5\xe8\xad\xa6(\xe4\xb8\xbb\xe5\x8a\xa8\xe8\xbe\x8a\xe6\x9c\xaa\xe4\xb8\x8b\xe5\x8e\x8b\xe6\x88\x96\xe6\xb2\xa1\xe6\xa0\x87\xe7\xad\xbe)")));
    addAndMakeVisible (juce__label5.get());
    juce__label5->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    juce__label5->setJustificationType (juce::Justification::centred);
    juce__label5->setEditable (false, false, false);
    juce__label5->setColour (juce::Label::backgroundColourId, juce::Colour (0x00009cda));
    juce__label5->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label5->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label5->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label6.reset (new juce::Label ("new label",
                                         juce::CharPointer_UTF8 ("2\xe5\x8f\xb7\xe6\x95\xb4\xe5\xae\x9a\xe5\xa4\xb1\xe8\xb4\xa5\xe6\x8a\xa5\xe8\xad\xa6(\xe4\xb8\xbb\xe5\x8a\xa8\xe8\xbe\x8a\xe6\x9c\xaa\xe4\xb8\x8b\xe5\x8e\x8b\xe6\x88\x96\xe6\xb2\xa1\xe6\xa0\x87\xe7\xad\xbe)")));
    addAndMakeVisible (juce__label6.get());
    juce__label6->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    juce__label6->setJustificationType (juce::Justification::centred);
    juce__label6->setEditable (false, false, false);
    juce__label6->setColour (juce::Label::backgroundColourId, juce::Colour (0x00009cda));
    juce__label6->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label6->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label6->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label7.reset (new juce::Label ("new label",
                                         juce::CharPointer_UTF8 ("3\xe5\x8f\xb7\xe6\x95\xb4\xe5\xae\x9a\xe5\xa4\xb1\xe8\xb4\xa5\xe6\x8a\xa5\xe8\xad\xa6(\xe4\xb8\xbb\xe5\x8a\xa8\xe8\xbe\x8a\xe6\x9c\xaa\xe4\xb8\x8b\xe5\x8e\x8b\xe6\x88\x96\xe6\xb2\xa1\xe6\xa0\x87\xe7\xad\xbe)")));
    addAndMakeVisible (juce__label7.get());
    juce__label7->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    juce__label7->setJustificationType (juce::Justification::centred);
    juce__label7->setEditable (false, false, false);
    juce__label7->setColour (juce::Label::backgroundColourId, juce::Colour (0x00009cda));
    juce__label7->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label7->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label7->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label8.reset (new juce::Label ("new label",
                                         juce::CharPointer_UTF8 ("4\xe5\x8f\xb7\xe6\x95\xb4\xe5\xae\x9a\xe5\xa4\xb1\xe8\xb4\xa5\xe6\x8a\xa5\xe8\xad\xa6(\xe4\xb8\xbb\xe5\x8a\xa8\xe8\xbe\x8a\xe6\x9c\xaa\xe4\xb8\x8b\xe5\x8e\x8b\xe6\x88\x96\xe6\xb2\xa1\xe6\xa0\x87\xe7\xad\xbe)")));
    addAndMakeVisible (juce__label8.get());
    juce__label8->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    juce__label8->setJustificationType (juce::Justification::centred);
    juce__label8->setEditable (false, false, false);
    juce__label8->setColour (juce::Label::backgroundColourId, juce::Colour (0x00009cda));
    juce__label8->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label8->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label8->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label9.reset (new juce::Label ("new label",
                                         juce::CharPointer_UTF8 ("1\xe5\x8f\xb7\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba\xe7\xb4\xaf\xe5\x8a\xa0\xe8\xb4\xb4\xe6\xa0\x87\xe6\x8a\xa5\xe8\xad\xa6(1\xe5\x8f\xb7\xe8\xb4\xb4\xe6\xa0\x87\xe8\xb6\x85\xe4\xb8\x8a\xe9\x99\x90)")));
    addAndMakeVisible (juce__label9.get());
    juce__label9->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    juce__label9->setJustificationType (juce::Justification::centred);
    juce__label9->setEditable (false, false, false);
    juce__label9->setColour (juce::Label::backgroundColourId, juce::Colour (0x00009cda));
    juce__label9->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label9->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label9->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label10.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("2\xe5\x8f\xb7\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba\xe7\xb4\xaf\xe5\x8a\xa0\xe8\xb4\xb4\xe6\xa0\x87\xe6\x8a\xa5\xe8\xad\xa6(2\xe5\x8f\xb7\xe8\xb4\xb4\xe6\xa0\x87\xe8\xb6\x85\xe4\xb8\x8a\xe9\x99\x90)")));
    addAndMakeVisible (juce__label10.get());
    juce__label10->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    juce__label10->setJustificationType (juce::Justification::centred);
    juce__label10->setEditable (false, false, false);
    juce__label10->setColour (juce::Label::backgroundColourId, juce::Colour (0x00009cda));
    juce__label10->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label10->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label10->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label11.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("3\xe5\x8f\xb7\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba\xe7\xb4\xaf\xe5\x8a\xa0\xe8\xb4\xb4\xe6\xa0\x87\xe6\x8a\xa5\xe8\xad\xa6(3\xe5\x8f\xb7\xe8\xb4\xb4\xe6\xa0\x87\xe8\xb6\x85\xe4\xb8\x8a\xe9\x99\x90)")));
    addAndMakeVisible (juce__label11.get());
    juce__label11->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    juce__label11->setJustificationType (juce::Justification::centred);
    juce__label11->setEditable (false, false, false);
    juce__label11->setColour (juce::Label::backgroundColourId, juce::Colour (0x00009cda));
    juce__label11->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label11->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label11->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label12.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("4\xe5\x8f\xb7\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba\xe7\xb4\xaf\xe5\x8a\xa0\xe8\xb4\xb4\xe6\xa0\x87\xe6\x8a\xa5\xe8\xad\xa6(4\xe5\x8f\xb7\xe8\xb4\xb4\xe6\xa0\x87\xe8\xb6\x85\xe4\xb8\x8a\xe9\x99\x90)")));
    addAndMakeVisible (juce__label12.get());
    juce__label12->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    juce__label12->setJustificationType (juce::Justification::centred);
    juce__label12->setEditable (false, false, false);
    juce__label12->setColour (juce::Label::backgroundColourId, juce::Colour (0x00009cda));
    juce__label12->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label12->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label12->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__toggleButton.reset (new juce::ToggleButton ("new toggle button"));
    addAndMakeVisible (juce__toggleButton.get());
    juce__toggleButton->setButtonText (juce::CharPointer_UTF8 ("\xe4\xbf\xa1\xe5\x8f\xb7\xe6\xb5\x8b\xe8\xaf\x95"));
    juce__toggleButton->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnTop | juce::Button::ConnectedOnBottom);
    juce__toggleButton->addListener (this);
    juce__toggleButton->setColour (juce::ToggleButton::textColourId, juce::Colours::black);

    juce__label13.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("1\xe5\x8f\xb7\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba")));
    addAndMakeVisible (juce__label13.get());
    juce__label13->setFont (juce::Font (11.50f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label13->setJustificationType (juce::Justification::centredLeft);
    juce__label13->setEditable (false, false, false);
    juce__label13->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label13->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label13->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__textButton3.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (juce__textButton3.get());
    juce__textButton3->setButtonText (juce::CharPointer_UTF8 ("\xe5\x85\xb3\xe9\x97\xad"));
    juce__textButton3->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnTop | juce::Button::ConnectedOnBottom);
    juce__textButton3->addListener (this);
    juce__textButton3->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff009cda));
    juce__textButton3->setColour (juce::TextButton::textColourOnId, juce::Colours::white);

    juce__textButton4.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (juce__textButton4.get());
    juce__textButton4->setButtonText (juce::CharPointer_UTF8 ("\xe5\xbc\x80\xe5\x90\xaf"));
    juce__textButton4->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnTop | juce::Button::ConnectedOnBottom);
    juce__textButton4->addListener (this);
    juce__textButton4->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff009cda));
    juce__textButton4->setColour (juce::TextButton::textColourOnId, juce::Colours::white);

    juce__label14.reset (new juce::Label ("new label",
                                          TRANS("0")));
    addAndMakeVisible (juce__label14.get());
    juce__label14->setFont (juce::Font (11.40f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label14->setJustificationType (juce::Justification::centred);
    juce__label14->setEditable (false, false, false);
    juce__label14->setColour (juce::Label::backgroundColourId, juce::Colour (0xff009cda));
    juce__label14->setColour (juce::Label::textColourId, juce::Colours::white);
    juce__label14->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label14->setColour (juce::TextEditor::backgroundColourId, juce::Colours::red);

    juce__label15.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("2\xe5\x8f\xb7\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba")));
    addAndMakeVisible (juce__label15.get());
    juce__label15->setFont (juce::Font (11.50f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label15->setJustificationType (juce::Justification::centredLeft);
    juce__label15->setEditable (false, false, false);
    juce__label15->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label15->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label15->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__textButton5.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (juce__textButton5.get());
    juce__textButton5->setButtonText (juce::CharPointer_UTF8 ("\xe5\x85\xb3\xe9\x97\xad"));
    juce__textButton5->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnTop | juce::Button::ConnectedOnBottom);
    juce__textButton5->addListener (this);
    juce__textButton5->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff009cda));
    juce__textButton5->setColour (juce::TextButton::textColourOnId, juce::Colours::white);

    juce__textButton6.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (juce__textButton6.get());
    juce__textButton6->setButtonText (juce::CharPointer_UTF8 ("\xe5\xbc\x80\xe5\x90\xaf"));
    juce__textButton6->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnTop | juce::Button::ConnectedOnBottom);
    juce__textButton6->addListener (this);
    juce__textButton6->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff009cda));
    juce__textButton6->setColour (juce::TextButton::textColourOnId, juce::Colours::white);

    juce__label16.reset (new juce::Label ("new label",
                                          TRANS("0")));
    addAndMakeVisible (juce__label16.get());
    juce__label16->setFont (juce::Font (11.40f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label16->setJustificationType (juce::Justification::centred);
    juce__label16->setEditable (false, false, false);
    juce__label16->setColour (juce::Label::backgroundColourId, juce::Colour (0xff009cda));
    juce__label16->setColour (juce::Label::textColourId, juce::Colours::white);
    juce__label16->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label16->setColour (juce::TextEditor::backgroundColourId, juce::Colours::red);

    juce__label17.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("3\xe5\x8f\xb7\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba")));
    addAndMakeVisible (juce__label17.get());
    juce__label17->setFont (juce::Font (11.50f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label17->setJustificationType (juce::Justification::centredLeft);
    juce__label17->setEditable (false, false, false);
    juce__label17->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label17->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label17->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__textButton7.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (juce__textButton7.get());
    juce__textButton7->setButtonText (juce::CharPointer_UTF8 ("\xe5\x85\xb3\xe9\x97\xad"));
    juce__textButton7->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnTop | juce::Button::ConnectedOnBottom);
    juce__textButton7->addListener (this);
    juce__textButton7->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff009cda));
    juce__textButton7->setColour (juce::TextButton::textColourOnId, juce::Colours::white);

    juce__textButton8.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (juce__textButton8.get());
    juce__textButton8->setButtonText (juce::CharPointer_UTF8 ("\xe5\xbc\x80\xe5\x90\xaf"));
    juce__textButton8->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnTop | juce::Button::ConnectedOnBottom);
    juce__textButton8->addListener (this);
    juce__textButton8->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff009cda));
    juce__textButton8->setColour (juce::TextButton::textColourOnId, juce::Colours::white);

    juce__label18.reset (new juce::Label ("new label",
                                          TRANS("0")));
    addAndMakeVisible (juce__label18.get());
    juce__label18->setFont (juce::Font (11.40f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label18->setJustificationType (juce::Justification::centred);
    juce__label18->setEditable (false, false, false);
    juce__label18->setColour (juce::Label::backgroundColourId, juce::Colour (0xff009cda));
    juce__label18->setColour (juce::Label::textColourId, juce::Colours::white);
    juce__label18->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label18->setColour (juce::TextEditor::backgroundColourId, juce::Colours::red);

    juce__label19.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("4\xe5\x8f\xb7\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba")));
    addAndMakeVisible (juce__label19.get());
    juce__label19->setFont (juce::Font (11.50f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label19->setJustificationType (juce::Justification::centredLeft);
    juce__label19->setEditable (false, false, false);
    juce__label19->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label19->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label19->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__textButton9.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (juce__textButton9.get());
    juce__textButton9->setButtonText (juce::CharPointer_UTF8 ("\xe5\x85\xb3\xe9\x97\xad"));
    juce__textButton9->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnTop | juce::Button::ConnectedOnBottom);
    juce__textButton9->addListener (this);
    juce__textButton9->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff009cda));
    juce__textButton9->setColour (juce::TextButton::textColourOnId, juce::Colours::white);

    juce__textButton10.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (juce__textButton10.get());
    juce__textButton10->setButtonText (juce::CharPointer_UTF8 ("\xe5\xbc\x80\xe5\x90\xaf"));
    juce__textButton10->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnTop | juce::Button::ConnectedOnBottom);
    juce__textButton10->addListener (this);
    juce__textButton10->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff009cda));
    juce__textButton10->setColour (juce::TextButton::textColourOnId, juce::Colours::white);

    juce__label20.reset (new juce::Label ("new label",
                                          TRANS("0")));
    addAndMakeVisible (juce__label20.get());
    juce__label20->setFont (juce::Font (11.40f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label20->setJustificationType (juce::Justification::centred);
    juce__label20->setEditable (false, false, false);
    juce__label20->setColour (juce::Label::backgroundColourId, juce::Colour (0xff009cda));
    juce__label20->setColour (juce::Label::textColourId, juce::Colours::white);
    juce__label20->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label20->setColour (juce::TextEditor::backgroundColourId, juce::Colours::red);

    juce__label21.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("1\xe5\x8f\xb7\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba\xe6\xa0\x87\xe7\xad\xbe\xe6\x96\xad\xe5\xb8\xa6\xe6\x8a\xa5\xe8\xad\xa6(\xe8\x89\xb2\xe6\xa0\x87\xe8\xbf\x9e\xe7\xbb\xad\xe6\x84\x9f\xe5\xba\x94\xe5\x88\xb0\xe6\xa0\x87\xe7\xad\xbe""3\xe6\xac\xa1)")));
    addAndMakeVisible (juce__label21.get());
    juce__label21->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    juce__label21->setJustificationType (juce::Justification::centred);
    juce__label21->setEditable (false, false, false);
    juce__label21->setColour (juce::Label::backgroundColourId, juce::Colour (0x00009cda));
    juce__label21->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label21->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label21->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label22.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("2\xe5\x8f\xb7\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba\xe6\xa0\x87\xe7\xad\xbe\xe6\x96\xad\xe5\xb8\xa6\xe6\x8a\xa5\xe8\xad\xa6(\xe8\x89\xb2\xe6\xa0\x87\xe8\xbf\x9e\xe7\xbb\xad\xe6\x84\x9f\xe5\xba\x94\xe5\x88\xb0\xe6\xa0\x87\xe7\xad\xbe""3\xe6\xac\xa1)")));
    addAndMakeVisible (juce__label22.get());
    juce__label22->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    juce__label22->setJustificationType (juce::Justification::centred);
    juce__label22->setEditable (false, false, false);
    juce__label22->setColour (juce::Label::backgroundColourId, juce::Colour (0x00009cda));
    juce__label22->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label22->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label22->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label23.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("3\xe5\x8f\xb7\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba\xe6\xa0\x87\xe7\xad\xbe\xe6\x96\xad\xe5\xb8\xa6\xe6\x8a\xa5\xe8\xad\xa6(\xe8\x89\xb2\xe6\xa0\x87\xe8\xbf\x9e\xe7\xbb\xad\xe6\x84\x9f\xe5\xba\x94\xe5\x88\xb0\xe6\xa0\x87\xe7\xad\xbe""3\xe6\xac\xa1)")));
    addAndMakeVisible (juce__label23.get());
    juce__label23->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    juce__label23->setJustificationType (juce::Justification::centred);
    juce__label23->setEditable (false, false, false);
    juce__label23->setColour (juce::Label::backgroundColourId, juce::Colour (0x00009cda));
    juce__label23->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label23->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label23->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label24.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("4\xe5\x8f\xb7\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba\xe6\xa0\x87\xe7\xad\xbe\xe6\x96\xad\xe5\xb8\xa6\xe6\x8a\xa5\xe8\xad\xa6(\xe8\x89\xb2\xe6\xa0\x87\xe8\xbf\x9e\xe7\xbb\xad\xe6\x84\x9f\xe5\xba\x94\xe5\x88\xb0\xe6\xa0\x87\xe7\xad\xbe""3\xe6\xac\xa1)")));
    addAndMakeVisible (juce__label24.get());
    juce__label24->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    juce__label24->setJustificationType (juce::Justification::centred);
    juce__label24->setEditable (false, false, false);
    juce__label24->setColour (juce::Label::backgroundColourId, juce::Colour (0x00009cda));
    juce__label24->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label24->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label24->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label25.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("1\xe5\x8f\xb7\xe5\x87\xba\xe7\x8e\xb0\xe8\xbf\x9e\xe7\xbb\xad\xe6\x89\x93\xe6\xa0\x87\xe6\x8a\xa5\xe8\xad\xa6\xe5\x81\x9c\xe6\x9c\xba")));
    addAndMakeVisible (juce__label25.get());
    juce__label25->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    juce__label25->setJustificationType (juce::Justification::centred);
    juce__label25->setEditable (false, false, false);
    juce__label25->setColour (juce::Label::backgroundColourId, juce::Colour (0x00009cda));
    juce__label25->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label25->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label25->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label26.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("2\xe5\x8f\xb7\xe5\x87\xba\xe7\x8e\xb0\xe8\xbf\x9e\xe7\xbb\xad\xe6\x89\x93\xe6\xa0\x87\xe6\x8a\xa5\xe8\xad\xa6\xe5\x81\x9c\xe6\x9c\xba")));
    addAndMakeVisible (juce__label26.get());
    juce__label26->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    juce__label26->setJustificationType (juce::Justification::centred);
    juce__label26->setEditable (false, false, false);
    juce__label26->setColour (juce::Label::backgroundColourId, juce::Colour (0x00009cda));
    juce__label26->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label26->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label26->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label27.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("3\xe5\x8f\xb7\xe5\x87\xba\xe7\x8e\xb0\xe8\xbf\x9e\xe7\xbb\xad\xe6\x89\x93\xe6\xa0\x87\xe6\x8a\xa5\xe8\xad\xa6\xe5\x81\x9c\xe6\x9c\xba")));
    addAndMakeVisible (juce__label27.get());
    juce__label27->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    juce__label27->setJustificationType (juce::Justification::centred);
    juce__label27->setEditable (false, false, false);
    juce__label27->setColour (juce::Label::backgroundColourId, juce::Colour (0x00009cda));
    juce__label27->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label27->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label27->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label28.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("4\xe5\x8f\xb7\xe5\x87\xba\xe7\x8e\xb0\xe8\xbf\x9e\xe7\xbb\xad\xe6\x89\x93\xe6\xa0\x87\xe6\x8a\xa5\xe8\xad\xa6\xe5\x81\x9c\xe6\x9c\xba")));
    addAndMakeVisible (juce__label28.get());
    juce__label28->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    juce__label28->setJustificationType (juce::Justification::centred);
    juce__label28->setEditable (false, false, false);
    juce__label28->setColour (juce::Label::backgroundColourId, juce::Colour (0x00009cda));
    juce__label28->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label28->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label28->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label29.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("\xe6\x80\xbb\xe8\xb4\xb4\xe6\xa0\x87\xe6\x95\xb0\xe8\xb6\x85\xe9\x87\x8f")));
    addAndMakeVisible (juce__label29.get());
    juce__label29->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    juce__label29->setJustificationType (juce::Justification::centred);
    juce__label29->setEditable (false, false, false);
    juce__label29->setColour (juce::Label::backgroundColourId, juce::Colour (0x00009cda));
    juce__label29->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label29->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label29->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label30.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("\xe6\x9c\x8d\xe5\x8a\xa1\xe5\x99\xa8\xe5\x92\x8cPLC\xe7\xbd\x91\xe7\xbb\x9c\xe9\x80\x9a\xe8\xae\xaf\xe6\x96\xad\xe5\xbc\x80\xe6\x8a\xa5\xe8\xad\xa6")));
    addAndMakeVisible (juce__label30.get());
    juce__label30->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    juce__label30->setJustificationType (juce::Justification::centred);
    juce__label30->setEditable (false, false, false);
    juce__label30->setColour (juce::Label::backgroundColourId, juce::Colour (0x00009cda));
    juce__label30->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label30->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label30->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label31.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba\xe4\xbc\xba\xe6\x9c\x8d\xe9\xa9\xb1\xe5\x8a\xa8\xe5\x99\xa8\xe6\x8a\xa5\xe8\xad\xa6(\xe6\xa3\x80\xe6\x9f\xa5\xe9\xa9\xb1\xe5\x8a\xa8\xe5\x99\xa8)")));
    addAndMakeVisible (juce__label31.get());
    juce__label31->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    juce__label31->setJustificationType (juce::Justification::centred);
    juce__label31->setEditable (false, false, false);
    juce__label31->setColour (juce::Label::backgroundColourId, juce::Colour (0x00009cda));
    juce__label31->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label31->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label31->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__groupComponent2.reset (new juce::GroupComponent ("new group",
                                                           juce::CharPointer_UTF8 ("\xe4\xb8\x80\xe9\x94\xae\xe6\x95\xb4\xe5\xae\x9a\xe6\xa0\x87\xe7\xad\xbe")));
    addAndMakeVisible (juce__groupComponent2.get());
    juce__groupComponent2->setColour (juce::GroupComponent::outlineColourId, juce::Colours::white);

    juce__textButton11.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (juce__textButton11.get());
    juce__textButton11->setButtonText (juce::CharPointer_UTF8 ("\xe8\xbd\xb4""1"));
    juce__textButton11->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnTop | juce::Button::ConnectedOnBottom);
    juce__textButton11->addListener (this);
    juce__textButton11->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff009cda));
    juce__textButton11->setColour (juce::TextButton::textColourOnId, juce::Colours::white);

    juce__textButton12.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (juce__textButton12.get());
    juce__textButton12->setButtonText (juce::CharPointer_UTF8 ("\xe8\xbd\xb4""2"));
    juce__textButton12->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnTop | juce::Button::ConnectedOnBottom);
    juce__textButton12->addListener (this);
    juce__textButton12->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff009cda));
    juce__textButton12->setColour (juce::TextButton::textColourOnId, juce::Colours::white);

    juce__textButton13.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (juce__textButton13.get());
    juce__textButton13->setButtonText (juce::CharPointer_UTF8 ("\xe8\xbd\xb4""3"));
    juce__textButton13->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnTop | juce::Button::ConnectedOnBottom);
    juce__textButton13->addListener (this);
    juce__textButton13->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff009cda));
    juce__textButton13->setColour (juce::TextButton::textColourOnId, juce::Colours::white);

    juce__textButton14.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (juce__textButton14.get());
    juce__textButton14->setButtonText (juce::CharPointer_UTF8 ("\xe8\xbd\xb4""4"));
    juce__textButton14->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnTop | juce::Button::ConnectedOnBottom);
    juce__textButton14->addListener (this);
    juce__textButton14->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff009cda));
    juce__textButton14->setColour (juce::TextButton::textColourOnId, juce::Colours::white);

    juce__label32.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("1\xe5\x8f\xb7\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba:")));
    addAndMakeVisible (juce__label32.get());
    juce__label32->setFont (juce::Font (11.50f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label32->setJustificationType (juce::Justification::centredLeft);
    juce__label32->setEditable (false, false, false);
    juce__label32->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label32->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label32->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label33.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba\xe4\xb8\xaa\xe6\x95\xb0")));
    addAndMakeVisible (juce__label33.get());
    juce__label33->setFont (juce::Font (11.50f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label33->setJustificationType (juce::Justification::centredLeft);
    juce__label33->setEditable (false, false, false);
    juce__label33->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label33->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label33->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label34.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba\xe6\x94\xb6\xe5\x88\xb0\xe4\xbf\xa1\xe5\x8f\xb7\xe4\xb8\xaa\xe6\x95\xb0")));
    addAndMakeVisible (juce__label34.get());
    juce__label34->setFont (juce::Font (11.50f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label34->setJustificationType (juce::Justification::centredLeft);
    juce__label34->setEditable (false, false, false);
    juce__label34->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label34->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label34->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label35.reset (new juce::Label ("new label",
                                          TRANS("0")));
    addAndMakeVisible (juce__label35.get());
    juce__label35->setFont (juce::Font (11.40f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label35->setJustificationType (juce::Justification::centred);
    juce__label35->setEditable (false, false, false);
    juce__label35->setColour (juce::Label::backgroundColourId, juce::Colour (0xff009cda));
    juce__label35->setColour (juce::Label::textColourId, juce::Colours::white);
    juce__label35->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label35->setColour (juce::TextEditor::backgroundColourId, juce::Colours::red);

    juce__label36.reset (new juce::Label ("new label",
                                          TRANS("0")));
    addAndMakeVisible (juce__label36.get());
    juce__label36->setFont (juce::Font (11.40f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label36->setJustificationType (juce::Justification::centred);
    juce__label36->setEditable (false, false, false);
    juce__label36->setColour (juce::Label::backgroundColourId, juce::Colour (0xff009cda));
    juce__label36->setColour (juce::Label::textColourId, juce::Colours::white);
    juce__label36->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label36->setColour (juce::TextEditor::backgroundColourId, juce::Colours::red);

    juce__label37.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("2\xe5\x8f\xb7\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba:")));
    addAndMakeVisible (juce__label37.get());
    juce__label37->setFont (juce::Font (11.50f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label37->setJustificationType (juce::Justification::centredLeft);
    juce__label37->setEditable (false, false, false);
    juce__label37->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label37->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label37->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label38.reset (new juce::Label ("new label",
                                          TRANS("0")));
    addAndMakeVisible (juce__label38.get());
    juce__label38->setFont (juce::Font (11.40f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label38->setJustificationType (juce::Justification::centred);
    juce__label38->setEditable (false, false, false);
    juce__label38->setColour (juce::Label::backgroundColourId, juce::Colour (0xff009cda));
    juce__label38->setColour (juce::Label::textColourId, juce::Colours::white);
    juce__label38->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label38->setColour (juce::TextEditor::backgroundColourId, juce::Colours::red);

    juce__label39.reset (new juce::Label ("new label",
                                          TRANS("0")));
    addAndMakeVisible (juce__label39.get());
    juce__label39->setFont (juce::Font (11.40f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label39->setJustificationType (juce::Justification::centred);
    juce__label39->setEditable (false, false, false);
    juce__label39->setColour (juce::Label::backgroundColourId, juce::Colour (0xff009cda));
    juce__label39->setColour (juce::Label::textColourId, juce::Colours::white);
    juce__label39->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label39->setColour (juce::TextEditor::backgroundColourId, juce::Colours::red);

    juce__label40.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("3\xe5\x8f\xb7\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba:")));
    addAndMakeVisible (juce__label40.get());
    juce__label40->setFont (juce::Font (11.50f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label40->setJustificationType (juce::Justification::centredLeft);
    juce__label40->setEditable (false, false, false);
    juce__label40->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label40->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label40->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label41.reset (new juce::Label ("new label",
                                          TRANS("0")));
    addAndMakeVisible (juce__label41.get());
    juce__label41->setFont (juce::Font (11.40f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label41->setJustificationType (juce::Justification::centred);
    juce__label41->setEditable (false, false, false);
    juce__label41->setColour (juce::Label::backgroundColourId, juce::Colour (0xff009cda));
    juce__label41->setColour (juce::Label::textColourId, juce::Colours::white);
    juce__label41->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label41->setColour (juce::TextEditor::backgroundColourId, juce::Colours::red);

    juce__label42.reset (new juce::Label ("new label",
                                          TRANS("0")));
    addAndMakeVisible (juce__label42.get());
    juce__label42->setFont (juce::Font (11.40f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label42->setJustificationType (juce::Justification::centred);
    juce__label42->setEditable (false, false, false);
    juce__label42->setColour (juce::Label::backgroundColourId, juce::Colour (0xff009cda));
    juce__label42->setColour (juce::Label::textColourId, juce::Colours::white);
    juce__label42->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label42->setColour (juce::TextEditor::backgroundColourId, juce::Colours::red);

    juce__label43.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("4\xe5\x8f\xb7\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba:")));
    addAndMakeVisible (juce__label43.get());
    juce__label43->setFont (juce::Font (11.50f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label43->setJustificationType (juce::Justification::centredLeft);
    juce__label43->setEditable (false, false, false);
    juce__label43->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label43->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label43->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label44.reset (new juce::Label ("new label",
                                          TRANS("0")));
    addAndMakeVisible (juce__label44.get());
    juce__label44->setFont (juce::Font (11.40f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label44->setJustificationType (juce::Justification::centred);
    juce__label44->setEditable (false, false, false);
    juce__label44->setColour (juce::Label::backgroundColourId, juce::Colour (0xff009cda));
    juce__label44->setColour (juce::Label::textColourId, juce::Colours::white);
    juce__label44->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label44->setColour (juce::TextEditor::backgroundColourId, juce::Colours::red);

    juce__label45.reset (new juce::Label ("new label",
                                          TRANS("0")));
    addAndMakeVisible (juce__label45.get());
    juce__label45->setFont (juce::Font (11.40f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label45->setJustificationType (juce::Justification::centred);
    juce__label45->setEditable (false, false, false);
    juce__label45->setColour (juce::Label::backgroundColourId, juce::Colour (0xff009cda));
    juce__label45->setColour (juce::Label::textColourId, juce::Colours::white);
    juce__label45->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label45->setColour (juce::TextEditor::backgroundColourId, juce::Colours::red);

    juce__label46.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("1\xe5\x8f\xb7sensor\xe6\x84\x9f\xe5\xba\x94\xe5\x88\xb0\xe6\xa0\x87\xe7\xad\xbe\xe6\x95\xb0\xe7\x9b\xae")));
    addAndMakeVisible (juce__label46.get());
    juce__label46->setFont (juce::Font (11.50f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label46->setJustificationType (juce::Justification::centredLeft);
    juce__label46->setEditable (false, false, false);
    juce__label46->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label46->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label46->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label47.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("2\xe5\x8f\xb7sensor\xe6\x84\x9f\xe5\xba\x94\xe5\x88\xb0\xe6\xa0\x87\xe7\xad\xbe\xe6\x95\xb0\xe7\x9b\xae")));
    addAndMakeVisible (juce__label47.get());
    juce__label47->setFont (juce::Font (11.50f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label47->setJustificationType (juce::Justification::centredLeft);
    juce__label47->setEditable (false, false, false);
    juce__label47->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label47->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label47->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label48.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("3\xe5\x8f\xb7sensor\xe6\x84\x9f\xe5\xba\x94\xe5\x88\xb0\xe6\xa0\x87\xe7\xad\xbe\xe6\x95\xb0\xe7\x9b\xae")));
    addAndMakeVisible (juce__label48.get());
    juce__label48->setFont (juce::Font (11.50f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label48->setJustificationType (juce::Justification::centredLeft);
    juce__label48->setEditable (false, false, false);
    juce__label48->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label48->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label48->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label49.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("4\xe5\x8f\xb7sensor\xe6\x84\x9f\xe5\xba\x94\xe5\x88\xb0\xe6\xa0\x87\xe7\xad\xbe\xe6\x95\xb0\xe7\x9b\xae")));
    addAndMakeVisible (juce__label49.get());
    juce__label49->setFont (juce::Font (11.50f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label49->setJustificationType (juce::Justification::centredLeft);
    juce__label49->setEditable (false, false, false);
    juce__label49->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label49->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label49->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label50.reset (new juce::Label ("new label",
                                          TRANS("0")));
    addAndMakeVisible (juce__label50.get());
    juce__label50->setFont (juce::Font (11.40f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label50->setJustificationType (juce::Justification::centred);
    juce__label50->setEditable (false, false, false);
    juce__label50->setColour (juce::Label::backgroundColourId, juce::Colour (0xff009cda));
    juce__label50->setColour (juce::Label::textColourId, juce::Colours::white);
    juce__label50->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label50->setColour (juce::TextEditor::backgroundColourId, juce::Colours::red);

    juce__label51.reset (new juce::Label ("new label",
                                          TRANS("0")));
    addAndMakeVisible (juce__label51.get());
    juce__label51->setFont (juce::Font (11.40f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label51->setJustificationType (juce::Justification::centred);
    juce__label51->setEditable (false, false, false);
    juce__label51->setColour (juce::Label::backgroundColourId, juce::Colour (0xff009cda));
    juce__label51->setColour (juce::Label::textColourId, juce::Colours::white);
    juce__label51->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label51->setColour (juce::TextEditor::backgroundColourId, juce::Colours::red);

    juce__label52.reset (new juce::Label ("new label",
                                          TRANS("0")));
    addAndMakeVisible (juce__label52.get());
    juce__label52->setFont (juce::Font (11.40f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label52->setJustificationType (juce::Justification::centred);
    juce__label52->setEditable (false, false, false);
    juce__label52->setColour (juce::Label::backgroundColourId, juce::Colour (0xff009cda));
    juce__label52->setColour (juce::Label::textColourId, juce::Colours::white);
    juce__label52->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label52->setColour (juce::TextEditor::backgroundColourId, juce::Colours::red);

    juce__label53.reset (new juce::Label ("new label",
                                          TRANS("0")));
    addAndMakeVisible (juce__label53.get());
    juce__label53->setFont (juce::Font (11.40f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label53->setJustificationType (juce::Justification::centred);
    juce__label53->setEditable (false, false, false);
    juce__label53->setColour (juce::Label::backgroundColourId, juce::Colour (0xff009cda));
    juce__label53->setColour (juce::Label::textColourId, juce::Colours::white);
    juce__label53->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label53->setColour (juce::TextEditor::backgroundColourId, juce::Colours::red);

    juce__label54.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("1\xe5\x8f\xb7\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba\xe6\x98\xaf\xe5\x90\xa6\xe5\x90\xaf\xe7\x94\xa8")));
    addAndMakeVisible (juce__label54.get());
    juce__label54->setFont (juce::Font (11.50f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label54->setJustificationType (juce::Justification::centredLeft);
    juce__label54->setEditable (false, false, false);
    juce__label54->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label54->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label54->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label55.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("2\xe5\x8f\xb7\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba\xe6\x98\xaf\xe5\x90\xa6\xe5\x90\xaf\xe7\x94\xa8")));
    addAndMakeVisible (juce__label55.get());
    juce__label55->setFont (juce::Font (11.50f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label55->setJustificationType (juce::Justification::centredLeft);
    juce__label55->setEditable (false, false, false);
    juce__label55->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label55->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label55->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label56.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("3\xe5\x8f\xb7\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba\xe6\x98\xaf\xe5\x90\xa6\xe5\x90\xaf\xe7\x94\xa8")));
    addAndMakeVisible (juce__label56.get());
    juce__label56->setFont (juce::Font (11.50f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label56->setJustificationType (juce::Justification::centredLeft);
    juce__label56->setEditable (false, false, false);
    juce__label56->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label56->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label56->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label57.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("4\xe5\x8f\xb7\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xba\xe6\x98\xaf\xe5\x90\xa6\xe5\x90\xaf\xe7\x94\xa8")));
    addAndMakeVisible (juce__label57.get());
    juce__label57->setFont (juce::Font (11.50f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label57->setJustificationType (juce::Justification::centredLeft);
    juce__label57->setEditable (false, false, false);
    juce__label57->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label57->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label57->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__groupComponent3.reset (new juce::GroupComponent ("new group",
                                                           juce::CharPointer_UTF8 ("\xe8\xb4\xb4\xe6\xa0\x87\xe6\x9c\xbaPLC\xe8\xbf\x9e\xe6\x8e\xa5")));
    addAndMakeVisible (juce__groupComponent3.get());
    juce__groupComponent3->setColour (juce::GroupComponent::outlineColourId, juce::Colours::white);

    juce__label58.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("PLC\xe8\xbf\x9e\xe6\x8e\xa5\xe7\x8a\xb6\xe6\x80\x81")));
    addAndMakeVisible (juce__label58.get());
    juce__label58->setFont (juce::Font (14.90f, juce::Font::plain).withTypefaceStyle ("Regular"));
    juce__label58->setJustificationType (juce::Justification::centredLeft);
    juce__label58->setEditable (false, false, false);
    juce__label58->setColour (juce::Label::backgroundColourId, juce::Colour (0x00009cda));
    juce__label58->setColour (juce::Label::textColourId, juce::Colours::white);
    juce__label58->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label58->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label59.reset (new juce::Label ("new label",
                                          TRANS("IP")));
    addAndMakeVisible (juce__label59.get());
    juce__label59->setFont (juce::Font (14.90f, juce::Font::plain).withTypefaceStyle ("Regular"));
    juce__label59->setJustificationType (juce::Justification::centredLeft);
    juce__label59->setEditable (false, false, false);
    juce__label59->setColour (juce::Label::backgroundColourId, juce::Colour (0x00009cda));
    juce__label59->setColour (juce::Label::textColourId, juce::Colours::white);
    juce__label59->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label59->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label60.reset (new juce::Label ("new label",
                                          TRANS("PORT")));
    addAndMakeVisible (juce__label60.get());
    juce__label60->setFont (juce::Font (14.90f, juce::Font::plain).withTypefaceStyle ("Regular"));
    juce__label60->setJustificationType (juce::Justification::centredLeft);
    juce__label60->setEditable (false, false, false);
    juce__label60->setColour (juce::Label::backgroundColourId, juce::Colour (0x00009cda));
    juce__label60->setColour (juce::Label::textColourId, juce::Colours::white);
    juce__label60->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label60->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label61.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("\xe8\xb4\xb4\xe6\xa0\x87\xe6\x8a\xa5\xe8\xad\xa6\xe6\x95\xb0\xe7\x9b\xae:")));
    addAndMakeVisible (juce__label61.get());
    juce__label61->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    juce__label61->setJustificationType (juce::Justification::centredLeft);
    juce__label61->setEditable (false, false, false);
    juce__label61->setColour (juce::Label::backgroundColourId, juce::Colour (0x00009cda));
    juce__label61->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label61->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label61->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label62.reset (new juce::Label ("new label",
                                          TRANS("0")));
    addAndMakeVisible (juce__label62.get());
    juce__label62->setFont (juce::Font (11.40f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label62->setJustificationType (juce::Justification::centred);
    juce__label62->setEditable (false, false, false);
    juce__label62->setColour (juce::Label::backgroundColourId, juce::Colour (0xff009cda));
    juce__label62->setColour (juce::Label::textColourId, juce::Colours::white);
    juce__label62->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label62->setColour (juce::TextEditor::backgroundColourId, juce::Colours::red);

    juce__label63.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("Sensor\xe6\x95\xb0\xe7\x9b\xae\xe5\xa4\x9a\xe4\xba\x8e\xe8\xb4\xb4\xe6\xa0\x87\xe6\x95\xb0\xe7\x9b\xae\xe6\x8a\xa5\xe8\xad\xa6:")));
    addAndMakeVisible (juce__label63.get());
    juce__label63->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    juce__label63->setJustificationType (juce::Justification::centredLeft);
    juce__label63->setEditable (false, false, false);
    juce__label63->setColour (juce::Label::backgroundColourId, juce::Colour (0x00009cda));
    juce__label63->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label63->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label63->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label64.reset (new juce::Label ("new label",
                                          juce::CharPointer_UTF8 ("Sensor\xe6\x95\xb0\xe7\x9b\xae\xe5\xb0\x91\xe4\xba\x8e\xe8\xb4\xb4\xe6\xa0\x87\xe6\x95\xb0\xe7\x9b\xae\xe6\x8a\xa5\xe8\xad\xa6:")));
    addAndMakeVisible (juce__label64.get());
    juce__label64->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    juce__label64->setJustificationType (juce::Justification::centredLeft);
    juce__label64->setEditable (false, false, false);
    juce__label64->setColour (juce::Label::backgroundColourId, juce::Colour (0x00009cda));
    juce__label64->setColour (juce::Label::textColourId, juce::Colours::black);
    juce__label64->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label64->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label65.reset (new juce::Label ("new label",
                                          TRANS("0")));
    addAndMakeVisible (juce__label65.get());
    juce__label65->setFont (juce::Font (11.40f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label65->setJustificationType (juce::Justification::centred);
    juce__label65->setEditable (true, false, false);
    juce__label65->setColour (juce::Label::backgroundColourId, juce::Colour (0xff009cda));
    juce__label65->setColour (juce::Label::textColourId, juce::Colours::white);
    juce__label65->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label65->setColour (juce::TextEditor::backgroundColourId, juce::Colours::red);

    juce__label66.reset (new juce::Label ("new label",
                                          TRANS("0")));
    addAndMakeVisible (juce__label66.get());
    juce__label66->setFont (juce::Font (11.40f, juce::Font::plain).withTypefaceStyle ("Regular").withExtraKerningFactor (-0.035f));
    juce__label66->setJustificationType (juce::Justification::centred);
    juce__label66->setEditable (true, false, false);
    juce__label66->setColour (juce::Label::backgroundColourId, juce::Colour (0xff009cda));
    juce__label66->setColour (juce::Label::textColourId, juce::Colours::white);
    juce__label66->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label66->setColour (juce::TextEditor::backgroundColourId, juce::Colours::red);

    juce__textButton15.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (juce__textButton15.get());
    juce__textButton15->setButtonText (juce::CharPointer_UTF8 ("\xe6\xb8\x85\xe7\xa9\xba\xe4\xbf\xa1\xe6\x81\xaf"));
    juce__textButton15->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnTop | juce::Button::ConnectedOnBottom);
    juce__textButton15->addListener (this);
    juce__textButton15->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff009cda));
    juce__textButton15->setColour (juce::TextButton::textColourOnId, juce::Colours::white);

    juce__textButton16.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (juce__textButton16.get());
    juce__textButton16->setButtonText (juce::CharPointer_UTF8 ("\xe4\xbf\x9d\xe5\xad\x98"));
    juce__textButton16->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnTop | juce::Button::ConnectedOnBottom);
    juce__textButton16->addListener (this);
    juce__textButton16->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff009cda));
    juce__textButton16->setColour (juce::TextButton::textColourOnId, juce::Colours::white);

    juce__textButton17.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (juce__textButton17.get());
    juce__textButton17->setButtonText (juce::CharPointer_UTF8 ("\xe9\x80\x80\xe5\x87\xba"));
    juce__textButton17->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnTop | juce::Button::ConnectedOnBottom);
    juce__textButton17->addListener (this);
    juce__textButton17->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff009cda));
    juce__textButton17->setColour (juce::TextButton::textColourOnId, juce::Colours::white);

    juce__textEditor.reset(new juce::TextEditor("new text editor"));
    addAndMakeVisible(juce__textEditor.get());
    juce__textEditor->setMultiLine(true);
    juce__textEditor->setReturnKeyStartsNewLine(false);
    juce__textEditor->setReadOnly(false);
    juce__textEditor->setScrollbarsShown(true);
    juce__textEditor->setCaretVisible(true);
    juce__textEditor->setPopupMenuEnabled(true);
    juce__textEditor->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0xff009cda));
    juce__textEditor->setText(juce::String());

    
    WarningInfo[0] = L"1号漏标报警(1号打标机色标感应器未感应到)";
    WarningInfo[1] = L"2号漏标报警(2号打标机色标感应器未感应到)";
    WarningInfo[2] = L"3号漏标报警(3号打标机色标感应器未感应到)";
    WarningInfo[3] = L"4号漏标报警(4号打标机色标感应器未感应到)";
    WarningInfo[4] = L"1号整定失败报警(主动棍未下压或没标签)";
    WarningInfo[5] = L"2号整定失败报警(主动棍未下压或没标签)";
    WarningInfo[6] = L"3号整定失败报警(主动棍未下压或没标签)";
    WarningInfo[7] = L"4号整定失败报警(主动棍未下压或没标签)";
    WarningInfo[8] = L"打标机伺服驱动器报警(检查驱动器)";
    WarningInfo[9] = L"总打标数超量";
    WarningInfo[10] = L"1号贴标机累加打标报警(1号打标超限)";
    WarningInfo[11] = L"2号贴标机累加打标报警(2号打标超限)";
    WarningInfo[12] = L"3号贴标机累加打标报警(3号打标超限)";
    WarningInfo[13] = L"4号贴标机累加打标报警(4号打标超限)";
    WarningInfo[14] = L"1号打标机未启用";
    WarningInfo[15] = L"2号打标机未启用";
    WarningInfo[16] = L"3号打标机未启用";
    WarningInfo[17] = L"4号打标机未启用";
    WarningInfo[18] = L"1号打标机标签断带报警(色标连续感应到标签3次)";
    WarningInfo[19] = L"2号打标机标签断带报警(色标连续感应到标签3次)";
    WarningInfo[20] = L"3号打标机标签断带报警(色标连续感应到标签3次)";
    WarningInfo[21] = L"4号打标机标签断带报警(色标连续感应到标签3次)";
    WarningInfo[22] = L"CCD上位机和打标机PLC网络通讯断开报警";
    WarningInfo[23] = L"1号出现连续打标报警停机";
    WarningInfo[24] = L"2号出现连续打标报警停机";
    WarningInfo[25] = L"3号出现连续打标报警停机";
    WarningInfo[26] = L"4号出现连续打标报警停机";
    WarningInfo[27] = L"轴1一键整定标签";
    WarningInfo[28] = L"轴2一键整定标签";
    WarningInfo[29] = L"轴2一键整定标签";
    WarningInfo[30] = L"轴2一键整定标签";

    Info = L"";
	m_bIsConnected = false;

 /* transferfins = new TransferFins(_strPLC.toStdString().c_str(), 9600);
    transferfins->Initialize();
    if (!transferfins->IsConnected())
        juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::WarningIcon, "", L"    连接到PLC失败");*/
        
    
	m_linkState = 0;
    m_pulse = 0;
    m_bState.reset();

    


    m_configFile.GetMarkerState(isMarkerOnUse);

    if (isMarkerOnUse[0])
    {
        juce__textButton4->setColour(TextButton::ColourIds::buttonColourId, Colours::green);
        juce__label14->setText(String(1), NotificationType::dontSendNotification);
    }

    else
    {
        juce__textButton3->setColour(TextButton::ColourIds::buttonColourId, Colours::red);
    }

    if (isMarkerOnUse[1])
    {
        juce__textButton6->setColour(TextButton::ColourIds::buttonColourId, Colours::green);
        juce__label16->setText(String(1), NotificationType::dontSendNotification);
    }

    else
    {
        juce__textButton5->setColour(TextButton::ColourIds::buttonColourId, Colours::red);
    }

    if (isMarkerOnUse[2])
    {
        juce__textButton8->setColour(TextButton::ColourIds::buttonColourId, Colours::green);
        juce__label18->setText(String(1), NotificationType::dontSendNotification);
    }

    else
    {
        juce__textButton7->setColour(TextButton::ColourIds::buttonColourId, Colours::red);
    }

    if (isMarkerOnUse[3])
    {
        juce__textButton10->setColour(TextButton::ColourIds::buttonColourId, Colours::green);
        juce__label20->setText(String(1), NotificationType::dontSendNotification);
    }

    else
    {
        juce__textButton9->setColour(TextButton::ColourIds::buttonColourId, Colours::red);
    }
    //[UserPreSize]
    //[/UserPreSize]


    

    //startTimer(1, 120);  //心跳timer
    //startTimer(2, 1000); //读打标数
    //startTimer(3, 100);  //检测报警信息
	g_StickMarker = this;

    memset(m_prevstickNumCount, 0, sizeof(NumCount) * 4);
    m_edition = 0;
    startThread();

    setSize (900, 600);
	

	
	// 心跳信号线程
	//std::thread th1(threadOfHeartBeat);
	//th1.detach();

	Thread::ThreadID pID = getThreadId();

	m_prebState = m_bState;

    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

StickMarker::~StickMarker()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    juce__groupComponent = nullptr;
    juce__textButton = nullptr;
    juce__textButton2 = nullptr;
    juce__label = nullptr;
    juce__label2 = nullptr;
    juce__label3 = nullptr;
    juce__label4 = nullptr;
    juce__label5 = nullptr;
    juce__label6 = nullptr;
    juce__label7 = nullptr;
    juce__label8 = nullptr;
    juce__label9 = nullptr;
    juce__label10 = nullptr;
    juce__label11 = nullptr;
    juce__label12 = nullptr;
    juce__toggleButton = nullptr;
    juce__label13 = nullptr;
    juce__textButton3 = nullptr;
    juce__textButton4 = nullptr;
    juce__label14 = nullptr;
    juce__label15 = nullptr;
    juce__textButton5 = nullptr;
    juce__textButton6 = nullptr;
    juce__label16 = nullptr;
    juce__label17 = nullptr;
    juce__textButton7 = nullptr;
    juce__textButton8 = nullptr;
    juce__label18 = nullptr;
    juce__label19 = nullptr;
    juce__textButton9 = nullptr;
    juce__textButton10 = nullptr;
    juce__label20 = nullptr;
    juce__label21 = nullptr;
    juce__label22 = nullptr;
    juce__label23 = nullptr;
    juce__label24 = nullptr;
    juce__label25 = nullptr;
    juce__label26 = nullptr;
    juce__label27 = nullptr;
    juce__label28 = nullptr;
    juce__label29 = nullptr;
    juce__label30 = nullptr;
    juce__label31 = nullptr;
    juce__groupComponent2 = nullptr;
    juce__textButton11 = nullptr;
    juce__textButton12 = nullptr;
    juce__textButton13 = nullptr;
    juce__textButton14 = nullptr;
    juce__label32 = nullptr;
    juce__label33 = nullptr;
    juce__label34 = nullptr;
    juce__label35 = nullptr;
    juce__label36 = nullptr;
    juce__label37 = nullptr;
    juce__label38 = nullptr;
    juce__label39 = nullptr;
    juce__label40 = nullptr;
    juce__label41 = nullptr;
    juce__label42 = nullptr;
    juce__label43 = nullptr;
    juce__label44 = nullptr;
    juce__label45 = nullptr;
    juce__label46 = nullptr;
    juce__label47 = nullptr;
    juce__label48 = nullptr;
    juce__label49 = nullptr;
    juce__label50 = nullptr;
    juce__label51 = nullptr;
    juce__label52 = nullptr;
    juce__label53 = nullptr;
    juce__label54 = nullptr;
    juce__label55 = nullptr;
    juce__label56 = nullptr;
    juce__label57 = nullptr;
    juce__groupComponent3 = nullptr;
    juce__label58 = nullptr;
    juce__label59 = nullptr;
    juce__label60 = nullptr;
    juce__label61 = nullptr;
    juce__label62 = nullptr;
    juce__label63 = nullptr;
    juce__label64 = nullptr;
    juce__label65 = nullptr;
    juce__label66 = nullptr;
    juce__textButton15 = nullptr;
    juce__textButton16 = nullptr;
    juce__textButton17 = nullptr;
    juce__textEditor = nullptr;
    
	//transferfins->UnInitialize();
	//delete transferfins;
	//transferfins = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void StickMarker::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff78d5ff));

    {
        int x = proportionOfWidth (0.5989f), y = proportionOfHeight (0.0335f), width = proportionOfWidth (0.3956f), height = proportionOfHeight (0.1506f);
        juce::Colour fillColour = juce::Colour (0xff009cda);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }



    {
        float x = static_cast<float> (proportionOfWidth(0.5495f)), y = static_cast<float> (proportionOfHeight(0.0586f)), width = static_cast<float> (proportionOfWidth(0.0110f)), height = static_cast<float> (proportionOfHeight(0.0167f));
        juce::Colour fillColour;
        if (m_bState[0])
            fillColour = Colours::red;
        else
            fillColour = Colours::green;
        g.setColour(fillColour);
        g.fillEllipse(x, y, width, height);
    }

    {
        float x = static_cast<float> (proportionOfWidth(0.5495f)), y = static_cast<float> (proportionOfHeight(0.1004f)), width = static_cast<float> (proportionOfWidth(0.0110f)), height = static_cast<float> (proportionOfHeight(0.0167f));
        juce::Colour fillColour;
        if (m_bState[1])
            fillColour = Colours::red;
        else
            fillColour = Colours::green;
        g.setColour(fillColour);
        g.fillEllipse(x, y, width, height);
    }

    {
        float x = static_cast<float> (proportionOfWidth(0.5495f)), y = static_cast<float> (proportionOfHeight(0.1423f)), width = static_cast<float> (proportionOfWidth(0.0110f)), height = static_cast<float> (proportionOfHeight(0.0167f));
        juce::Colour fillColour;
        if (m_bState[2])
            fillColour = Colours::red;
        else
            fillColour = Colours::green;
		g.setColour(fillColour);
        g.fillEllipse(x, y, width, height);
    }

    {
        float x = static_cast<float> (proportionOfWidth(0.5495f)), y = static_cast<float> (proportionOfHeight(0.1841f)), width = static_cast<float> (proportionOfWidth(0.0110f)), height = static_cast<float> (proportionOfHeight(0.0167f));
        juce::Colour fillColour;
        if (m_bState[3])
            fillColour = Colours::red;
        else
            fillColour = Colours::green;
        g.setColour(fillColour);
        g.fillEllipse(x, y, width, height);
    }

    {
        float x = static_cast<float> (proportionOfWidth(0.5495f)), y = static_cast<float> (proportionOfHeight(0.2259f)), width = static_cast<float> (proportionOfWidth(0.0110f)), height = static_cast<float> (proportionOfHeight(0.0167f));
        juce::Colour fillColour;
        if (m_bState[4])
            fillColour = Colours::red;
        else
            fillColour = Colours::green;
        g.setColour(fillColour);
        g.fillEllipse(x, y, width, height);
    }

    {
        float x = static_cast<float> (proportionOfWidth(0.5495f)), y = static_cast<float> (proportionOfHeight(0.2678f)), width = static_cast<float> (proportionOfWidth(0.0110f)), height = static_cast<float> (proportionOfHeight(0.0167f));
        juce::Colour fillColour;
        if (m_bState[5])
            fillColour = Colours::red;
        else
            fillColour = Colours::green;
        g.setColour(fillColour);
        g.fillEllipse(x, y, width, height);
    }

    {
        float x = static_cast<float> (proportionOfWidth(0.5495f)), y = static_cast<float> (proportionOfHeight(0.3096f)), width = static_cast<float> (proportionOfWidth(0.0110f)), height = static_cast<float> (proportionOfHeight(0.0167f));
        juce::Colour fillColour;
        if (m_bState[6])
            fillColour = Colours::red;
        else
            fillColour = Colours::green;
        g.setColour(fillColour);
        g.fillEllipse(x, y, width, height);
    }

    {
        float x = static_cast<float> (proportionOfWidth(0.5495f)), y = static_cast<float> (proportionOfHeight(0.3515f)), width = static_cast<float> (proportionOfWidth(0.0110f)), height = static_cast<float> (proportionOfHeight(0.0167f));
        juce::Colour fillColour;
        if (m_bState[7])
            fillColour = Colours::red;
        else
            fillColour = Colours::green;
        g.setColour(fillColour);
        g.fillEllipse(x, y, width, height);
    }

    {
        float x = static_cast<float> (proportionOfWidth(0.5495f)), y = static_cast<float> (proportionOfHeight(0.3933f)), width = static_cast<float> (proportionOfWidth(0.0110f)), height = static_cast<float> (proportionOfHeight(0.0167f));
        juce::Colour fillColour;
        if (m_bState[10])
            fillColour = Colours::red;
        else
            fillColour = Colours::green;
        g.setColour(fillColour);
        g.fillEllipse(x, y, width, height);
    }

    {
        float x = static_cast<float> (proportionOfWidth(0.5495f)), y = static_cast<float> (proportionOfHeight(0.4351f)), width = static_cast<float> (proportionOfWidth(0.0110f)), height = static_cast<float> (proportionOfHeight(0.0167f));
        juce::Colour fillColour;
        if (m_bState[11])
            fillColour = Colours::red;
        else
            fillColour = Colours::green;
        g.setColour(fillColour);
        g.fillEllipse(x, y, width, height);
    }

    {
        float x = static_cast<float> (proportionOfWidth(0.5495f)), y = static_cast<float> (proportionOfHeight(0.4770f)), width = static_cast<float> (proportionOfWidth(0.0110f)), height = static_cast<float> (proportionOfHeight(0.0167f));
        juce::Colour fillColour;
        if (m_bState[12])
            fillColour = Colours::red;
        else
            fillColour = Colours::green;
        g.setColour(fillColour);
        g.fillEllipse(x, y, width, height);
    }

    {
        float x = static_cast<float> (proportionOfWidth(0.5495f)), y = static_cast<float> (proportionOfHeight(0.5188f)), width = static_cast<float> (proportionOfWidth(0.0110f)), height = static_cast<float> (proportionOfHeight(0.0167f));
        juce::Colour fillColour;
        if (m_bState[13])
            fillColour = Colours::red;
        else
            fillColour = Colours::green;
        g.setColour(fillColour);
        g.fillEllipse(x, y, width, height);
    }

    {
        float x = static_cast<float> (proportionOfWidth(0.5495f)), y = static_cast<float> (proportionOfHeight(0.5607f)), width = static_cast<float> (proportionOfWidth(0.0110f)), height = static_cast<float> (proportionOfHeight(0.0167f));
        juce::Colour fillColour;
        if (m_bState[18])
            fillColour = Colours::red;
        else
            fillColour = Colours::green;
        g.setColour(fillColour);
        g.fillEllipse(x, y, width, height);
    }

    {
        float x = static_cast<float> (proportionOfWidth(0.5495f)), y = static_cast<float> (proportionOfHeight(0.6025f)), width = static_cast<float> (proportionOfWidth(0.0110f)), height = static_cast<float> (proportionOfHeight(0.0167f));
        juce::Colour fillColour;
        if (m_bState[19])
            fillColour = Colours::red;
        else
            fillColour = Colours::green;
        g.setColour(fillColour);
        g.fillEllipse(x, y, width, height);
    }

    {
        float x = static_cast<float> (proportionOfWidth(0.5495f)), y = static_cast<float> (proportionOfHeight(0.6444f)), width = static_cast<float> (proportionOfWidth(0.0110f)), height = static_cast<float> (proportionOfHeight(0.0167f));
        juce::Colour fillColour;
        if (m_bState[20])
            fillColour = Colours::red;
        else
            fillColour = Colours::green;
        g.setColour(fillColour);
        g.fillEllipse(x, y, width, height);
    }

    {
        float x = static_cast<float> (proportionOfWidth(0.5495f)), y = static_cast<float> (proportionOfHeight(0.6862f)), width = static_cast<float> (proportionOfWidth(0.0110f)), height = static_cast<float> (proportionOfHeight(0.0167f));
        juce::Colour fillColour;
        if (m_bState[21])
            fillColour = Colours::red;
        else
            fillColour = Colours::green;
        g.setColour(fillColour);
        g.fillEllipse(x, y, width, height);
    }

    {
        float x = static_cast<float> (proportionOfWidth(0.5495f)), y = static_cast<float> (proportionOfHeight(0.7280f)), width = static_cast<float> (proportionOfWidth(0.0110f)), height = static_cast<float> (proportionOfHeight(0.0167f));
        juce::Colour fillColour;
        if (m_bState[23])
            fillColour = Colours::red;
        else
            fillColour = Colours::green;
        g.setColour(fillColour);
        g.fillEllipse(x, y, width, height);
    }

    {
        float x = static_cast<float> (proportionOfWidth(0.5495f)), y = static_cast<float> (proportionOfHeight(0.7699f)), width = static_cast<float> (proportionOfWidth(0.0110f)), height = static_cast<float> (proportionOfHeight(0.0167f));
        juce::Colour fillColour;
        if (m_bState[24])
            fillColour = Colours::red;
        else
            fillColour = Colours::green;
        g.setColour(fillColour);
        g.fillEllipse(x, y, width, height);
    }

    {
        float x = static_cast<float> (proportionOfWidth(0.5495f)), y = static_cast<float> (proportionOfHeight(0.8117f)), width = static_cast<float> (proportionOfWidth(0.0110f)), height = static_cast<float> (proportionOfHeight(0.0167f));
        juce::Colour fillColour;
        if (m_bState[25])
            fillColour = Colours::red;
        else
            fillColour = Colours::green;
        g.setColour(fillColour);
        g.fillEllipse(x, y, width, height);
    }

    {
        float x = static_cast<float> (proportionOfWidth(0.5495f)), y = static_cast<float> (proportionOfHeight(0.8536f)), width = static_cast<float> (proportionOfWidth(0.0110f)), height = static_cast<float> (proportionOfHeight(0.0167f));
        juce::Colour fillColour;
        if (m_bState[26])
            fillColour = Colours::red;
        else
            fillColour = Colours::green;
        g.setColour(fillColour);
        g.fillEllipse(x, y, width, height);
    }

    {
        float x = static_cast<float> (proportionOfWidth(0.5495f)), y = static_cast<float> (proportionOfHeight(0.8870f)), width = static_cast<float> (proportionOfWidth(0.0110f)), height = static_cast<float> (proportionOfHeight(0.0167f));
        juce::Colour fillColour;
        if (m_bState[9])
            fillColour = Colours::red;
        else
            fillColour = Colours::green;
        g.setColour(fillColour);
        g.fillEllipse(x, y, width, height);
    }

    {
        float x = static_cast<float> (proportionOfWidth(0.5495f)), y = static_cast<float> (proportionOfHeight(0.9289f)), width = static_cast<float> (proportionOfWidth(0.0110f)), height = static_cast<float> (proportionOfHeight(0.0167f));
        juce::Colour fillColour;
        if (m_bState[22])
            fillColour = Colours::red;
        else
            fillColour = Colours::green;
        g.setColour(fillColour);
        g.fillEllipse(x, y, width, height);
    }

    {
        float x = static_cast<float> (proportionOfWidth(0.5495f)), y = static_cast<float> (proportionOfHeight(0.9707f)), width = static_cast<float> (proportionOfWidth(0.0110f)), height = static_cast<float> (proportionOfHeight(0.0167f));
        juce::Colour fillColour;
        if (m_bState[8])
            fillColour = Colours::red;
        else
            fillColour = Colours::green;
        g.setColour(fillColour);
        g.fillEllipse(x, y, width, height);
    }

    {
        float x = static_cast<float> (proportionOfWidth(0.1868f)), y = static_cast<float> (proportionOfHeight(0.8703f)), width = static_cast<float> (proportionOfWidth(0.0110f)), height = static_cast<float> (proportionOfHeight(0.0167f));
        juce::Colour fillColour;
        if (!m_bState[28])
            fillColour = Colours::red;
        else
            fillColour = Colours::green;
        g.setColour(fillColour);
        g.fillEllipse(x, y, width, height);
    }

    {
        float x = static_cast<float> (proportionOfWidth(0.1868f)), y = static_cast<float> (proportionOfHeight(0.9038f)), width = static_cast<float> (proportionOfWidth(0.0110f)), height = static_cast<float> (proportionOfHeight(0.0167f));
        juce::Colour fillColour;
        if (!m_bState[29])
            fillColour = Colours::red;
        else
            fillColour = Colours::green;
        g.setColour(fillColour);
        g.fillEllipse(x, y, width, height);
    }

    {
        float x = static_cast<float> (proportionOfWidth(0.1868f)), y = static_cast<float> (proportionOfHeight(0.9456f)), width = static_cast<float> (proportionOfWidth(0.0110f)), height = static_cast<float> (proportionOfHeight(0.0167f));
        juce::Colour fillColour;
        if (!m_bState[30])
            fillColour = Colours::red;
        else
            fillColour = Colours::green;
        g.setColour(fillColour);
        g.fillEllipse(x, y, width, height);
    }

    {
        float x = static_cast<float> (proportionOfWidth(0.1868f)), y = static_cast<float> (proportionOfHeight(0.9791f)), width = static_cast<float> (proportionOfWidth(0.0110f)), height = static_cast<float> (proportionOfHeight(0.0167f));
        juce::Colour fillColour;
        if (!m_bState[31])
            fillColour = Colours::red;
        else
            fillColour = Colours::green;
        g.setColour(fillColour);
        g.fillEllipse(x, y, width, height);
    }

    {
        float x = static_cast<float> (proportionOfWidth (0.7363f)), y = static_cast<float> (proportionOfHeight (0.1172f)), width = static_cast<float> (proportionOfWidth (0.0110f)), height = static_cast<float> (proportionOfHeight (0.0167f));
        juce::Colour fillColour = juce::Colours::red;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        if (m_linkState)
            g.setColour(juce::Colours::green);
        else
            g.setColour (fillColour);
        g.fillEllipse (x, y, width, height);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void StickMarker::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    juce__groupComponent->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.0167f), proportionOfWidth (0.5934f), proportionOfHeight (0.9875f));
    juce__textButton->setBounds (proportionOfWidth (0.0110f), proportionOfHeight (0.0670f), proportionOfWidth (0.0989f), proportionOfHeight (0.0502f));
    juce__textButton2->setBounds (proportionOfWidth (0.1209f), proportionOfHeight (0.0670f), proportionOfWidth (0.0989f), proportionOfHeight (0.0502f));
    juce__label->setBounds (proportionOfWidth (0.2363f), proportionOfHeight (0.0502f), proportionOfWidth (0.2967f), proportionOfHeight (0.0335f));
    juce__label2->setBounds (proportionOfWidth (0.2363f), proportionOfHeight (0.0921f), proportionOfWidth (0.2967f), proportionOfHeight (0.0335f));
    juce__label3->setBounds (proportionOfWidth (0.2363f), proportionOfHeight (0.1339f), proportionOfWidth (0.2967f), proportionOfHeight (0.0335f));
    juce__label4->setBounds (proportionOfWidth (0.2363f), proportionOfHeight (0.1757f), proportionOfWidth (0.2967f), proportionOfHeight (0.0335f));
    juce__label5->setBounds (proportionOfWidth (0.2473f), proportionOfHeight (0.2176f), proportionOfWidth (0.2857f), proportionOfHeight (0.0335f));
    juce__label6->setBounds (proportionOfWidth (0.2473f), proportionOfHeight (0.2594f), proportionOfWidth (0.2857f), proportionOfHeight (0.0335f));
    juce__label7->setBounds (proportionOfWidth (0.2473f), proportionOfHeight (0.3013f), proportionOfWidth (0.2857f), proportionOfHeight (0.0335f));
    juce__label8->setBounds (proportionOfWidth (0.2473f), proportionOfHeight (0.3431f), proportionOfWidth (0.2857f), proportionOfHeight (0.0335f));
    juce__label9->setBounds (proportionOfWidth (0.2582f), proportionOfHeight (0.3849f), proportionOfWidth (0.2747f), proportionOfHeight (0.0335f));
    juce__label10->setBounds (proportionOfWidth (0.2582f), proportionOfHeight (0.4268f), proportionOfWidth (0.2747f), proportionOfHeight (0.0335f));
    juce__label11->setBounds (proportionOfWidth (0.2582f), proportionOfHeight (0.4686f), proportionOfWidth (0.2747f), proportionOfHeight (0.0335f));
    juce__label12->setBounds (proportionOfWidth (0.2582f), proportionOfHeight (0.5105f), proportionOfWidth (0.2747f), proportionOfHeight (0.0335f));
    juce__toggleButton->setBounds (proportionOfWidth (0.0110f), proportionOfHeight (0.1255f), proportionOfWidth (0.0989f), proportionOfHeight (0.0251f));
    juce__label13->setBounds (proportionOfWidth (0.0055f), proportionOfHeight (0.1674f), proportionOfWidth (0.0769f), proportionOfHeight (0.0251f));
    juce__textButton3->setBounds (proportionOfWidth (0.0824f), proportionOfHeight (0.1590f), proportionOfWidth (0.0495f), proportionOfHeight (0.0418f));
    juce__textButton4->setBounds (proportionOfWidth (0.1374f), proportionOfHeight (0.1590f), proportionOfWidth (0.0495f), proportionOfHeight (0.0418f));
    juce__label14->setBounds (proportionOfWidth (0.1923f), proportionOfHeight (0.1674f), proportionOfWidth (0.0275f), proportionOfHeight (0.0251f));
    juce__label15->setBounds (proportionOfWidth (0.0055f), proportionOfHeight (0.2176f), proportionOfWidth (0.0769f), proportionOfHeight (0.0251f));
    juce__textButton5->setBounds (proportionOfWidth (0.0824f), proportionOfHeight (0.2092f), proportionOfWidth (0.0495f), proportionOfHeight (0.0418f));
    juce__textButton6->setBounds (proportionOfWidth (0.1374f), proportionOfHeight (0.2092f), proportionOfWidth (0.0495f), proportionOfHeight (0.0418f));
    juce__label16->setBounds (proportionOfWidth (0.1923f), proportionOfHeight (0.2176f), proportionOfWidth (0.0275f), proportionOfHeight (0.0251f));
    juce__label17->setBounds (proportionOfWidth (0.0055f), proportionOfHeight (0.2678f), proportionOfWidth (0.0769f), proportionOfHeight (0.0251f));
    juce__textButton7->setBounds (proportionOfWidth (0.0824f), proportionOfHeight (0.2594f), proportionOfWidth (0.0495f), proportionOfHeight (0.0418f));
    juce__textButton8->setBounds (proportionOfWidth (0.1374f), proportionOfHeight (0.2594f), proportionOfWidth (0.0495f), proportionOfHeight (0.0418f));
    juce__label18->setBounds (proportionOfWidth (0.1923f), proportionOfHeight (0.2678f), proportionOfWidth (0.0275f), proportionOfHeight (0.0251f));
    juce__label19->setBounds (proportionOfWidth (0.0055f), proportionOfHeight (0.3180f), proportionOfWidth (0.0769f), proportionOfHeight (0.0251f));
    juce__textButton9->setBounds (proportionOfWidth (0.0824f), proportionOfHeight (0.3096f), proportionOfWidth (0.0495f), proportionOfHeight (0.0418f));
    juce__textButton10->setBounds (proportionOfWidth (0.1374f), proportionOfHeight (0.3096f), proportionOfWidth (0.0495f), proportionOfHeight (0.0418f));
    juce__label20->setBounds (proportionOfWidth (0.1923f), proportionOfHeight (0.3180f), proportionOfWidth (0.0275f), proportionOfHeight (0.0251f));
    juce__label21->setBounds (proportionOfWidth (0.1868f), proportionOfHeight (0.5523f), proportionOfWidth (0.3462f), proportionOfHeight (0.0335f));
    juce__label22->setBounds (proportionOfWidth (0.1868f), proportionOfHeight (0.5941f), proportionOfWidth (0.3462f), proportionOfHeight (0.0335f));
    juce__label23->setBounds (proportionOfWidth (0.1868f), proportionOfHeight (0.6360f), proportionOfWidth (0.3462f), proportionOfHeight (0.0335f));
    juce__label24->setBounds (proportionOfWidth (0.1868f), proportionOfHeight (0.6778f), proportionOfWidth (0.3462f), proportionOfHeight (0.0335f));
    juce__label25->setBounds (proportionOfWidth (0.3022f), proportionOfHeight (0.7197f), proportionOfWidth (0.2308f), proportionOfHeight (0.0335f));
    juce__label26->setBounds (proportionOfWidth (0.3022f), proportionOfHeight (0.7615f), proportionOfWidth (0.2308f), proportionOfHeight (0.0335f));
    juce__label27->setBounds (proportionOfWidth (0.3022f), proportionOfHeight (0.8033f), proportionOfWidth (0.2308f), proportionOfHeight (0.0335f));
    juce__label28->setBounds (proportionOfWidth (0.3022f), proportionOfHeight (0.8452f), proportionOfWidth (0.2308f), proportionOfHeight (0.0335f));
    juce__label29->setBounds (proportionOfWidth (0.4121f), proportionOfHeight (0.8870f), proportionOfWidth (0.1209f), proportionOfHeight (0.0251f));
    juce__label30->setBounds (proportionOfWidth (0.3022f), proportionOfHeight (0.9205f), proportionOfWidth (0.2308f), proportionOfHeight (0.0251f));
    juce__label31->setBounds (proportionOfWidth (0.2857f), proportionOfHeight (0.9623f), proportionOfWidth (0.2527f), proportionOfHeight (0.0251f));
    juce__groupComponent2->setBounds (proportionOfWidth (0.0055f), proportionOfHeight (0.3598f), proportionOfWidth (0.2363f), proportionOfHeight (0.1172f));
    juce__textButton11->setBounds (proportionOfWidth (0.0165f), proportionOfHeight (0.4100f), proportionOfWidth (0.0495f), proportionOfHeight (0.0502f));
    juce__textButton12->setBounds (proportionOfWidth (0.0714f), proportionOfHeight (0.4100f), proportionOfWidth (0.0495f), proportionOfHeight (0.0502f));
    juce__textButton13->setBounds (proportionOfWidth (0.1264f), proportionOfHeight (0.4100f), proportionOfWidth (0.0495f), proportionOfHeight (0.0502f));
    juce__textButton14->setBounds (proportionOfWidth (0.1813f), proportionOfHeight (0.4100f), proportionOfWidth (0.0495f), proportionOfHeight (0.0502f));
    juce__label32->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.5356f), proportionOfWidth (0.0714f), proportionOfHeight (0.0251f));
    juce__label33->setBounds (proportionOfWidth (0.0495f), proportionOfHeight (0.4937f), proportionOfWidth (0.0769f), proportionOfHeight (0.0251f));
    juce__label34->setBounds (proportionOfWidth (0.1209f), proportionOfHeight (0.4937f), proportionOfWidth (0.1209f), proportionOfHeight (0.0251f));
    juce__label35->setBounds (proportionOfWidth (0.0769f), proportionOfHeight (0.5356f), proportionOfWidth (0.0385f), proportionOfHeight (0.0251f));
    juce__label36->setBounds (proportionOfWidth (0.1429f), proportionOfHeight (0.5356f), proportionOfWidth (0.0385f), proportionOfHeight (0.0251f));
    juce__label37->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.5774f), proportionOfWidth (0.0714f), proportionOfHeight (0.0251f));
    juce__label38->setBounds (proportionOfWidth (0.0769f), proportionOfHeight (0.5774f), proportionOfWidth (0.0385f), proportionOfHeight (0.0251f));
    juce__label39->setBounds (proportionOfWidth (0.1429f), proportionOfHeight (0.5774f), proportionOfWidth (0.0385f), proportionOfHeight (0.0251f));
    juce__label40->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.6192f), proportionOfWidth (0.0714f), proportionOfHeight (0.0251f));
    juce__label41->setBounds (proportionOfWidth (0.0769f), proportionOfHeight (0.6192f), proportionOfWidth (0.0385f), proportionOfHeight (0.0251f));
    juce__label42->setBounds (proportionOfWidth (0.1429f), proportionOfHeight (0.6192f), proportionOfWidth (0.0385f), proportionOfHeight (0.0251f));
    juce__label43->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.6611f), proportionOfWidth (0.0714f), proportionOfHeight (0.0251f));
    juce__label44->setBounds (proportionOfWidth (0.0769f), proportionOfHeight (0.6611f), proportionOfWidth (0.0385f), proportionOfHeight (0.0251f));
    juce__label45->setBounds (proportionOfWidth (0.1429f), proportionOfHeight (0.6611f), proportionOfWidth (0.0385f), proportionOfHeight (0.0251f));
    juce__label46->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.7113f), proportionOfWidth (0.1648f), proportionOfHeight (0.0251f));
    juce__label47->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.7531f), proportionOfWidth (0.1648f), proportionOfHeight (0.0251f));
    juce__label48->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.7950f), proportionOfWidth (0.1648f), proportionOfHeight (0.0251f));
    juce__label49->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.8368f), proportionOfWidth (0.1648f), proportionOfHeight (0.0251f));
    juce__label50->setBounds (proportionOfWidth (0.1758f), proportionOfHeight (0.7113f), proportionOfWidth (0.0385f), proportionOfHeight (0.0251f));
    juce__label51->setBounds (proportionOfWidth (0.1758f), proportionOfHeight (0.7531f), proportionOfWidth (0.0385f), proportionOfHeight (0.0251f));
    juce__label52->setBounds (proportionOfWidth (0.1758f), proportionOfHeight (0.7950f), proportionOfWidth (0.0385f), proportionOfHeight (0.0251f));
    juce__label53->setBounds (proportionOfWidth (0.1758f), proportionOfHeight (0.8368f), proportionOfWidth (0.0385f), proportionOfHeight (0.0251f));
    juce__label54->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.8703f), proportionOfWidth (0.1648f), proportionOfHeight (0.0251f));
    juce__label55->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.9038f), proportionOfWidth (0.1648f), proportionOfHeight (0.0251f));
    juce__label56->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.9372f), proportionOfWidth (0.1648f), proportionOfHeight (0.0251f));
    juce__label57->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.9707f), proportionOfWidth (0.1648f), proportionOfHeight (0.0251f));
    juce__groupComponent3->setBounds (proportionOfWidth (0.5989f), proportionOfHeight (0.0335f), proportionOfWidth (0.4011f), proportionOfHeight (0.1506f));
    juce__label58->setBounds (proportionOfWidth (0.6044f), proportionOfHeight (0.1088f), proportionOfWidth (0.1154f), proportionOfHeight (0.0335f));
    juce__label59->setBounds (proportionOfWidth (0.7582f), proportionOfHeight (0.1088f), proportionOfWidth (0.1374f), proportionOfHeight (0.0335f));
    juce__label60->setBounds (proportionOfWidth (0.9011f), proportionOfHeight (0.1088f), proportionOfWidth (0.0879f), proportionOfHeight (0.0335f));
    juce__label61->setBounds (proportionOfWidth (0.5989f), proportionOfHeight (0.8117f), proportionOfWidth (0.1209f), proportionOfHeight (0.0335f));
    juce__label62->setBounds (proportionOfWidth (0.7253f), proportionOfHeight (0.8201f), proportionOfWidth (0.0549f), proportionOfHeight (0.0251f));
    juce__label63->setBounds (proportionOfWidth (0.5989f), proportionOfHeight (0.8619f), proportionOfWidth (0.2198f), proportionOfHeight (0.0251f));
    juce__label64->setBounds (proportionOfWidth (0.5989f), proportionOfHeight (0.9038f), proportionOfWidth (0.2198f), proportionOfHeight (0.0251f));
    juce__label65->setBounds (proportionOfWidth (0.8187f), proportionOfHeight (0.8619f), proportionOfWidth (0.0549f), proportionOfHeight (0.0251f));
    juce__label66->setBounds (proportionOfWidth (0.8187f), proportionOfHeight (0.9038f), proportionOfWidth (0.0549f), proportionOfHeight (0.0251f));
    juce__textButton15->setBounds (proportionOfWidth (0.8846f), proportionOfHeight (0.8619f), proportionOfWidth (0.1099f), proportionOfHeight (0.0586f));
    juce__textButton16->setBounds (proportionOfWidth (0.6209f), proportionOfHeight (0.9372f), proportionOfWidth (0.1099f), proportionOfHeight (0.0586f));
    juce__textButton17->setBounds (proportionOfWidth (0.8571f), proportionOfHeight (0.9372f), proportionOfWidth (0.1099f), proportionOfHeight (0.0586f));
    juce__textEditor->setBounds(proportionOfWidth(0.5989f), proportionOfHeight(0.2008f), proportionOfWidth(0.3956f), proportionOfHeight(0.5941f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}
void StickMarker::ClearLessMore()
{
	_allowLess = 0;
	_allowMore = 0;
	juce__label65->setText(String(_allowMore),juce::NotificationType::sendNotificationAsync);
	juce__label66->setText(String(_allowLess), juce::NotificationType::sendNotificationAsync);

}
void StickMarker::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == juce__textButton.get())
    {
        //[UserButtonCode_juce__textButton] -- add your button handler code here..
        //[/UserButtonCode_juce__textButton]
		SendCmd(4208, 1);
		//bool ret = transferfins->SetDWordData(4208, 1);
  //      if (ret)
  //          juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::InfoIcon, "", L"报警复位成功");
  //      else
  //          juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::InfoIcon, "", L"报警复位失败");
    }
    else if (buttonThatWasClicked == juce__textButton2.get())
    {
        //[UserButtonCode_juce__textButton2] -- add your button handler code here..
        //[/UserButtonCode_juce__textButton2]
        juce__label35->setText(juce::String(0), juce::NotificationType::dontSendNotification);
        juce__label38->setText(juce::String(0), juce::NotificationType::dontSendNotification);
        juce__label41->setText(juce::String(0), juce::NotificationType::dontSendNotification);
        juce__label44->setText(juce::String(0), juce::NotificationType::dontSendNotification);
        juce__label36->setText(juce::String(0), juce::NotificationType::dontSendNotification);
        juce__label39->setText(juce::String(0), juce::NotificationType::dontSendNotification);
        juce__label42->setText(juce::String(0), juce::NotificationType::dontSendNotification);
        juce__label45->setText(juce::String(0), juce::NotificationType::dontSendNotification);
        
    }
    else if (buttonThatWasClicked == juce__toggleButton.get())
    {
        //[UserButtonCode_juce__toggleButton] -- add your button handler code here..
        //[/UserButtonCode_juce__toggleButton]
    }
    else if (buttonThatWasClicked == juce__textButton3.get())
    {
        //[UserButtonCode_juce__textButton3] -- add your button handler code here..
        //[/UserButtonCode_juce__textButton3]
        juce__textButton3->setColour(TextButton::ColourIds::buttonColourId, Colours::red);
        juce__textButton4->setColour(TextButton::ColourIds::buttonColourId, Colour(0xff009cda));
        juce__label14->setText(String(0), NotificationType::dontSendNotification);
        isMarkerOnUse[0] = 0;
        m_configFile.SaveMarkerState(isMarkerOnUse);
    }
    else if (buttonThatWasClicked == juce__textButton4.get())
    {
        //[UserButtonCode_juce__textButton4] -- add your button handler code here..
        //[/UserButtonCode_juce__textButton4]
        juce__textButton4->setColour(TextButton::ColourIds::buttonColourId, Colours::green);
        juce__textButton3->setColour(TextButton::ColourIds::buttonColourId, Colour(0xff009cda));
        juce__label14->setText(String(1), NotificationType::dontSendNotification);
        isMarkerOnUse[0] = 1;
        m_configFile.SaveMarkerState(isMarkerOnUse);
    }
    else if (buttonThatWasClicked == juce__textButton5.get())
    {
        //[UserButtonCode_juce__textButton5] -- add your button handler code here..
        //[/UserButtonCode_juce__textButton5]
        juce__textButton5->setColour(TextButton::ColourIds::buttonColourId, Colours::red);
        juce__textButton6->setColour(TextButton::ColourIds::buttonColourId, Colour(0xff009cda));
        juce__label16->setText(String(0), NotificationType::dontSendNotification);
        isMarkerOnUse[1] = 0;
        m_configFile.SaveMarkerState(isMarkerOnUse);
    }
    else if (buttonThatWasClicked == juce__textButton6.get())
    {
        //[UserButtonCode_juce__textButton6] -- add your button handler code here..
        //[/UserButtonCode_juce__textButton6]
        juce__textButton6->setColour(TextButton::ColourIds::buttonColourId, Colours::green);
        juce__textButton5->setColour(TextButton::ColourIds::buttonColourId, Colour(0xff009cda));
        juce__label16->setText(String(1), NotificationType::dontSendNotification);
        isMarkerOnUse[1] = 1;
        m_configFile.SaveMarkerState(isMarkerOnUse);
    }
    else if (buttonThatWasClicked == juce__textButton7.get())
    {
        //[UserButtonCode_juce__textButton7] -- add your button handler code here..
        //[/UserButtonCode_juce__textButton7]
        juce__textButton7->setColour(TextButton::ColourIds::buttonColourId, Colours::red);
        juce__textButton8->setColour(TextButton::ColourIds::buttonColourId, Colour(0xff009cda));
        juce__label18->setText(String(0), NotificationType::dontSendNotification);
        isMarkerOnUse[2] = 0;
        m_configFile.SaveMarkerState(isMarkerOnUse);
    }
    else if (buttonThatWasClicked == juce__textButton8.get())
    {
        //[UserButtonCode_juce__textButton8] -- add your button handler code here..
        //[/UserButtonCode_juce__textButton8]
        juce__textButton8->setColour(TextButton::ColourIds::buttonColourId, Colours::green);
        juce__textButton7->setColour(TextButton::ColourIds::buttonColourId, Colour(0xff009cda));
        juce__label18->setText(String(1), NotificationType::dontSendNotification);
        isMarkerOnUse[2] = 1;
        m_configFile.SaveMarkerState(isMarkerOnUse);
    }
    else if (buttonThatWasClicked == juce__textButton9.get())
    {
        //[UserButtonCode_juce__textButton9] -- add your button handler code here..
        //[/UserButtonCode_juce__textButton9]
        juce__textButton9->setColour(TextButton::ColourIds::buttonColourId, Colours::red);
        juce__textButton10->setColour(TextButton::ColourIds::buttonColourId, Colour(0xff009cda));
        juce__label20->setText(String(0), NotificationType::dontSendNotification);
        isMarkerOnUse[3] = 0;
        m_configFile.SaveMarkerState(isMarkerOnUse);
    }
    else if (buttonThatWasClicked == juce__textButton10.get())
    {
        //[UserButtonCode_juce__textButton10] -- add your button handler code here..
        //[/UserButtonCode_juce__textButton10]
        juce__textButton10->setColour(TextButton::ColourIds::buttonColourId, Colours::green);
        juce__textButton9->setColour(TextButton::ColourIds::buttonColourId, Colour(0xff009cda));
        juce__label20->setText(String(1), NotificationType::dontSendNotification);
        isMarkerOnUse[3] = 1;
        m_configFile.SaveMarkerState(isMarkerOnUse);
    }
    else if (buttonThatWasClicked == juce__textButton11.get())
    {
        //[UserButtonCode_juce__textButton11] -- add your button handler code here..
        //[/UserButtonCode_juce__textButton11]
    }
    else if (buttonThatWasClicked == juce__textButton12.get())
    {
        //[UserButtonCode_juce__textButton12] -- add your button handler code here..
        //[/UserButtonCode_juce__textButton12]
    }
    else if (buttonThatWasClicked == juce__textButton13.get())
    {
        //[UserButtonCode_juce__textButton13] -- add your button handler code here..
        //[/UserButtonCode_juce__textButton13]
    }
    else if (buttonThatWasClicked == juce__textButton14.get())
    {
        //[UserButtonCode_juce__textButton14] -- add your button handler code here..
        //[/UserButtonCode_juce__textButton14]
    }
    else if (buttonThatWasClicked == juce__textButton15.get())
    {
        //[UserButtonCode_juce__textButton15] -- add your button handler code here..
        //[/UserButtonCode_juce__textButton15]
    }
    else if (buttonThatWasClicked == juce__textButton16.get())
    {
        //[UserButtonCode_juce__textButton16] -- add your button handler code here..
		String strAdd = juce__label65->getText();
		String strSub = juce__label66->getText();
		_allowMore = strAdd.getIntValue();
		_allowLess = strSub.getIntValue();

		//[/UserButtonCode_juce__textButton16]
    }
    else if (buttonThatWasClicked == juce__textButton17.get())
    {
        //[UserButtonCode_juce__textButton17] -- add your button handler code here..
		auto dw = findParentComponentOfClass<juce::DialogWindow>();
		if (dw)
		{
			dw->setVisible(false);
		}
		//[/UserButtonCode_juce__textButton17]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void StickMarker::timerCallback()
{

}
bool StickMarker::SendCmd(int addr, int val) //新卷需要向4204地址写0(结束卷)
{
	_cs.enter();
	COMMAND_WRITE data;
	data.flag = val;
	data.regAddr = addr;
	_cmds.push_back(data);
	_cs.exit();
	return true;
}
void StickMarker::Reconnect()
{
	//停止线程后再处理fins创建
	//transferfins->UnInitialize();
	//wait(2000);
	//transferfins->Connect();

	//if (transferfins->IsConnected())
	//	juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::WarningIcon, "", L"重新连接到PLC成功");
	//else
	//	juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::WarningIcon, "", L"重新连接到PLC失败");

	m_bReconnect = true;

	//if (transferfins)
	//{
	//	transferfins->Disconnect();// UnInitialize();
	//}
}

bool isNumCountChange(NumCount& numcount)
{
	auto func = [](int* num1, int* num2)->bool
	{
		for (int i = 0; i < 3; ++i)
		{
			if (num1[i] != num2[i])
				return true;
		}

		return false;
	};

	switch (numcount.id)
	{
	case 0:
		return func(numcount.num, g_StickMarker->m_prevstickNumCount[0].num);
		break;
	case 1:
		return func(numcount.num, g_StickMarker->m_prevstickNumCount[1].num);
		break;
	case 2:
		return func(numcount.num, g_StickMarker->m_prevstickNumCount[2].num);
		break;
	case 3:
		return func(numcount.num, g_StickMarker->m_prevstickNumCount[3].num);
		break;
	default:
		return false;
		break;

	}
}

#ifdef OMRON_COAT_LIB
int getSensorData(TransferFins* transferfins, int response[4])
{
	std::vector<short> stickRes;
	if (transferfins)
	{
		if (transferfins->IsConnected())
		{
			int i = 0;
			while (i < 3)
			{
				if ((transferfins->GetMultiWordData(610, 8, stickRes)) != 0)
				{
					++i;
					juce::Thread::sleep(10);
					continue;
				}
				else
				{
					for (int i = 0; i < 4; i++)
					{
						response[i] = stickRes[2 * i];
					}
					return 0;
				}
			}
		}
	}
	return 1;
}

int getStickResData(TransferFins* transferfins, int sticked[4], int rets[4], int response[4])
{

	std::vector<int> stickRes;
	if (transferfins)
	{
		if (transferfins->IsConnected())
		{
			int i = 0;
			while (i < 3)
			{
				if ((transferfins->GetMultiDWordData(20, 16, stickRes)) != 0)
				{
					++i;
					juce::Thread::sleep(10);
					continue;
				}
				else
				{
					for (int i = 0; i < 4; i++)
					{
						sticked[i] = stickRes[2 * i];
						response[i] = stickRes[2 * i + 8];
					}
					return 0;
				}
			}
		}
	}
	return 1;
}
int getDword(TransferFins* transferfins, int address, int& value)
{
	if (transferfins)
	{
		if (transferfins->IsConnected())
		{
			int i = 0;
			while (i < 10)
			{
				if ((transferfins->GetDWordData(address, value)) != 0)
				{
					++i;
					juce::Thread::sleep(10);
					continue;
				}
				else
				{
					return 0;
				}

			}
			if (i == 10)
				return -1;
			else
				return 0;
		}

		else
		{
			LOGWT("准备重新连接");
			transferfins->Connect();
			return -2;
		}
	}
	return 2;
}

void getStickNumCount(TransferFins* transferfins, int sticked[4], int response[4], int sensors[4])
{
	if (!transferfins->IsConnected())
		return;
	NumCount numcount;
	//for (int i = 0; i < 4; ++i)
	//{
	//    numcount.id = i;
	//    transferfins->GetWordData(36+4*i, numcount.num[0]);
	//    transferfins->GetWordData(20+4*i, numcount.num[1]);
	//    transferfins->GetWordData(610+2*i, numcount.num[2]);
	//    if (isNumCountChange(numcount))
	//    {
	//        std::lock_guard<std::mutex> lg(mute);
	//        g_stickNumCount.push_back(numcount);
	//    }
	//    memcpy(&m_prevstickNumCount[i], &numcount, sizeof(int) * 3);
	//}
	for (int i = 0; i < 4; ++i)
	{
		numcount.id = i;
		numcount.num[0] = response[i];
		numcount.num[1] = sticked[i];
		numcount.num[2] = sensors[i];
		if (numcount.num[0] < 0 || numcount.num[1] < 0 || numcount.num[2] < 0)
			continue;

		if (isNumCountChange(numcount))
		{
			//std::lock_guard<std::mutex> lg(mute);
			mute.enter();
			g_stickNumCount.push_back(numcount);
			mute.exit();
		}
		memcpy(&g_StickMarker->m_prevstickNumCount[i], &numcount, sizeof(NumCount));
	}
}
#else

int getSensorData(CFinsCLI_JQ* transferfins, int response[4])
{
	short stickRes[8];
	if (transferfins)
	{
		if (transferfins->IsConnected())
		{
			int i = 0;
			while (i < 3)
			{
				if ((transferfins->GetMultiWordData(610, 8, stickRes)) != 0)
				{
					++i;
					juce::Thread::sleep(10);
					continue;
				}
				else
				{
					for (int i = 0; i < 4; i++)
					{
						response[i] = stickRes[2 * i];
					}
					return 0;
				}
			}
		}
	}
	return 1;
}

int getStickResData(CFinsCLI_JQ* transferfins, int sticked[4], int rets[4], int response[4])
{

	int stickRes[16];
	if (transferfins)
	{
		if (transferfins->IsConnected())
		{
			int i = 0;
			while (i < 3)
			{
				if ((transferfins->GetMultiDWordData(20, 16, stickRes)) != 0)
				{
					++i;
					juce::Thread::sleep(10);
					continue;
				}
				else
				{
					for (int i = 0; i < 4; i++)
					{
						sticked[i] = stickRes[2 * i];
						response[i] = stickRes[2 * i + 8];
					}
					return 0;
				}
			}
		}
	}
	return 1;
}
int getDword(CFinsCLI_JQ* transferfins, int address, int& value)
{
	if (transferfins)
	{
		if (transferfins->IsConnected())
		{
			int i = 0;
			while (i < 10)
			{
				if ((transferfins->GetDWordData(address, value)) != 0)
				{
					++i;
					juce::Thread::sleep(10);
					continue;
				}
				else
				{
					return 0;
				}

			}
			if (i == 10)
				return -1;
			else
				return 0;
		}

		else
		{
			LOGWT("准备重新连接");
			transferfins->Connect();
			return -2;
		}
	}
	return 2;
}

void getStickNumCount(CFinsCLI_JQ* transferfins, int sticked[4], int response[4], int sensors[4])
{
	if (!transferfins->IsConnected())
		return;
	NumCount numcount;
	//for (int i = 0; i < 4; ++i)
	//{
	//    numcount.id = i;
	//    transferfins->GetWordData(36+4*i, numcount.num[0]);
	//    transferfins->GetWordData(20+4*i, numcount.num[1]);
	//    transferfins->GetWordData(610+2*i, numcount.num[2]);
	//    if (isNumCountChange(numcount))
	//    {
	//        std::lock_guard<std::mutex> lg(mute);
	//        g_stickNumCount.push_back(numcount);
	//    }
	//    memcpy(&m_prevstickNumCount[i], &numcount, sizeof(int) * 3);
	//}
	for (int i = 0; i < 4; ++i)
	{
		numcount.id = i;
		numcount.num[0] = response[i];
		numcount.num[1] = sticked[i];
		numcount.num[2] = sensors[i];
		if (numcount.num[0] < 0 || numcount.num[1] < 0 || numcount.num[2] < 0)
			continue;

		if (isNumCountChange(numcount))
		{
			//std::lock_guard<std::mutex> lg(mute);
			mute.enter();
			g_stickNumCount.push_back(numcount);
			mute.exit();
		}
		memcpy(&g_StickMarker->m_prevstickNumCount[i], &numcount, sizeof(NumCount));
	}
}


#endif

void StickMarker::handleAsyncUpdate()
{
	juce__textEditor->setText(Info, false);

	for (int i = 0; i < 4; ++i)
	{
		g_MainComponent->Label_MarkCount[i]->setText(L"打标机" + juce::String(i + 1) + L"计数  " + String(g_MainComponent->Num_Mark[i]), NotificationType::dontSendNotification);
	}

	juce__label35->setText(juce::String(sticked[0]), juce::NotificationType::dontSendNotification);
	juce__label38->setText(juce::String(sticked[1]), juce::NotificationType::dontSendNotification);
	juce__label41->setText(juce::String(sticked[2]), juce::NotificationType::dontSendNotification);
	juce__label44->setText(juce::String(sticked[3]), juce::NotificationType::dontSendNotification);

	juce__label36->setText(juce::String(response[0]), juce::NotificationType::dontSendNotification);
	juce__label39->setText(juce::String(response[1]), juce::NotificationType::dontSendNotification);
	juce__label42->setText(juce::String(response[2]), juce::NotificationType::dontSendNotification);
	juce__label45->setText(juce::String(response[3]), juce::NotificationType::dontSendNotification);


	juce__label50->setText(juce::String(sensorRes[0]), juce::NotificationType::dontSendNotification);
	juce__label51->setText(juce::String(sensorRes[1]), juce::NotificationType::dontSendNotification);
	juce__label52->setText(juce::String(sensorRes[2]), juce::NotificationType::dontSendNotification);
	juce__label53->setText(juce::String(sensorRes[3]), juce::NotificationType::dontSendNotification);

	if(m_prebState != m_bState)
		repaint();
	m_prebState = m_bState;
}

void StickMarker::run()
{
#ifdef OMRON_COAT_LIB
	TransferFins* transferfins = new TransferFins(_strPLC.toStdString().c_str(), 9600);
#else
	CFinsCLI_JQ *transferfins = new CFinsCLI_JQ(_strPLC.toStdString().c_str(), 9600);
#endif
	transferfins->Initialize();
	if (!transferfins->IsConnected())
	{
		m_bIsConnected = false;
		juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::WarningIcon, "", L"    连接打标机PLC失败");
	}
	else
	{
		m_bIsConnected = true;
	}
    //int sticked[4];
    //int response[4];
	//int sensorRes[4];
    int markNum = 0;
    int markNum2 = 0;
    int address = 4300;
    int isWarn = 0;
	int rets[4] = { 1,1,1,1 };
	int nTry = 0;
	int nSumFail = 0;
	bool bCanRead = false;
	int nReadTime = 0;
	juce::int64 passTime = 0;
	int iRead = 0;
	int iWrite = 0;
	int failWrite = 0;
	int nFailedRes = 0;
    while (!threadShouldExit())
    {
		bool bUpdate = false;
		int nTryTime = 0;
        for (int i = 0; i < 4; ++i)
        {
            m_bState[i + 28] = isMarkerOnUse[i];
        }
		if (m_bReconnect)
		{
			transferfins->UnInitialize();
			wait(3000);
			transferfins->Connect();
			m_bReconnect = false;
			
			if (transferfins->IsConnected())
				m_bIsConnected = true;
		}

		if (transferfins)
		{
			if (transferfins->IsConnected())
			{				
				if (iWrite % 2 == 0)
				{
					iWrite = 0;
					m_pulse = !m_pulse;
					juce::int64 curTime = Time::currentTimeMillis();
					m_linkState = transferfins->SetDWordData(4200, m_pulse);//与plc通信心跳
					if (nReadTime < 101)
					{
						passTime += (Time::currentTimeMillis() - curTime);
						nReadTime++;
					}
					if (!m_linkState)
					{
						failWrite++;
					}
					else
					{
						failWrite = 0;
					}
					if (failWrite > 4)
					{
						transferfins->Disconnect();
						transferfins->Connect();
						failWrite = 0;
					}
				}
				iWrite++;

				if (nReadTime == 100)
				{
					LOGWT("100次网络平均执行时间:%d ms", passTime);
				}
				

				_cs.enter();
				while (_cmds.size())
				{
					COMMAND_WRITE data = _cmds[0];
					transferfins->SetDWordData(data.regAddr, data.flag);
					_cmds.erase(_cmds.begin());
				}
				_cs.exit();
			}
			else
			{
				m_bIsConnected = false;
				wait(2000);
				transferfins->Connect();
				if (transferfins->IsConnected())
				{
					m_bIsConnected = true;
					LOGWT("重新连接到PLC成功");
					Info << L"重新连接到PLC成功";
					Info << L"\n";
					//juce__textEditor->setText(Info, false);
					nSumFail = 0;
					bUpdate = true;
				}
				else
				{
					LOGWT("重新连接到PLC失败!");
					Info << L"重新连接到PLC失败";
					Info << L"\n";
					//juce__textEditor->setText(Info, false);
					bUpdate = true;
				}
			}

			if (iRead >= 10)
			{
				iRead = 0;

				//确保某带路出标后才读取 //version T
				//bool bHaveSticked = false;
				//juce::int64 curTime2 = juce::Time::currentTimeMillis();
				//for (int i = 0; i < 4; i++)
				//{
				//	if (!isMarkerOnUse[i])
				//		continue;
				//	juce::int64 passTime = curTime2 - _preStickTime[i];
				//	if (_preStickTime[i] < 0 || (passTime > 1000 && passTime <= 10000))
				//	{
				//		bHaveSticked = true;
				//		break;
				//	}
				//}
				
				if (!m_bLocalC4)
				{
					juce::int64 curTime = juce::Time::currentTimeMillis();
					for (int i = 0; i < 4; i++)
					{
						if (!isMarkerOnUse[i] || !m_bC4Mark[i])
							continue;

						juce::int64 passTime = curTime - _preStickTime[i];
						int resC4 = 0;
						int ret1 = getDword(transferfins,4450, resC4);
						if (ret1 == 0 && resC4) //带路1完成C4
						{
							LOGWT("带路%d完成4连标", i + 1);
							m_bC4Mark[i] = false;
						}
					}
				}

				//if (!bHaveSticked)//version T 优化读取
				//{
				//	wait(100);
				//	iRead++;
				//	continue;
				//}
				for (int k = 0; k < 4; k++)
				{
					sticked[k] = response[k] = -1;
					rets[k] = 0;
				}
				int ret = getStickResData(transferfins,sticked, rets,response);

				markNum = sticked[0];
				//int ret = getDword(20, markNum);
				//sticked[0] = markNum;
				//rets[0] = ret;
				if (ret == 0)
				{
					bUpdate = true;
					//juce__label35->setText(juce::String(markNum), juce::NotificationType::dontSendNotification);
				}
				else
				{
					nTryTime++;
					LOGWT("%s", "读取1号贴标数失败");
				}
				//juce__label35->setText(juce::String(sticked[0]), juce::NotificationType::dontSendNotification);
				//juce__label38->setText(juce::String(sticked[1]), juce::NotificationType::dontSendNotification);
				//juce__label41->setText(juce::String(sticked[2]), juce::NotificationType::dontSendNotification);
				//juce__label44->setText(juce::String(sticked[3]), juce::NotificationType::dontSendNotification);

		

				//ret = getDword(24, markNum);
				//rets[1] = ret;
				//sticked[1] = markNum;
				markNum = sticked[1];
				if (ret == 0)
					bUpdate = true;//juce__label38->setText(juce::String(markNum), juce::NotificationType::dontSendNotification);
				else
				{
					nTryTime++;
					LOGWT("%s", "读取2号贴表数目失败");
				}
				//ret = getDword(28, markNum);
				//sticked[2] = markNum;
				//rets[2] = ret;
				markNum = sticked[2];
				if (ret == 0)
					bUpdate = true; //juce__label41->setText(juce::String(markNum), juce::NotificationType::dontSendNotification);
				else
				{
					nTryTime++;
					LOGWT("%s", "读取3号贴标数失败");
				}
				//ret = getDword(32, markNum);
				//sticked[3] = markNum;
				//rets[3] = ret;
				markNum = sticked[3];
				if (ret == 0)
					bUpdate = true; //juce__label44->setText(juce::String(markNum), juce::NotificationType::dontSendNotification);
				else
				{
					LOGWT("%s", "读取4号贴标数失败");
					nTryTime++;
				}
				//juce__label36->setText(juce::String(response[0]), juce::NotificationType::dontSendNotification);
				//juce__label39->setText(juce::String(response[1]), juce::NotificationType::dontSendNotification);
				//juce__label42->setText(juce::String(response[2]), juce::NotificationType::dontSendNotification);
				//juce__label45->setText(juce::String(response[3]), juce::NotificationType::dontSendNotification);

				
				//ret = getDword(36, markNum);
				////response[0] = markNum;
				markNum = response[0];
				if (ret == 0)
					bUpdate = true; //juce__label36->setText(juce::String(markNum), juce::NotificationType::dontSendNotification);
				else
				{
					nTryTime++;
					LOGWT("%s", "读取贴标机1收到的信号数失败");
				}
				//ret = getDword(40, markNum);
				////response[1] = markNum;
				markNum = response[1];
				if (ret == 0)
					bUpdate = true; //juce__label39->setText(juce::String(markNum), juce::NotificationType::dontSendNotification);
				else
				{
					nTryTime++;
					LOGWT("%s", "读取贴标机2收到的信号数失败");
				}
				//ret = getDword(44, markNum);
				////response[2] = markNum;
				markNum = response[2];
				if (ret == 0)
					bUpdate = true; //juce__label42->setText(juce::String(markNum), juce::NotificationType::dontSendNotification);
				else
				{
					nTryTime++;
					LOGWT("%s", "读取贴标机3收到的信号数失败");
				}
				//ret = getDword(48, markNum);
				////response[3] = markNum;
				markNum = response[3];
				if (ret == 0)
					bUpdate = true; //juce__label45->setText(juce::String(markNum), juce::NotificationType::dontSendNotification);
				else
				{
					nTryTime++;
					LOGWT("%s", "读取贴标机4收到的信号数失败");
				}
						
				for (int k = 0; k < 4; k++)
					sensorRes[k] = -1;

				//juce__label50->setText(juce::String(sensorRes[0]), juce::NotificationType::dontSendNotification);
				//juce__label51->setText(juce::String(sensorRes[1]), juce::NotificationType::dontSendNotification);
				//juce__label52->setText(juce::String(sensorRes[2]), juce::NotificationType::dontSendNotification);
				//juce__label53->setText(juce::String(sensorRes[3]), juce::NotificationType::dontSendNotification);

				int ret2 = getSensorData(transferfins,sensorRes);
				int sensorNum = sensorRes[0];
				//int sensorNum = 0;
				//ret = getDword(610, sensorNum);
				if (ret2 == 0)
                {
					bUpdate = true; //juce__label50->setText(juce::String(sensorNum), juce::NotificationType::dontSendNotification);
                }
				else
				{
					LOGWT("%s", "读取sensor1失败");
					nTryTime++;
				}
				
				sensorNum = sensorRes[1];
				//ret = getDword(612, sensorNum);
				if (ret2 == 0)
                {
					bUpdate = true; //juce__label51->setText(juce::String(sensorNum), juce::NotificationType::dontSendNotification);
  
                }
				else
				{
					LOGWT("%s", "读取sensor2失败");
					nTryTime++;
				}
				
				sensorNum = sensorRes[2];
				//ret = getDword(614, sensorNum);
                if (ret2 == 0)
                {
					bUpdate = true; //juce__label52->setText(juce::String(sensorNum), juce::NotificationType::dontSendNotification);

                }
				else
				{
					LOGWT("%s", "读取sensor3失败");
					nTryTime++;
				}

				sensorNum = sensorRes[3];
				//ret = getDword(616, sensorNum);
                if (ret2 == 0)
                {
					bUpdate = true; //juce__label53->setText(juce::String(sensorNum), juce::NotificationType::dontSendNotification);

                }
				else
				{
					LOGWT("%s", "读取sensor4失败");
					nTryTime++;
				}
				if (nTryTime)
					nTry++;
				else
					nTry = 0;

				if (ret == 0 && ret2 == 0)
				{
					getStickNumCount(transferfins,sticked, response, sensorRes);
				}
				//安全性检查,发出打标信号2s后开始检查接收的信号数
				juce::int64 curTime = juce::Time::currentTimeMillis();
				if (g_MainComponent != nullptr)
				{
					//for (int i = 0; i < 4; ++i)
					//{				
					//	g_MainComponent->Label_MarkCount[i]->setText(L"打标机" + juce::String(i + 1) + L"计数  " + String(g_MainComponent->Num_Mark[i]), NotificationType::dontSendNotification);
					//}

					if (nTry > 0 && nTry <= 2)//只要发生一次失败重新读取
					{
						LOGWT("读取打标数失败!");
						bUpdate = true;						
						//continue;
					}
					if (nTry > 2)
					{
						Info << juce::Time::getCurrentTime().formatted("%H:%M:%S") << ": ";
						Info << L"多次读取打标数失败，需要重新连接";
						//juce__textEditor->setText(Info, false);
						LOGWT("多次读取打标数失败,需要重新连接!!!");
						bUpdate = true;
						g_MainComponent->_finsLock.enter();
						//transferfins->UnInitialize();
						//delete transferfins;
						//transferfins = nullptr;

						//transferfins = new TransferFins(_strPLC.toStdString().c_str(), 9600);
						//transferfins->Initialize();
						m_bIsConnected = false;
						transferfins->UnInitialize();
						wait(2000);
						transferfins->Connect();
						g_MainComponent->_finsLock.exit();

						if(transferfins->IsConnected())
						{
							m_bIsConnected = true;
							LOGWT("重新连接到PLC成功");
							Info << L"重新连接到PLC成功\n";
							//juce__textEditor->setText(Info, false);
							nSumFail = 0;
							bUpdate = true;
						}
						else
						{
							m_bIsConnected = false;
							LOGWT("重新连接到PLC失败");
							Info << L"重新连接到PLC失败\n";
							//juce__textEditor->setText(Info, false);
							bUpdate = true;
						}


						nTry = 0;
						nSumFail++;
						if (nSumFail >= 1 && g_client)
						{
							/*INTER_MESSAGE data;
							data._spec = 201404;
							data._flag = -92;
							g_client->SendData(data);*/
                            StopMachine(L"多次读取打标数失败,需要重新连接!!!");
							nSumFail = 0;
						}
						continue;
					}
					for (int i = 0; i < 4; i++)
					{
						if (!isMarkerOnUse[i])
							continue;
						int minElap = 3000;
						int maxElap = 6000;
						if (m_bLocalC4 && m_bC4Mark[i])
						{
							minElap = 5000;
							maxElap = 8000;
						}

						juce::int64 passTime = curTime - _preStickTime[i];
						if ((_preStickTime[i] > 0 && passTime > minElap && passTime <= maxElap))
							//||(g_halEA > 300. && _preStickLenPos[i] > 0. && (_curRunningLenPos[i] - _preStickLenPos[i]) >= g_halEA))
						{
							if (m_bC4Mark[i])
								m_bC4Mark[i] = false;

							bool bErr = false;
							if (_allowLess > 0)
							{
								if ((sensorRes[i] + _allowLess) < g_MainComponent->Num_Mark[i])
								{
									bErr = true;
								}
							}
							else if (sensorRes[i] > sticked[i])
							{
								if ((sensorRes[i] - _allowMore) < g_MainComponent->Num_Mark[i])
								{
									bErr = true;
								}
							}
							else if (_allowLess == 0 || _allowMore == 0)
							{
								if (sensorRes[i] < g_MainComponent->Num_Mark[i])
								{
									bErr = true;
								}
							}

							//_preStickLenPos[i] = -1.;
							if (sticked[i] < g_MainComponent->Num_Mark[i] || bErr)
							{
								Info << juce::Time::getCurrentTime().formatted("%H:%M:%S") << ": ";
								Info << i + 1 << L"号打标数不一致(发出:接收:反馈)" << g_MainComponent->Num_Mark[i] << L":" << sticked[i] << L":" << sensorRes[i]<<'\n'; //检测到报警信息，报警停机;
								//juce__textEditor->setText(Info, false);
								bUpdate = true;
								String reason = String(i + 1);
								reason << L"号打标机指令数与贴标数不一致";
								m_bState[i] = 1;								
								LOGWT("允许sensor数少%d", _allowLess);
								LOGWT("允许sensor数多%d", _allowMore);
								LOGWT("%d号打标机报警停机：发出指令数%d,贴标数%d 信号反馈数%d", i + 1, g_MainComponent->Num_Mark[i], sticked[i], sensorRes[i]);

								if (g_client)
								{
									/*INTER_MESSAGE data;
									data._spec = 201404;
									data._flag = -92;
									g_client->SendData(data);*/
                                    StopMachine(reason);
								}
								LOGWT("报警停机：发出指令位置%f,当前位置%f", _preStickLenPos[i], _curRunningLenPos[i]);
								LOGWT("%d号打标机报警停机：发出指令数%d,贴标数%d 信号反馈数%d", i + 1, g_MainComponent->Num_Mark[i], sticked[i], sensorRes[i]);

							}
						}
					}
				}
				//
				std::vector<int> responses(27);
				if (transferfins)
				{
					if (transferfins->IsConnected())
					{

				#ifdef OMRON_COAT_LIB
						std::vector<int> responses2;
				#else
						int responses2[54] = { 0 };
				#endif
						int ret2 = transferfins->GetMultiDWordData(4300, 27*2, responses2);
		
						if (ret2 == 0)
						{
							nFailedRes = 0;
							for (int i = 0; i < 27; ++i)
							{
								responses[i] = responses2[2 * i];
								m_bState[i] = responses[i];
							}
							for (int i = 4; i < 27; ++i)//for (int i = 4; i < 27; ++i)
							{
								if (i <= 7 && !isMarkerOnUse[i - 4])
									continue;
								//0~8  0~3 0~3
								//if (!isMarkerOnUse[0] && (i==0 || i==4))
								//{
								//	continue;
								//}
								//else if (!isMarkerOnUse[1] && (i == 1 || i == 5))
								//{
								//	continue;
								//}
								//else if (!isMarkerOnUse[2] && (i == 2 || i == 6))
								//{
								//	continue;
								//}
								//else if (!isMarkerOnUse[3] && (i == 3 || i == 7))
								//{
								//	continue;
								//}
								//if (i <= 7 && !isMarkerOnUse[i - 4])
								//	continue;
								if (responses[i] == 1)
								{
									
									Info << juce::Time::getCurrentTime().formatted("%H:%M:%S") << ": ";
									Info << WarningInfo[i] << '\n'; //检测到报警信息，报警停机;
									//juce__textEditor->setText(Info, false);
									bUpdate = true;
									if (g_client)
									{
										LOGWT("报警停机WarningInfo[%d]", i);
										StopMachine(WarningInfo[i]);
										break;
									}
								}
							}							
						}
						else
						{
							nFailedRes++;
							if (nFailedRes >= 2)
							{
								Info << juce::Time::getCurrentTime().formatted("%H:%M:%S") << ": ";
								Info << L"多次读取状态反馈信息失败,停机处理" << '\n'; //检测到报警信息，报警停机;
								//juce__textEditor->setText(Info, false);
								bUpdate = true;
								if (g_client)
								{
									LOGWT("多次读取状态反馈信息失败,停机处理");
									StopMachine(L"多次读取状态反馈信息失败,停机处理");	
									nFailedRes = 0;
								}
							}
						}
					}
					else //网络断开连接
					{
						if (g_client)
						{
							LOGWT("多次读取反馈信息失败,停机处理");
							StopMachine(L"多次读取反馈信息失败,停机处理");
						}
					}
				}

				//for (int i = 4; i < 27; ++i)
				//{
				//	if (i <= 7 && !isMarkerOnUse[i-4])
				//		continue;

				//	isWarn = 0;
				//	int ret2 = getDword(4300 + 4 * i, isWarn);
				//	if (ret2 == 0)
				//	{
				//		if (isWarn == 1)
				//		{
				//			Info << juce::Time::getCurrentTime().formatted("%H:%M:%S") << ": ";
				//			Info << WarningInfo[i] << '\n'; //检测到报警信息，报警停机;
				//			juce__textEditor->setText(Info, false);
				//			if (g_client)
				//			{
				//				LOGWT("报警停机%s", WarningInfo[i].toStdString());
    //                            StopMachine(WarningInfo[i]);
				//				break;
				//			}
				//		}
				//	}
				//	else
				//		LOGWT("%d,%s", address, "地址报警读取失败");
				//}//endfor

			}
			iRead++;

		}
        wait(100);

		if (bUpdate)
		{
			triggerAsyncUpdate();
		}
        //getStickNumCount();
    }

	if (g_client)
	{
		LOGWT("打标服务器数据处理结束！");
	}

	transferfins->UnInitialize();
	delete transferfins;
}

void StickMarker::StopMachine(const String& info)
{
    if (m_edition == 0) //先导
    {
        Pci1230WriteDoBit(15, 1, 1);
        juce::Thread::sleep(50);
        Pci1230WriteDoBit(15, 1, 0);
    }

    else if (m_edition == 1)//海目星
    {
        if (g_client)
        {
            INTER_MESSAGE data;
            data._spec = 201404;
            data._flag = -92;
            g_client->SendData(data);
        }
    }
	if (g_MainComponent)
	{
		static juce::int64 preReportTime = 0;
		juce::int64 curTime = juce::Time::currentTimeMillis();
		static String preInfo = L"";
		if (preInfo == info && curTime - preReportTime > 15000)
		{
			g_MainComponent->m_mfClient.SendMsgData(g_MainComponent->m_bNewUI, info);
			preReportTime = curTime;
		}
		else if (preInfo != info)
		{
			g_MainComponent->m_mfClient.SendMsgData(g_MainComponent->m_bNewUI, info);
			preReportTime = curTime;
		}
		preInfo = info;

	}
}


//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="StickMarker" componentName=""
                 parentClasses="public juce::Component" constructorParams="" variableInitialisers=""
                 snapPixels="4" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="900" initialHeight="600">
  <BACKGROUND backgroundColour="ff78d5ff">
    <RECT pos="59.89% 3.347% 39.56% 15.063%" fill="solid: ff009cda" hasStroke="0"/>
    <PATH pos="0 0 100 100" fill="solid: ff43a52a" hasStroke="0" nonZeroWinding="1">s 584 28 l 624 28 l 540 28 x</PATH>
    <RECT pos="59.89% 20.084% 39.56% 59.414%" fill="solid: ff009cda" hasStroke="0"/>
    <ELLIPSE pos="54.945% 5.858% 1.099% 1.674%" fill="solid: ffff0000" hasStroke="0"/>
    <ELLIPSE pos="54.945% 10.042% 1.099% 1.674%" fill="solid: ffff0000" hasStroke="0"/>
    <ELLIPSE pos="54.945% 14.226% 1.099% 1.674%" fill="solid: ffff0000" hasStroke="0"/>
    <ELLIPSE pos="54.945% 18.41% 1.099% 1.674%" fill="solid: ffff0000" hasStroke="0"/>
    <ELLIPSE pos="54.945% 22.594% 1.099% 1.674%" fill="solid: ffff0000" hasStroke="0"/>
    <ELLIPSE pos="54.945% 26.778% 1.099% 1.674%" fill="solid: ffff0000" hasStroke="0"/>
    <ELLIPSE pos="54.945% 30.962% 1.099% 1.674%" fill="solid: ffff0000" hasStroke="0"/>
    <ELLIPSE pos="54.945% 35.146% 1.099% 1.674%" fill="solid: ffff0000" hasStroke="0"/>
    <ELLIPSE pos="54.945% 39.331% 1.099% 1.674%" fill="solid: ffff0000" hasStroke="0"/>
    <ELLIPSE pos="54.945% 43.515% 1.099% 1.674%" fill="solid: ffff0000" hasStroke="0"/>
    <ELLIPSE pos="54.945% 47.699% 1.099% 1.674%" fill="solid: ffff0000" hasStroke="0"/>
    <ELLIPSE pos="54.945% 51.883% 1.099% 1.674%" fill="solid: ffff0000" hasStroke="0"/>
    <ELLIPSE pos="54.945% 56.067% 1.099% 1.674%" fill="solid: ffff0000" hasStroke="0"/>
    <ELLIPSE pos="54.945% 60.251% 1.099% 1.674%" fill="solid: ffff0000" hasStroke="0"/>
    <ELLIPSE pos="54.945% 64.435% 1.099% 1.674%" fill="solid: ffff0000" hasStroke="0"/>
    <ELLIPSE pos="54.945% 68.619% 1.099% 1.674%" fill="solid: ffff0000" hasStroke="0"/>
    <ELLIPSE pos="54.945% 72.803% 1.099% 1.674%" fill="solid: ffff0000" hasStroke="0"/>
    <ELLIPSE pos="54.945% 76.987% 1.099% 1.674%" fill="solid: ffff0000" hasStroke="0"/>
    <ELLIPSE pos="54.945% 81.172% 1.099% 1.674%" fill="solid: ffff0000" hasStroke="0"/>
    <ELLIPSE pos="54.945% 85.356% 1.099% 1.674%" fill="solid: ffff0000" hasStroke="0"/>
    <ELLIPSE pos="54.945% 88.703% 1.099% 1.674%" fill="solid: ffff0000" hasStroke="0"/>
    <ELLIPSE pos="54.945% 92.887% 1.099% 1.674%" fill="solid: ffff0000" hasStroke="0"/>
    <ELLIPSE pos="54.945% 97.071% 1.099% 1.674%" fill="solid: ffff0000" hasStroke="0"/>
    <ELLIPSE pos="18.681% 87.029% 1.099% 1.674%" fill="solid: ffff0000" hasStroke="0"/>
    <ELLIPSE pos="18.681% 90.377% 1.099% 1.674%" fill="solid: ffff0000" hasStroke="0"/>
    <ELLIPSE pos="18.681% 94.561% 1.099% 1.674%" fill="solid: ffff0000" hasStroke="0"/>
    <ELLIPSE pos="18.681% 97.908% 1.099% 1.674%" fill="solid: ffff0000" hasStroke="0"/>
    <ELLIPSE pos="73.626% 11.715% 1.099% 1.674%" fill="solid: ffff0000" hasStroke="0"/>
  </BACKGROUND>
  <GROUPCOMPONENT name="" id="1048a24f4a38545b" memberName="juce__groupComponent"
                  virtualName="" explicitFocusOrder="0" pos="0% 1.674% 59.341% 98.745%"
                  outlinecol="ffffffff" textcol="ff000000" title="&#36148;&#26631;&#26426;PLC&#25253;&#35686;&#29366;&#24577;"/>
  <TEXTBUTTON name="new button" id="219f6d7a29b44576" memberName="juce__textButton"
              virtualName="" explicitFocusOrder="0" pos="1.099% 6.695% 9.89% 5.021%"
              bgColOff="ff009cda" textColOn="ffffffff" buttonText="&#25253;&#35686;&#22797;&#20301;"
              connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="6cf45b32bb4e7ddb" memberName="juce__textButton2"
              virtualName="" explicitFocusOrder="0" pos="12.088% 6.695% 9.89% 5.021%"
              bgColOff="ff009cda" textColOn="ffffffff" buttonText="&#35745;&#25968;&#28165;&#38646;"
              connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="d150e5e5b4619203" memberName="juce__label"
         virtualName="" explicitFocusOrder="0" pos="23.626% 5.021% 29.67% 3.347%"
         bkgCol="9cda" textCol="ff000000" edTextCol="ff000000" edBkgCol="0"
         labelText="1&#21495;&#28431;&#26631;&#25253;&#35686;(1&#21495;&#36148;&#26631;&#33394;&#26631;&#24863;&#24212;&#22120;&#26410;&#24863;&#24212;&#21040;)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="14.9" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <LABEL name="new label" id="136b087cfbcafa6" memberName="juce__label2"
         virtualName="" explicitFocusOrder="0" pos="23.626% 9.205% 29.67% 3.347%"
         bkgCol="9cda" textCol="ff000000" edTextCol="ff000000" edBkgCol="0"
         labelText="2&#21495;&#28431;&#26631;&#25253;&#35686;(2&#21495;&#36148;&#26631;&#33394;&#26631;&#24863;&#24212;&#22120;&#26410;&#24863;&#24212;&#21040;)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <LABEL name="new label" id="7e8892a6b0788da1" memberName="juce__label3"
         virtualName="" explicitFocusOrder="0" pos="23.626% 13.389% 29.67% 3.347%"
         bkgCol="9cda" textCol="ff000000" edTextCol="ff000000" edBkgCol="0"
         labelText="3&#21495;&#28431;&#26631;&#25253;&#35686;(3&#21495;&#36148;&#26631;&#33394;&#26631;&#24863;&#24212;&#22120;&#26410;&#24863;&#24212;&#21040;)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <LABEL name="new label" id="6097d756bd2cbbc1" memberName="juce__label4"
         virtualName="" explicitFocusOrder="0" pos="23.626% 17.573% 29.67% 3.347%"
         bkgCol="9cda" textCol="ff000000" edTextCol="ff000000" edBkgCol="0"
         labelText="4&#21495;&#28431;&#26631;&#25253;&#35686;(4&#21495;&#36148;&#26631;&#33394;&#26631;&#24863;&#24212;&#22120;&#26410;&#24863;&#24212;&#21040;)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <LABEL name="new label" id="7ed5e94c5a021aba" memberName="juce__label5"
         virtualName="" explicitFocusOrder="0" pos="24.725% 21.757% 28.571% 3.347%"
         bkgCol="9cda" textCol="ff000000" edTextCol="ff000000" edBkgCol="0"
         labelText="1&#21495;&#25972;&#23450;&#22833;&#36133;&#25253;&#35686;(&#20027;&#21160;&#36746;&#26410;&#19979;&#21387;&#25110;&#27809;&#26631;&#31614;)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <LABEL name="new label" id="bb5431df233feefb" memberName="juce__label6"
         virtualName="" explicitFocusOrder="0" pos="24.725% 25.941% 28.571% 3.347%"
         bkgCol="9cda" textCol="ff000000" edTextCol="ff000000" edBkgCol="0"
         labelText="2&#21495;&#25972;&#23450;&#22833;&#36133;&#25253;&#35686;(&#20027;&#21160;&#36746;&#26410;&#19979;&#21387;&#25110;&#27809;&#26631;&#31614;)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <LABEL name="new label" id="d8c7acaa523991f" memberName="juce__label7"
         virtualName="" explicitFocusOrder="0" pos="24.725% 30.126% 28.571% 3.347%"
         bkgCol="9cda" textCol="ff000000" edTextCol="ff000000" edBkgCol="0"
         labelText="3&#21495;&#25972;&#23450;&#22833;&#36133;&#25253;&#35686;(&#20027;&#21160;&#36746;&#26410;&#19979;&#21387;&#25110;&#27809;&#26631;&#31614;)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <LABEL name="new label" id="7fa728313e96e1a9" memberName="juce__label8"
         virtualName="" explicitFocusOrder="0" pos="24.725% 34.31% 28.571% 3.347%"
         bkgCol="9cda" textCol="ff000000" edTextCol="ff000000" edBkgCol="0"
         labelText="4&#21495;&#25972;&#23450;&#22833;&#36133;&#25253;&#35686;(&#20027;&#21160;&#36746;&#26410;&#19979;&#21387;&#25110;&#27809;&#26631;&#31614;)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <LABEL name="new label" id="e45c8ee18452cbf1" memberName="juce__label9"
         virtualName="" explicitFocusOrder="0" pos="25.824% 38.494% 27.473% 3.347%"
         bkgCol="9cda" textCol="ff000000" edTextCol="ff000000" edBkgCol="0"
         labelText="1&#21495;&#36148;&#26631;&#26426;&#32047;&#21152;&#36148;&#26631;&#25253;&#35686;(1&#21495;&#36148;&#26631;&#36229;&#19978;&#38480;)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <LABEL name="new label" id="f99eeadc70682458" memberName="juce__label10"
         virtualName="" explicitFocusOrder="0" pos="25.824% 42.678% 27.473% 3.347%"
         bkgCol="9cda" textCol="ff000000" edTextCol="ff000000" edBkgCol="0"
         labelText="2&#21495;&#36148;&#26631;&#26426;&#32047;&#21152;&#36148;&#26631;&#25253;&#35686;(2&#21495;&#36148;&#26631;&#36229;&#19978;&#38480;)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <LABEL name="new label" id="830eba13d4063dc2" memberName="juce__label11"
         virtualName="" explicitFocusOrder="0" pos="25.824% 46.862% 27.473% 3.347%"
         bkgCol="9cda" textCol="ff000000" edTextCol="ff000000" edBkgCol="0"
         labelText="3&#21495;&#36148;&#26631;&#26426;&#32047;&#21152;&#36148;&#26631;&#25253;&#35686;(3&#21495;&#36148;&#26631;&#36229;&#19978;&#38480;)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <LABEL name="new label" id="ef137f7521eb9245" memberName="juce__label12"
         virtualName="" explicitFocusOrder="0" pos="25.824% 51.046% 27.473% 3.347%"
         bkgCol="9cda" textCol="ff000000" edTextCol="ff000000" edBkgCol="0"
         labelText="4&#21495;&#36148;&#26631;&#26426;&#32047;&#21152;&#36148;&#26631;&#25253;&#35686;(4&#21495;&#36148;&#26631;&#36229;&#19978;&#38480;)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <TOGGLEBUTTON name="new toggle button" id="490aa7c95bbc94bb" memberName="juce__toggleButton"
                virtualName="" explicitFocusOrder="0" pos="1.099% 12.552% 9.89% 2.51%"
                txtcol="ff000000" buttonText="&#20449;&#21495;&#27979;&#35797;"
                connectedEdges="15" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="new label" id="392418dac3b37159" memberName="juce__label13"
         virtualName="" explicitFocusOrder="0" pos="0.549% 16.736% 7.692% 2.51%"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="1&#21495;&#36148;&#26631;&#26426;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="11.5" kerning="-0.035" bold="0"
         italic="0" justification="33"/>
  <TEXTBUTTON name="new button" id="d5c3a265219d5501" memberName="juce__textButton3"
              virtualName="" explicitFocusOrder="0" pos="8.242% 15.9% 4.945% 4.184%"
              bgColOff="ff009cda" textColOn="ffffffff" buttonText="&#20851;&#38381;"
              connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="791ea2f79dc21f89" memberName="juce__textButton4"
              virtualName="" explicitFocusOrder="0" pos="13.736% 15.9% 4.945% 4.184%"
              bgColOff="ff009cda" textColOn="ffffffff" buttonText="&#24320;&#21551;"
              connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="fde860882c0c7ada" memberName="juce__label14"
         virtualName="" explicitFocusOrder="0" pos="19.231% 16.736% 2.747% 2.51%"
         bkgCol="ff009cda" textCol="ffffffff" edTextCol="ff000000" edBkgCol="ffff0000"
         labelText="0" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="11.4"
         kerning="-0.035" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="ef7475858ba31198" memberName="juce__label15"
         virtualName="" explicitFocusOrder="0" pos="0.549% 21.757% 7.692% 2.51%"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="2&#21495;&#36148;&#26631;&#26426;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="11.5" kerning="-0.035" bold="0"
         italic="0" justification="33"/>
  <TEXTBUTTON name="new button" id="3d96924128dacad0" memberName="juce__textButton5"
              virtualName="" explicitFocusOrder="0" pos="8.242% 20.921% 4.945% 4.184%"
              bgColOff="ff009cda" textColOn="ffffffff" buttonText="&#20851;&#38381;"
              connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="c9330fa3805585a0" memberName="juce__textButton6"
              virtualName="" explicitFocusOrder="0" pos="13.736% 20.921% 4.945% 4.184%"
              bgColOff="ff009cda" textColOn="ffffffff" buttonText="&#24320;&#21551;"
              connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="ea63d167c66ea315" memberName="juce__label16"
         virtualName="" explicitFocusOrder="0" pos="19.231% 21.757% 2.747% 2.51%"
         bkgCol="ff009cda" textCol="ffffffff" edTextCol="ff000000" edBkgCol="ffff0000"
         labelText="0" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="11.4"
         kerning="-0.035" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="403327609bde9ff" memberName="juce__label17"
         virtualName="" explicitFocusOrder="0" pos="0.549% 26.778% 7.692% 2.51%"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="3&#21495;&#36148;&#26631;&#26426;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="11.5" kerning="-0.035" bold="0"
         italic="0" justification="33"/>
  <TEXTBUTTON name="new button" id="e074427ab81cb6f7" memberName="juce__textButton7"
              virtualName="" explicitFocusOrder="0" pos="8.242% 25.941% 4.945% 4.184%"
              bgColOff="ff009cda" textColOn="ffffffff" buttonText="&#20851;&#38381;"
              connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="8b38c8b47d7fa8a3" memberName="juce__textButton8"
              virtualName="" explicitFocusOrder="0" pos="13.736% 25.941% 4.945% 4.184%"
              bgColOff="ff009cda" textColOn="ffffffff" buttonText="&#24320;&#21551;"
              connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="7764200c0f084c38" memberName="juce__label18"
         virtualName="" explicitFocusOrder="0" pos="19.231% 26.778% 2.747% 2.51%"
         bkgCol="ff009cda" textCol="ffffffff" edTextCol="ff000000" edBkgCol="ffff0000"
         labelText="0" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="11.4"
         kerning="-0.035" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="cc57406938e68e40" memberName="juce__label19"
         virtualName="" explicitFocusOrder="0" pos="0.549% 31.799% 7.692% 2.51%"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="4&#21495;&#36148;&#26631;&#26426;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="11.5" kerning="-0.035" bold="0"
         italic="0" justification="33"/>
  <TEXTBUTTON name="new button" id="ec238942f0d9f39c" memberName="juce__textButton9"
              virtualName="" explicitFocusOrder="0" pos="8.242% 30.962% 4.945% 4.184%"
              bgColOff="ff009cda" textColOn="ffffffff" buttonText="&#20851;&#38381;"
              connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="a6b721752d9edecc" memberName="juce__textButton10"
              virtualName="" explicitFocusOrder="0" pos="13.736% 30.962% 4.945% 4.184%"
              bgColOff="ff009cda" textColOn="ffffffff" buttonText="&#24320;&#21551;"
              connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="1bcf00574594656e" memberName="juce__label20"
         virtualName="" explicitFocusOrder="0" pos="19.231% 31.799% 2.747% 2.51%"
         bkgCol="ff009cda" textCol="ffffffff" edTextCol="ff000000" edBkgCol="ffff0000"
         labelText="0" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="11.4"
         kerning="-0.035" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="68d7f40bd0ab7a04" memberName="juce__label21"
         virtualName="" explicitFocusOrder="0" pos="18.681% 55.23% 34.615% 3.347%"
         bkgCol="9cda" textCol="ff000000" edTextCol="ff000000" edBkgCol="0"
         labelText="1&#21495;&#36148;&#26631;&#26426;&#26631;&#31614;&#26029;&#24102;&#25253;&#35686;(&#33394;&#26631;&#36830;&#32493;&#24863;&#24212;&#21040;&#26631;&#31614;3&#27425;)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <LABEL name="new label" id="d8a93917b3786dfa" memberName="juce__label22"
         virtualName="" explicitFocusOrder="0" pos="18.681% 59.414% 34.615% 3.347%"
         bkgCol="9cda" textCol="ff000000" edTextCol="ff000000" edBkgCol="0"
         labelText="2&#21495;&#36148;&#26631;&#26426;&#26631;&#31614;&#26029;&#24102;&#25253;&#35686;(&#33394;&#26631;&#36830;&#32493;&#24863;&#24212;&#21040;&#26631;&#31614;3&#27425;)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <LABEL name="new label" id="269877348f4c0fe6" memberName="juce__label23"
         virtualName="" explicitFocusOrder="0" pos="18.681% 63.598% 34.615% 3.347%"
         bkgCol="9cda" textCol="ff000000" edTextCol="ff000000" edBkgCol="0"
         labelText="3&#21495;&#36148;&#26631;&#26426;&#26631;&#31614;&#26029;&#24102;&#25253;&#35686;(&#33394;&#26631;&#36830;&#32493;&#24863;&#24212;&#21040;&#26631;&#31614;3&#27425;)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <LABEL name="new label" id="5e10376555c9ea11" memberName="juce__label24"
         virtualName="" explicitFocusOrder="0" pos="18.681% 67.782% 34.615% 3.347%"
         bkgCol="9cda" textCol="ff000000" edTextCol="ff000000" edBkgCol="0"
         labelText="4&#21495;&#36148;&#26631;&#26426;&#26631;&#31614;&#26029;&#24102;&#25253;&#35686;(&#33394;&#26631;&#36830;&#32493;&#24863;&#24212;&#21040;&#26631;&#31614;3&#27425;)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <LABEL name="new label" id="20cf274af1c56535" memberName="juce__label25"
         virtualName="" explicitFocusOrder="0" pos="30.22% 71.967% 23.077% 3.347%"
         bkgCol="9cda" textCol="ff000000" edTextCol="ff000000" edBkgCol="0"
         labelText="1&#21495;&#20986;&#29616;&#36830;&#32493;&#25171;&#26631;&#25253;&#35686;&#20572;&#26426;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <LABEL name="new label" id="6987bdb5a387d388" memberName="juce__label26"
         virtualName="" explicitFocusOrder="0" pos="30.22% 76.151% 23.077% 3.347%"
         bkgCol="9cda" textCol="ff000000" edTextCol="ff000000" edBkgCol="0"
         labelText="2&#21495;&#20986;&#29616;&#36830;&#32493;&#25171;&#26631;&#25253;&#35686;&#20572;&#26426;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <LABEL name="new label" id="cbfd147de03da7f9" memberName="juce__label27"
         virtualName="" explicitFocusOrder="0" pos="30.22% 80.335% 23.077% 3.347%"
         bkgCol="9cda" textCol="ff000000" edTextCol="ff000000" edBkgCol="0"
         labelText="3&#21495;&#20986;&#29616;&#36830;&#32493;&#25171;&#26631;&#25253;&#35686;&#20572;&#26426;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <LABEL name="new label" id="68330b92cb695db8" memberName="juce__label28"
         virtualName="" explicitFocusOrder="0" pos="30.22% 84.519% 23.077% 3.347%"
         bkgCol="9cda" textCol="ff000000" edTextCol="ff000000" edBkgCol="0"
         labelText="4&#21495;&#20986;&#29616;&#36830;&#32493;&#25171;&#26631;&#25253;&#35686;&#20572;&#26426;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <LABEL name="new label" id="f957ec1a62b22200" memberName="juce__label29"
         virtualName="" explicitFocusOrder="0" pos="41.209% 88.703% 12.088% 2.51%"
         bkgCol="9cda" textCol="ff000000" edTextCol="ff000000" edBkgCol="0"
         labelText="&#24635;&#36148;&#26631;&#25968;&#36229;&#37327;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <LABEL name="new label" id="8891011e8cb1274d" memberName="juce__label30"
         virtualName="" explicitFocusOrder="0" pos="30.22% 92.05% 23.077% 2.51%"
         bkgCol="9cda" textCol="ff000000" edTextCol="ff000000" edBkgCol="0"
         labelText="&#26381;&#21153;&#22120;&#21644;PLC&#32593;&#32476;&#36890;&#35759;&#26029;&#24320;&#25253;&#35686;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <LABEL name="new label" id="bfc5147da4af5c3f" memberName="juce__label31"
         virtualName="" explicitFocusOrder="0" pos="28.571% 96.234% 25.275% 2.51%"
         bkgCol="9cda" textCol="ff000000" edTextCol="ff000000" edBkgCol="0"
         labelText="&#36148;&#26631;&#26426;&#20282;&#26381;&#39537;&#21160;&#22120;&#25253;&#35686;(&#26816;&#26597;&#39537;&#21160;&#22120;)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <GROUPCOMPONENT name="new group" id="1da4d2f4eb0c5134" memberName="juce__groupComponent2"
                  virtualName="" explicitFocusOrder="0" pos="0.549% 35.983% 23.626% 11.715%"
                  outlinecol="ffffffff" title="&#19968;&#38190;&#25972;&#23450;&#26631;&#31614;"/>
  <TEXTBUTTON name="new button" id="e0c820b0da7caa2d" memberName="juce__textButton11"
              virtualName="" explicitFocusOrder="0" pos="1.648% 41.004% 4.945% 5.021%"
              bgColOff="ff009cda" textColOn="ffffffff" buttonText="&#36724;1"
              connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="3d018869130261d1" memberName="juce__textButton12"
              virtualName="" explicitFocusOrder="0" pos="7.143% 41.004% 4.945% 5.021%"
              bgColOff="ff009cda" textColOn="ffffffff" buttonText="&#36724;2"
              connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="d34079db15cca221" memberName="juce__textButton13"
              virtualName="" explicitFocusOrder="0" pos="12.637% 41.004% 4.945% 5.021%"
              bgColOff="ff009cda" textColOn="ffffffff" buttonText="&#36724;3"
              connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="b795993ceb99428a" memberName="juce__textButton14"
              virtualName="" explicitFocusOrder="0" pos="18.132% 41.004% 4.945% 5.021%"
              bgColOff="ff009cda" textColOn="ffffffff" buttonText="&#36724;4"
              connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="9bcfaea1c8cb272a" memberName="juce__label32"
         virtualName="" explicitFocusOrder="0" pos="0% 53.556% 7.143% 2.51%"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="1&#21495;&#36148;&#26631;&#26426;:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="11.5" kerning="-0.035" bold="0"
         italic="0" justification="33"/>
  <LABEL name="new label" id="fd73fe8ec02cefdc" memberName="juce__label33"
         virtualName="" explicitFocusOrder="0" pos="4.945% 49.372% 7.692% 2.51%"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="&#36148;&#26631;&#26426;&#20010;&#25968;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="11.5" kerning="-0.035" bold="0"
         italic="0" justification="33"/>
  <LABEL name="new label" id="618b17d1d1811372" memberName="juce__label34"
         virtualName="" explicitFocusOrder="0" pos="12.088% 49.372% 12.088% 2.51%"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="&#36148;&#26631;&#26426;&#25910;&#21040;&#20449;&#21495;&#20010;&#25968;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="11.5" kerning="-0.035" bold="0"
         italic="0" justification="33"/>
  <LABEL name="new label" id="9773bb8a535bc0ba" memberName="juce__label35"
         virtualName="" explicitFocusOrder="0" pos="7.692% 53.556% 3.846% 2.51%"
         bkgCol="ff009cda" textCol="ffffffff" edTextCol="ff000000" edBkgCol="ffff0000"
         labelText="0" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="11.4"
         kerning="-0.035" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="9380d3e62c457eba" memberName="juce__label36"
         virtualName="" explicitFocusOrder="0" pos="14.286% 53.556% 3.846% 2.51%"
         bkgCol="ff009cda" textCol="ffffffff" edTextCol="ff000000" edBkgCol="ffff0000"
         labelText="0" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="11.4"
         kerning="-0.035" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="84b60cc552b7d1d4" memberName="juce__label37"
         virtualName="" explicitFocusOrder="0" pos="0% 57.741% 7.143% 2.51%"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="2&#21495;&#36148;&#26631;&#26426;:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="11.5" kerning="-0.035" bold="0"
         italic="0" justification="33"/>
  <LABEL name="new label" id="549976ecf124244e" memberName="juce__label38"
         virtualName="" explicitFocusOrder="0" pos="7.692% 57.741% 3.846% 2.51%"
         bkgCol="ff009cda" textCol="ffffffff" edTextCol="ff000000" edBkgCol="ffff0000"
         labelText="0" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="11.4"
         kerning="-0.035" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="41abaa184932480e" memberName="juce__label39"
         virtualName="" explicitFocusOrder="0" pos="14.286% 57.741% 3.846% 2.51%"
         bkgCol="ff009cda" textCol="ffffffff" edTextCol="ff000000" edBkgCol="ffff0000"
         labelText="0" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="11.4"
         kerning="-0.035" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="f6bdb35564e6685f" memberName="juce__label40"
         virtualName="" explicitFocusOrder="0" pos="0% 61.925% 7.143% 2.51%"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="3&#21495;&#36148;&#26631;&#26426;:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="11.5" kerning="-0.035" bold="0"
         italic="0" justification="33"/>
  <LABEL name="new label" id="fd871490988dd3f4" memberName="juce__label41"
         virtualName="" explicitFocusOrder="0" pos="7.692% 61.925% 3.846% 2.51%"
         bkgCol="ff009cda" textCol="ffffffff" edTextCol="ff000000" edBkgCol="ffff0000"
         labelText="0" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="11.4"
         kerning="-0.035" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="bde3a96b971fe32c" memberName="juce__label42"
         virtualName="" explicitFocusOrder="0" pos="14.286% 61.925% 3.846% 2.51%"
         bkgCol="ff009cda" textCol="ffffffff" edTextCol="ff000000" edBkgCol="ffff0000"
         labelText="0" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="11.4"
         kerning="-0.035" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="494a8550f1d07e97" memberName="juce__label43"
         virtualName="" explicitFocusOrder="0" pos="0% 66.109% 7.143% 2.51%"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="4&#21495;&#36148;&#26631;&#26426;:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="11.5" kerning="-0.035" bold="0"
         italic="0" justification="33"/>
  <LABEL name="new label" id="5bf75c0dd8bcb60c" memberName="juce__label44"
         virtualName="" explicitFocusOrder="0" pos="7.692% 66.109% 3.846% 2.51%"
         bkgCol="ff009cda" textCol="ffffffff" edTextCol="ff000000" edBkgCol="ffff0000"
         labelText="0" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="11.4"
         kerning="-0.035" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="fbea54dde5fd824b" memberName="juce__label45"
         virtualName="" explicitFocusOrder="0" pos="14.286% 66.109% 3.846% 2.51%"
         bkgCol="ff009cda" textCol="ffffffff" edTextCol="ff000000" edBkgCol="ffff0000"
         labelText="0" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="11.4"
         kerning="-0.035" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="caccdd9013af69ed" memberName="juce__label46"
         virtualName="" explicitFocusOrder="0" pos="0% 71.13% 16.484% 2.51%"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="1&#21495;sensor&#24863;&#24212;&#21040;&#26631;&#31614;&#25968;&#30446;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="11.5" kerning="-0.035" bold="0"
         italic="0" justification="33"/>
  <LABEL name="new label" id="2cc02f6d6c2d88ef" memberName="juce__label47"
         virtualName="" explicitFocusOrder="0" pos="0% 75.314% 16.484% 2.51%"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="2&#21495;sensor&#24863;&#24212;&#21040;&#26631;&#31614;&#25968;&#30446;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="11.5" kerning="-0.035" bold="0"
         italic="0" justification="33"/>
  <LABEL name="new label" id="4a304238a97537d6" memberName="juce__label48"
         virtualName="" explicitFocusOrder="0" pos="0% 79.498% 16.484% 2.51%"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="3&#21495;sensor&#24863;&#24212;&#21040;&#26631;&#31614;&#25968;&#30446;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="11.5" kerning="-0.035" bold="0"
         italic="0" justification="33"/>
  <LABEL name="new label" id="a428ab0fe0eb63f3" memberName="juce__label49"
         virtualName="" explicitFocusOrder="0" pos="0% 83.682% 16.484% 2.51%"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="4&#21495;sensor&#24863;&#24212;&#21040;&#26631;&#31614;&#25968;&#30446;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="11.5" kerning="-0.035" bold="0"
         italic="0" justification="33"/>
  <LABEL name="new label" id="45181475eee8a7e9" memberName="juce__label50"
         virtualName="" explicitFocusOrder="0" pos="17.582% 71.13% 3.846% 2.51%"
         bkgCol="ff009cda" textCol="ffffffff" edTextCol="ff000000" edBkgCol="ffff0000"
         labelText="0" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="11.4"
         kerning="-0.035" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="76caa3af2acfe86c" memberName="juce__label51"
         virtualName="" explicitFocusOrder="0" pos="17.582% 75.314% 3.846% 2.51%"
         bkgCol="ff009cda" textCol="ffffffff" edTextCol="ff000000" edBkgCol="ffff0000"
         labelText="0" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="11.4"
         kerning="-0.035" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="3a14df4ece6b794b" memberName="juce__label52"
         virtualName="" explicitFocusOrder="0" pos="17.582% 79.498% 3.846% 2.51%"
         bkgCol="ff009cda" textCol="ffffffff" edTextCol="ff000000" edBkgCol="ffff0000"
         labelText="0" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="11.4"
         kerning="-0.035" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="122deb0d9c890eaf" memberName="juce__label53"
         virtualName="" explicitFocusOrder="0" pos="17.582% 83.682% 3.846% 2.51%"
         bkgCol="ff009cda" textCol="ffffffff" edTextCol="ff000000" edBkgCol="ffff0000"
         labelText="0" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="11.4"
         kerning="-0.035" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="d6512cdce38cfea4" memberName="juce__label54"
         virtualName="" explicitFocusOrder="0" pos="0% 87.029% 16.484% 2.51%"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="1&#21495;&#36148;&#26631;&#26426;&#26159;&#21542;&#21551;&#29992;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="11.5" kerning="-0.035" bold="0"
         italic="0" justification="33"/>
  <LABEL name="new label" id="452caf3ced3996d9" memberName="juce__label55"
         virtualName="" explicitFocusOrder="0" pos="0% 90.377% 16.484% 2.51%"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="2&#21495;&#36148;&#26631;&#26426;&#26159;&#21542;&#21551;&#29992;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="11.5" kerning="-0.035" bold="0"
         italic="0" justification="33"/>
  <LABEL name="new label" id="b822d4cd3d6f8aa3" memberName="juce__label56"
         virtualName="" explicitFocusOrder="0" pos="0% 93.724% 16.484% 2.51%"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="3&#21495;&#36148;&#26631;&#26426;&#26159;&#21542;&#21551;&#29992;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="11.5" kerning="-0.035" bold="0"
         italic="0" justification="33"/>
  <LABEL name="new label" id="15bb3125ea78bd47" memberName="juce__label57"
         virtualName="" explicitFocusOrder="0" pos="0% 97.071% 16.484% 2.51%"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="4&#21495;&#36148;&#26631;&#26426;&#26159;&#21542;&#21551;&#29992;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="11.5" kerning="-0.035" bold="0"
         italic="0" justification="33"/>
  <GROUPCOMPONENT name="new group" id="4c4d5479e6f1adb5" memberName="juce__groupComponent3"
                  virtualName="" explicitFocusOrder="0" pos="59.89% 3.347% 40.11% 15.063%"
                  outlinecol="ffffffff" title="&#36148;&#26631;&#26426;PLC&#36830;&#25509;"/>
  <LABEL name="new label" id="56ca3109089d9c17" memberName="juce__label58"
         virtualName="" explicitFocusOrder="0" pos="60.44% 10.879% 11.538% 3.347%"
         bkgCol="9cda" textCol="ffffffff" edTextCol="ff000000" edBkgCol="0"
         labelText="PLC&#36830;&#25509;&#29366;&#24577;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="14.9" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="55d8faf840cc5a77" memberName="juce__label59"
         virtualName="" explicitFocusOrder="0" pos="75.824% 10.879% 13.736% 3.347%"
         bkgCol="9cda" textCol="ffffffff" edTextCol="ff000000" edBkgCol="0"
         labelText="IP" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="14.9"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="e2bfb682971dc87b" memberName="juce__label60"
         virtualName="" explicitFocusOrder="0" pos="90.11% 10.879% 8.791% 3.347%"
         bkgCol="9cda" textCol="ffffffff" edTextCol="ff000000" edBkgCol="0"
         labelText="PORT" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="14.9"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="17b8e32cf605ed08" memberName="juce__label61"
         virtualName="" explicitFocusOrder="0" pos="59.89% 81.172% 12.088% 3.347%"
         bkgCol="9cda" textCol="ff000000" edTextCol="ff000000" edBkgCol="0"
         labelText="&#36148;&#26631;&#25253;&#35686;&#25968;&#30446;:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <LABEL name="new label" id="23e2f832381875b8" memberName="juce__label62"
         virtualName="" explicitFocusOrder="0" pos="72.527% 82.008% 5.495% 2.51%"
         bkgCol="ff009cda" textCol="ffffffff" edTextCol="ff000000" edBkgCol="ffff0000"
         labelText="0" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="11.4"
         kerning="-0.035" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="cc4244f413375195" memberName="juce__label63"
         virtualName="" explicitFocusOrder="0" pos="59.89% 86.192% 21.978% 2.51%"
         bkgCol="9cda" textCol="ff000000" edTextCol="ff000000" edBkgCol="0"
         labelText="Sensor&#25968;&#30446;&#22810;&#20110;&#36148;&#26631;&#25968;&#30446;&#25253;&#35686;:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <LABEL name="new label" id="c7bbde5e26f7b983" memberName="juce__label64"
         virtualName="" explicitFocusOrder="0" pos="59.89% 90.377% 21.978% 2.51%"
         bkgCol="9cda" textCol="ff000000" edTextCol="ff000000" edBkgCol="0"
         labelText="Sensor&#25968;&#30446;&#23569;&#20110;&#36148;&#26631;&#25968;&#30446;&#25253;&#35686;:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <LABEL name="new label" id="125e1bc655b1bcb1" memberName="juce__label65"
         virtualName="" explicitFocusOrder="0" pos="81.868% 86.192% 5.495% 2.51%"
         bkgCol="ff009cda" textCol="ffffffff" edTextCol="ff000000" edBkgCol="ffff0000"
         labelText="0" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="11.4"
         kerning="-0.035" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="a670cc3d21c06d65" memberName="juce__label66"
         virtualName="" explicitFocusOrder="0" pos="81.868% 90.377% 5.495% 2.51%"
         bkgCol="ff009cda" textCol="ffffffff" edTextCol="ff000000" edBkgCol="ffff0000"
         labelText="0" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="11.4"
         kerning="-0.035" bold="0" italic="0" justification="36"/>
  <TEXTBUTTON name="new button" id="2c960db2ea9d68a8" memberName="juce__textButton15"
              virtualName="" explicitFocusOrder="0" pos="88.462% 86.192% 10.989% 5.858%"
              bgColOff="ff009cda" textColOn="ffffffff" buttonText="&#28165;&#31354;&#20449;&#24687;"
              connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="341ebc6469a32092" memberName="juce__textButton16"
              virtualName="" explicitFocusOrder="0" pos="62.088% 93.724% 10.989% 5.858%"
              bgColOff="ff009cda" textColOn="ffffffff" buttonText="&#20445;&#23384;"
              connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="a3c6ef93a0ad4fdb" memberName="juce__textButton17"
              virtualName="" explicitFocusOrder="0" pos="85.714% 93.724% 10.989% 5.858%"
              bgColOff="ff009cda" textColOn="ffffffff" buttonText="&#36864;&#20986;"
              connectedEdges="15" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

