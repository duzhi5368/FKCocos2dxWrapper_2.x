//*************************************************************************
//	创建日期:	2014-12-12
//	文件名称:	Test_Action_Flash.h
//  创 建 人:   WilhanTian	
//	版权所有:	MIT
//	说    明:	节点闪光效果
//  测试结果: 
//*************************************************************************
#pragma once
//-------------------------------------------------------------------------
#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
class CTest_Action_FlashLayer : public CCLayer
{
public:
	virtual bool init()
	{
		m_nCurIndex = 0;

		m_pSprite = CCSprite::create(s_szHalfPhotoPath);
		m_pSprite->setPosition(SCREEN_CENTER_POS + ccp(-200,0));
		this->addChild(m_pSprite);

		CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(
			s_szArmatureCowboyPic, s_szArmatureCowboyPlist, s_szArmatureCowboy);
		CCArmature* armature = CCArmature::create("Cowboy");
		armature->getAnimation()->playWithIndex(0);
		armature->setPosition(SCREEN_CENTER_POS + ccp(200,0));
		armature->setScale(0.2f);
		addChild(armature);

		armature->runAction(CCRepeatForever::create(CCSequence::create(
			FKCW_Action_Flash::Create(0.2f, ccWHITE),
			NULL)));


		CCLabelTTF* lable = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("点我对精灵执行闪光动作").c_str(), "Arial", 26 );
		lable->setColor(ccc3(230,155,3));

		CCMenuItemLabel* itemLable = CCMenuItemLabel::create(lable, this, menu_selector(CTest_Action_FlashLayer::menuCallFunc));
		CCMenu* menu = CCMenu::create(itemLable, NULL);
		menu->setPosition(SCREEN_CENTER_POS + CCPoint(0, -200));
		this->addChild(menu);

		return true;
	}
public:
	void menuCallFunc(CCObject* pSender)
	{
		CCLOG("-----------------------------");
		switch ( m_nCurIndex % 3 )
		{
		case 0:
			m_pSprite->runAction(FKCW_Action_Flash::Create( 0.5f, ccWHITE));
			break;
		case 1:
			m_pSprite->runAction(FKCW_Action_Flash::Create( 0.2f, ccGREEN));
			break;
		case 2:
			m_pSprite->runAction(FKCW_Action_Flash::Create( 0.5f, ccRED));
			break;
		default:
			break;
		}
		m_nCurIndex++;
		CCLOG("-----------------------------");
	}
public:
	CREATE_FUNC( CTest_Action_FlashLayer );
private:
	CCSprite*	m_pSprite;
	int			m_nCurIndex;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(Action, Flash, 节点闪光动作，例如当游戏里的人物收到伤害时会有该效果出现（忽略不透明部分）)
//-------------------------------------------------------------------------