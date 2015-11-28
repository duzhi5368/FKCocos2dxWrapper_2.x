//-------------------------------------------------------------------------
#include "../Include/FKCW_UIWidget_ImageView.h"
//-------------------------------------------------------------------------
FKCW_UIWidget_ImageView::FKCW_UIWidget_ImageView()
{
	setThisObject(this);

	setAnchorPoint(FKCW_UIWIDGET_BASIC_DEFAULT_ANCHOR_POINT);
	setContentSize(FKCW_UIWIDGET_BASIC_DEFAULT_CONTENT_SIZE);
}
//-------------------------------------------------------------------------
ENUM_WidgetTouchModel FKCW_UIWidget_ImageView::onTouchBegan(CCTouch *pTouch)
{
	FKCW_UIWIDGET_LONGCLICK_ONTOUCHBEGAN;

	return eWidgetTouchTransient;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ImageView::onTouchMoved(CCTouch *pTouch, float fDuration)
{
	FKCW_UIWIDGET_LONGCLICK_ONTOUCHMOVED;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ImageView::onTouchEnded(CCTouch *pTouch, float fDuration)
{
	FKCW_UIWIDGET_LONGCLICK_ONTOUCHENDED;

	CCPoint touchPointInView = m_pParent->convertToNodeSpace(pTouch->getLocation());
	if( boundingBox().containsPoint(touchPointInView) )
	{
		executeClickHandler(this);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ImageView::onTouchCancelled(CCTouch *pTouch, float fDuration)
{
	FKCW_UIWIDGET_LONGCLICK_ONTOUCHCANCELLED;
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_ImageView::init()
{
	return CCSprite::init();
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ImageView* FKCW_UIWidget_ImageView::createWithTexture(CCTexture2D *pTexture)
{
	FKCW_UIWidget_ImageView *pobSprite = new FKCW_UIWidget_ImageView();
	if (pobSprite && pobSprite->initWithTexture(pTexture))
	{
		pobSprite->setTouchEnabled(false);
		pobSprite->autorelease();
		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ImageView* FKCW_UIWidget_ImageView::createWithTexture(CCTexture2D *pTexture, const CCRect& rect)
{
	FKCW_UIWidget_ImageView *pobSprite = new FKCW_UIWidget_ImageView();
	if (pobSprite && pobSprite->initWithTexture(pTexture, rect))
	{
		pobSprite->setTouchEnabled(false);
		pobSprite->autorelease();
		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ImageView* FKCW_UIWidget_ImageView::create(const char *pszFileName)
{
	FKCW_UIWidget_ImageView *pobSprite = new FKCW_UIWidget_ImageView();
	if (pobSprite && pobSprite->initWithFile(pszFileName))
	{
		pobSprite->setTouchEnabled(false);
		pobSprite->autorelease();
		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ImageView* FKCW_UIWidget_ImageView::create(const char *pszFileName, const CCRect& rect)
{
	FKCW_UIWidget_ImageView *pobSprite = new FKCW_UIWidget_ImageView();
	if (pobSprite && pobSprite->initWithFile(pszFileName, rect))
	{
		pobSprite->setTouchEnabled(false);
		pobSprite->autorelease();
		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ImageView* FKCW_UIWidget_ImageView::createWithSpriteFrame(CCSpriteFrame *pSpriteFrame)
{
	FKCW_UIWidget_ImageView *pobSprite = new FKCW_UIWidget_ImageView();
	if (pSpriteFrame && pobSprite && pobSprite->initWithSpriteFrame(pSpriteFrame))
	{
		pobSprite->setTouchEnabled(false);
		pobSprite->autorelease();
		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ImageView* FKCW_UIWidget_ImageView::createWithSpriteFrameName(const char *pszSpriteFrameName)
{
	CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pszSpriteFrameName);

#if COCOS2D_DEBUG > 0
	char msg[256] = {0};
	sprintf(msg, "Invalid spriteFrameName: %s", pszSpriteFrameName);
	CCAssert(pFrame != NULL, msg);
#endif

	return createWithSpriteFrame(pFrame);
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ImageView* FKCW_UIWidget_ImageView::create()
{
	FKCW_UIWidget_ImageView *pSprite = new FKCW_UIWidget_ImageView();
	if (pSprite && pSprite->init())
	{
		pSprite->setTouchEnabled(false);
		pSprite->autorelease();
		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);
	return NULL;
}
//-------------------------------------------------------------------------