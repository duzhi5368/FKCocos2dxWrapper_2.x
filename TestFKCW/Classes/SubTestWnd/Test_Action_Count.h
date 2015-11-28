//*************************************************************************
//	创建日期:	2014-12-12
//	文件名称:	Test_Action_Count.h
//  创 建 人:   WilhanTian	
//	版权所有:	MIT
//	说    明:	数字翻转测试
//  测试结果: 
//*************************************************************************
#pragma once
//-------------------------------------------------------------------------
#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
class CTest_Action_CountLayer : public CCLayer
{
public:
	virtual bool init()
	{
		m_pCountLabel1 = CCLabelTTF::create("Action_Count Test", "Arial", 26 );
		m_pCountLabel1->setPosition(SCREEN_CENTER_POS);
		this->addChild(m_pCountLabel1);

		m_pCountLabel2 = CCLabelTTF::create("Action_Count Test", "Arial", 26 );
		m_pCountLabel2->setPosition(SCREEN_CENTER_POS + ccp(0,70));
		this->addChild(m_pCountLabel2);

		CCLabelTTF* label2 = CCLabelTTF::create("0", "Helvetica", 32);
		label2->setPosition(SCREEN_CENTER_POS + ccp(0,150));
		addChild(label2);
		label2->runAction(FKCW_Action_Count::Create(30, 1000, 0, "%d"));

		CCLabelTTF* lable = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("点我对数字执行翻转动作").c_str(), "Arial", 26 );
		lable->setColor(ccc3(230,155,3));

		CCMenuItemLabel* itemLable = CCMenuItemLabel::create(lable, this, menu_selector(CTest_Action_CountLayer::menuCallFunc));
		CCMenu* menu = CCMenu::create(itemLable, NULL);
		menu->setPosition(SCREEN_CENTER_POS + CCPoint(0, -200));
		this->addChild(menu);

		return true;
	}
public:
	void menuCallFunc(CCObject* pSender)
	{
		int nEnd = RANDOM_X_Y_INT(0,50000);
		m_pCountLabel1->runAction(FKCW_Action_Count::Create(1.5f, 0, nEnd, "HP = %d"));
		int nStart = atoi( m_pCountLabel2->getString() );
		m_pCountLabel2->runAction(FKCW_Action_Count::Create(1.5f, nStart, nEnd, "%d"));
	}
public:
	CREATE_FUNC( CTest_Action_CountLayer );
private:
	CCLabelTTF* m_pCountLabel1;
	CCLabelTTF* m_pCountLabel2;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(Action, Count, 数字翻转效果|类似球场上的记分牌)
//-------------------------------------------------------------------------