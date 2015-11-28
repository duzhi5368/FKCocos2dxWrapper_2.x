//*************************************************************************
//	创建日期:	2015-1-8
//	文件名称:	Test_RenderEx_BlurTextureFilter.h
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
class CTest_RenderEx_BlurTextureFilterLayer : public CCLayer
{
public:
	bool init()
	{
		do 
		{
			CC_BREAK_IF( !CCLayer::init() );

			// 必须使用RGBA8888格式的图片
			CCTexture2D::setDefaultAlphaPixelFormat( kCCTexture2DPixelFormat_RGBA8888 );
			{
				// 创建 Mutable 纹理
				FKCW_RenderEx_Texture2DMutable* pAllMutableTex = new FKCW_RenderEx_Texture2DMutable();
				CCImage* pImage = new CCImage();
				if( pImage && pImage->initWithImageFile( s_szHalfPhotoPath ))
				{
					pImage->autorelease();
				}
				if( pAllMutableTex && pAllMutableTex->initWithData( pImage->getData(), 
					kCCTexture2DPixelFormat_RGBA8888, pImage->getWidth(), pImage->getHeight(),
					CCSizeMake(pImage->getWidth(), pImage->getHeight() ) ) )
				{
					pAllMutableTex->autorelease();
				}
				else
				{
					return false;
				}
				// 创建 Mutable 纹理
				FKCW_RenderEx_Texture2DMutable* pFragMutableTex = new FKCW_RenderEx_Texture2DMutable();
				CCImage* pSubImage = new CCImage();
				if( pSubImage && pSubImage->initWithImageFile( s_szHalfPhotoPath ))
				{
					pSubImage->autorelease();
				}
				if( pFragMutableTex && pFragMutableTex->initWithData( pSubImage->getData(), 
					kCCTexture2DPixelFormat_RGBA8888, pSubImage->getWidth(), pSubImage->getHeight(),
					CCSizeMake(pSubImage->getWidth(), pSubImage->getHeight() ) ) )
				{
					pFragMutableTex->autorelease();
				}
				else
				{
					return false;
				}
				// 创建 NoMutable 纹理
				CCTexture2D* pNoMutableTex = pAllMutableTex->copyMutable( false );
				pNoMutableTex->autorelease();

				// 给 Mutable 纹理全面混合模糊
				FKCW_RenderEx_BlurTextureFilter AllFilter;
				AllFilter.blur( pAllMutableTex, 8 );
				// 给 Mutable 纹理部分混合模糊
				FKCW_RenderEx_BlurTextureFilter FragFilter;
				FragFilter.blur( pFragMutableTex, 8, CCRectMake( 0, 0, 380, 250 ));


				// 显示 Mutable 纹理 和 NoMutable 纹理
				CCSprite* pSprite = CCSprite::createWithTexture( pNoMutableTex );
				pSprite->setPosition( SCREEN_CENTER_POS + ccp(-280, 0) );
				CCSprite* pFragBlurSprite = CCSprite::createWithTexture( pFragMutableTex );
				pFragBlurSprite->setPosition( SCREEN_CENTER_POS + ccp(0, 0) );
				CCSprite* pAllBlurSprite = CCSprite::createWithTexture( pAllMutableTex );
				pAllBlurSprite->setPosition( SCREEN_CENTER_POS + ccp(280, 0) );

				this->addChild( pSprite );
				this->addChild( pFragBlurSprite );
				this->addChild( pAllBlurSprite );
			}
			return true;
		} while ( 0 );
		return false;
	}
public:
	CREATE_FUNC( CTest_RenderEx_BlurTextureFilterLayer );
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(RenderEx, BlurTextureFilter, 纹理模糊器，可以对图片字体等进行模糊处理)
//-------------------------------------------------------------------------