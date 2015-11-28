/********************************************************************
	created:	2014/12/16
	file base:	Test_UIWidget_TableView
	author:		FreeKnight
	
	purpose:	
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
class CTest_UIWidget_TableViewLayer : public CCLayer
{
public:
	bool init()
	{
		//创建基本容器
		m_pWindow = FKCW_UIWidget_WidgetWindow::create();
		m_pWindow->setMultiTouchEnabled(true);
		this->addChild(m_pWindow);

		m_lDataQueue.push_back("B");
		m_lDataQueue.push_back("C");
		m_lDataQueue.push_back("D");
		m_lDataQueue.push_back("E");
		m_lDataQueue.push_back("F");
		m_lDataQueue.push_back("G");
		m_lDataQueue.push_back("H");
		m_lDataQueue.push_back("I");
		m_lDataQueue.push_back("J");
		m_lDataQueue.push_back("K");
		m_lDataQueue.push_back("L");
		m_lDataQueue.push_back("M");
		m_lDataQueue.push_back("N");
		m_lDataQueue.push_back("O");
		m_lDataQueue.push_back("P");
		m_lDataQueue.push_back("Q");
		m_lDataQueue.push_back("R");
		m_lDataQueue.push_back("S");
		m_lDataQueue.push_back("T");
		m_lDataQueue.push_back("U");
		m_lDataQueue.push_back("V");
		m_lDataQueue.push_back("W");
		m_lDataQueue.push_back("X");
		m_lDataQueue.push_back("Y");
		m_lDataQueue.push_back("Z");

		m_vDatas.push_back("A");

		pTable = FKCW_UIWidget_TableView::create(
			CCSize(150.0f, 54.0f * 5),
			CCSize(150.0f, 54.0f),
			m_vDatas.size(), this, 
			fkcw_uiwidget_datasource_adapter_selector(CTest_UIWidget_TableViewLayer::tableviewDataSource));
		pTable->setDirection(eScrollViewDirectionVertical);
		pTable->setAutoRelocate(true);
		pTable->setPosition(SCREEN_CENTER_POS);
		m_pWindow->addChild(pTable);
		pTable->reloadData();

		FKCW_UIWidget_Button* pButton = FKCW_UIWidget_Button::createWith9Sprite(CCSizeMake(150, 50), 
			s_szSprite9Btn1Path, s_szSprite9Btn2Path );
		pButton->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_UIWidget_TableViewLayer::onClick));
		pButton->setPosition(CCPoint(200, 320));
		pButton->getLabel()->setFontSize(25.0f);
		pButton->getLabel()->setString(FKCW_Base_StringConvert::a2u( "添加按钮" ).c_str());
		m_pWindow->addChild(pButton);

		return true;
	}
public:
	CCObject* tableviewDataSource(CCObject* pConvertView, unsigned int idx)
	{
		FKCW_UIWidget_TableViewCell* pCell = (FKCW_UIWidget_TableViewCell*) pConvertView;
		FKCW_UIWidget_Button* pButton = NULL;

		if(!pCell)
		{
			pCell = new FKCW_UIWidget_TableViewCell();
			pCell->autorelease();

			pButton = FKCW_UIWidget_Button::createWith9Sprite(CCSizeMake(150, 50), s_szSprite9Btn1Path, s_szSprite9Btn2Path );
			pButton->setPosition(CCPoint(150.0f / 2, 54.0f / 2));
			pButton->getLabel()->setFontSize(25.0f);
			pButton->setTag(1);
			pCell->addChild(pButton);
		}
		else
		{
			pButton = (FKCW_UIWidget_Button*) pCell->getChildByTag(1);
		}

		pButton->getLabel()->setString(m_vDatas[idx].c_str());
		pButton->setUserTag(idx);

		return pCell;
	}

	void onClick(CCObject* pSender)
	{
		if( m_lDataQueue.empty() )
		{
			return;
		}

		m_vDatas.push_back(m_lDataQueue.front());
		m_lDataQueue.pop_front();

		pTable->setCountOfCell(m_vDatas.size());
		pTable->reloadData();
		pTable->setContentOffset(pTable->getMaxOffset());
	}
public:
	CREATE_FUNC( CTest_UIWidget_TableViewLayer );
public:
	FKCW_UIWidget_WidgetWindow* m_pWindow;
	std::list<string> m_lDataQueue;
	std::vector<string> m_vDatas;
	FKCW_UIWidget_TableView* pTable;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(UIWidget, TableView, 纵向滚动表格)