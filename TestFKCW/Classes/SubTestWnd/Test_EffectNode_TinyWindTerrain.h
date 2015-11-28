//*************************************************************************
//	创建日期:	2014-12-24
//	文件名称:	Test_EffectNode_TinyWindTerrain.h
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
class CTest_EffectNode_TinyWindTerrainLayer : public CCLayer
{
public:
	bool init()
	{
		m_tinyWingsTerrainSprite=NULL;
		m_isDebugOn=false;

		this->setTouchEnabled(true);

		CCSize winSize=CCDirector::sharedDirector()->getWinSize();
		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();


		m_tinyWingsTerrainSprite=new FKCW_EffectNode_TinyWingsTerrainSprite();
		m_tinyWingsTerrainSprite->autorelease();
		const float controlPointDis=180;
		const float terrainWidth=visibleSize.width;
		const float terrainHeight=150;
		m_tinyWingsTerrainSprite->init(terrainWidth,terrainHeight,(int)(visibleSize.width/controlPointDis));
		m_tinyWingsTerrainSprite->setPosition(ccp(origin.x,winSize.height/2-120));
		addChild(m_tinyWingsTerrainSprite);

		// DEBUG面板
		{
			CCScale9Sprite* btnUp=CCScale9Sprite::create(s_szSprite9Btn1Path);
			CCScale9Sprite* btnDn=CCScale9Sprite::create(s_szSprite9Btn2Path);
			CCLabelTTF*title=CCLabelTTF::create("debug", "Helvetica", 26);
			CCControlButton* controlButton=CCControlButton::create(title, btnUp);
			controlButton->setBackgroundSpriteForState(btnDn,CCControlStateHighlighted);
			controlButton->setPreferredSize(CCSize(100,50));
			controlButton->setPosition(ccp(origin.x+controlButton->getContentSize().width/2,winSize.height-150));
			controlButton->addTargetWithActionForControlEvents(this, (SEL_CCControlHandler)(&CTest_EffectNode_TinyWindTerrainLayer::controlButtonEvent_debug), CCControlEventTouchDown);
			this->addChild(controlButton);
			m_controlButton_debug=controlButton;

			m_label_debugState = CCLabelTTF::create(A2U("DEBUG模式 : 关闭").c_str(), "Arial", 26);
			m_label_debugState->setPosition(ccp(m_controlButton_debug->getPositionX()+m_controlButton_debug->getContentSize().width/2+m_label_debugState->getContentSize().width/2+10,
				m_controlButton_debug->getPositionY()));
			this->addChild(m_label_debugState, 1);
		}
		{
			CCSize winSize = CCDirector::sharedDirector()->getWinSize();
			CCControlSlider *slider = CCControlSlider::create(s_szProgressBGPath,
				s_szProgressPath ,
				s_szSliderPath );
			slider->setAnchorPoint(ccp(0.5f, 0.5f));
			slider->setMinimumValue(0);
			slider->setMaximumValue(1);
			slider->setValue(m_tinyWingsTerrainSprite->getTexNonlinearFactor());
			slider->setPosition(ccp(winSize.width *0.5, 100));
			slider->addTargetWithActionForControlEvents(this, cccontrol_selector(CTest_EffectNode_TinyWindTerrainLayer::sliderAction), CCControlEventValueChanged);
			m_pSliderCtl=slider;
			addChild(m_pSliderCtl,100);

			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("纹理线性").c_str(), "Arial", 26);
			pLabel->setPosition(ccp(slider->getPositionX()-slider->getContentSize().width/2-pLabel->getContentSize().width/2, slider->getPositionY()));
			this->addChild(pLabel, 1);

			m_label=CCLabelTTF::create(FKCW_Base_Utils::FloatToString(m_tinyWingsTerrainSprite->getTexNonlinearFactor()).c_str(), "Arial", 26);
			m_label->setPosition(ccp(20+slider->getPositionX()+slider->getContentSize().width/2+m_label->getContentSize().width/2, slider->getPositionY()));
			this->addChild(m_label);

		}

