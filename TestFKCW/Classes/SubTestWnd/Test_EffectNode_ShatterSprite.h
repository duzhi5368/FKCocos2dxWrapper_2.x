//*************************************************************************
//	创建日期:	2014-12-24
//	文件名称:	Test_EffectNode_ShatterSprite.h
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
class CTest_EffectNode_ShatterSpriteLayer : public CCLayer
{
public:
	bool init()
	{
		this->setTouchEnabled(true);

		m_shatterSprite=new FKCW_EffectNode_ShatterSprite();
		m_shatterSprite->autorelease();
		m_shatterSprite->init( s_szCardPath );
		m_shatterSprite->setPosition( SCREEN_CENTER_POS );
		this->addChild(m_shatterSprite);

		return true;
	}
protected:
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
			if(m_shatterSprite->getOpacity()==0)
			{
				m_shatterSprite->setOpacity(255);
			}
			else
			{
				m_shatterSprite->setOpacity(0);
				m_shatterSprite->stopAllActions();
				FKCW_Action_ShatterAction* shatter = FKCW_Action_ShatterAction::create(4);
				m_shatterSprite->runAction(shatter);
			}
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
protected:
	FKCW_EffectNode_ShatterSprite*	m_shatterSprite;
public:
	CREATE_FUNC( CTest_EffectNode_ShatterSpriteLayer );
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(EffectNode, ShatterSprite, 中心破碎效果精灵)
//-------------------------------------------------------------------------