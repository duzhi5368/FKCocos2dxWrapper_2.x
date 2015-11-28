//*************************************************************************
//	创建日期:	2015-1-9
//	文件名称:	Test_RenderEx_SpotLightSprite.h
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
class CTest_RenderEx_SpotLightSpriteLayer : public CCLayer
{
public:
	virtual bool init()
	{
		do 
		{
			CC_BREAK_IF( !CCLayer::init() );


			CCSprite* pBKSprite = CCSprite::create( s_szHalfPhotoPath );
			pBKSprite->setPosition( SCREEN_CENTER_POS );
			this->addChild( pBKSprite );

			CCRenderTexture* pRenderLayer = CCRenderTexture::create( SCERRN_WIDTH, SCERRN_HEIGHT );
			pRenderLayer->setPosition( SCREEN_CENTER_POS );
			ccBlendFunc blendFunc = { GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA };
			pRenderLayer->getSprite()->setBlendFunc( blendFunc );
			this->addChild( pRenderLayer, 0 );

			m_pSpotLight = FKCW_RenderEx_SpotLightSprite::Create( pRenderLayer, 100.0f, ccc4( 0, 0, 20, 220 ) );
			this->addChild( m_pSpotLight, 1 );

			// 开启Input消息
			this->setTouchEnabled(true);

			return true;
		} while ( 0 );
		return false;
	}
public:
	void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
	{
		if( m_pSpotLight )
		{
			cocos2d::CCTouch* pTouch = (cocos2d::CCTouch*)pTouches->anyObject();
			cocos2d::CCPoint touchLocation = pTouch->getLocation();
			m_pSpotLight->setPosition( touchLocation );
		}
	}
	void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
	{
		if( m_pSpotLight )
		{
			cocos2d::CCTouch* pTouch = (cocos2d::CCTouch*)pTouches->anyObject();
			cocos2d::CCPoint touchLocation = pTouch->getLocation();
			m_pSpotLight->setPosition( touchLocation );
		}
	}
	void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
	{
		DelSpotLight();
	}
	void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
	{
	}
	void DelSpotLight()
	{
		if( m_pSpotLight )
		{
			m_pSpotLight->RemoveSelf();
			m_pSpotLight = NULL;
		}
		
	}
public:
	CREATE_FUNC( CTest_RenderEx_SpotLightSpriteLayer );
private:
	FKCW_RenderEx_SpotLightSprite*	m_pSpotLight;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(RenderEx, SpotLightSprite, 聚光灯效果)
//-------------------------------------------------------------------------