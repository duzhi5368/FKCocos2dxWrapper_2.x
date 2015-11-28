//FKCW_UIWidget_ToggleView
//*************************************************************************
//	创建日期:	2014-12-15
//	文件名称:	Test_UIWidget_ToggleView.h
//  创 建 人:   WilhanTIan
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
class CTest_UIWidget_ToggleViewLayer : public CCLayer
{
public:
	bool init()
	{
		//创建基本容器
		m_pWindow = FKCW_UIWidget_WidgetWindow::create();
		m_pWindow->setMultiTouchEnabled(true);
		this->addChild(m_pWindow);

		FKCW_UIWidget_ToggleView* pToggle1 = FKCW_UIWidget_ToggleView::create(s_szToggle3Path, s_szToggle1Path);
		pToggle1->setOnCheckListener(this, fkcw_uiwidget_check_selector(CTest_UIWidget_ToggleViewLayer::onCheck));
		pToggle1->setPosition(CCPoint(200, 450));
		pToggle1->setExclusion(1);
		m_pWindow->addChild(pToggle1);

		FKCW_UIWidget_ToggleView* pToggle2 = FKCW_UIWidget_ToggleView::create(s_szToggle3Path, s_szToggle1Path);
		pToggle2->setOnCheckListener(this, fkcw_uiwidget_check_selector(CTest_UIWidget_ToggleViewLayer::onCheck));
		pToggle2->setPosition(CCPoint(200, 350));
		pToggle2->setExclusion(1);
		m_pWindow->addChild(pToggle2);

		FKCW_UIWidget_ToggleView* pToggle3 = FKCW_UIWidget_ToggleView::create(s_szToggle3Path, s_szToggle1Path);
		pToggle3->setOnCheckListener(this, fkcw_uiwidget_check_selector(CTest_UIWidget_ToggleViewLayer::onCheck));
		pToggle3->setPosition(CCPoint(200, 250));
		pToggle3->setExclusion(1);
		m_pWindow->addChild(pToggle3);

		FKCW_UIWidget_Label* pText1 = FKCW_UIWidget_Label::create();
		pText1->setAnchorPoint(CCPoint(0, 0.5));
		pText1->setPosition(CCPoint(300, 450));
		pText1->setFontSize(35.0f);
		pText1->setString("none");
		m_pWindow->addChild(pText1);
		pToggle1->setUserObject(pText1);

		FKCW_UIWidget_Label* pText2 = FKCW_UIWidget_Label::create();
		pText2->setAnchorPoint(CCPoint(0, 0.5));
		pText2->setPosition(CCPoint(300, 350));
		pText2->setFontSize(35.0f);
		pText2->setString("none");
		m_pWindow->addChild(pText2);
		pToggle2->setUserObject(pText2);

		FKCW_UIWidget_Label* pText3 = FKCW_UIWidget_Label::create();
		pText3->setAnchorPoint(CCPoint(0, 0.5));
		pText3->setPosition(CCPoint(300, 250));
		pText3->setFontSize(35.0f);
		pText3->setString("none");
		m_pWindow->addChild(pText3);
		pToggle3->setUserObject(pText3);


		FKCW_UIWidget_ToggleView* pToggle4 = FKCW_UIWidget_ToggleView::create(s_szToggle3Path, s_szToggle1Path);
		pToggle4->setPosition(CCPoint(600, 250));
		pToggle4->setExclusion(2);
		m_pWindow->addChild(pToggle4);

		FKCW_UIWidget_Label* pText4 = FKCW_UIWidget_Label::create();
		pText4->setPosition(CCPoint(700, 250));
		pText4->setFontSize(35.0f);
		pText4->setString(FKCW_Base_StringConvert::a2u("组2").c_str());
		m_pWindow->addChild(pText4);
		pToggle4->setUserObject(pText4);

		FKCW_UIWidget_ToggleView* pToggle5 = FKCW_UIWidget_ToggleView::create(s_szToggle3Path, s_szToggle1Path);
		pToggle5->setPosition(CCPoint(600, 350));
		pToggle5->setExclusion(3);
		m_pWindow->addChild(pToggle5);

		FKCW_UIWidget_Label* pText5 = FKCW_UIWidget_Label::create();
		pText5->setPosition(CCPoint(700, 350));
		pText5->setFontSize(35.0f);
		pText5->setString(FKCW_Base_StringConvert::a2u("组3").c_str());
		m_pWindow->addChild(pText5);
		pToggle5->setUserObject(pText5);


		return true;
	}
public:
	void onCheck(CCObject* pSender, bool bChecked)
	{
		FKCW_UIWidget_ToggleView* pToggle = (FKCW_UIWidget_ToggleView*) pSender;
		FKCW_UIWidget_Label* pText = (FKCW_UIWidget_Label*) pToggle->getUserObject();

		if( bChecked )
		{
			pText->setString(FKCW_Base_StringConvert::a2u("组1：选中").c_str());
		}
		else
		{
			pText->setString(FKCW_Base_StringConvert::a2u("组1：未选中").c_str());
		}
	}
public:
	CREATE_FUNC( CTest_UIWidget_ToggleViewLayer );
public:
	FKCW_UIWidget_WidgetWindow* m_pWindow;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(UIWidget, ToggleView, 开关View)