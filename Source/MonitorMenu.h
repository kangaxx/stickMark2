/*
  ==============================================================================

	MonitorMenu.h
	Created: 10 Mar 2021 10:12:05am
	Author:  Tizer

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
enum CommandIDs
{
	//menu system
	menuSystemInitial = 0x2001,
	//menu function
	menuFunctionClearwarning = 0x3001,
	//menu help
	menuSystemhelp = 0x4001
};

enum class MenuBarPosition
{
    juce::CharPointer_UTF8 ("\xe7\xb3\xbb\xe7\xbb\x9f"),
    juce::CharPointer_UTF8 ("\xe5\x8a\x9f\xe8\x83\xbd"),
	juce::CharPointer_UTF8 ("\xe5\xb8\xae\xe5\x8a\xa9")
};

class MonitorMenu : public juce::Component,
	public juce::MenuBarModel,
	public juce::Button::Listener
{
public:
	MonitorMenu();
	~MonitorMenu() override;

	void paint(juce::Graphics&) override;
	void resized() override;


private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MonitorMenu)
};
