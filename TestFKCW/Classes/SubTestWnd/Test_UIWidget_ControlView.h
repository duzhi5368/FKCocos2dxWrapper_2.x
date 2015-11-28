//*************************************************************************
//	创建日期:	2014-12-13
//	文件名称:	Test_UIWidget_ControlView.h
//  创 建 人:   WilhanTian	
//	版权所有:	MIT
//	说    明:	ControlView摇杆控件
//  测试结果:	
//*************************************************************************

#pragma once

#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"

class CTest_UIWidget_ControlViewLayer : public CCLayer
{
public:
	virtual bool init()
	{
		m_pWindow = FKCW_UIWidget_WidgetWindow::create();
		m_pWindow->setMultiTouchEnabled(true);
		addChild(m_pWindow);

		FKCW_UIWidget_ControlView* controlView = FKCW_UIWidget_ControlView::create(s_szBaseBoardPath, s_szJoystickPath);
		controlView->setPosition(SCREEN_CENTER_POS);
		controlView->setRadius( 128/2 );
		controlView->setOnControlListener(this, fkcw_uiwidget_control_selector(CTest_UIWidget_ControlViewLayer::onControlListener));
		m_pWindow->addChild(controlView);

		m_pText = FKCW_UIWidget_Label::create();
		m_pText->setPosition(SCREEN_CENTER_POS+CCPoint(0, -200));
		m_pText->setFontSize(24.0f);
		m_pText->setString("");
		m_pWindow->addChild(m_pText);

		return true;
	}
public:
	void onControlListener(CCObject* pSender, float fx, float fy)
	{
		char szInfo[256];
		memset( szInfo, 0, 256 );
		sprintf( szInfo, "X = %f, Y = %f", fx, fy );
		m_pText->setString( szInfo );
	}
public:
	CREATE_FUNC( CTest_UIWidget_ControlViewLayer );
private:
	FKCW_UIWidget_WidgetWindow* m_pWindow;
	FKCW_UIWidget_Label* m_pText;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(UIWidget, ControlView, 摇杆控制层|街机游戏里的摇杆)