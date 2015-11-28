//*************************************************************************
//	创建日期:	2014-12-12
//	文件名称:	Test_Action_TreeFadeIn.h
//  创 建 人:   WilhanTian	
//	版权所有:	MIT
//	说    明:	渐隐节点树，渐隐该节点的所有子节点
//  测试结果: 
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
class CTest_Action_TreeFadeInLayer : public CCLayer
{
public:
	virtual bool init()
	{
		m_pRootSprite = CCSprite::create(s_szLetterAImagePath);
		m_pRootSprite->setOpacity( 0 );
		CCSprite* pSpriteB = CCSprite::create(s_szLetterBImagePath);
		pSpriteB->setOpacity( 0 );
		CCSprite* pSpriteC = CCSprite::create(s_szLetterCImagePath);
		pSpriteC->setOpacity( 0 );
		
		m_pRootSprite->setPosition(SCREEN_CENTER_POS);
		
		this->addChild(m_pRootSprite);
		m_pRootSprite->addChild(pSpriteB);
		pSpriteB->addChild(pSpriteC);

		CCLabelTTF* lable = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("点我对精灵A执行渐显动作").c_str(), "Arial", 26 );
		lable->setColor(ccc3(230,155,3));

		CCMenuItemLabel* itemLable = CCMenuItemLabel::create(lable, this, menu_selector(CTest_Action_TreeFadeInLayer::menuCallFunc));
		CCMenu* menu = CCMenu::create(itemLable, NULL);
		menu->setPosition(SCREEN_CENTER_POS + CCPoint(0, -200));
		this->addChild(menu);

		return true;
	}
public:
	void menuCallFunc(CCObject* pSender)
	{
		m_pRootSprite->runAction(FKCW_Action_TreeFadeIn::Create(3.0f));
	}
public:
	CREATE_FUNC( CTest_Action_TreeFadeInLayer );
private:
	CCSprite* m_pRootSprite;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(Action, TreeFadeIn, 渐显节点树，渐显该节点的所有子节点（A是根节点、B的父节点是A、C的父节点是B）)
//-------------------------------------------------------------------------