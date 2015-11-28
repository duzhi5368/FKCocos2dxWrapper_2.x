//-------------------------------------------------------------------------
#include "../Include/FKCW_RenderEx_PredrawSprite.h"
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
FKCW_RenderEx_PredrawSprite::FKCW_RenderEx_PredrawSprite()
	: m_bIsShouldUpdateBlendFunc( true )
	, m_pPreDrawFunction( NULL )
{

}
//-------------------------------------------------------------------------
FKCW_RenderEx_PredrawSprite::~FKCW_RenderEx_PredrawSprite()
{
	CC_SAFE_RELEASE( m_pPreDrawFunction );
}
//-------------------------------------------------------------------------
FKCW_RenderEx_PredrawSprite* FKCW_RenderEx_PredrawSprite::create()
{
	FKCW_RenderEx_PredrawSprite *pSprite = new FKCW_RenderEx_PredrawSprite();
	if (pSprite && pSprite->init())
	{
		pSprite->autorelease();
		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_RenderEx_PredrawSprite* FKCW_RenderEx_PredrawSprite::create(const char *pszFileName)
{
	FKCW_RenderEx_PredrawSprite *pobSprite = new FKCW_RenderEx_PredrawSprite();
	if (pobSprite && pobSprite->initWithFile(pszFileName))
	{
		pobSprite->autorelease();
		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_RenderEx_PredrawSprite* FKCW_RenderEx_PredrawSprite::create(const char *pszFileName, const CCRect& rect)
{
	FKCW_RenderEx_PredrawSprite *pobSprite = new FKCW_RenderEx_PredrawSprite();
	if (pobSprite && pobSprite->initWithFile(pszFileName, rect))
	{
		pobSprite->autorelease();
		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_RenderEx_PredrawSprite* FKCW_RenderEx_PredrawSprite::createWithTexture(CCTexture2D *pTexture)
{
	FKCW_RenderEx_PredrawSprite *pobSprite = new FKCW_RenderEx_PredrawSprite();
	if (pobSprite && pobSprite->initWithTexture(pTexture))
	{
		pobSprite->autorelease();
		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_RenderEx_PredrawSprite* FKCW_RenderEx_PredrawSprite::createWithTexture(CCTexture2D *pTexture, const CCRect& rect)
{
	FKCW_RenderEx_PredrawSprite *pobSprite = new FKCW_RenderEx_PredrawSprite();
	if (pobSprite && pobSprite->initWithTexture(pTexture, rect))
	{
		pobSprite->autorelease();
		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_RenderEx_PredrawSprite* FKCW_RenderEx_PredrawSprite::createWithSpriteFrame(CCSpriteFrame *pSpriteFrame)
{
	FKCW_RenderEx_PredrawSprite *pobSprite = new FKCW_RenderEx_PredrawSprite();
	if (pSpriteFrame && pobSprite && pobSprite->initWithSpriteFrame(pSpriteFrame))
	{
		pobSprite->autorelease();
		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_RenderEx_PredrawSprite* FKCW_RenderEx_PredrawSprite::createWithSpriteFrameName(const char *pszSpriteFrameName)
{
	CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pszSpriteFrameName);
	return createWithSpriteFrame(pFrame);
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_PredrawSprite::draw()
{
	if(m_pPreDrawFunction)
		m_pPreDrawFunction->execute();
	CCSprite::draw();
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_PredrawSprite::setTexture(CCTexture2D *texture)
{
	// 如果batchNode，纹理需要相同
	CCAssert(! m_pobBatchNode || texture->getName() == m_pobBatchNode->getTexture()->getName(), "FKCW_RenderEx_Sprite: Batched sprites should use the same texture as the batchnode");
	CCAssert( !texture || dynamic_cast<CCTexture2D*>(texture), "setTexture expects a CCTexture2D. Invalid argument");

	if (NULL == texture)
	{
		// 通过key获取纹理
		texture = CCTextureCache::sharedTextureCache()->textureForKey(CC_2x2_WHITE_IMAGE_KEY);

		// 如果纹理不在缓冲中，则通过raw数据创建
		if (NULL == texture)
		{
			CCImage* image = new CCImage();
			bool isOK = image->initWithImageData(s_2x2WhiteImage, sizeof(s_2x2WhiteImage), CCImage::kFmtRawData, 2, 2, 8);
			CCAssert(isOK, "The 2x2 empty texture was created unsuccessfully.");

			texture = CCTextureCache::sharedTextureCache()->addUIImage(image, CC_2x2_WHITE_IMAGE_KEY);
			CC_SAFE_RELEASE(image);
		}
	}

	if (!m_pobBatchNode && m_pobTexture != texture)
	{
		CC_SAFE_RETAIN(texture);
		CC_SAFE_RELEASE(m_pobTexture);
		m_pobTexture = texture;
		if(m_bIsShouldUpdateBlendFunc)
		{
			updateBlendFunc();
		}
	}
}
//-------------------------------------------------------------------------