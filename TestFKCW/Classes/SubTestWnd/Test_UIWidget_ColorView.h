//*************************************************************************
//	创建日期:	2014-12-13
//	文件名称:	Test_UIWidget_ColorView.h
//  创 建 人:   WilhanTian	
//	版权所有:	MIT
//	说    明:	ColorView颜色块
//  测试结果:		
//*************************************************************************

#pragma once

#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"

class CTest_UIWidget_ColorViewLayer : public CCLayer
{
public:
	virtual bool init()
	{
		m_pWindow = FKCW_UIWidget_WidgetWindow::create();
		m_pWindow->setMultiTouchEnabled(true);
		addChild(m_pWindow);

		colorView = FKCW_UIWidget_ColorView::create(cc4BLACK);
		colorView->setPosition(SCREEN_CENTER_POS);
		colorView->setContentSize(CCSize(100, 200));
		m_pWindow->addChild(colorView);

		CCLabelTTF* lable = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("点我随机更改View大小").c_str(), "Arial", 26 );
		lable->setColor(ccc3(230,155,3));

		CCMenuItemLabel* itemLable = CCMenuItemLabel::create(lable, this, menu_selector(CTest_UIWidget_ColorViewLayer::menuCallFunc));
		CCMenu* menu = CCMenu::create(itemLable, NULL);
		menu->setPosition(SCREEN_CENTER_POS + CCPoint(0, -200));
		this->addChild(menu);

		return true;
	}
public:
	void menuCallFunc(CCObject* pSender)
	{
		colorView->setColor( ccc3( 
			static_cast<const GLubyte>(FKCW_Base_Utils::RandomInRange(5, 255)), 
			static_cast<const GLubyte>(FKCW_Base_Utils::RandomInRange(5, 255)),
			static_cast<const GLubyte>(FKCW_Base_Utils::RandomInRange(5, 255)) ));
		colorView->setOpacity( static_cast<GLubyte>(FKCW_Base_Utils::RandomInRange(20, 255)) );
		colorView->setContentSize(CCSize(FKCW_Base_Utils::RandomInRange(50, 500), 
			FKCW_Base_Utils::RandomInRange(50, 500)));
	}
public:
	CREATE_FUNC( CTest_UIWidget_ColorViewLayer );
private:
	FKCW_UIWidget_ColorView* colorView;
	FKCW_UIWidget_WidgetWindow* m_pWindow;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(UIWidget, ColorView, ColorView颜色块)