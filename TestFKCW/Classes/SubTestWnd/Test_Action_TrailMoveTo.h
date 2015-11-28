//*************************************************************************
//	创建日期:	2014-12-14
//	文件名称:	Test_Action_TrailMoveTo.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	拖尾移动
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
class CTest_Action_TrailMoveToLayer : public CCLayer
{
public:
	virtual bool init()
	{
		m_pStaticSprite = CCSprite::create( s_szCocosLogoPath );
		m_pStaticSprite->setPosition(ccp(910, 590));
		addChild(m_pStaticSprite);

		CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(
			s_szArmatureCowboyPic, s_szArmatureCowboyPlist, s_szArmatureCowboy);
		m_pArmature = CCArmature::create("Cowboy");
		m_pArmature->getAnimation()->playWithIndex(0);
		m_pArmature->setPosition(SCREEN_CENTER_POS+ccp(-400, 0));
		m_pArmature->setScale(-0.2f, 0.2f);
		addChild(m_pArmature);

		CCLabelTTF* lable = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("点我执行拖影移动动作").c_str(), "Arial", 26 );
		lable->setColor(ccc3(230,155,3));

		CCMenuItemLabel* itemLable = CCMenuItemLabel::create(lable, this, menu_selector(CTest_Action_TrailMoveToLayer::menuCallFunc));
		CCMenu* menu = CCMenu::create(itemLable, NULL);
		menu->setPosition(SCREEN_CENTER_POS + CCPoint(0, -200));
		this->addChild(menu);

		return true;
	}
public:
	void menuCallFunc(CCObject* pSender)
	{
		CCPoint dst = ccp( RANDOM_X_Y_INT(50, 910), RANDOM_X_Y_INT( 50, 490 ));
		CCPoint cur = m_pStaticSprite->getPosition();
		float fTime = cur.getDistance( dst ) / 500.0f;
		FKCW_Action_TrailMoveTo* move = FKCW_Action_TrailMoveTo::CreateWithFileName(fTime, dst, s_szCocosLogoPath,
			16 / CC_CONTENT_SCALE_FACTOR(), 10, ccc3(50, 50, 50), ccc4(100, 100, 100, 255));
		m_pStaticSprite->runAction(move);

		CCPoint dst2 = SCREEN_CENTER_POS+ccp(+400, 0);
		FKCW_Action_TrailMoveTo* m = FKCW_Action_TrailMoveTo::CreateWithArmature(3, dst2, "Cowboy",
			0, 16 / CC_CONTENT_SCALE_FACTOR(), 7, ccc3(10, 100, 40), ccc4(50, 50, 50, 255));
		m_pArmature->runAction(m);
	}
public:
	CREATE_FUNC( CTest_Action_TrailMoveToLayer );
public:
	CCSprite*			m_pStaticSprite;
	CCArmature*			m_pArmature;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(Action, TrailMoveTo, 拖尾移动动作)
//-------------------------------------------------------------------------