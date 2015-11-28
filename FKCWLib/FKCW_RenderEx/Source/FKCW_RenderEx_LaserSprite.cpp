//-------------------------------------------------------------------------
#include "../Include/FKCW_RenderEx_LaserSprite.h"
#include "../../FKCW_Shader/Include/FKCW_Shader_ShaderManager.h"
//-------------------------------------------------------------------------
static unsigned char s_2x2WhiteImage[] = 
{
	// RGBA8888
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF
};

#define CC_2x2_WHITE_IMAGE_KEY  "s_2x2WhiteImage"
//-------------------------------------------------------------------------
FKCW_RenderEx_LaserSprite::FKCW_RenderEx_LaserSprite()
{

}
//-------------------------------------------------------------------------
FKCW_RenderEx_LaserSprite::~FKCW_RenderEx_LaserSprite()
{

}
//-------------------------------------------------------------------------
FKCW_RenderEx_LaserSprite* FKCW_RenderEx_LaserSprite::Create( float w, float h )
{
	FKCW_RenderEx_LaserSprite* s = new FKCW_RenderEx_LaserSprite();
	s->initWithTexture(NULL);
	s->setContentSize(CCSizeMake(w, h));
	s->setTextureRect(CCRectMake(0, 0, w, h));
	s->setTextureCoords(CCRectMake(0, 0, 2, 2));
	return (FKCW_RenderEx_LaserSprite*)s->autorelease();
}
//-------------------------------------------------------------------------
bool FKCW_RenderEx_LaserSprite::initWithTexture(CCTexture2D *pTexture)
{
	// 该精灵不能设置用户定义的纹理
	if(!pTexture || CCTextureCache::sharedTextureCache()->textureForKey(CC_2x2_WHITE_IMAGE_KEY) != pTexture) 
	{
		pTexture = CCTextureCache::sharedTextureCache()->textureForKey(CC_2x2_WHITE_IMAGE_KEY);
		if (!pTexture) 
		{
			CCImage* image = new CCImage();
			bool isOK = image->initWithImageData(s_2x2WhiteImage, sizeof(s_2x2WhiteImage), CCImage::kFmtRawData, 2, 2, 8);
			CCAssert(isOK, "The 2x2 empty texture was created unsuccessfully.");
			pTexture = CCTextureCache::sharedTextureCache()->addUIImage(image, CC_2x2_WHITE_IMAGE_KEY);
			CC_SAFE_RELEASE(image);
		}
	}

	if(!CCSprite::initWithTexture(pTexture)) 
	{
		return false;
	}

	// 设置镭射激光的shader
	setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderLaser ));

	return true;
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_LaserSprite::setTexture(CCTexture2D *texture)
{
	if(CCTextureCache::sharedTextureCache()->textureForKey(CC_2x2_WHITE_IMAGE_KEY) != texture) 
	{
		CCLOGWARN("Doesn't support set texture to FKCW_RenderEx_LaserSprite");
	} 
	else 
	{
		CCSprite::setTexture(texture);
	}
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_LaserSprite::setWidth(float w)
{
	CCSize size = getContentSize();
	size.width = w;
	setContentSize(size);
	setTextureRect(CCRectMake(0, 0, w, size.height));
	setTextureCoords(CCRectMake(0, 0, 2, 2));
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_LaserSprite::setHeight(float h)
{
	CCSize size = getContentSize();
	size.height = h;
	setContentSize(size);
	setTextureRect(CCRectMake(0, 0, size.width, h));
	setTextureCoords(CCRectMake(0, 0, 2, 2));
}
//-------------------------------------------------------------------------