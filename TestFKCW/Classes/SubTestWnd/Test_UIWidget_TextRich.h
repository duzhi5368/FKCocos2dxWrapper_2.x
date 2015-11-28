//*************************************************************************
//	创建日期:	2014-12-15
//	文件名称:	Test_UIWidget_TextRich.h
//  创 建 人:   WilhanTIan
//	版权所有:	MIT
//	说    明:	富文本
//  测试结果:		
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
class CTest_UIWidget_TextRichLayer : public CCLayer
{
public:
	bool init()
	{
		//创建基本容器
		m_pWindow = FKCW_UIWidget_WidgetWindow::create();
		m_pWindow->setMultiTouchEnabled(true);
		this->addChild(m_pWindow);

		FKCW_UIWidget_TextRich* textRich = FKCW_UIWidget_TextRich::create();
		textRich->setPosition(SCREEN_CENTER_POS);
		textRich->setFontSize(12);
		textRich->setFontName("Arial");
		textRich->setMaxLineLength(20);
		textRich->setVerticalSpacing(15.0f);
		textRich->setOnTextRichClickListener(this, fkcw_uiwidget_textrichclick_selector(CTest_UIWidget_TextRichLayer::onClickTextRich));

		textRich->insertElement(FKCW_Base_StringConvert::a2u("我是一个默认白色文字").c_str());
		textRich->insertElement(FKCW_Base_StringConvert::a2u("颜色-黄色-字号-24 ").c_str(), "", 24, ccYELLOW, FKCW_Base_StringConvert::a2u("你点击了黄色文字").c_str());
		CCSprite* pIcon = CCSprite::create(s_szEmoticon);
		pIcon->runAction(CCRepeatForever::create(CCBlink::create(1.0f, 5)));
		textRich->insertElement(pIcon, 0, FKCW_Base_StringConvert::a2u("你点击了表情").c_str());
		textRich->insertElement(FKCW_Base_StringConvert::a2u("随便点下 ").c_str(), "", 30, ccMAGENTA, FKCW_Base_StringConvert::a2u("你点击了粉红文字").c_str());
		textRich->insertElement(FKCW_Base_StringConvert::a2u("点击每段富文本，会产生不同事件哦").c_str(), "", 26, ccBLUE, FKCW_Base_StringConvert::a2u("你点击了蓝色文字").c_str());
		
		textRich->reloadData();
		m_pWindow->addChild(textRich);


		m_label = CCLabelTTF::create("", "", 26);
		m_label->setColor(ccBLACK);
		m_label->setPosition(SCREEN_CENTER_POS + CCPoint(0, -150));
		this->addChild(m_label);

		return true;
	}
public:
	void onClickTextRich(CCObject* pSender, const char* description)
	{
		if(description == NULL)
			m_label->setString(FKCW_Base_StringConvert::a2u("你点击的富文本没有描述文字").c_str());
		else
			m_label->setString(description);
	}
public:
	CREATE_FUNC( CTest_UIWidget_TextRichLayer );
protected:
	FKCW_UIWidget_WidgetWindow* m_pWindow;
	CCLabelTTF* m_label;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(UIWidget, TextRich, 富文本)