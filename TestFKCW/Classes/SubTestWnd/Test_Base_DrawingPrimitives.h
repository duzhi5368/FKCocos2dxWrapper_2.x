//*************************************************************************
//	创建日期:	2014-12-18
//	文件名称:	Test_Base_DrawingPrimitives.h
//  创 建 人:   WilhanTian	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
class CTest_Base_DrawingPrimitivesLayer : public CCLayer
{
public:
	bool init()
	{
		m_nTotalNum = 0;
		//创建基本容器
		m_pWindow = FKCW_UIWidget_WidgetWindow::create();
		m_pWindow->setMultiTouchEnabled(true);
		this->addChild(m_pWindow);
		//按钮
		FKCW_UIWidget_Button* pRectButton = FKCW_UIWidget_Button::createWith9Sprite(CCSizeMake(150, 50), 
			s_szSprite9Btn1Path, s_szSprite9Btn2Path );
		pRectButton->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_Base_DrawingPrimitivesLayer::onClick));
		pRectButton->setPosition(CCPoint(200, 320));
		pRectButton->getLabel()->setFontSize(12.0f);
		pRectButton->getLabel()->setString(FKCW_Base_StringConvert::a2u( "添加随机矩形" ).c_str());
		pRectButton->setTag(0);
		m_pWindow->addChild(pRectButton, 100);

		FKCW_UIWidget_Button* pLineButton = FKCW_UIWidget_Button::createWith9Sprite(CCSizeMake(150, 50), 
			s_szSprite9Btn1Path, s_szSprite9Btn2Path );
		pLineButton->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_Base_DrawingPrimitivesLayer::onClick));
		pLineButton->setPosition(CCPoint(200 + 170, 320));
		pLineButton->getLabel()->setFontSize(12.0f);
		pLineButton->getLabel()->setString(FKCW_Base_StringConvert::a2u( "添加随机线" ).c_str());
		pLineButton->setTag(1);
		m_pWindow->addChild(pLineButton, 100);

		FKCW_UIWidget_Button* pDotButton = FKCW_UIWidget_Button::createWith9Sprite(CCSizeMake(150, 50), 
			s_szSprite9Btn1Path, s_szSprite9Btn2Path );
		pDotButton->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_Base_DrawingPrimitivesLayer::onClick));
		pDotButton->setPosition(CCPoint(200 + 170 * 2, 320));
		pDotButton->getLabel()->setFontSize(12.0f);
		pDotButton->getLabel()->setString(FKCW_Base_StringConvert::a2u( "添加随机点" ).c_str());
		pDotButton->setTag(2);
		m_pWindow->addChild(pDotButton, 100);

		return true;
	}

	void onExit()
	{
		this->unscheduleAllSelectors();
		CCLayer::onExit();
	}
public:
	void onClick(CCObject* pSender)
	{
		m_nType = ((CCNode*)pSender)->getTag();
		this->schedule(schedule_selector(CTest_Base_DrawingPrimitivesLayer::timer), 0.1f);
	}

	void timer(float time)
	{
		if(m_nType == 0)
		{
			addRect();
		}
		if(m_nType == 1)
		{
			addLine();
		}
		if(m_nType == 2)
		{
			addDot();
		}
		m_nTotalNum++;
		if( m_nTotalNum == 50 )
		{
			m_nTotalNum = 0;
			this->unscheduleAllSelectors();
		}
	}

	void addDot()
	{
		float x = 465 + FKCW_Base_Utils::RandomInRange(0, 30);
		float y = 500 + FKCW_Base_Utils::RandomInRange(0, 30);
		ccColor4F color = {FKCW_Base_Utils::Random01(), FKCW_Base_Utils::Random01(), FKCW_Base_Utils::Random01(), FKCW_Base_Utils::Random01()};
		CCDotNode* dotNode = CCDotNode::createWithPoint(ccp(x, y), color);
		m_pWindow->addChild(dotNode);
	}
	void addLine()
	{
		float x1 = FKCW_Base_Utils::RandomInRange(0, SCERRN_WIDTH);
		float y1 = FKCW_Base_Utils::RandomInRange(0, SCERRN_HEIGHT);
		float x2 = FKCW_Base_Utils::RandomInRange(0, SCERRN_WIDTH);
		float y2 = FKCW_Base_Utils::RandomInRange(0, SCERRN_HEIGHT);
		ccColor4F color = {FKCW_Base_Utils::Random01(), FKCW_Base_Utils::Random01(), FKCW_Base_Utils::Random01(), FKCW_Base_Utils::Random01()};
		CCLineNode* lineNode = CCLineNode::createWithPoints(ccp(x1, y1), ccp(x2, y2), color);
		m_pWindow->addChild(lineNode);
	}
	void addRect()
	{
		float x = FKCW_Base_Utils::RandomInRange(0, SCERRN_WIDTH);
		float y = FKCW_Base_Utils::RandomInRange(0, SCERRN_HEIGHT);
		float w = FKCW_Base_Utils::RandomInRange(10, 300);
		float h = FKCW_Base_Utils::RandomInRange(10, 300);
		ccColor4F color = {FKCW_Base_Utils::Random01(), FKCW_Base_Utils::Random01(), FKCW_Base_Utils::Random01(), FKCW_Base_Utils::Random01()};
		CCRectNode* rectNode = CCRectNode::createWithRect(CCRect(x, y, w, h), color, FKCW_Base_Utils::Random01() > 0.5f);
		m_pWindow->addChild(rectNode);
	}
public:
	CREATE_FUNC( CTest_Base_DrawingPrimitivesLayer );
protected:
	FKCW_UIWidget_WidgetWindow* m_pWindow;
	int							m_nType;
	int							m_nTotalNum;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(Base, DrawingPrimitives, 基本绘制元素)
//-------------------------------------------------------------------------