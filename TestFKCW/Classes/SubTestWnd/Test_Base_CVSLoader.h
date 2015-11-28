//*************************************************************************
//	创建日期:	2014-12-22
//	文件名称:	Test_Base_CVSLoader.h
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
class CTest_Base_CVSLoaderLayer : public CCLayer
{
public:
	virtual bool init()
	{
		//创建基本容器
		m_pWindow = FKCW_UIWidget_WidgetWindow::create();
		m_pWindow->setMultiTouchEnabled(true);
		this->addChild(m_pWindow);

		m_pLoader = new FKCW_Base_CSVLoader();
		if( m_pLoader == NULL )
			return false;

		CCScale9Sprite* pEditBoxBG = CCScale9Sprite::create(s_szSprite9Btn1Path);
		m_pFileEdit = CCEditBox::create(CCSizeMake(800, 50), pEditBoxBG );
		m_pFileEdit->setPosition( SCREEN_CENTER_POS + ccp(0, 200));
		m_pFileEdit->setFontSize( 48 );
		m_pFileEdit->setText(FKCW_Base_StringConvert::a2u( s_szCVSFile ).c_str());
		m_pWindow->addChild( m_pFileEdit );

		FKCW_UIWidget_Button* pButton = FKCW_UIWidget_Button::createWith9Sprite(CCSizeMake(150, 50), 
			s_szSprite9Btn1Path, s_szSprite9Btn2Path );
		pButton->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_Base_CVSLoaderLayer::onLoadCVS));
		pButton->setPosition(SCREEN_CENTER_POS+ccp(0, 100));
		pButton->setTextFontSize(30.0f);
		pButton->setText(FKCW_Base_StringConvert::a2u( "加载文件" ).c_str());
		m_pWindow->addChild(pButton);

		m_pListView = FKCW_UIWidget_ListView::create(CCSize(640, 350));
		m_pListView->setBackgroundImage(s_szBackground1Path);
		m_pListView->setPosition(SCREEN_CENTER_POS+ccp(0, -120));
		m_pListView->setDirection(eScrollViewDirectionVertical);
		m_pWindow->addChild(m_pListView);
		
		return true;
	}
	void onExit()
	{
		if( m_pLoader )
		{
			delete m_pLoader;
			m_pLoader = NULL;
		}
		CCLayer::onExit();
	}
public:
	void onLoadCVS(CCObject* pSender)
	{
		string strFilePath = m_pFileEdit->getText();
		m_pLoader->Clear();
		if( !m_pLoader->OpenAndResolveFile( strFilePath.c_str() ) )
		{
			FKLOG("需要解析的CVS文件不存在");
			return;
		}
		for( int i = m_pLoader->GetRowLength()-1; i >= 0; --i )
		{
			string str;
			for( int j = 0; j < m_pLoader->GetColLength(); ++j )
			{
				str += m_pLoader->GetData(i, j);
				str += "   ";
			}
			_AddToList( str.c_str() );
		}
	}
protected:
	void _AddToList(const char* pStr)
	{
		FKCW_UIWidget_Layout* pLayout = FKCW_UIWidget_Layout::create();
		pLayout->setContentSize(CCSize(640, 30));

		FKCW_UIWidget_ColorView* pColor = FKCW_UIWidget_ColorView::create(ccc4(0, 128, 0, 255));
		pColor->setContentSize(CCSize(636, 28));
		pColor->setPosition(CCPoint(640/2, 30/2));
		pLayout->addChild(pColor);

		FKCW_UIWidget_Label* pLabel = FKCW_UIWidget_Label::create(
			FKCW_Base_StringConvert::a2u(pStr).c_str(), "", 30.0f);
		pLabel->setPosition(CCPoint(640 /2, 30 /2));
		pLayout->addChild(pLabel);
		m_pListView->insertNodeAtFront(pLayout);
		m_pListView->reloadData();
	}
public:
	CREATE_FUNC( CTest_Base_CVSLoaderLayer );
public:
	FKCW_Base_CSVLoader*		m_pLoader;
	FKCW_UIWidget_WidgetWindow* m_pWindow;
	CCEditBox*					m_pFileEdit;
	FKCW_UIWidget_ListView*		m_pListView;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(Base, CVSLoader, CVS格式加载器)