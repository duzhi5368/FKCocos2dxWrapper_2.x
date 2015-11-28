//*************************************************************************
//	创建日期:	2014-12-24
//	文件名称:	Test_EffectNode_TailSprite.h
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
class CTest_EffectNode_TailSpriteLayer : public CCLayer
{
public:
	bool init()
	{
		m_isDebugOn = false;

		setTouchEnabled( true );

		CCSize winSize=CCDirector::sharedDirector()->getWinSize();
		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

		CCLabelTTF* pInfo = CCLabelTTF::create(A2U( "请鼠标随意滑动" ).c_str(), "Arial", 36 );
		pInfo->setPosition( SCREEN_CENTER_POS );
		addChild( pInfo );

		// DEBUG面板
		{
			CCScale9Sprite* btnUp=CCScale9Sprite::create(s_szSprite9Btn1Path);
			CCScale9Sprite* btnDn=CCScale9Sprite::create(s_szSprite9Btn2Path);
			CCLabelTTF*title=CCLabelTTF::create("debug", "Helvetica", 26);
			CCControlButton* controlButton=CCControlButton::create(title, btnUp);
			controlButton->setBackgroundSpriteForState(btnDn,CCControlStateHighlighted);
			controlButton->setPreferredSize(CCSize(100,50));
			controlButton->setPosition(ccp(origin.x+controlButton->getContentSize().width/2,winSize.height-150));
			controlButton->addTargetWithActionForControlEvents(this, (SEL_CCControlHandler)(&CTest_EffectNode_TailSpriteLayer::controlButtonEvent_debug), CCControlEventTouchDown);
			this->addChild(controlButton);
			m_controlButton_debug=controlButton;

			m_label_debugState = CCLabelTTF::create(A2U("DEBUG模式 : 关闭").c_str(), "Arial", 26);
			m_label_debugState->setPosition(ccp(m_controlButton_debug->getPositionX()+m_controlButton_debug->getContentSize().width/2+m_label_debugState->getContentSize().width/2+10,
				m_controlButton_debug->getPositionY()));
			this->addChild(m_label_debugState, 1);
		}

		return true;
	}
protected:
	void update(float dt)
	{
		int nTailSprite=(int)m_tailSpriteList.size();
		for(int i=0;i<nTailSprite-1;i++)
		{
			FKCW_EffectNode_TailSprite* tailSprite=m_tailSpriteList[i];
			if(tailSprite->getIsEmpty())
			{
				this->removeChild(tailSprite);
				m_tailSpriteList.erase(m_tailSpriteList.begin()+i);
				i--;
				nTailSprite--;
			}
		}
	}
	virtual void ccTouchesBegan(cocos2d::CCSet* touches , cocos2d::CCEvent* event)
	{
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		CCSetIterator it;
		CCTouch* touch;
		for( it = touches->begin(); it != touches->end(); it++)
		{
			touch = (CCTouch*)(*it);
			if(!touch)
				break;

			CCPoint loc_winSpace = touch->getLocationInView();
			CCPoint loc_GLSpace = CCDirector::sharedDirector()->convertToGL(loc_winSpace);

			FKCW_EffectNode_TailSprite* tailSprite=new FKCW_EffectNode_TailSprite();
			tailSprite->autorelease();
			tailSprite->init( s_szQuadDotPath );
			tailSprite->setAnchorPoint(ccp(0.5,0.5));
			tailSprite->setPosition(ccp(winSize.width/2, winSize.height/2));
			tailSprite->setMinDis(14);
			this->addChild(tailSprite);
			tailSprite->scheduleUpdate();
			tailSprite->setPosition(loc_GLSpace);
			tailSprite->setIsShowWire(m_isDebugOn);
			m_tailSpriteList.push_back(tailSprite);
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
			if(!m_tailSpriteList.empty())
			{
				m_tailSpriteList[(int)m_tailSpriteList.size()-1]->setPosition(loc_GLSpace);
			}
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
			int nTailSprite=(int)m_tailSpriteList.size();
			for(int i=0;i<nTailSprite;i++)
			{
				FKCW_EffectNode_TailSprite* tailSprite=m_tailSpriteList[i];
				tailSprite->setIsShowWire(true);
			}
		}
		else
		{
			m_label_debugState->setString(A2U("DEBUG模式 : 关闭").c_str()) ;
			int nTailSprite=(int)m_tailSpriteList.size();
			for(int i=0;i<nTailSprite;i++)
			{
				FKCW_EffectNode_TailSprite* tailSprite=m_tailSpriteList[i];
				tailSprite->setIsShowWire(false);
			}
		}
	}
protected:
	CCControlButton*	m_controlButton_debug;
	CCLabelTTF*			m_label_debugState;
	bool				m_isDebugOn;
protected:
	vector<FKCW_EffectNode_TailSprite*> m_tailSpriteList;
public:
	CREATE_FUNC( CTest_EffectNode_TailSpriteLayer );
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(EffectNode, TailSprite, 残影拖尾精灵)
//-------------------------------------------------------------------------