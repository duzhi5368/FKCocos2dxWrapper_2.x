//*************************************************************************
//	创建日期:	2014-12-18
//	文件名称:	Test_EffectNode_2DSoftShadow.h
//  创 建 人:   WilhanTian	
//	版权所有:	MIT
//	说    明:	
//  备    注:	不会用。。。
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
class CTest_EffectNode_2DSoftShadowLayer : public CCLayer
{
public:
	bool init()
	{
		m_sprite			=NULL;
		m_sprite2			=NULL;
		m_sprite3			=NULL;
		m_sprite_light		=NULL;
		m_shadowObj			=NULL;
		m_shadowObj2		=NULL;
		m_shadowObj3		=NULL;
		m_light				=NULL;
		m_backGround		=NULL;
		m_shadowRoot		=NULL;
		m_isDebugOn			=false;

		this->setTouchEnabled(true);

		CCSize winSize=CCDirector::sharedDirector()->getWinSize();
		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

		m_backGround=CCSprite::create();
		m_backGround->initWithFile( s_szBackground1Path );
		m_backGround->setScale(2.0f);
		m_backGround->setPosition(ccp(winSize.width/2,winSize.height/2));
		addChild(m_backGround);

		m_shadowRoot=new C2DSoftShadowRoot();
		m_shadowRoot->autorelease();
		m_shadowRoot->init();
		addChild(m_shadowRoot);

		m_light=new FKCW_EffectNode_2DSoftShadow_LightNode();
		m_light->autorelease();
		m_light->init(48);
		m_shadowRoot->setLight(m_light);
		m_light->setPosition(ccp(winSize.width/4*3,winSize.height/3*2));

		m_shadowObj=new FKCW_Effect_2DSoftShadow_ShadowObj();
		m_shadowObj->autorelease();
		m_shadowObj->init(FKCW_Base_Utils::MakeRectPolygon(73/2,80/2));
		m_shadowObj->setLight(m_light);
		m_shadowObj->setRotation(37.0f);
		m_shadowRoot->addObj(m_shadowObj);
		m_shadowObj->setPosition(ccp(winSize.width/2,winSize.height/2));

		m_shadowObj2=new FKCW_Effect_2DSoftShadow_ShadowObj();
		m_shadowObj2->autorelease();
		m_shadowObj2->init(FKCW_Base_Utils::MakeRegularPolygon(16, 16));
		m_shadowObj2->setLight(m_light);
		m_shadowRoot->addObj(m_shadowObj2);
		m_shadowObj2->setPosition(ccp(winSize.width/3*2,winSize.height/3));

		m_shadowObj3=new FKCW_Effect_2DSoftShadow_ShadowObj();
		m_shadowObj3->autorelease();
		m_shadowObj3->init(FKCW_Base_Utils::MakeRegularPolygon(42,8));
		m_shadowObj3->setLight(m_light);
		m_shadowRoot->addObj(m_shadowObj3);
		m_shadowObj3->setPosition(ccp(winSize.width/2,winSize.height/6*5));

		m_sprite=CCSprite::create();
		m_sprite->initWithFile(s_szCocosLogoPath);
		addChild(m_sprite,20);
		m_sprite->setRotation(37.0f);
		m_sprite->setPosition(ccp(winSize.width/2,winSize.height/2));

		m_sprite2=CCSprite::create();
		m_sprite2->initWithFile(s_szEmoticon);
		addChild(m_sprite2,20);
		m_sprite2->setPosition(ccp(winSize.width/3*2,winSize.height/3));

		m_sprite3=CCSprite::create();
		m_sprite3->initWithFile(s_szDishPath);
		addChild(m_sprite3,20);
		m_sprite3->setScale(0.5f);
		m_sprite3->setRotation(360.0f/8/2);
		m_sprite3->setPosition(ccp(winSize.width/2,winSize.height/6*5));

		m_sprite_light=CCSprite::create();
		m_sprite_light->initWithFile( s_szLightPath );
		addChild(m_sprite_light,20);
		m_sprite_light->setPosition(ccp(winSize.width/4*3,winSize.height/3*2));
		m_sprite_light->setScale(static_cast<float>( 4.5*m_light->getR()/(m_sprite_light->getContentSize().width/2)) );
		ccBlendFunc blendFunc={GL_ONE,GL_ONE};
		m_sprite_light->setBlendFunc(blendFunc);

		// 亮度控制
		{
			CCSize winSize = CCDirector::sharedDirector()->getWinSize();
			CCControlSlider *slider = CCControlSlider::create( s_szProgressBGPath,
				s_szProgressPath ,
				s_szSliderPath );
			slider->setAnchorPoint(ccp(0.5f, 0.5f));
			slider->setMinimumValue(0);
			slider->setMaximumValue(1);
			slider->setValue(m_shadowRoot->getShadowDarkness());
			slider->setPosition(ccp(winSize.width *0.5, 80));
			slider->addTargetWithActionForControlEvents(this, cccontrol_selector(CTest_EffectNode_2DSoftShadowLayer::sliderAction), 
				CCControlEventValueChanged);
			m_pSliderCtl=slider;
			addChild(m_pSliderCtl,100);

			CCLabelTTF* pLabel = CCLabelTTF::create( A2U("暗度").c_str(), "Arial", 26);
			pLabel->setPosition(ccp(slider->getPositionX()-slider->getContentSize().width/2-pLabel->getContentSize().width/2,
				slider->getPositionY()));
			this->addChild(pLabel, 1);

			m_label=CCLabelTTF::create(FKCW_Base_Utils::FloatToString(m_shadowRoot->getShadowDarkness()).c_str(), "Arial", 26);
			m_label->setPosition(ccp(20+slider->getPositionX()+slider->getContentSize().width/2+m_label->getContentSize().width/2,
				slider->getPositionY()));
			this->addChild(m_label);
		}
		// DEBUG按钮
		{
			CCScale9Sprite* btnUp=CCScale9Sprite::create( s_szSprite9Btn1Path );
			CCScale9Sprite* btnDn=CCScale9Sprite::create( s_szSprite9Btn2Path );
			CCLabelTTF*title=CCLabelTTF::create("debug", "Helvetica", 26);
			CCControlButton* controlButton=CCControlButton::create(title, btnUp);
			controlButton->setBackgroundSpriteForState(btnDn,CCControlStateHighlighted);
			controlButton->setPreferredSize(CCSize(100,50));
			controlButton->setPosition(ccp(origin.x+controlButton->getContentSize().width/2,winSize.height-150));
			controlButton->addTargetWithActionForControlEvents(this,
				(SEL_CCControlHandler)(&CTest_EffectNode_2DSoftShadowLayer::controlButtonEvent_debug), CCControlEventTouchDown);
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
	virtual void ccTouchesBegan(CCSet* touches , CCEvent* event)
	{

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
			m_light->setPosition(location);
			m_sprite_light->setPosition(location);
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
	void sliderAction(CCObject* sender, CCControlEvent controlEvent)
	{
		CCControlSlider* pSlider = (CCControlSlider*)sender;
		float value=pSlider->getValue();
		m_shadowRoot->setShadowDarkness(value);
		m_label->setString(FKCW_Base_Utils::FloatToString(value).c_str());
		m_label->setPosition(ccp(
			20+m_pSliderCtl->getPositionX()+m_pSliderCtl->getContentSize().width/2+m_label->getContentSize().width/2,
			m_pSliderCtl->getPositionY()));
	}
	void controlButtonEvent_debug(CCObject *senderz, CCControlEvent controlEvent)
	{
		m_isDebugOn=!m_isDebugOn;

		if(m_isDebugOn)
		{
			m_label_debugState->setString(A2U("DEBUG模式 : 开启").c_str()) ;
			m_shadowRoot->setIsDrawDebug(true);
		}
		else
		{
			m_label_debugState->setString(A2U("DEBUG模式 : 关闭").c_str()) ;
			m_shadowRoot->setIsDrawDebug(false);
		}
	}
protected:
	CCControlSlider*							m_pSliderCtl;
	CCLabelTTF*									m_label;
	CCControlButton*							m_controlButton_debug;
	CCLabelTTF*									m_label_debugState;
	bool										m_isDebugOn;
protected:
	CCSprite*									m_backGround;
	CCSprite*									m_sprite;
	CCSprite*									m_sprite2;
	CCSprite*									m_sprite3;
	CCSprite*									m_sprite_light;
	C2DSoftShadowRoot*							m_shadowRoot;
	FKCW_Effect_2DSoftShadow_ShadowObj*			m_shadowObj;
	FKCW_Effect_2DSoftShadow_ShadowObj*			m_shadowObj2;
	FKCW_Effect_2DSoftShadow_ShadowObj*			m_shadowObj3;
	FKCW_EffectNode_2DSoftShadow_LightNode*		m_light;
public:
	CREATE_FUNC( CTest_EffectNode_2DSoftShadowLayer );
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(EffectNode, 2DSoftShadow, 2D软阴影节点)