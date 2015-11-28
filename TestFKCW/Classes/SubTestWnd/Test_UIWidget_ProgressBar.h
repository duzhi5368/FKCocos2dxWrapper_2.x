//*************************************************************************
//	创建日期:	2014-12-15
//	文件名称:	Test_UIWidget_ProgressBar.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
class CTest_UIWidget_ProgressBarLayer : public CCLayer
{
public:
	bool init()
	{
		//创建基本容器
		m_pWindow = FKCW_UIWidget_WidgetWindow::create();
		m_pWindow->setMultiTouchEnabled(true);
		this->addChild(m_pWindow);

		CCSprite* m_pBg = CCSprite::create(s_szBackground1Path);
		m_pBg->setScale(1.3f);
		m_pBg->setPosition(SCREEN_CENTER_POS);
		m_pWindow->addChild(m_pBg);

		//创建进度条
		m_pProgressBar = FKCW_UIWidget_ProgressBar::create( s_szProgressPath );
		m_pProgressBar->setBackgroundImage( s_szProgressBGPath );
		m_pProgressBar->setPosition(SCREEN_CENTER_POS);
		m_pProgressBar->setMaxValue(100);
		m_pProgressBar->setMinValue(0);
		m_pProgressBar->setDirection(eProgressBarDirectionLeftToRight);
		m_pProgressBar->setOnValueChangedListener(
			this, fkcw_uiwidget_valuechanged_selector(CTest_UIWidget_ProgressBarLayer::onProgressing));
		m_pProgressBar->setContentSize(CCSize(SCERRN_WIDTH, SCERRN_HEIGHT));
		m_pWindow->addChild(m_pProgressBar);

		FKCW_UIWidget_Button* pButton = FKCW_UIWidget_Button::createWith9Sprite(CCSize(150, 60),
			s_szSprite9Btn1Path, s_szSprite9Btn2Path );
		pButton->setPosition(CCPoint(340, 450));
		pButton->setUserTag(1);
		pButton->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_UIWidget_ProgressBarLayer::onClick));
		pButton->initText(FKCW_Base_StringConvert::a2u("增加进度").c_str(), "", 30);
		m_pWindow->addChild(pButton);

		FKCW_UIWidget_Button* pButton2 = FKCW_UIWidget_Button::createWith9Sprite(CCSize(150, 60),
			s_szSprite9Btn1Path, s_szSprite9Btn2Path );
		pButton2->setPosition(CCPoint(540, 450));
		pButton2->setUserTag(2);
		pButton2->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_UIWidget_ProgressBarLayer::onClick));
		pButton2->initText(FKCW_Base_StringConvert::a2u("减少进度").c_str(), "", 30);
		m_pWindow->addChild(pButton2);

		FKCW_UIWidget_Button* pButton3 = FKCW_UIWidget_Button::createWith9Sprite(CCSize(200, 60),
			s_szSprite9Btn1Path, s_szSprite9Btn2Path );
		pButton3->setPosition(CCPoint(740, 450));
		pButton3->setUserTag(3);
		pButton3->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_UIWidget_ProgressBarLayer::onClick));
		pButton3->initText(FKCW_Base_StringConvert::a2u("调整方向").c_str(), "", 30);
		m_pWindow->addChild(pButton3);

		FKCW_UIWidget_Button* pButton4 = FKCW_UIWidget_Button::createWith9Sprite(CCSize(150, 60),
			s_szSprite9Btn1Path, s_szSprite9Btn2Path );
		pButton4->setPosition(CCPoint(420, 200));
		pButton4->setUserTag(4);
		pButton4->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_UIWidget_ProgressBarLayer::onClick));
		pButton4->initText(FKCW_Base_StringConvert::a2u("启动进度条").c_str(), "", 30);
		pButton4->setCascadeTextSizeEnabled(true, CCSize(50, 30));
		m_pWindow->addChild(pButton4);

		FKCW_UIWidget_Button* pButton5 = FKCW_UIWidget_Button::createWith9Sprite(CCSize(150, 60),
			s_szSprite9Btn1Path, s_szSprite9Btn2Path );
		pButton5->setPosition(CCPoint(740, 200));
		pButton5->setUserTag(5);
		pButton5->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_UIWidget_ProgressBarLayer::onClick));
		pButton5->initText(FKCW_Base_StringConvert::a2u("停止进度条").c_str(), "", 30);
		pButton5->setCascadeTextSizeEnabled(true, CCSize(50, 30));
		m_pWindow->addChild(pButton5);
		
		m_pText = FKCW_UIWidget_Label::create();
		m_pText->setAnchorPoint(CCPoint(0, 0.5));
		m_pText->setPosition(CCPoint(270, 400));
		m_pText->setFontSize(35.0f);
		m_pText->setString("none");
		m_pWindow->addChild(m_pText);

		return true;
	}
public:
	void onProgressing(CCObject* pSender, int nValue)
	{
		char buff[128] = {0};
		sprintf(buff, "值 = %d, 进度 = %.2f", m_pProgressBar->getValue(), m_pProgressBar->getPercentage());
		m_pText->setString(FKCW_Base_StringConvert::a2u(buff).c_str());
	}

	void onClick(CCObject* pSender)
	{
		FKCW_UIWidget_Button* pButton = (FKCW_UIWidget_Button*) pSender;
		if( pButton->getUserTag() == 1 )
		{
			m_pProgressBar->setValue(m_pProgressBar->getValue() + 10);
		}
		else if( pButton->getUserTag() == 2 )
		{
			m_pProgressBar->setValue(m_pProgressBar->getValue() - 5);
		}
		else if( pButton->getUserTag() == 3 )
		{
			if( m_pProgressBar->getDirection() == eProgressBarDirectionLeftToRight )
			{
				m_pProgressBar->setDirection(eProgressBarDirectionRightToLeft);
			}
			else
			{
				m_pProgressBar->setDirection(eProgressBarDirectionLeftToRight);
			}
		}
		else if( pButton->getUserTag() == 4 )
		{
			m_pProgressBar->startProgress(100, 5.0f);
		}
		else if( pButton->getUserTag() == 5 )
		{
			m_pProgressBar->stopProgress();
		}
	}
public:

public:
	CREATE_FUNC( CTest_UIWidget_ProgressBarLayer );
public:
	FKCW_UIWidget_WidgetWindow* m_pWindow;
	FKCW_UIWidget_ProgressBar* m_pProgressBar;
	FKCW_UIWidget_Label* m_pText;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(UIWidget, ProgressBar, 进度条)