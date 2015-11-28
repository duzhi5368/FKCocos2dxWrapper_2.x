//*************************************************************************
//	创建日期:	2014-12-19
//	文件名称:	Test_RenderEx_JoystickNode.h
//  创 建 人:   WilhanTian	
//	版权所有:	MIT
//	说    明:	
//  备    注:
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
class CTest_RenderEx_NumberCountNodeLayer : public CCLayer
{
public:
	bool init()
	{
		m_pAtlasLabel = CCLabelAtlas::create("", s_szMathLabelAtlasPath, 15, 26, '0');
		m_pAtlasLabel->setPosition(SCREEN_CENTER_POS);
		m_pAtlasLabel->setScale(2.0f);
		this->addChild(m_pAtlasLabel);

		m_pCountNode = new FKCW_RenderEx_NumberCountNode();
		m_pCountNode->setLabelAtlas(m_pAtlasLabel);
		m_pCountNode->setNumber(10, true);

		CCLabelTTF* pLabel = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("点我随机更改数字").c_str(), "Arial", 36 );
		pLabel->setColor( ccc3(230,155,3) );
		CCMenuItemLabel* pMenuItem = CCMenuItemLabel::create(pLabel, this, menu_selector(CTest_RenderEx_NumberCountNodeLayer::menuCallback));
		CCMenu* pMenu = CCMenu::create(pMenuItem, NULL);
		pMenu->setPosition( SCREEN_CENTER_POS + ccp( 0, -50 ));
		addChild(pMenu, 1);

		return true;
	}
	void onExit()
	{
		CC_SAFE_DELETE( m_pCountNode );
		CCLayer::onExit();
	}
public:
	void menuCallback(CCObject* pSender)
	{
		m_pCountNode->setLabelAtlas(m_pAtlasLabel);
		m_pCountNode->setNumber(static_cast<int>(FKCW_Base_Utils::RandomInRange(0, 200)), true);
	}
public:
	CREATE_FUNC( CTest_RenderEx_NumberCountNodeLayer );
protected:
	CCLabelAtlas*					m_pAtlasLabel;
	FKCW_RenderEx_NumberCountNode*	m_pCountNode;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(RenderEx, NumberCountNode, 变速跳转的数字翻动精灵)
//-------------------------------------------------------------------------