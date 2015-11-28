//*************************************************************************
//	创建日期:	2014-12-19
//	文件名称:	Test_RenderEx_JoystickNode.h
//  创 建 人:   WilhanTian	
//	版权所有:	MIT
//	说    明:	
//  备    注:
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
class CTest_RenderEx_SimpleShaderColorPainterLayer : public CCLayer
{
public:
	bool init()
	{
		m_nEffectTag = 0;

		m_pSprite = CCSprite::create(s_szHalfPhotoPath);
		m_pSprite->setPosition(SCREEN_CENTER_POS);
		this->addChild(m_pSprite);

		CCSprite* pSprite = CCSprite::create(s_szHalfPhotoPath);
		pSprite->setPosition(SCREEN_CENTER_POS+ccp(300,0));
		this->addChild(pSprite);

		m_pLabel = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("点我更改特效").c_str(), "Arial", 36 );
		m_pLabel->setColor( ccc3(230,155,3) );
		CCMenuItemLabel* pMenuItem = CCMenuItemLabel::create(m_pLabel, this, 
			menu_selector(CTest_RenderEx_SimpleShaderColorPainterLayer::menuCallback));
		CCMenu* pMenu = CCMenu::create(pMenuItem, NULL);
		pMenu->setPosition( SCREEN_CENTER_POS + ccp( 0, -220 ));
		addChild(pMenu, 1);

		return true;
	}

	void onExit()
	{
		FKCW_RenderEx_SimpleShaderColorPainter::destroy();
		CCLayer::onExit();
	}
public:
	void menuCallback(CCObject* pSender)
	{
		FKCW_RenderEx_SimpleShaderColorPainter* painter = FKCW_RenderEx_SimpleShaderColorPainter::getInstance();
		int nodeTag = painter->addEffectNode(m_pSprite);
		switch (m_nEffectTag)
		{
		case 0:
			m_pLabel->setString(FKCW_Base_StringConvert::a2u("没有特效").c_str());
			break;
		case 1:
			m_pLabel->setString(FKCW_Base_StringConvert::a2u("黑白化").c_str());
			break;
		case 2:
			m_pLabel->setString(FKCW_Base_StringConvert::a2u("老照片").c_str());
			break;
		case 3:
			m_pLabel->setString(FKCW_Base_StringConvert::a2u("反相").c_str());
			break;
		case 4:
			m_pLabel->setString(FKCW_Base_StringConvert::a2u("灼伤（偏红）").c_str());
			break;
		case 5:
			m_pLabel->setString(FKCW_Base_StringConvert::a2u("中毒（偏绿）").c_str());
			break;
		case 6:
			m_pLabel->setString(FKCW_Base_StringConvert::a2u("寒冷（偏蓝）").c_str());
			break;
		}
		painter->setEffect(nodeTag, m_nEffectTag++);
		if( m_nEffectTag > 6 )
		{
			m_nEffectTag = 0;
		}
	}
public:
	CREATE_FUNC( CTest_RenderEx_SimpleShaderColorPainterLayer );
protected:
	CCSprite*	m_pSprite;
	CCLabelTTF* m_pLabel;
	int			m_nEffectTag;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(RenderEx, SimpleShaderColorPainter, 简单的node颜色修正涂色器)