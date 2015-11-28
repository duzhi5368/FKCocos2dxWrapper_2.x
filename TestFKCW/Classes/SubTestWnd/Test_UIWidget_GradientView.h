//*************************************************************************
//	创建日期:	2014-12-13
//	文件名称:	Test_UIWidget_GradientView.h
//  创 建 人:   WilhanTian	
//	版权所有:	MIT
//	说    明:	GradientView
//  测试结果:	
//*************************************************************************

#pragma once

#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"

class CTest_UIWidget_GradientViewLayer : public CCLayer
{
public:
	virtual bool init()
	{
		m_pWindow = FKCW_UIWidget_WidgetWindow::create();
		m_pWindow->setMultiTouchEnabled(true);
		addChild(m_pWindow);

		FKCW_UIWidget_GradientView* gradientView1 = 
			FKCW_UIWidget_GradientView::create(cc4BLUE, cc4YELLOW, CCPoint(1.0f, 0.0f));
		gradientView1->setPosition(SCREEN_CENTER_POS+ccp(-200,0));
		m_pWindow->addChild(gradientView1);

		FKCW_UIWidget_GradientView* gradientView2 = 
			FKCW_UIWidget_GradientView::create(cc4BLUE, cc4YELLOW, CCPoint(0.0f, 1.0f));
		gradientView2->setPosition(SCREEN_CENTER_POS+ccp(-70,0));
		gradientView2->setStartOpacity(0);
		gradientView2->setEndOpacity(255);
		m_pWindow->addChild(gradientView2);

		FKCW_UIWidget_GradientView* gradientView3 = 
			FKCW_UIWidget_GradientView::create(cc4RED, cc4GREEN, CCPoint(1.0f, 1.0f));
		gradientView3->setPosition(SCREEN_CENTER_POS+ccp(70,0));
		m_pWindow->addChild(gradientView3);

		FKCW_UIWidget_GradientView* gradientView4 = 
			FKCW_UIWidget_GradientView::create(cc4BLUE, cc4YELLOW, CCPoint(1.0f, 0.0f));
		gradientView4->setPosition(SCREEN_CENTER_POS+ccp(200,0));
		gradientView4->setTag( 0x123 );
		m_pWindow->addChild(gradientView4);

		FKCW_UIWidget_Button* btn = FKCW_UIWidget_Button::create(s_szButtonNormalPath, s_szButtonPressPath, s_szButtonDisablePath);
		btn->setPosition(SCREEN_CENTER_POS+ccp(200,-100));
		btn->initText(FKCW_Base_StringConvert::a2u("更改4号向量").c_str(), "", 28);
		btn->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_UIWidget_GradientViewLayer::onBtnClick));
		m_pWindow->addChild(btn);

		return true;
	}
public:
	void onBtnClick(CCObject* pSender)
	{
		FKCW_UIWidget_GradientView* p = (FKCW_UIWidget_GradientView*)(m_pWindow->getChildByTag( 0x123 ));
		p->setVector( p->getVector() + ccp(0, 0.1f));	
		char szBuf[64];
		memset(szBuf, 0, 64);
		sprintf( szBuf, "修改向量 x = %f, y = %f",  p->getVector().x, p->getVector().y );
		FKCOLORLOG( eConsolBKColor_Green, szBuf );
	}
public:
	CREATE_FUNC( CTest_UIWidget_GradientViewLayer );
private:
	FKCW_UIWidget_WidgetWindow* m_pWindow;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(UIWidget, GradientView, 渐变控件)