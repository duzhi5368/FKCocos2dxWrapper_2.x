//*************************************************************************
//	创建日期:	2014-12-22
//	文件名称:	Test_Base_DecodeHtml.h
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
static const string s_sTestHTMLStr = "I'll &quot;walk&quot; the &lt;b&gt;dog&lt;/b&gt; now";
class CTest_Base_DecodeHtmlLayer : public CCLayer
{
public:
	virtual bool init()
	{
		m_pWindow = FKCW_UIWidget_WidgetWindow::create();
		m_pWindow->setMultiTouchEnabled(true);
		addChild(m_pWindow);

		FKCW_UIWidget_Button* pButtonFileBaseDecode = FKCW_UIWidget_Button::createWith9Sprite(CCSize(150, 50),
			s_szSprite9Btn1Path, s_szSprite9Btn2Path );
		pButtonFileBaseDecode->setPosition(SCREEN_CENTER_POS + ccp(0, 0));
		pButtonFileBaseDecode->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_Base_DecodeHtmlLayer::onClickDenodeHTML));
		pButtonFileBaseDecode->initText(FKCW_Base_StringConvert::a2u("解析处理HTML").c_str(), "", 30);
		m_pWindow->addChild(pButtonFileBaseDecode);

		CCScale9Sprite* pEditBoxBG1 = CCScale9Sprite::create(s_szSprite9Btn1Path);
		m_pSrcEdit = CCEditBox::create(CCSizeMake(800, 50), pEditBoxBG1 );
		m_pSrcEdit->setPosition( SCREEN_CENTER_POS + ccp(0, 100));
		m_pSrcEdit->setFontName("Atlas");
		m_pSrcEdit->setFontSize( 28 );
		string tmp = s_sTestHTMLStr;
		m_pSrcEdit->setText(tmp.c_str());
		m_pWindow->addChild( m_pSrcEdit );

		CCScale9Sprite* pEditBoxBG2 = CCScale9Sprite::create(s_szSprite9Btn1Path);
		m_pDestEdit = CCEditBox::create(CCSizeMake(800, 50), pEditBoxBG2 );
		m_pDestEdit->setPosition( SCREEN_CENTER_POS + ccp(0, -100) );
		m_pDestEdit->setFontColor( ccRED );
		m_pDestEdit->setFontName("Atlas");
		m_pDestEdit->setFontSize( 28 );
		m_pDestEdit->setEnabled( false );
		m_pDestEdit->setText(FKCW_Base_StringConvert::a2u("这里输出处理后的数据").c_str());
		m_pWindow->addChild( m_pDestEdit );

		return true;
	}
public:
	void onClickDenodeHTML(CCObject* pSender)
	{
		string strSrc = m_pSrcEdit->getText();
		char szDst[128];
		memset( szDst, 0, 128 );
		if( decode_html_entities_utf8( szDst, strSrc.c_str()) != 0 )
		{
			m_pDestEdit->setText( szDst );
		}
		else
		{
			m_pDestEdit->setText( FKCW_Base_StringConvert::a2u("非法HTML数据或解析失败").c_str() );
		}
	}
public:
	CREATE_FUNC( CTest_Base_DecodeHtmlLayer );
private:
	FKCW_UIWidget_WidgetWindow* m_pWindow;
	CCEditBox* m_pDestEdit;
	CCEditBox* m_pSrcEdit;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(Base, DecodeHtml, 处理Html格式字符串)
//-------------------------------------------------------------------------