		{
			CCSize winSize = CCDirector::sharedDirector()->getWinSize();
			CCControlSlider *slider = CCControlSlider::create(s_szProgressBGPath,
				s_szProgressPath ,
				s_szSliderPath );
			slider->setAnchorPoint(ccp(0.5f, 0.5f));
			slider->setMinimumValue(0); 
			slider->setMaximumValue(180);
			slider->setValue(m_tinyWingsTerrainSprite->getRibbonRotation());
			slider->setPosition(ccp(winSize.width *0.5, 60));
			slider->addTargetWithActionForControlEvents(this, cccontrol_selector(CTest_EffectNode_TinyWindTerrainLayer::sliderAction_ribbonRotation), CCControlEventValueChanged);
			addChild(slider,100);
			m_pSliderCtl_ribbonRotation=slider;

			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("旋转率").c_str(), "Arial", 26);
			pLabel->setPosition(ccp(slider->getPositionX()-slider->getContentSize().width/2-pLabel->getContentSize().width/2, slider->getPositionY()));
			this->addChild(pLabel, 1);

			CCLabelTTF*label=CCLabelTTF::create(FKCW_Base_Utils::FloatToString(m_tinyWingsTerrainSprite->getRibbonRotation()).c_str(), "Arial", 26);
			label->setPosition(ccp(
				20+slider->getPositionX()+slider->getContentSize().width/2+label->getContentSize().width/2,
				slider->getPositionY()));
			this->addChild(label);
			m_label_ribbonRotation=label;

		}
		{
			CCSize winSize = CCDirector::sharedDirector()->getWinSize();
			CCControlSlider *slider = CCControlSlider::create(s_szProgressBGPath,
				s_szProgressPath ,
				s_szSliderPath );
			slider->setAnchorPoint(ccp(0.5f, 0.5f));
			slider->setMinimumValue(0); 
			slider->setMaximumValue(30);
			slider->setValue(m_tinyWingsTerrainSprite->getRibbonRepeat());
			slider->setPosition(ccp(winSize.width *0.5, 20));
			slider->addTargetWithActionForControlEvents(this, cccontrol_selector(CTest_EffectNode_TinyWindTerrainLayer::sliderAction_ribbonRepeat), CCControlEventValueChanged);
			addChild(slider,100);
			m_pSliderCtl_ribbonRepeat=slider;

			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("重复率").c_str(), "Arial", 26);
			pLabel->setPosition(ccp(slider->getPositionX()-slider->getContentSize().width/2-pLabel->getContentSize().width/2, slider->getPositionY()));
			this->addChild(pLabel, 1);

			CCLabelTTF*label=CCLabelTTF::create(FKCW_Base_Utils::FloatToString(m_tinyWingsTerrainSprite->getRibbonRepeat()).c_str(), "Arial", 26);
			label->setPosition(ccp(
				20+slider->getPositionX()+slider->getContentSize().width/2+label->getContentSize().width/2,
				slider->getPositionY()));
			this->addChild(label);
			m_label_ribbonRepeat=label;

		}
		// 重新生成按钮
		{
			CCScale9Sprite* btnUp=CCScale9Sprite::create(s_szSprite9Btn1Path);
			CCScale9Sprite* btnDn=CCScale9Sprite::create(s_szSprite9Btn2Path);
			CCLabelTTF*title=CCLabelTTF::create( A2U("重新生成").c_str(), "Helvetica", 25);
			CCControlButton* controlButton=CCControlButton::create(title, btnUp);
			controlButton->setBackgroundSpriteForState(btnDn,CCControlStateHighlighted);
			controlButton->setPreferredSize(CCSize(150,50));
			controlButton->setPosition(ccp(winSize.width/2,150));
			controlButton->addTargetWithActionForControlEvents(this, (SEL_CCControlHandler)(&CTest_EffectNode_TinyWindTerrainLayer::controlButtonEvent_reGenerate), CCControlEventTouchUpInside);
			this->addChild(controlButton);
			m_controlButton_reGenerate=controlButton;
		}

		return true;
	}
