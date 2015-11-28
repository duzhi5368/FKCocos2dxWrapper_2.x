//*************************************************************************
//	创建日期:	2014-12-24
//	文件名称:	Test_EffectNode_NormalMapped.h
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
class CTest_EffectNode_NormalMappedLayer : public CCLayer
{
public:
	bool init()
	{
		m_normalMappedSprite = NULL;
		m_lightSprite = NULL;

		this->setTouchEnabled(true);

		CCSize winSize=CCDirector::sharedDirector()->getWinSize();
		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

		m_lightSprite=new FKCW_EffectNode_NormalMappedLightSprite();
		m_lightSprite->autorelease();
		m_lightSprite->init( s_szLightPath );
		m_lightSprite->setPosition(ccp(winSize.width/4, winSize.height/4*3));
		this->addChild(m_lightSprite);
		m_lightSprite->setZ(50);

		m_normalMappedSprite=new FKCW_EffectNode_NormalMappedSprite();
		m_normalMappedSprite->autorelease();
		m_normalMappedSprite->init( s_szStartPicPath, s_szStartNormalPicPath);
		m_normalMappedSprite->setPosition(ccp(winSize.width/2, winSize.height/2));
		this->addChild(m_normalMappedSprite);
		m_normalMappedSprite->setLightSprite(m_lightSprite);

		// 参考对照
		CCSprite* pSprite = CCSprite::create( s_szStartPicPath );
		pSprite->setPosition( SCREEN_CENTER_POS + ccp(0, 100) );
		addChild( pSprite );

		{
			CCSize winSize = CCDirector::sharedDirector()->getWinSize();
			CCControlSlider *slider = CCControlSlider::create(s_szProgressBGPath,
				s_szProgressPath ,
				s_szSliderPath );
			slider->setAnchorPoint(ccp(0.5f, 0.5f));
			slider->setMinimumValue(0);
			slider->setMaximumValue(1);
			slider->setValue(m_lightSprite->getDiffuse().r);
			slider->setPosition(ccp(winSize.width *0.5, 180));
			slider->addTargetWithActionForControlEvents(this, cccontrol_selector(CTest_EffectNode_NormalMappedLayer::sliderAction), CCControlEventValueChanged);
			m_pSliderCtl=slider;
			addChild(m_pSliderCtl,100);

			CCLabelTTF* pLabel = CCLabelTTF::create("R ", "Arial", 26);
			pLabel->setPosition(ccp(slider->getPositionX()-slider->getContentSize().width/2-pLabel->getContentSize().width/2, slider->getPositionY()));
			this->addChild(pLabel, 1);

		}
		{
			CCSize winSize = CCDirector::sharedDirector()->getWinSize();
			CCControlSlider *slider = CCControlSlider::create(s_szProgressBGPath,
				s_szProgressPath ,
				s_szSliderPath );
			slider->setAnchorPoint(ccp(0.5f, 0.5f));
			slider->setMinimumValue(0);
			slider->setMaximumValue(1);
			slider->setValue(m_lightSprite->getDiffuse().g);
			slider->setPosition(ccp(winSize.width *0.5, 180-40));
			slider->addTargetWithActionForControlEvents(this, cccontrol_selector(CTest_EffectNode_NormalMappedLayer::sliderAction2), CCControlEventValueChanged);
			m_pSliderCtl2=slider;
			addChild(m_pSliderCtl2,100);

			CCLabelTTF* pLabel = CCLabelTTF::create("G ", "Arial", 26);
			pLabel->setPosition(ccp(slider->getPositionX()-slider->getContentSize().width/2-pLabel->getContentSize().width/2, slider->getPositionY()));
			this->addChild(pLabel, 1);

		}

		{
			CCSize winSize = CCDirector::sharedDirector()->getWinSize();
			CCControlSlider *slider = CCControlSlider::create(s_szProgressBGPath,
				s_szProgressPath ,
				s_szSliderPath );
			slider->setAnchorPoint(ccp(0.5f, 0.5f));
			slider->setMinimumValue(0);
			slider->setMaximumValue(1);
			slider->setValue(m_lightSprite->getDiffuse().b);
			slider->setPosition(ccp(winSize.width *0.5, 180-80));
			slider->addTargetWithActionForControlEvents(this, cccontrol_selector(CTest_EffectNode_NormalMappedLayer::sliderAction3), CCControlEventValueChanged);
			m_pSliderCtl3=slider;
			addChild(m_pSliderCtl3,100);

			CCLabelTTF* pLabel = CCLabelTTF::create("B ", "Arial", 26);
			pLabel->setPosition(ccp(slider->getPositionX()-slider->getContentSize().width/2-pLabel->getContentSize().width/2, slider->getPositionY()));
			this->addChild(pLabel, 1);
		}
		{
			CCSize winSize = CCDirector::sharedDirector()->getWinSize();
			CCControlSlider *slider = CCControlSlider::create(s_szProgressBGPath,
				s_szProgressPath ,
				s_szSliderPath );
			slider->setAnchorPoint(ccp(0.5f, 0.5f));
			slider->setMinimumValue(1);
			slider->setMaximumValue(4);
			slider->setValue(m_normalMappedSprite->getKBump());
			slider->setPosition(ccp(winSize.width *0.5, 180-120));
			slider->addTargetWithActionForControlEvents(this, cccontrol_selector(CTest_EffectNode_NormalMappedLayer::sliderAction4), CCControlEventValueChanged);
			m_pSliderCtl4=slider;
			addChild(m_pSliderCtl4,100);

			CCLabelTTF* pLabel = CCLabelTTF::create("kBump ", "Arial", 26);
			pLabel->setPosition(ccp(slider->getPositionX()-slider->getContentSize().width/2-pLabel->getContentSize().width/2, slider->getPositionY()));
			this->addChild(pLabel, 1);
		}

		return true;
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
			m_lightSprite->setPosition(loc_GLSpace);
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
			m_lightSprite->setPosition(loc_GLSpace);
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
			m_lightSprite->setPosition(loc_GLSpace);
		}
	}
	void sliderAction(CCObject* sender, CCControlEvent controlEvent)
	{
		CCControlSlider* pSlider = (CCControlSlider*)sender;
		float value=pSlider->getValue();
		ccColor4F diffuse=m_lightSprite->getDiffuse();
		diffuse.r=value;
		m_lightSprite->setDiffuse(diffuse);
	}
	void sliderAction2(CCObject* sender, CCControlEvent controlEvent)
	{
		CCControlSlider* pSlider = (CCControlSlider*)sender;
		float value=pSlider->getValue();
		ccColor4F diffuse=m_lightSprite->getDiffuse();
		diffuse.g=value;
		m_lightSprite->setDiffuse(diffuse);
	}
	void sliderAction3(CCObject* sender, CCControlEvent controlEvent)
	{
		CCControlSlider* pSlider = (CCControlSlider*)sender;
		float value=pSlider->getValue();
		ccColor4F diffuse=m_lightSprite->getDiffuse();
		diffuse.b=value;
		m_lightSprite->setDiffuse(diffuse);
	}
	void sliderAction4(CCObject* sender, CCControlEvent controlEvent)
	{
		CCControlSlider* pSlider = (CCControlSlider*)sender;
		float value=pSlider->getValue();
		m_normalMappedSprite->setKBump(value);
	}
protected:
	CCControlSlider* m_pSliderCtl;
	CCControlSlider* m_pSliderCtl2;
	CCControlSlider* m_pSliderCtl3;
	CCControlSlider* m_pSliderCtl4;
protected:
	FKCW_EffectNode_NormalMappedSprite*			m_normalMappedSprite;
	FKCW_EffectNode_NormalMappedLightSprite*	m_lightSprite;
public:
	CREATE_FUNC( CTest_EffectNode_NormalMappedLayer );
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(EffectNode, NormalMapped, NormalMapped法线精灵)
//-------------------------------------------------------------------------