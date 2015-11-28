//*************************************************************************
//	创建日期:	2014-12-12
//	文件名称:	Test_Action_Missile.h
//  创 建 人:   WilhanTian	
//	版权所有:	MIT
//	说    明:	导弹追踪动作
//  测试结果: 
//*************************************************************************
#pragma once
//-------------------------------------------------------------------------
#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
class CTest_Action_MissileLayer : public CCLayer
{
public:
	virtual bool init()
	{
		//创建一个被追踪的Sprite
		m_pSprite = CCSprite::create(s_szCocosLogoPath);
		m_pSprite->setPosition(SCREEN_CENTER_POS + CCPoint(0, 200));
		this->addChild(m_pSprite);

		m_pInfoLable = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("").c_str(), "Arial", 26 );
		m_pInfoLable->setColor(ccc3(230,155,3));
		m_pInfoLable->setPosition(SCREEN_CENTER_POS + CCPoint(0, -250));
		this->addChild(m_pInfoLable);

		CCLabelTTF* lable = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("点击我生成一个导弹").c_str(), "Arial", 26 );
		lable->setColor(ccc3(230,155,3));

		CCMenuItemLabel* itemLable = CCMenuItemLabel::create(lable, this, menu_selector(CTest_Action_MissileLayer::menuCallFunc));
		CCMenu* menu = CCMenu::create(itemLable, NULL);
		menu->setPosition(SCREEN_CENTER_POS + CCPoint(0, -100));
		this->addChild(menu);

		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);

		return true;
	}
	void onExit()
	{
		CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate( this );
		CCLayer::onExit();
	}
public:
	void menuCallFunc(CCObject* pSender)
	{
		//创建一个导弹Sprite
		CCSprite* missile = CCSprite::create(s_szMissilePath);
		missile->setPosition(SCREEN_CENTER_POS + CCPoint(0, -100));
		this->addChild(missile);

		m_pInfoLable->setString("");

		missile->runAction(FKCW_Action_Missile::Create(400.0f, m_pSprite, 0.0f,
			CCCallFunc::create(this, callfunc_selector(CTest_Action_MissileLayer::onHit))));
	}

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) 
	{
		m_pSprite->setPosition(pTouch->getLocation());
		return true;
	}

	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) 
	{
		m_pSprite->setPosition(pTouch->getLocation());
	}

	void onHit()
	{
		m_pInfoLable->setString(FKCW_Base_StringConvert::a2u("子弹碰撞目标").c_str());
	}
public:
	CREATE_FUNC( CTest_Action_MissileLayer );
private:
	CCSprite*		m_pSprite;
	CCLabelTTF*		m_pInfoLable;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(Action, Missile, 导弹动作|弹幕游戏中的追踪导弹效果（点击屏幕更改cocos的Logo位置）)
//-------------------------------------------------------------------------