public:
	void sliderAction(CCObject* sender, CCControlEvent controlEvent)
	{
		CCControlSlider* pSlider = (CCControlSlider*)sender;
		float value=pSlider->getValue();
		m_tinyWingsTerrainSprite->setTexNonlinearFactor(value);
		m_label->setString(FKCW_Base_Utils::FloatToString(value).c_str());
		m_label->setPosition(ccp(
			20+m_pSliderCtl->getPositionX()+m_pSliderCtl->getContentSize().width/2+m_label->getContentSize().width/2,
			m_pSliderCtl->getPositionY()));
	}
	void sliderAction_ribbonRotation(CCObject* sender, CCControlEvent controlEvent)
	{
		CCControlSlider* pSlider = (CCControlSlider*)sender;
		float value=pSlider->getValue();
		m_tinyWingsTerrainSprite->setRibbonRotation(value);
		m_label_ribbonRotation->setString(FKCW_Base_Utils::FloatToString(value).c_str());
		m_label_ribbonRotation->setPosition(ccp(
			20+m_pSliderCtl_ribbonRotation->getPositionX()+m_pSliderCtl_ribbonRotation->getContentSize().width/2+m_label_ribbonRotation->getContentSize().width/2,
			m_pSliderCtl_ribbonRotation->getPositionY()));
	}
	void sliderAction_ribbonRepeat(CCObject* sender, CCControlEvent controlEvent)
	{
		CCControlSlider* pSlider = (CCControlSlider*)sender;
		float value=pSlider->getValue();
		m_tinyWingsTerrainSprite->setRibbonRepeat(value);
		m_label_ribbonRepeat->setString(FKCW_Base_Utils::FloatToString(value).c_str());
		m_label_ribbonRepeat->setPosition(ccp(
			20+m_pSliderCtl_ribbonRepeat->getPositionX()+m_pSliderCtl_ribbonRepeat->getContentSize().width/2+m_label_ribbonRepeat->getContentSize().width/2,
			m_pSliderCtl_ribbonRepeat->getPositionY()));
	}
	void controlButtonEvent_debug(CCObject *senderz, CCControlEvent controlEvent)
	{
		m_isDebugOn=!m_isDebugOn;

		if(m_isDebugOn)
		{
			m_label_debugState->setString(A2U("DEBUG模式 : 开启").c_str()) ;
			m_tinyWingsTerrainSprite->setIsDrawDebug(true);
		}
		else
		{
			m_label_debugState->setString(A2U("DEBUG模式 : 关闭").c_str()) ;
			m_tinyWingsTerrainSprite->setIsDrawDebug(false);
		}
	}
	void controlButtonEvent_reGenerate(CCObject *senderz, CCControlEvent controlEvent)
	{
		m_tinyWingsTerrainSprite->reGenerateShape(m_tinyWingsTerrainSprite->getInsertControlPointCount());
		m_tinyWingsTerrainSprite->reGenerateColors();
	}
protected:
	CCControlSlider*	m_pSliderCtl;
	CCLabelTTF*			m_label;
	CCControlSlider*	m_pSliderCtl_ribbonRotation;
	CCLabelTTF*			m_label_ribbonRotation;
	CCControlSlider*	m_pSliderCtl_ribbonRepeat;
	CCLabelTTF*			m_label_ribbonRepeat;
protected:
	CCControlButton*	m_controlButton_debug;
	CCLabelTTF*			m_label_debugState;
	bool				m_isDebugOn;
	CCControlButton*	m_controlButton_reGenerate;
protected:
	FKCW_EffectNode_TinyWingsTerrainSprite*	m_tinyWingsTerrainSprite;
public:
	CREATE_FUNC( CTest_EffectNode_TinyWindTerrainLayer );
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(EffectNode, TinyWindTerrain, 类似游戏《TinyWnd》的地形)
//-------------------------------------------------------------------------