//Test_UIWidget_ImageView.h
//*************************************************************************
//	创建日期:	2014-12-13
//	文件名称:	Test_UIWidget_ImageView.h
//  创 建 人:   WilhanTian	
//	版权所有:	MIT
//	说    明:	ImageView图片控件
//*************************************************************************

#pragma once

#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"

class CTest_UIWidget_ImageViewLayer : public CCLayer
{
public:
	virtual bool init()
	{
		m_pWindow = FKCW_UIWidget_WidgetWindow::create();
		m_pWindow->setMultiTouchEnabled(true);
		addChild(m_pWindow);

		FKCW_UIWidget_ImageView* imageView = FKCW_UIWidget_ImageView::create(s_szCocosLogoPath, CCRect(0,0,200,200));
		imageView->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_UIWidget_ImageViewLayer::onClickListener));
		imageView->setOnLongClickListener(this, fkcw_uiwidget_longclick_selector(CTest_UIWidget_ImageViewLayer::onLongClickListener));
		imageView->setPosition(SCREEN_CENTER_POS);
		imageView->setTouchEnabled( true );
		m_pWindow->addChild(imageView);

		FKCW_UIWidget_ImageView* imageView1 = FKCW_UIWidget_ImageView::createWithSpriteFrame(CCSpriteFrame::create(s_szCocosLogoPath, CCRect(10, 10, 50, 50)));
		imageView1->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_UIWidget_ImageViewLayer::onClickListener));
		imageView1->setOnLongClickListener(this, fkcw_uiwidget_longclick_selector(CTest_UIWidget_ImageViewLayer::onLongClickListener));
		imageView1->setPosition(SCREEN_CENTER_POS + CCPoint(0, 130));
		imageView1->setTouchEnabled( true );
		m_pWindow->addChild(imageView1);

		m_pInfoLabel = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("请点击图片控件，该控件已开启接受点击事件和长按事件").c_str(), "Arial", 26 );
		m_pInfoLabel->setColor( ccc3(230,155,3) );
		m_pInfoLabel->setPosition(SCREEN_CENTER_POS + CCPoint(0, -200));
		m_pWindow->addChild(m_pInfoLabel);

		return true;
	}
public:
	void onClickListener(CCObject* pSender)
	{
		m_pInfoLabel->setString(FKCW_Base_StringConvert::a2u("控件被点击").c_str());
	}
	bool onLongClickListener(CCObject* pSender, CCTouch* touch)
	{
		m_pInfoLabel->setString(FKCW_Base_StringConvert::a2u("控件被长按").c_str());
		return true;
	}
public:
	CREATE_FUNC( CTest_UIWidget_ImageViewLayer );
private:
	CCLabelTTF* m_pInfoLabel;
	FKCW_UIWidget_WidgetWindow* m_pWindow;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(UIWidget, ImageView, 图片控件)