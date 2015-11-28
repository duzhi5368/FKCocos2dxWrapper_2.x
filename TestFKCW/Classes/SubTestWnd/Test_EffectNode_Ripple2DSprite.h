//*************************************************************************
//	创建日期:	2014-12-24
//	文件名称:	Test_EffectNode_Ripple2DSprite.h
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
class CTest_EffectNode_Ripple2DSpriteLayer : public CCLayer
{
public:
	bool init()
	{
		m_ripple_horizontalNode=NULL;
		m_isDebugOn=false;

		this->setTouchEnabled(true);

		CCSize winSize=CCDirector::sharedDirector()->getWinSize();
		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

		m_ripple_horizontalNode=new FKCW_EffectNode_Ripple2DSprite();
		m_ripple_horizontalNode->autorelease();
		m_ripple_horizontalNode->init( s_szWaterPath );
		m_ripple_horizontalNode->setPosition(SCREEN_CENTER_POS+ccp(0, -200));
		m_ripple_horizontalNode->scheduleUpdate();
		addChild(m_ripple_horizontalNode);

		// DEBUG面板
		{
			CCScale9Sprite* btnUp=CCScale9Sprite::create(s_szSprite9Btn1Path);
			CCScale9Sprite* btnDn=CCScale9Sprite::create(s_szSprite9Btn2Path);
			CCLabelTTF*title=CCLabelTTF::create("debug", "Helvetica", 26);
			CCControlButton* controlButton=CCControlButton::create(title, btnUp);
			controlButton->setBackgroundSpriteForState(btnDn,CCControlStateHighlighted);
			controlButton->setPreferredSize(CCSize(100,50));
			controlButton->setPosition(ccp(origin.x+controlButton->getContentSize().width/2,winSize.height-150));
			controlButton->addTargetWithActionForControlEvents(this, (SEL_CCControlHandler)(&CTest_EffectNode_Ripple2DSpriteLayer::controlButtonEvent_debug), CCControlEventTouchDown);
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
	virtual void ccTouchesBegan(CCSet* touches , CCEvent* event)
	{
		CCSetIterator it;
		CCTouch* touch;
		for( it = touches->begin(); it != touches->end(); it++)
		{
			touch = (CCTouch*)(*it);
			if(!touch)
				break;

			CCPoint location = touch->getLocationInView();
			location = CCDirector::sharedDirector()->convertToGL(location);
			m_ripple_horizontalNode->pressAtX(location.x, 15,180);
			break;
		}
	}
	virtual void ccTouchesMoved(CCSet* touches , CCEvent* event)
	{
		CCSetIterator it;
		CCTouch* touch;
		for( it = touches->begin(); it != touches->end(); it++)
		{
			touch = (CCTouch*)(*it);
			if(!touch)
				break;

			CCPoint location = touch->getLocationInView();
			location = CCDirector::sharedDirector()->convertToGL(location);
			m_ripple_horizontalNode->pressAtX(location.x, 0.8f);
			break;
		}
	}
	virtual void ccTouchesEnded(CCSet* touches , CCEvent* event)
	{
		CCSetIterator it;
		CCTouch* touch;
		for( it = touches->begin(); it != touches->end(); it++)
		{
			touch = (CCTouch*)(*it);
			if(!touch)
				break;

			CCPoint location = touch->getLocationInView();
			location = CCDirector::sharedDirector()->convertToGL(location);
			break;
		}
	}
	void controlButtonEvent_debug(CCObject *senderz, CCControlEvent controlEvent)
	{
		m_isDebugOn=!m_isDebugOn;

		if(m_isDebugOn)
		{
			m_label_debugState->setString(A2U("DEBUG模式 : 开启").c_str()) ;
			m_ripple_horizontalNode->setIsDrawDebug(true);
		}
		else
		{
			m_label_debugState->setString(A2U("DEBUG模式 : 关闭").c_str()) ;
			m_ripple_horizontalNode->setIsDrawDebug(false);
		}
	}
protected:
	CCControlButton*	m_controlButton_debug;
	CCLabelTTF*			m_label_debugState;
	bool				m_isDebugOn;
protected:
	FKCW_EffectNode_Ripple2DSprite*	m_ripple_horizontalNode;
public:
	CREATE_FUNC( CTest_EffectNode_Ripple2DSpriteLayer );
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(EffectNode, Ripple2DSprite, 2D波纹精灵)
//-------------------------------------------------------------------------