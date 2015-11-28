//*************************************************************************
//	创建日期:	2014-12-12
//	文件名称:	Test_Action_ClipIn.h
//  创 建 人:   WilhanTian	
//	版权所有:	MIT
//	说    明:	
//  测试结果: 没大会用？
//*************************************************************************
#pragma once
//-------------------------------------------------------------------------
#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
class CTest_Action_ClipInLayer : public CCLayer
{
public:
	virtual bool init()
	{
		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

		m_pClippingNode = CCClippingNode::create();
		CCSprite* pSprite = CCSprite::create(s_szHalfPhotoPath);
		CCDrawNode* pStencil = CCDrawNode::create();
		m_pClippingNode->setStencil(pStencil);
		pSprite->setAnchorPoint(CCPointZero);
		pSprite->setPosition(CCPointZero);
		m_pClippingNode->addChild(pSprite);
		m_pClippingNode->ignoreAnchorPointForPosition(false);
		m_pClippingNode->setAnchorPoint(ccp(0.5f, 0.5f));
		m_pClippingNode->setContentSize(pSprite->getContentSize());
		m_pClippingNode->setPosition( SCREEN_CENTER_POS );
		addChild(m_pClippingNode, 1);

		CCLabelTTF* lable = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("点我对精灵执行渐入渐出动作").c_str(), "Arial", 26 );
		lable->setColor(ccc3(230,155,3));

		CCMenuItemLabel* itemLable = CCMenuItemLabel::create(lable, this, menu_selector(CTest_Action_ClipInLayer::menuCallFunc));
		CCMenu* menu = CCMenu::create(itemLable, NULL);
		menu->setPosition(SCREEN_CENTER_POS + CCPoint(0, -200));
		this->addChild(menu);

		return true;
	}
public:
	void menuCallFunc(CCObject* pSender)
	{
		FKCW_Action_ClipIn* in1 = FKCW_Action_ClipIn::Create(2.0f);
		FKCW_Action_ClipOut* out1 = (FKCW_Action_ClipOut*)in1->reverse();
		FKCW_Action_ClipIn* in2 = FKCW_Action_ClipIn::Create(2, ccp(1, 1));
		FKCW_Action_ClipOut* out2 = (FKCW_Action_ClipOut*)in2->reverse();
		FKCW_Action_ClipIn* in3 = FKCW_Action_ClipIn::Create(2, ccp(-1, 1));
		FKCW_Action_ClipOut* out3 = (FKCW_Action_ClipOut*)in3->reverse();
		FKCW_Action_ClipIn* in4 = FKCW_Action_ClipIn::Create(2, ccp(-1, -1));
		FKCW_Action_ClipOut* out4 = (FKCW_Action_ClipOut*)in4->reverse();
		FKCW_Action_ClipIn* in5 = FKCW_Action_ClipIn::Create(2, ccp(1, -1));
		FKCW_Action_ClipOut* out5 = (FKCW_Action_ClipOut*)in5->reverse();
		FKCW_Action_ClipIn* in6 = FKCW_Action_ClipIn::Create(2, ccp(0, -1));
		FKCW_Action_ClipOut* out6 = (FKCW_Action_ClipOut*)in6->reverse();
		FKCW_Action_ClipIn* in7 = FKCW_Action_ClipIn::Create(2, ccp(0, 1));
		FKCW_Action_ClipOut* out7 = (FKCW_Action_ClipOut*)in7->reverse();
		FKCW_Action_ClipIn* in8 = FKCW_Action_ClipIn::Create(2, ccp(-1, 0));
		FKCW_Action_ClipOut* out8 = (FKCW_Action_ClipOut*)in8->reverse();
		m_pClippingNode->runAction(CCSequence::create(in1,
			out1, in2, out2, in3, out3, in4, out4, in5,
				  out5, in6, out6, in7, out7, in8, out8, NULL));
	}
public:
	CREATE_FUNC( CTest_Action_ClipInLayer );
private:
	CCClippingNode* m_pClippingNode;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(Action, ClipIn, 渐入渐出动作)
	//-------------------------------------------------------------------------