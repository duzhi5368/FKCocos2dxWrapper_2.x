//*************************************************************************
//	创建日期:	2014-12-30
//	文件名称:	Test_Font_Test.h
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
class CTest_Font_TestLayer : public CCLayer
{
public:
	virtual bool init()
	{
		
		const static int nMaxCaralogType = 3;
		FKCW_Font_Catalog* pFontCatalog[nMaxCaralogType];

		pFontCatalog[0] = FKCW_Font_Factory::GetInstance()->CreateFont(
			"Stliti", "STLITI.ttf", 0xffffffff, 32, e_plain, 0.0f, 0xffffffff, 0);
		this->__CreateFontLabel(1,"STLITI.ttf",ccc3(255,0,0),SCREEN_CENTER_POS + ccp(0,200));

		pFontCatalog[1] = FKCW_Font_Factory::GetInstance()->CreateFont(
			"MarkerFelt", s_szTestFontFile, 0xffffffff, 24, e_shadow, 1.0f, 0xff000000, 0);
		pFontCatalog[1]->AddHackFont(s_szTestFontFile, latin_charset(), -1);
		this->__CreateFontLabel(2,s_szTestFontFile,ccc3(255,0,0),SCREEN_CENTER_POS + ccp(0,150));

		pFontCatalog[2] = FKCW_Font_Factory::GetInstance()->CreateFont(
			"Msyh", "msyh.ttf", 0xffffffff, 20, e_border, 1.0f, 0xff000000, 0);
		pFontCatalog[2]->AddHackFont("msyh.ttf", latin_charset(), 5);
		this->__CreateFontLabel(3,"msyh.ttf",ccc3(255,0,0),SCREEN_CENTER_POS + ccp(0,100));

		
		

		for( int j = 0; j < nMaxCaralogType; ++j )
		{
			for( unsigned int i = 0; i < 127; ++i )
			{
				pFontCatalog[j]->require_char( static_cast<char>( i ) );
			}
			pFontCatalog[j]->flush();
		}

		m_pLable = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("点我输出当前加载的三种字库").c_str(), "Arial", 26 );
		m_pLable->setColor(ccc3(230,155,3));

		CCMenuItemLabel* itemLable = CCMenuItemLabel::create(m_pLable, this, menu_selector(CTest_Font_TestLayer::menuCallFunc));
		m_pMenu = CCMenu::create(itemLable, NULL);
		m_pMenu->setPosition(SCREEN_CENTER_POS);
		this->addChild(m_pMenu);

		return true;
	}

	virtual void onExit()
	{
		CCLayer::onExit();
	}
public:
	void menuCallFunc(CCObject* pSender)
	{
		FKCW_Font_Factory::GetInstance()->DumpTextures();
		m_pLable->setString( A2U("输出完成。请查看应用程序所在目录...").c_str() );
		m_pMenu->setEnabled( false );
	}
public:
	CREATE_FUNC( CTest_Font_TestLayer );
public:
	CCLabelTTF* m_pLable;
	CCMenu* m_pMenu;

private:
	void __CreateFontLabel(int nIndex,std::string strFontName,ccColor3B ccc3,CCPoint  pos)
	{
		char temp[120];
		std::string fullpath = CCFileUtils::sharedFileUtils()->fullPathForFilename(strFontName.c_str());
		sprintf(temp, "%d.%s",nIndex,fullpath.c_str());
		CCLabelTTF * fontLabel = CCLabelTTF::create(FKCW_Base_StringConvert::a2u(temp).c_str(), "Arial", 26 );
		fontLabel->setColor(ccc3);
		fontLabel->setPosition(pos);
		addChild(fontLabel);
	}
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(Font, Test, 测试FreeType2字体支持)
//-------------------------------------------------------------------------