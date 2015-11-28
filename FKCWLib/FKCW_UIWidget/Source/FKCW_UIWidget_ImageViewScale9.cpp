//-------------------------------------------------------------------------
#include "../Include/FKCW_UIWidget_ImageViewScale9.h"
//-------------------------------------------------------------------------
FKCW_UIWidget_ImageViewScale9::FKCW_UIWidget_ImageViewScale9()
{
	setThisObject(this);

	setAnchorPoint(FKCW_UIWIDGET_BASIC_DEFAULT_ANCHOR_POINT);
	setContentSize(FKCW_UIWIDGET_BASIC_DEFAULT_CONTENT_SIZE);
}
//-------------------------------------------------------------------------
ENUM_WidgetTouchModel FKCW_UIWidget_ImageViewScale9::onTouchBegan(CCTouch *pTouch)
{
	FKCW_UIWIDGET_LONGCLICK_ONTOUCHBEGAN;

	return eWidgetTouchTransient;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ImageViewScale9::onTouchMoved(CCTouch *pTouch, float fDuration)
{
	FKCW_UIWIDGET_LONGCLICK_ONTOUCHMOVED;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ImageViewScale9::onTouchEnded(CCTouch *pTouch, float fDuration)
{
	FKCW_UIWIDGET_LONGCLICK_ONTOUCHENDED;

	CCPoint touchPointInView = m_pParent->convertToNodeSpace(pTouch->getLocation());
	if( boundingBox().containsPoint(touchPointInView) )
	{
		executeClickHandler(this);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ImageViewScale9::onTouchCancelled(CCTouch *pTouch, float fDuration)
{
	FKCW_UIWIDGET_LONGCLICK_ONTOUCHCANCELLED;
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_ImageViewScale9::init()
{
	bool bRet = FKCW_UIWidget_Scale9Sprite::init();

	setTouchEnabled(false);

	setAnchorPoint(FKCW_UIWIDGET_BASIC_DEFAULT_ANCHOR_POINT);
	setContentSize(FKCW_UIWIDGET_BASIC_DEFAULT_CONTENT_SIZE);

	return true;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ImageViewScale9* FKCW_UIWidget_ImageViewScale9::create(const CCSize& size, const char* file)
{
	FKCW_UIWidget_ImageViewScale9* pReturn = new FKCW_UIWidget_ImageViewScale9();
	if( pReturn && pReturn->initWithFile(file) )
	{
		pReturn->setTouchEnabled(false);
		pReturn->setAnchorPoint(FKCW_UIWIDGET_BASIC_DEFAULT_ANCHOR_POINT);
		pReturn->setContentSize(size);
		pReturn->autorelease();
		return pReturn;
	}
	CC_SAFE_DELETE(pReturn);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ImageViewScale9* FKCW_UIWidget_ImageViewScale9::create(const char* file, CCRect rect,  CCRect capInsets)
{
	FKCW_UIWidget_ImageViewScale9* pReturn = new FKCW_UIWidget_ImageViewScale9();
	if( pReturn && pReturn->initWithFile(file, rect, capInsets) )
	{
		pReturn->setTouchEnabled(false);
		pReturn->setAnchorPoint(FKCW_UIWIDGET_BASIC_DEFAULT_ANCHOR_POINT);
		pReturn->autorelease();
		return pReturn;
	}
	CC_SAFE_DELETE(pReturn);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ImageViewScale9* FKCW_UIWidget_ImageViewScale9::create(const char* file, CCRect rect)
{
	FKCW_UIWidget_ImageViewScale9* pReturn = new FKCW_UIWidget_ImageViewScale9();
	if( pReturn && pReturn->initWithFile(file, rect) )
	{
		pReturn->setTouchEnabled(false);
		pReturn->setAnchorPoint(FKCW_UIWIDGET_BASIC_DEFAULT_ANCHOR_POINT);
		pReturn->autorelease();
		return pReturn;
	}
	CC_SAFE_DELETE(pReturn);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ImageViewScale9* FKCW_UIWidget_ImageViewScale9::create(CCRect capInsets, const char* file)
{
	FKCW_UIWidget_ImageViewScale9* pReturn = new FKCW_UIWidget_ImageViewScale9();
	if( pReturn && pReturn->initWithFile(capInsets, file) )
	{
		pReturn->setTouchEnabled(false);
		pReturn->setAnchorPoint(FKCW_UIWIDGET_BASIC_DEFAULT_ANCHOR_POINT);
		pReturn->autorelease();
		return pReturn;
	}
	CC_SAFE_DELETE(pReturn);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ImageViewScale9* FKCW_UIWidget_ImageViewScale9::create(const char* file)
{
	FKCW_UIWidget_ImageViewScale9* pReturn = new FKCW_UIWidget_ImageViewScale9();
	if( pReturn && pReturn->initWithFile(file) )
	{
		pReturn->setTouchEnabled(false);
		pReturn->setAnchorPoint(FKCW_UIWIDGET_BASIC_DEFAULT_ANCHOR_POINT);
		pReturn->autorelease();
		return pReturn;
	}
	CC_SAFE_DELETE(pReturn);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ImageViewScale9* FKCW_UIWidget_ImageViewScale9::createWithSpriteFrame(CCSpriteFrame* spriteFrame, CCRect capInsets)
{
	FKCW_UIWidget_ImageViewScale9* pReturn = new FKCW_UIWidget_ImageViewScale9();
	if( pReturn && pReturn->initWithSpriteFrame(spriteFrame, capInsets) )
	{
		pReturn->setTouchEnabled(false);
		pReturn->setAnchorPoint(FKCW_UIWIDGET_BASIC_DEFAULT_ANCHOR_POINT);
		pReturn->autorelease();
		return pReturn;
	}
	CC_SAFE_DELETE(pReturn);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ImageViewScale9* FKCW_UIWidget_ImageViewScale9::createWithSpriteFrame(CCSpriteFrame* spriteFrame)
{
	FKCW_UIWidget_ImageViewScale9* pReturn = new FKCW_UIWidget_ImageViewScale9();
	if( pReturn && pReturn->initWithSpriteFrame(spriteFrame) )
	{
		pReturn->setTouchEnabled(false);
		pReturn->setAnchorPoint(FKCW_UIWIDGET_BASIC_DEFAULT_ANCHOR_POINT);
		pReturn->autorelease();
		return pReturn;
	}
	CC_SAFE_DELETE(pReturn);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ImageViewScale9* FKCW_UIWidget_ImageViewScale9::createWithSpriteFrameName(const char* spriteFrameName, CCRect capInsets)
{
	FKCW_UIWidget_ImageViewScale9* pReturn = new FKCW_UIWidget_ImageViewScale9();
	if( pReturn && pReturn->initWithSpriteFrameName(spriteFrameName, capInsets) )
	{
		pReturn->setTouchEnabled(false);
		pReturn->setAnchorPoint(FKCW_UIWIDGET_BASIC_DEFAULT_ANCHOR_POINT);
		pReturn->autorelease();
		return pReturn;
	}
	CC_SAFE_DELETE(pReturn);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ImageViewScale9* FKCW_UIWidget_ImageViewScale9::createWithSpriteFrameName(const char* spriteFrameName)
{
	CCAssert(spriteFrameName != NULL, "spriteFrameName must be non-NULL");

	FKCW_UIWidget_ImageViewScale9* pReturn = new FKCW_UIWidget_ImageViewScale9();
	if( pReturn && pReturn->initWithSpriteFrameName(spriteFrameName) )
	{
		pReturn->setTouchEnabled(false);
		pReturn->setAnchorPoint(FKCW_UIWIDGET_BASIC_DEFAULT_ANCHOR_POINT);
		pReturn->autorelease();
		return pReturn;
	}
	CC_SAFE_DELETE(pReturn);

	CCLog("Could not allocate CImageViewScale9()");
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ImageViewScale9* FKCW_UIWidget_ImageViewScale9::createWithTexture(CCTexture2D* pTexture)
{
	FKCW_UIWidget_ImageViewScale9* pReturn = new FKCW_UIWidget_ImageViewScale9();
	if( pReturn && pReturn->initWithTexture(pTexture) )
	{
		pReturn->setTouchEnabled(false);
		pReturn->setAnchorPoint(FKCW_UIWIDGET_BASIC_DEFAULT_ANCHOR_POINT);
		pReturn->autorelease();
		return pReturn;
	}
	CC_SAFE_DELETE(pReturn);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ImageViewScale9* FKCW_UIWidget_ImageViewScale9::createWithTexture(CCTexture2D* pTexture, CCRect capInsets)
{
	FKCW_UIWidget_ImageViewScale9* pReturn = new FKCW_UIWidget_ImageViewScale9();
	if( pReturn && pReturn->initWithTexture(pTexture, capInsets) )
	{
		pReturn->setTouchEnabled(false);
		pReturn->setAnchorPoint(FKCW_UIWIDGET_BASIC_DEFAULT_ANCHOR_POINT);
		pReturn->autorelease();
		return pReturn;
	}
	CC_SAFE_DELETE(pReturn);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ImageViewScale9* FKCW_UIWidget_ImageViewScale9::create()
{ 
	FKCW_UIWidget_ImageViewScale9 *pReturn = new FKCW_UIWidget_ImageViewScale9();
	if( pReturn && pReturn->init() )
	{
		pReturn->setTouchEnabled(false);
		pReturn->setAnchorPoint(FKCW_UIWIDGET_BASIC_DEFAULT_ANCHOR_POINT);
		pReturn->autorelease();   
		return pReturn;
	} 
	CC_SAFE_DELETE(pReturn);
	return NULL;
}
//-------------------------------------------------------------------------