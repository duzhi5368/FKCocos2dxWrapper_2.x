//*************************************************************************
//	创建日期:	2014-12-18
//	文件名称:	Test_RenderEx_EraserSprite.h
//  创 建 人:   WilhanTian	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
class CTest_RenderEx_EraserSpriteLayer : public CCLayer
{
public:
	bool init()
	{
		CCSprite* sprite = CCSprite::create(s_szGuaGuaLePath);
		sprite->setPosition(SCREEN_CENTER_POS);
		this->addChild(sprite);

		m_pSprite = FKCW_RenderEx_EraserSprite::create(s_szGuaGuaLe2Path, true, 1, 
			FKCW_Base_Utils::RandomInRange( 1.0f, 10.0f ));
		m_pSprite->setPosition(SCREEN_CENTER_POS+ccp(80,-105));
		m_pSprite->setDrawWidth(FKCW_Base_Utils::RandomInRange(1, 20));
		this->addChild(m_pSprite);

		return true;
	}
public:
	CREATE_FUNC( CTest_RenderEx_EraserSpriteLayer );
protected:
	FKCW_RenderEx_EraserSprite* m_pSprite;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(RenderEx, EraserSprite, 橡皮擦擦除精灵)
//-------------------------------------------------------------------------