//*************************************************************************
//	创建日期:	2014-12-24
//	文件名称:	Test_EffectNode_BreakSprite.h
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
class CTest_EffectNode_BreakSpriteLayer : public CCLayer
{
public:
	virtual bool init()
	{
		m_breakSprite	=NULL;
		m_isDebugOn		=false;

		this->setTouchEnabled(true);

		CCSize winSize=CCDirector::sharedDirector()->getWinSize();
		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

		m_breakSprite=new FKCW_EffectNode_BreakSprite();
		m_breakSprite->autorelease();
		m_breakSprite->init( s_szGuaGuaLePath );
		m_breakSprite->setAnchorPoint(ccp(0.5,0.5));
		m_breakSprite->setPosition(SCREEN_CENTER_POS);

		this->addChild(m_breakSprite);

		// DEBUG按钮
		{
			CCScale9Sprite* btnUp=CCScale9Sprite::create(s_szSprite9Btn1Path);
			CCScale9Sprite* btnDn=CCScale9Sprite::create(s_szSprite9Btn2Path);
			CCLabelTTF*title=CCLabelTTF::create("debug", "Helvetica", 26);
			CCControlButton* controlButton=CCControlButton::create(title, btnUp);
			controlButton->setBackgroundSpriteForState(btnDn,CCControlStateHighlighted);
			controlButton->setPreferredSize(CCSize(100,50));
			controlButton->setPosition(ccp(origin.x+controlButton->getContentSize().width/2,winSize.height-150));
			controlButton->addTargetWithActionForControlEvents(this,
				(SEL_CCControlHandler)(&CTest_EffectNode_BreakSpriteLayer::controlButtonEvent_debug), CCControlEventTouchDown);
			this->addChild(controlButton);
			m_controlButton_debug=controlButton;

			m_label_debugState = CCLabelTTF::create(A2U("DEBUG模式 : 关闭").c_str(), "Arial", 26);
			m_label_debugState->setPosition(ccp(m_controlButton_debug->getPositionX()+m_controlButton_debug->getContentSize().width/2+m_label_debugState->getContentSize().width/2+10,
				m_controlButton_debug->getPositionY()));
			this->addChild(m_label_debugState, 1);
		}
		return true;
	}
	void onExit()
	{
		CCLayer::onExit();
	}
protected:
	virtual void ccTouchesBegan(cocos2d::CCSet* touches , cocos2d::CCEvent* event)
	{
		CCSetIterator it;
		CCTouch* touch;
		for( it = touches->begin(); it != touches->end(); it++)
		{
			touch = (CCTouch*)(*it);
			if(!touch)
				break;
			CCPoint loc_winSpace = touch->getLocationInView();
			CCPoint loc_GLSpace = CCDirector::sharedDirector()->convertToGL(loc_winSpace);
			switch (m_breakSprite->getState()) 
			{
			case eBreakSpriteState_Well:
				m_breakSprite->doCrack(loc_GLSpace);
				break;
			case eBreakSpriteState_Crack:
				{
					m_breakSprite->generateDelayTimes(15);
					FKCW_Action_FallOffAction* breakAction = FKCW_Action_FallOffAction::create(30);
					m_breakSprite->runAction(breakAction);
				}
				break;
			case eBreakSpriteState_FallOff:
				m_breakSprite->reSet();
				break;
			default:
				break;
			}
		}
	}
	virtual void ccTouchesMoved(cocos2d::CCSet* touches , cocos2d::CCEvent* event)
	{
		CCSetIterator it;
		CCTouch* touch;
		for( it = touches->begin(); it != touches->end(); it++)
		{
			touch = (CCTouch*)(*it);
			if(!touch)
				break;
			CCPoint loc_winSpace = touch->getLocationInView();
			CCPoint loc_GLSpace = CCDirector::sharedDirector()->convertToGL(loc_winSpace);
		}
	}
	virtual void ccTouchesEnded(cocos2d::CCSet* touches , cocos2d::CCEvent* event)
	{
		CCSetIterator it;
		CCTouch* touch;
		for( it = touches->begin(); it != touches->end(); it++)
		{
			touch = (CCTouch*)(*it);
			if(!touch)
				break;
			CCPoint loc_winSpace = touch->getLocationInView();
			CCPoint loc_GLSpace = CCDirector::sharedDirector()->convertToGL(loc_winSpace);
		}
	}

	void controlButtonEvent_debug(CCObject *senderz, CCControlEvent controlEvent)
	{
		m_isDebugOn=!m_isDebugOn;

		if(m_isDebugOn)
		{
			m_label_debugState->setString(A2U("DEBUG模式 : 开启").c_str()) ;
			m_breakSprite->setIsDrawDebug(true);
		}
		else
		{
			m_label_debugState->setString(A2U("DEBUG模式 : 关闭").c_str()) ;
			m_breakSprite->setIsDrawDebug(false);
		}
	}
protected:
	CCControlButton*	m_controlButton_debug;
	CCLabelTTF*			m_label_debugState;
	bool				m_isDebugOn;
protected:
	FKCW_EffectNode_BreakSprite* m_breakSprite;
public:
	CREATE_FUNC( CTest_EffectNode_BreakSpriteLayer );
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(EffectNode, BreakSprite, 破碎掉落效果)
//-------------------------------------------------------------------------