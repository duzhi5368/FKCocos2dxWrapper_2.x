//*************************************************************************
//	创建日期:	2014-12-24
//	文件名称:	Test_EffectNode_LightingBolt.h
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
class CTest_EffectNode_LightingBoltLayer : public CCLayer
{
public:
	bool init()
	{
		m_lbNode=NULL;
		m_isDebugOn=false;

		CCSize winSize=CCDirector::sharedDirector()->getWinSize();
		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
		CCPoint start=CCPoint(0,winSize.height/2);
		CCPoint end=CCPoint(winSize.width,winSize.height/2);
    
		m_lbNode=new FKCW_EffectNode_LightingBoltNode();
		m_lbNode->autorelease();
		m_lbNode->init(start, end);
		addChild(m_lbNode);
		FKCW_Action_FlashAction* flash=FKCW_Action_FlashAction::create(-1.0f,1.0f);
		m_lbNode->runAction(flash);

		 // DEBUG面板
		{
			CCScale9Sprite* btnUp=CCScale9Sprite::create(s_szSprite9Btn1Path);
			CCScale9Sprite* btnDn=CCScale9Sprite::create(s_szSprite9Btn2Path);
			CCLabelTTF*title=CCLabelTTF::create("debug", "Helvetica", 26);
			CCControlButton* controlButton=CCControlButton::create(title, btnUp);
			controlButton->setBackgroundSpriteForState(btnDn,CCControlStateHighlighted);
			controlButton->setPreferredSize(CCSize(100,50));
			controlButton->setPosition(ccp(origin.x+controlButton->getContentSize().width/2,winSize.height-150));
			controlButton->addTargetWithActionForControlEvents(this, (SEL_CCControlHandler)(&CTest_EffectNode_LightingBoltLayer::controlButtonEvent_debug), CCControlEventTouchDown);
			this->addChild(controlButton);
			m_controlButton_debug=controlButton;

			m_label_debugState = CCLabelTTF::create(A2U("DEBUG模式 : 关闭").c_str(), "Arial", 26);
			m_label_debugState->setPosition(ccp(m_controlButton_debug->getPositionX()+m_controlButton_debug->getContentSize().width/2+m_label_debugState->getContentSize().width/2+10,
												m_controlButton_debug->getPositionY()));
			this->addChild(m_label_debugState, 1);
		}

		return true;
	}
public:
	void controlButtonEvent_debug(CCObject *senderz, CCControlEvent controlEvent)
	{
		m_isDebugOn=!m_isDebugOn;

		if(m_isDebugOn)
		{
			m_label_debugState->setString(A2U("DEBUG模式 : 开启").c_str()) ;
			m_lbNode->setIsDrawDebug(true);
		}
		else
		{
			m_label_debugState->setString(A2U("DEBUG模式 : 关闭").c_str()) ;
			m_lbNode->setIsDrawDebug(false);
		}
	}
public:
	CREATE_FUNC( CTest_EffectNode_LightingBoltLayer );
protected:
    CCControlButton*	m_controlButton_debug;
    CCLabelTTF*			m_label_debugState;
    bool				m_isDebugOn;
protected:
    FKCW_EffectNode_LightingBoltNode* m_lbNode;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(EffectNode, LightingBolt, 闪电精灵)
//-------------------------------------------------------------------------
