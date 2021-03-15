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
#include "../hds/FastDelegate.h"
using namespace fastdelegate;
using namespace juce;
//==============================================================================
/*
*/
struct tzGrid : public Component,
				public ComboBox::Listener
{
	enum columnComponentType
	{
		editReadOnly = 0x0001,
		editWriteRead = 0x0002,
		switchBox = 0x0004
	};

	enum columnDataType
	{
		stringColumn = 0x0001,
		intColumn = 0x0002,
		floatColumn = 0x0003,
		nullColumn = 0x9999
	};

	tzGrid()
	{
		setSize(750, 750);
		columnNum = 0;
		rowNum = 0;
	}

	typedef FastDelegate2<int, int> SIGNAL_II;
	void setFunctionII(SIGNAL_II somefunc) { m_HiddenDelegateII = somefunc; }

	

	void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override
	{
		//get this combox row index
		for (int i = 0; i < rowsComponents.size(); ++i)
		{
			if (rowsComponents[i] == comboBoxThatHasChanged)
			{
				int rowIdx = 0;
				rowIdx = ((i - i % getColumNum()) / getColumNum());
				//AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, "combox row idx", String(rowIdx));
				m_HiddenDelegateII(rowIdx, comboBoxThatHasChanged->getSelectedId());
				break;
			}
		}
	}

	//add column , with column title ,and width
	void addColoumn(String title, float width)
	{
		columns.add(new ColumnInfo(title, width));
	}

	void addColoumn(String title, float width, columnComponentType cct, columnDataType cdt)
	{
		columns.add(new ColumnInfo(title, width, cct, cdt));
	}

	void addRowData(StringArray data)
	{

		if (getRowNum() <= 0 || getColumNum() <= 0)
			throw new String("Error :rowData size error!");
		//if (getColumNum() * getRowNum() != data->size())
		//	throw new String("Error :rowData size error!");

		gridData = data;
	}

	void drawTitles()
	{
		float left = this->getBordSize();
		float widthScala = getWidthScala();
		//add all buttons to form
		for (int i = 0; i < columns.size(); ++i)
		{
			TextButton *tmp = new TextButton(columns[i]->getTitle());
			addAndMakeVisible(tmp);
			tmp->setButtonText(columns[i]->getTitle());
			titlesButtons.add(tmp);
			//btnTest->addListener(this);

			tmp->setBounds(left, getBordSize(), columns[i]->getWidth() * widthScala, getTitleHeight());
			//next columns left = currentLeft + currentWidth;
			left += columns[i]->getWidth() * widthScala;
		}
	}

	void redrawTitles()
	{
		//adjust buttons bounds for resize form
		if (titlesButtons.size() <= 0)
			drawTitles();
		float left = this->getBordSize();
		float widthScala = getWidthScala();
		for (int i = 0; i < titlesButtons.size(); ++i) {
			titlesButtons[i]->setBounds(left, getBordSize(), columns[i]->getWidth() * widthScala, getTitleHeight());
			left += columns[i]->getWidth() * widthScala;
		}
		return;
	}

	void drawRows()
	{
		for (int i = 0; i < getRowNum(); ++i)
		{
			drawRow(i, gridData);
		}
	}

	void drawRow(int rowIdx, StringArray data)
	{
		if (rowsComponents.size() == getRowNum() * getColumNum())
			return; //创建控件的过程只需要执行一次，如果今后需要添加resize功能则需要调整
		//if (data == NULL)
		//	return;
		float top = this->getBordSize() + getTitleHeight()  + rowIdx * getRowHeight();
		float left = this->getBordSize();
		float widthScala = getWidthScala();
		for (int i = 0; i < columns.size(); i++)
		{
			String text;
			//String temp = juce::CharPointer_UTF8("\xe5\x90\x8d\xe7\xa7\xb0");
			//char *c = new char[100];
			//CharPointer_UTF8::CharType *p = (CharPointer_UTF8::CharType*)c;
			//temp.copyToUTF8(p, 100);
			//text = std::string(c);
			//get column context;
			switch (columns[i]->dataType)
			{
			case columnDataType::floatColumn:
			case columnDataType::intColumn:
			case columnDataType::stringColumn:
				text = data.getReference(rowIdx * getColumNum() + i);
				
				
				break;
			default:
				text = "";
				break;
			}
			TextEditor *tmpText, *tmpEdit, *tmpDefault;
			ComboBox *tmpCombo;
			switch (columns[i]->componentType)
			{
			case columnComponentType::editReadOnly:
				tmpText = new TextEditor(columns[i]->getTitle() + String(rowIdx));
				addAndMakeVisible(tmpText);
				tmpText->setMultiLine(true);
				tmpText->setText(text);
				tmpText->setReadOnly(true);
				rowsComponents.add(tmpText);
				//tmpText->addListener(this);

				tmpText->setBounds(left, top, columns[i]->getWidth() * widthScala, getRowHeight());
				//next columns left = currentLeft + currentWidth;
				left += columns[i]->getWidth() * widthScala;				
				break;
			case columnComponentType::switchBox:
				tmpCombo = new ComboBox(columns[i]->getTitle() + String(rowIdx));
				addAndMakeVisible(tmpCombo);
				tmpCombo->setEditableText(false);
				tmpCombo->setJustificationType(juce::Justification::centredLeft);
				tmpCombo->setTextWhenNothingSelected(juce::String());
				tmpCombo->setTextWhenNoChoicesAvailable(TRANS("(no choices)"));
				tmpCombo->addItem(TRANS(juce::CharPointer_UTF8("\xe5\x85\xb3\xe6\x9c\xba")), 1); //1 关机
				tmpCombo->addItem(TRANS(juce::CharPointer_UTF8("\xe5\xbc\x80\xe6\x9c\xba")), 2);
				tmpCombo->setSelectedId(1); //默认关机
				tmpCombo->addListener(this);
				rowsComponents.add(tmpCombo);

				tmpCombo->setBounds(left, top, columns[i]->getWidth() * widthScala, getRowHeight());
				//next columns left = currentLeft + currentWidth;
				left += columns[i]->getWidth() * widthScala;
				break;
			case columnComponentType::editWriteRead:
				tmpEdit = new TextEditor(columns[i]->getTitle() + String(rowIdx));
				addAndMakeVisible(tmpText);
				tmpEdit->setMultiLine(true);
				tmpEdit->setText(text);
				tmpEdit->setReadOnly(false);
				rowsComponents.add(tmpEdit);
				//tmpText->addListener(this);

				tmpEdit->setBounds(left, top, columns[i]->getWidth() * widthScala, getRowHeight());
				//next columns left = currentLeft + currentWidth;
				left += columns[i]->getWidth() * widthScala;
				break;
			default:
				tmpDefault = new TextEditor(columns[i]->getTitle() + String(i));
				addAndMakeVisible(tmpDefault);
				tmpDefault->setMultiLine(true);
				tmpDefault->setText(text);
				tmpDefault->setReadOnly(true);
				rowsComponents.add(tmpDefault);
				//tmpDefault->addListener(this);

				tmpDefault->setBounds(left, top, columns[i]->getWidth() * widthScala, getRowHeight());
				//next columns left = currentLeft + currentWidth;
				left += columns[i]->getWidth() * widthScala;
				break;
			}
		}
	}

	void redrawRows()
	{
		if (rowsComponents.size() <= 0)
			drawRows();
		for (int i = 0; i < getRowNum(); ++i)
		{
			redrawRow(i, gridData);
		}
	}

	void redrawRow(int rowIdx, StringArray data)
	{

		float top = this->getBordSize() + getTitleHeight() + rowIdx * getRowHeight();
		float left = this->getBordSize();
		float widthScala = getWidthScala();
		for (int i = 0; i < columns.size(); i++)
		{
			String text;
			switch (columns[i]->dataType)
			{
			case columnDataType::floatColumn:
			case columnDataType::intColumn:
			case columnDataType::stringColumn:
				text = data.getReference(rowIdx * getColumNum() + i);


				break;
			default:
				text = "";
				break;
			}
			TextEditor *tmpText, *tmpEdit, *tmpDefault;
			ComboBox *tmpCombo;
			switch (columns[i]->componentType)
			{
			case columnComponentType::editReadOnly:
				tmpText = (TextEditor*)(rowsComponents[rowIdx * getColumNum() + i]);
				tmpText->setBounds(left, top, columns[i]->getWidth() * widthScala, getRowHeight());
				tmpText->setText(text);
				//next columns left = currentLeft + currentWidth;
				left += columns[i]->getWidth() * widthScala;
				break;
			case columnComponentType::switchBox:
				tmpCombo = (ComboBox*)(rowsComponents[rowIdx * getColumNum() + i]);
				tmpCombo->setBounds(left, top, columns[i]->getWidth() * widthScala, getRowHeight());
				left += columns[i]->getWidth() * widthScala;
				break;
			case columnComponentType::editWriteRead:
				tmpEdit = (TextEditor*)(rowsComponents[rowIdx * getColumNum() + i]);
				tmpEdit->setBounds(left, top, columns[i]->getWidth() * widthScala, getRowHeight());
				tmpEdit->setText(text);
				left += columns[i]->getWidth() * widthScala;
				break;
			default:
				tmpDefault = (TextEditor*)(rowsComponents[rowIdx * getColumNum() + i]);
				tmpDefault->setBounds(left, top, columns[i]->getWidth() * widthScala, getRowHeight());
				tmpDefault->setText(text);
				left += columns[i]->getWidth() * widthScala;
				break;
			}
		}
	}

	void setBackgroudColour(Colour colour)
	{
		this->backgroudColour = colour;
	}


	void paint(Graphics& g) override
	{
		g.fillAll(this->backgroudColour);
	}


	int getColumNum()
	{
		if (this->isShowTitle)
			return this->columns.size();
		else
			return columnNum;
	}

	void setRowNum(int num)
	{
		if (num > 0)
		{
			this->rowNum = num;
		}
	}

	int getRowNum()
	{
		return this->rowNum;
	}

	void resized() override
	{
		Grid grid;

		grid.rowGap = 5_px;
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


		grid.autoColumns = Track(1_fr);
		grid.autoRows = Track(1_fr);

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
		redrawTitles();
		redrawRows();
	}

	//==============================================================================
	struct GridCellPanel : public Component
	{
		GridCellPanel(Colour colourToUse, const String& textToUse)
			: colour(colourToUse),
			text(textToUse)
		{}


		void paint(Graphics& g) override
		{
			g.fillAll(colour.withAlpha(0.5f));

			g.setColour(Colours::black);
			g.drawText(text, getLocalBounds().withSizeKeepingCentre(100, 100), Justification::centred, false);
		}
		Colour colour;
		String text;
	};

	struct ColumnInfo
	{
		String titleName;
		float width;
		columnComponentType componentType;
		columnDataType dataType;
		ColumnInfo(String n, float w) :titleName(n), width(w) { componentType = columnComponentType::editReadOnly;  dataType = columnDataType::stringColumn; }
		ColumnInfo(String n, float w, columnComponentType component, columnDataType data) :titleName(n), width(w), componentType(component), dataType(data)
		{}
		String getTitle() { return titleName; }
		float getWidth() { return width; }
	};



	float getWidthScala() {
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

	void setRowHeight(float h) { rowHeight = h; }
	float getRowHeight() { return rowHeight; }
	float getTitleHeight() { return this->titleHeight; }
	void setTitleHeight(float h) { this->titleHeight = h; }
	float getBordSize() { return gridBordSize; }
	void setBordSize(float v) { gridBordSize = v; }
private:
	int columnNum = 0;
	int rowNum = 0;
	bool isShowTitle = true;
	Colour backgroudColour;
	float minRowHeight = 20.0; //px
	float defaultTitleHeight = 24.0; //px
	float titleHeight = defaultTitleHeight;
	float gridBordSize = 1.0; //
	float rowHeight = 0.0; //px
	std::unique_ptr<juce::TextButton> btnTest;
	std::unique_ptr<juce::Label> lblWarnNum;
	std::unique_ptr<juce::TextEditor> txtWarnNum;
	StringArray gridData; //智能指针数组
	OwnedArray<GridCellPanel> cells;
	OwnedArray<ColumnInfo> columns;
	OwnedArray<TextButton> buttons;
	OwnedArray<TextButton> titlesButtons;
	OwnedArray<Component> rowsComponents;
	SIGNAL_II m_HiddenDelegateII;
};
