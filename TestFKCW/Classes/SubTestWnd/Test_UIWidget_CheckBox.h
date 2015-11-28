//*************************************************************************
//	创建日期:	2014-12-13
//	文件名称:	Test_UIWidget_CheckBox.h
//  创 建 人:   WilhanTian	
//	版权所有:	MIT
//	说    明:	CheckBox选框
//  测试结果:	
//*************************************************************************

#pragma once

#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"

class CTest_UIWidget_CheckBoxLayer : public CCLayer
{
public:
	virtual bool init()
	{
		m_pWindow = FKCW_UIWidget_WidgetWindow::create();
		m_pWindow->setMultiTouchEnabled(true);
		addChild(m_pWindow);

		FKCW_UIWidget_CheckBox* checkBox = FKCW_UIWidget_CheckBox::create();
		checkBox->setNormalImage(s_szCkb_NormalPath);
		checkBox->setNormalPressImage(s_szCkb_NormalPressPath);
		checkBox->setCheckedImage(s_szCkb_CheckedPath);
		checkBox->setCheckedPressImage(s_szCkb_CheckedPressPath);
		checkBox->setDisabledNormalImage(s_szCkb_DisablePath);
		checkBox->setDisabledCheckedImage(s_szCkb_DisablePressPath);
		checkBox->setOnCheckListener(this, fkcw_uiwidget_check_selector(CTest_UIWidget_CheckBoxLayer::OnCheckListener));
		checkBox->setPosition(SCREEN_CENTER_POS);
		m_pWindow->addChild(checkBox);

		FKCW_UIWidget_CheckBox* checkBox2 = FKCW_UIWidget_CheckBox::create();
		checkBox2->setNormalImage(s_szCkb_NormalPath);
		checkBox2->setNormalPressImage(s_szCkb_NormalPressPath);
		checkBox2->setCheckedImage(s_szCkb_CheckedPath);
		checkBox2->setCheckedPressImage(s_szCkb_CheckedPressPath);
		checkBox2->setDisabledNormalImage(s_szCkb_DisablePath);
		checkBox2->setDisabledCheckedImage(s_szCkb_DisablePressPath);
		checkBox2->setOnCheckListener(this, fkcw_uiwidget_check_selector(CTest_UIWidget_CheckBoxLayer::OnCheck2Listener));
		checkBox2->setPosition(SCREEN_CENTER_POS+CCPoint(0, 100));
		checkBox2->setTag( 0x12345 );
		checkBox2->setChecked(true);
		checkBox2->setEnabled(false);
		m_pWindow->addChild(checkBox2);

		FKCW_UIWidget_Button* btn = FKCW_UIWidget_Button::create(s_szButtonNormalPath, s_szButtonPressPath, s_szButtonDisablePath);
		btn->setPosition(SCREEN_CENTER_POS+CCPoint(180,100));
		btn->initText(FKCW_Base_StringConvert::a2u("控制复选框").c_str(), "", 28);
		btn->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_UIWidget_CheckBoxLayer::onBtnClick));
		m_pWindow->addChild(btn);

		m_pText = FKCW_UIWidget_Label::create();
		m_pText->setPosition(SCREEN_CENTER_POS+CCPoint(0, -200));
		m_pText->setFontSize(26.0f);
		m_pText->setString("");
		m_pWindow->addChild(m_pText);

		return true;
	}
public:
	void OnCheckListener(CCObject* pSender, bool bChecked)
	{
		FKCW_UIWidget_CheckBox* pCheckBox = (FKCW_UIWidget_CheckBox*) pSender;
		if( bChecked /*pCheckBox->isChecked()*/ )
		{
			m_pText->setString("checked");
		}
		else
		{
			m_pText->setString("unchecked");
		}
	}
	void OnCheck2Listener(CCObject* pSender, bool bChecked)
	{
	}
	void onBtnClick(CCObject* pSender)
	{
		FKCW_UIWidget_CheckBox* pCheckBox = (FKCW_UIWidget_CheckBox*) m_pWindow->getChildByTag( 0x12345 );
		pCheckBox->setChecked( !pCheckBox->isChecked() );
	}
public:
	CREATE_FUNC( CTest_UIWidget_CheckBoxLayer );
private:
	FKCW_UIWidget_WidgetWindow* m_pWindow;
	FKCW_UIWidget_Label* m_pText;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(UIWidget, CheckBox, CheckBox选框)