/*
  ==============================================================================

    TzGrid.h
    Created: 11 Mar 2021 9:36:51am
    Author:  Tizer

  ==============================================================================
*/

#pragma once

#include<cstdarg>
#include <JuceHeader.h>
#include "CompnentUtilities.h"
using namespace juce;
//==============================================================================
/*
*/
struct tzGrid   : public Component
{
    tzGrid()
    {


		//btnTest.reset(new juce::TextButton("btnReset"));
		//addAndMakeVisible(btnTest.get());
		//btnTest->setButtonText(juce::CharPointer_UTF8("\xe4\xb8\x80\xe9\x94\xae\xe5\xa4\x8d\xe4\xbd\x8d"));
		////btnTest->addListener(this);

		//btnTest->setBounds(16, 8, 136, 24);

		//lblWarnNum.reset(new juce::Label("lblWarnNum",
		//	juce::CharPointer_UTF8("\xe8\xb4\xb4\xe6\xa0\x87\xe6\x8a\xa5\xe8\xad\xa6\xe6\x95\xb0\xe7\x9b\xae")));
		//addAndMakeVisible(lblWarnNum.get());
		//lblWarnNum->setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
		//lblWarnNum->setJustificationType(juce::Justification::centredLeft);
		//lblWarnNum->setEditable(false, false, false);
		//lblWarnNum->setColour(juce::TextEditor::textColourId, juce::Colours::black);
		//lblWarnNum->setColour(juce::TextEditor::outlineColourId, juce::Colours::white);
		//lblWarnNum->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

		//lblWarnNum->setBounds(46, 48, 112, 24);

		//txtWarnNum.reset(new juce::TextEditor("txtWarnNum"));
		//addAndMakeVisible(txtWarnNum.get());
		//txtWarnNum->setMultiLine(false);
		//txtWarnNum->setReturnKeyStartsNewLine(false);
		//txtWarnNum->setReadOnly(false);
		//txtWarnNum->setScrollbarsShown(true);
		//txtWarnNum->setCaretVisible(true);
		//txtWarnNum->setPopupMenuEnabled(true);
		//txtWarnNum->setText(TRANS("0"));

		//txtWarnNum->setBounds(123, 48, 56, 24);

		
        setSize (750, 750);
		columnNum = 0;
		rowNum = 0;
    }

	//add column , with column title ,and width
	void addColoumn(String title, int width)
	{
		columns.add(new ColumnInfo(title, width));
	}

	void drawTitles()
	{
		float currentLeft = this->gridBordSize;
		float widthScala = getWidthScala();
		for (int i = 0; i < columns.size(); ++i)
		{
			TextButton *tmp = new TextButton(columns[i]->getTitle());
			addAndMakeVisible(tmp);
			tmp->setButtonText(columns[i]->getTitle());
			titlesButtons.add(tmp);
			//btnTest->addListener(this);
			
			tmp->setBounds(currentLeft, gridBordSize, columns[i]->getWidth() * widthScala, getTitleHeight());
			//next columns left = currentLeft + currentWidth;
			currentLeft += columns[i]->getWidth() * widthScala;
		}

	}
	void setBackgroudColour(Colour colour)
	{
		this->backgroudColour = colour;
	}

    void addGridRowPanel(Colour colour, const char* text)
    {
		TextButton *button = new TextButton(text);
        addAndMakeVisible (buttons.add(button));
    }

    void paint (Graphics& g) override
    {
        g.fillAll (this->backgroudColour);
    }


	int getColumNum()
	{
		//if (this->isShowTitle)
		//	return this->titles.size();
		//else
		//	return columnNum;
		return 1;
	}

	void setRowNum(int rowNum)
	{
		if (rowNum > 0)
		{
			this->rowNum = rowNum;
			if (this->isShowTitle)
				++this->rowNum;
		}
	}

	int getRowNum()
	{
		return rowNum;
	}

    void resized() override
    {
        Grid grid;

        grid.rowGap    = 5_px;
        grid.columnGap = 1_px;

        using Track = Grid::TrackInfo;

		for (int i = 0; i < getColumNum(); ++i)
		{
			grid.templateColumns.add(Track(1_fr));
		}

		for (int i = 0; i < getRowNum(); ++i)
		{
			grid.templateRows.add(Track(1_fr));
		}
		//grid.templateRows = { Track(1_fr), Track(1_fr), Track(1_fr) };
        //grid.templateColumns = { Track (1_fr),Track (1_fr),Track (1_fr) };


        grid.autoColumns = Track (1_fr);
        grid.autoRows    = Track (1_fr);

        grid.autoFlow = Grid::AutoFlow::column;
		//
		//grid.items.addArray({ GridItem(rows[0]),
		//			   GridItem(rows[1]),
		//			   GridItem(rows[2]),
		//			   GridItem(rows[3]),
		//	GridItem(rows[4])
		//				
		//});

        //grid.performLayout (getLocalBounds());
		drawTitles();
    }

    //==============================================================================
    struct GridCellPanel  : public Component
    {
		GridCellPanel(Colour colourToUse, const String& textToUse)
            : colour (colourToUse),
              text (textToUse)
        {}


        void paint (Graphics& g) override
        {
            g.fillAll (colour.withAlpha (0.5f));

            g.setColour (Colours::black);
            g.drawText (text, getLocalBounds().withSizeKeepingCentre (100, 100), Justification::centred, false);
        }
        Colour colour;
        String text;
    };

	struct ColumnInfo
	{
		String titleName;
		float width;
		ColumnInfo(String n, float w) :titleName(n), width(w) {}
		String getTitle() { return titleName; }
		float getWidth() { return width; }
	};

	enum columnType
	{
		editReadOnly = 0x0001, 
		editWriteRead = 0x0002,
		btnsOpenClose = 0x0003
	};

	float getWidthScala(){
		float columnTotalWidth = 0.0;//
		for (int i = 0; i < columns.size(); ++i)
			columnTotalWidth += columns[i]->getWidth();
		if (columnTotalWidth <= 0.0)
			return 1.0;
		else 
		{
			return this->getBounds().getWidth() / columnTotalWidth;
		}	
	}
	float getTitleHeight() { return this->titleHeight; }
	void setTitleHeight(float h) { this->titleHeight = h; }
    OwnedArray<GridCellPanel> cells;
	OwnedArray<ColumnInfo> columns;
	OwnedArray<TextButton> buttons;
	OwnedArray<TextButton> titlesButtons;
private:
	int columnNum = 0;
	int rowNum = 0; 
	bool isShowTitle = true;
	Colour backgroudColour;
	float minRowHeight = 20.0; //px
	float defaultTitleHeight = 24.0; //px
	float titleHeight = defaultTitleHeight; 
	float gridBordSize = 1.0; //
	std::unique_ptr<juce::TextButton> btnTest;
	std::unique_ptr<juce::Label> lblWarnNum;
	std::unique_ptr<juce::TextEditor> txtWarnNum;
};
