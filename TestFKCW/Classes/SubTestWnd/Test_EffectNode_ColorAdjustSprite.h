//*************************************************************************
//	创建日期:	2014-12-24
//	文件名称:	Test_EffectNode_ColorAdjustSprite.h
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
class CTest_EffectNode_ColorAdjustSpriteLayer : public CCLayer
{
public:
	bool init()
	{
		CCSize winSize=CCDirector::sharedDirector()->getWinSize();
		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

		m_colorAdjustSprite=new FKCW_EffectNode_ColorAdjustSprite();
		m_colorAdjustSprite->autorelease();
		m_colorAdjustSprite->init( s_szHalfPhotoPath );
		m_colorAdjustSprite->setAnchorPoint(ccp(0.5,0.5));
		m_colorAdjustSprite->setPosition(SCREEN_CENTER_POS);
		this->addChild(m_colorAdjustSprite);


		{
			CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
			CCControlSlider *slider = CCControlSlider::create( s_szProgressBGPath,
				s_szProgressPath ,
				s_szSliderPath );
			slider->setAnchorPoint(ccp(0.5f, 0.5f));
			slider->setMinimumValue(-180);
			slider->setMaximumValue(+180);
			slider->setValue(m_colorAdjustSprite->getDH());
			slider->setPosition(ccp(screenSize.width *0.5, 110));
			slider->addTargetWithActionForControlEvents(this, 
				cccontrol_selector(CTest_EffectNode_ColorAdjustSpriteLayer::sliderAction), CCControlEventValueChanged);
			m_pSliderCtl=slider;
			addChild(m_pSliderCtl,100);

			CCLabelTTF* pLabel = CCLabelTTF::create( A2U("色彩度").c_str(), "Arial", 26);
			pLabel->setPosition(ccp(slider->getPositionX()-slider->getContentSize().width/2-pLabel->getContentSize().width/2, slider->getPositionY()));
			this->addChild(pLabel, 1);
	
			m_label=CCLabelTTF::create(FKCW_Base_Utils::FloatToString(m_colorAdjustSprite->getDH()).c_str(), "Arial", 26);
			m_label->setPosition(ccp(slider->getPositionX()+slider->getContentSize().width/2+m_label->getContentSize().width/2, slider->getPositionY()));
			this->addChild(m_label);
		}

		{
			CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
			CCControlSlider *slider = CCControlSlider::create( s_szProgressBGPath,
				s_szProgressPath ,
				s_szSliderPath );
			slider->setAnchorPoint(ccp(0.5f, 0.5f));
			slider->setMinimumValue(-1);
			slider->setMaximumValue(1); 
			slider->setValue(m_colorAdjustSprite->getDS());
			slider->setPosition(ccp(screenSize.width *0.5, 110-40));
			slider->addTargetWithActionForControlEvents(this, 
				cccontrol_selector(CTest_EffectNode_ColorAdjustSpriteLayer::sliderAction2), CCControlEventValueChanged);
			m_pSliderCtl2=slider;
			addChild(m_pSliderCtl2,100);

			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("饱和度").c_str(), "Arial", 26);
			pLabel->setPosition(ccp(slider->getPositionX()-slider->getContentSize().width/2-pLabel->getContentSize().width/2, slider->getPositionY()));
			this->addChild(pLabel, 1);
	
			m_label2=CCLabelTTF::create(FKCW_Base_Utils::FloatToString(m_colorAdjustSprite->getDH()).c_str(), "Arial", 26);
			m_label2->setPosition(ccp(slider->getPositionX()+slider->getContentSize().width/2+m_label2->getContentSize().width/2, slider->getPositionY()));
			this->addChild(m_label2);
		}

		{
			CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
			CCControlSlider *slider = CCControlSlider::create( s_szProgressBGPath,
				s_szProgressPath ,
				s_szSliderPath );
			slider->setAnchorPoint(ccp(0.5f, 0.5f));
			slider->setMinimumValue(-1);
			slider->setMaximumValue(1); 
			slider->setValue(m_colorAdjustSprite->getDH());
			slider->setPosition(ccp(screenSize.width *0.5, 110-80));
			slider->addTargetWithActionForControlEvents(this, 
				cccontrol_selector(CTest_EffectNode_ColorAdjustSpriteLayer::sliderAction3), CCControlEventValueChanged);
			m_pSliderCtl3=slider;
			addChild(m_pSliderCtl3,100);

			CCLabelTTF* pLabel = CCLabelTTF::create(A2U("明亮度").c_str(), "Arial", 26);
			pLabel->setPosition(ccp(slider->getPositionX()-slider->getContentSize().width/2-pLabel->getContentSize().width/2, slider->getPositionY()));
			this->addChild(pLabel, 1);

			m_label3=CCLabelTTF::create(FKCW_Base_Utils::FloatToString(m_colorAdjustSprite->getDH()).c_str(), "Arial", 26);
			m_label3->setPosition(ccp(slider->getPositionX()+slider->getContentSize().width/2+m_label2->getContentSize().width/2, slider->getPositionY()));
			this->addChild(m_label3);
		}

		return true;
	}
	void sliderAction(CCObject* sender, CCControlEvent controlEvent)
	{
		CCControlSlider* pSlider = (CCControlSlider*)sender;
		float value=pSlider->getValue();
		m_colorAdjustSprite->setDH(value);
		m_label->setString(FKCW_Base_Utils::FloatToString(value).c_str());
		m_label->setPosition(ccp(
			m_pSliderCtl->getPositionX()+m_pSliderCtl->getContentSize().width/2+m_label->getContentSize().width/2+15,
			m_pSliderCtl->getPositionY()));
	}
	void sliderAction2(CCObject* sender, CCControlEvent controlEvent)
	{
		CCControlSlider* pSlider = (CCControlSlider*)sender;
		float value=pSlider->getValue();
		m_colorAdjustSprite->setDS(value);
		m_label2->setString(FKCW_Base_Utils::FloatToString(value).c_str());
		m_label2->setPosition(ccp(
			m_pSliderCtl2->getPositionX()+m_pSliderCtl2->getContentSize().width/2+m_label2->getContentSize().width/2+15,
			m_pSliderCtl2->getPositionY()));
	}
	void sliderAction3(CCObject* sender, CCControlEvent controlEvent)
	{
		CCControlSlider* pSlider = (CCControlSlider*)sender;
		float value=pSlider->getValue();
		m_colorAdjustSprite->setDL(value);
		m_label3->setString(FKCW_Base_Utils::FloatToString(value).c_str());
		m_label3->setPosition(ccp(
			m_pSliderCtl3->getPositionX()+m_pSliderCtl3->getContentSize().width/2+m_label3->getContentSize().width/2+15,
			m_pSliderCtl3->getPositionY()));
	}
protected:
	CCControlSlider* m_pSliderCtl;
	CCLabelTTF* m_label;
	CCControlSlider* m_pSliderCtl2;
	CCLabelTTF* m_label2;
	CCControlSlider* m_pSliderCtl3;
	CCLabelTTF* m_label3;
protected:
	FKCW_EffectNode_ColorAdjustSprite* m_colorAdjustSprite;
public:
	CREATE_FUNC( CTest_EffectNode_ColorAdjustSpriteLayer );
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(EffectNode, ColorAdjustSprite, 色彩调整精灵)
//-------------------------------------------------------------------------