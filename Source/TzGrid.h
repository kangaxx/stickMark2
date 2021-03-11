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
        addGridItemPanel (Colours::aquamarine, "0");
        addGridItemPanel (Colours::red,        "1");
        addGridItemPanel (Colours::blue,       "2");
        addGridItemPanel (Colours::green,      "3");
        addGridItemPanel (Colours::orange,     "4");
        addGridItemPanel (Colours::white,      "5");
        addGridItemPanel (Colours::aquamarine, "6");
        addGridItemPanel (Colours::red,        "7");
        addGridItemPanel (Colours::blue,       "8");
        addGridItemPanel (Colours::green,      "9");
        addGridItemPanel (Colours::orange,     "10");
        addGridItemPanel (Colours::white,      "11");

        setSize (750, 750);
		columnNum = 0;
		rowNum = 0;
    }

    void addGridItemPanel (Colour colour, const char* text)
    {
        addAndMakeVisible (items.add (new GridItemPanel (colour, text)));
    }

    void paint (Graphics& g) override
    {
        g.fillAll (Colours::black);
    }

	void setTitles(int count, ...)
	{
		va_list arg_ptr;
		va_start(arg_ptr, count);
		if (count > 0)
			isShowTitle = true;
		//ui控件需要用智能指针管理，不能直接清空，所以考虑多次set的可能，需要看看是否需要重置内容
		int i = 0;
		for (i; i < this->titles.size(); ++i)
			this->titles.set(i, va_arg(arg_ptr, wchar_t*));

		for (i; i < count; ++i)
			this->titles.add(va_arg(arg_ptr, wchar_t*));
		va_end(arg_ptr);
	}

	void setTitles(StringArray titles)
	{
		if (titles.size() > 0)
			isShowTitle = true;
		int i = 0;
		for (i; i < this->titles.size(); ++i)
			this->titles.set(i, titles[i]);

		for (i; i < titles.size(); ++i)
			this->titles.add(titles[i]);
	}

	int getColumNum()
	{
		if (this->isShowTitle)
			return this->titles.size();
		else
			return columnNum;
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

        grid.rowGap    = 20_px;
        grid.columnGap = 20_px;

        using Track = Grid::TrackInfo;



		//根据titles设置列数
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

        grid.items.addArray ({ GridItem (items[0]).withArea (2, 2, 4, 4),
                               GridItem (items[1]),
                               GridItem (items[2]).withArea ({}, 3),
                               GridItem (items[3]),
                               GridItem (items[4]).withArea (GridItem::Span (2), {}),
                               GridItem (items[5]),
                               GridItem (items[6]),
                               GridItem (items[7]),
                               GridItem (items[8]),
                               GridItem (items[9]),
                               GridItem (items[10]),
                               GridItem (items[11])
                            });

        grid.performLayout (getLocalBounds());
    }

    //==============================================================================
    struct GridItemPanel  : public Component
    {
        GridItemPanel (Colour colourToUse, const String& textToUse)
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

    OwnedArray<GridItemPanel> items;
private:
	int columnNum = 0;
	int rowNum = 0; 
	//grid第一行默认是title，可以隐藏
	bool isShowTitle = true;
	StringArray titles;
};
