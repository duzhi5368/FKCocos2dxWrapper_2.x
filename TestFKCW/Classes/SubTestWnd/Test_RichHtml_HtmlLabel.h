//*************************************************************************
//	创建日期:	2015-1-3
//	文件名称:	Test_RichHtml_HtmlLabel.h
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
class CTest_RichHtml_HtmlLabelLayer : public CCLayer
{
public:
	virtual bool init()
	{
		FKCW_Font_Catalog* pFont = NULL;
		pFont = FKCW_Font_Factory::GetInstance()->CreateFont(
			"Stliti", "STLITI.ttf", 0xffffffff, 32, e_plain, 0.0f, 0xffffffff, 0);
		pFont = FKCW_Font_Factory::GetInstance()->CreateFont(
			"MarkerFelt", s_szTestFontFile, 0xffffffff, 24, e_shadow, 1.0f, 0xff000000, 0);
		pFont->AddHackFont( s_szTestFontFile, latin_charset(), -1);

		{
			CCSize vsize = CCDirector::sharedDirector()->getVisibleSize();
			CCString* str_utf8 = CCString::createWithContentsOfFile( s_szHTMLFile );

			FKCW_RichHtml_HTMLLabel* htmllabel = FKCW_RichHtml_HTMLLabel::createWithString(str_utf8->getCString(), 
				CCSize(vsize.width*0.8f, vsize.height), "default" );
			htmllabel->setAnchorPoint(ccp(0.5f,0.5f));
			htmllabel->setPosition(ccp(vsize.width*0.5f, vsize.height*0.5f));
			addChild(htmllabel);
			m_pHtmlLabel = htmllabel;

			htmllabel->registerListener(this, &CTest_RichHtml_HtmlLabelLayer::onHTMLClicked, &CTest_RichHtml_HtmlLabelLayer::onHTMLMoved );
		}
		{

			m_pMenu = CCMenu::create();
			m_pMenu->setPosition(CCPointZero);
			addChild(m_pMenu);

			CCMenuItemFont* pFont = CCMenuItemFont::create( A2U("点我重新加载HTML").c_str() );
			pFont->setTarget(this, menu_selector(CTest_RichHtml_HtmlLabelLayer::onClick));
			pFont->setFontSizeObj(26);
			pFont->setPosition(CCPoint(800, 40));
			pFont->setTag(1);
			m_pMenu->addChild(pFont);

			m_pMenu->setVisible(false);

			m_pInfoLabel = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("信息提示").c_str(), "Arial", 26 );
			m_pInfoLabel->setColor(ccc3(230,155,3));
			m_pInfoLabel->setPosition( SCREEN_CENTER_POS+ccp(0, -250) );
			addChild(m_pInfoLabel);
		}

		FKCW_Font_Factory::GetInstance()->DumpTextures();
		return true;
	}
public:

	void onClick(CCObject* sender)
	{
		if ( !m_pHtmlLabel )
		{
			return;
		}
		m_pHtmlLabel->setVisible(true);
		m_pMenu->setVisible(false);
	}

	void onHTMLClicked(
		FKCW_RichHtml_IRichNode* root, FKCW_RichHtml_IRichElement* ele, int _id)
	{
		char szInfo[32];
		memset( szInfo, 0, 32 );
		//sprintf_s( szInfo,"点击了id = %d", _id );
		sprintf(szInfo,"点击了id = %d", _id );

		m_pInfoLabel->setString( A2U( szInfo ).c_str() );

		if ( !m_pHtmlLabel )
		{
			return;
		}
		else if ( _id == 1002 ) // 关闭
		{
			m_pHtmlLabel->setVisible(false);
			m_pMenu->setVisible(true);
		}
		else if ( _id == 2000 ) // 重新加载
		{
			CCString* str_utf8 = CCString::createWithContentsOfFile( s_szHTMLFile );
			m_pHtmlLabel->setString(str_utf8->getCString());	
		}
	}
	void onHTMLMoved(
		FKCW_RichHtml_IRichNode* root, FKCW_RichHtml_IRichElement* ele, int _id,
		const CCPoint& location, const CCPoint& delta)
	{
		char szInfo[32];
		memset( szInfo, 0, 32 );
		sprintf( szInfo,"移动了id = %d", _id );
		m_pInfoLabel->setString( A2U( szInfo ).c_str() );

		if ( !m_pHtmlLabel )
		{
			return;
		}
		else if ( _id == 1001 )
		{
			m_pHtmlLabel->setPosition(ccpAdd(delta, m_pHtmlLabel->getPosition()));
		}
	}
public:
	CREATE_FUNC( CTest_RichHtml_HtmlLabelLayer );
private:
	FKCW_RichHtml_HTMLLabel*		m_pHtmlLabel;
	CCLabelTTF*						m_pInfoLabel;
	CCMenu*							m_pMenu;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(RichHtml, HtmlLabel, 解析HTML并渲染该富文本)
//-------------------------------------------------------------------------