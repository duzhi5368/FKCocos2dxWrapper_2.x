//-------------------------------------------------------------------------
#include "../Include/FKCW_UIWidget_Label.h"
//-------------------------------------------------------------------------
FKCW_UIWidget_Label::FKCW_UIWidget_Label()
{
	setThisObject(this);
}
//-------------------------------------------------------------------------
FKCW_UIWidget_Label::~FKCW_UIWidget_Label()
{

}
//-------------------------------------------------------------------------
FKCW_UIWidget_Label* FKCW_UIWidget_Label::create()
{
	FKCW_UIWidget_Label* pRet = new FKCW_UIWidget_Label();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_Label* FKCW_UIWidget_Label::create(const char *pString, const char *pFontName, float fFontSize)
{
	FKCW_UIWidget_Label * pRet = new FKCW_UIWidget_Label();
	if (pRet && pRet->initWithString(pString, pFontName, fFontSize))
	{
		pRet->setTouchEnabled(false);
		pRet->setAnchorPoint(FKCW_UIWIDGET_BASIC_DEFAULT_ANCHOR_POINT);
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_Label* FKCW_UIWidget_Label::create(const char *pString, const char *pFontName, float fFontSize,
					   const CCSize& tDimensions, CCTextAlignment hAlignment)
{
	FKCW_UIWidget_Label * pRet = new FKCW_UIWidget_Label();
	if (pRet && pRet->initWithString(pString, pFontName, fFontSize, tDimensions, hAlignment))
	{
		pRet->setTouchEnabled(false);
		pRet->setAnchorPoint(FKCW_UIWIDGET_BASIC_DEFAULT_ANCHOR_POINT);
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_Label* FKCW_UIWidget_Label::create(const char *pString, const char *pFontName, float fFontSize,
					   const CCSize& tDimensions, CCTextAlignment hAlignment, CCVerticalTextAlignment vAlignment)
{
	FKCW_UIWidget_Label * pRet = new FKCW_UIWidget_Label();
	if (pRet && pRet->initWithString(pString, pFontName, fFontSize, tDimensions, hAlignment, vAlignment))
	{
		pRet->setTouchEnabled(false);
		pRet->setAnchorPoint(FKCW_UIWIDGET_BASIC_DEFAULT_ANCHOR_POINT);
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_Label::init()
{
	setTouchEnabled(false);
	setAnchorPoint(FKCW_UIWIDGET_BASIC_DEFAULT_ANCHOR_POINT);

	return CCLabelTTF::init();
}
//-------------------------------------------------------------------------
ENUM_WidgetTouchModel FKCW_UIWidget_Label::onTouchBegan(CCTouch* pTouch)
{
	FKCW_UIWIDGET_LONGCLICK_ONTOUCHBEGAN;

	return eWidgetTouchTransient;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Label::onTouchMoved(CCTouch* pTouch, float fDuration)
{
	FKCW_UIWIDGET_LONGCLICK_ONTOUCHMOVED;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Label::onTouchEnded(CCTouch* pTouch, float fDuration)
{
	FKCW_UIWIDGET_LONGCLICK_ONTOUCHENDED;

	CCPoint tPoint = m_pParent->convertTouchToNodeSpace(pTouch);
	if( boundingBox().containsPoint(tPoint) )
	{
		executeClickHandler(this);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Label::onTouchCancelled(CCTouch* pTouch, float fDuration)
{
	FKCW_UIWIDGET_LONGCLICK_ONTOUCHCANCELLED;
}
//-------------------------------------------------------------------------