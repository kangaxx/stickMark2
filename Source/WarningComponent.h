#pragma once
#include<JuceHeader.h>
#include"MainComponent.h"
extern MainComponent* g_maincomponent;
extern void* g_MainWindow;

class WarningComponent :public juce::Component, public juce::Button::Listener
{
public:
	WarningComponent(){
		
		Confirm.reset(new juce::TextButton(L"È·¶¨"));
		Confirm->setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::lightgrey);
		Confirm->setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::black);
		Confirm->setBoundsRelative(0.4, 0.73, 0.5, 0.2);
		addAndMakeVisible(Confirm.get());
		Confirm->addListener(this);
		setSize(200, 200);
	}

	~WarningComponent() override{}

	void paint(juce::Graphics& g) override{
		float Width = getWidth();
		float Height = getHeight();

		g.setColour(juce::Colours::white);
		g.fillRect(0.0f, 0.0f, Width, 2.0 / 3 * Height);
		g.setColour(juce::Colours::lightgrey);
		g.fillRect(0.0f, 2.0 / 3 * Height, Width, 1.0 / 3 * Height);
		juce::Path warning_triangle;
		warning_triangle.addTriangle(1.0/3*Width,1.0 / 6 * Height, 1.0/6*Width, 1.0 / 2 * Height, 1.0 / 2* Width, 1.0 / 2 * Height);
		g.setColour(juce::Colours::yellow);
		g.fillPath(warning_triangle);
		g.setColour(juce::Colours::red);
		g.drawLine(1.0/3 * Width, 1.0 / 4 * Height, 1.0/3 * Width, 5.0 / 12 * Height, 10);
		g.drawLine(1.0 / 3 * Width, 10.5 / 24 * Height, 1.0 / 3 * Width, 11.5 / 24 * Height,10);
		g.setColour(juce::Colours::black);
		g.drawFittedText("open card failed!", 1.0 / 2 * Width, 7.0/24 * Height, 1.0 / 2 * Width, 1.0 / 6 * Height,juce::Justification::centred,2);
		
		
	}

	void resized() override{
		Confirm->setBoundsRelative(0.4, 0.73, 0.5, 0.2);
	}

	void buttonClicked(juce::Button* buttonthatwasclicked) {
		if (buttonthatwasclicked == Confirm.get()) {
			setVisible(false);
			((juce::Component*)g_MainWindow)->centreWithSize(g_maincomponent->getWidth(), g_maincomponent->getHeight());
			g_maincomponent->setVisible(true);
			//((juce::DocumentWindow*)g_MainWindow)->setContentComponent(g_maincomponent);
		}

	}
private:
	std::unique_ptr<juce::TextButton> Confirm;
};