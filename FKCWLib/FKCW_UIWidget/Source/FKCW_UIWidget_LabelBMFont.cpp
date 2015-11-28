//-------------------------------------------------------------------------
#include "../Include/FKCW_UIWidget_LabelBMFont.h"
//-------------------------------------------------------------------------
FKCW_UIWidget_LabelBMFont::FKCW_UIWidget_LabelBMFont()
{
	setThisObject(this);
}
//-------------------------------------------------------------------------
FKCW_UIWidget_LabelBMFont::~FKCW_UIWidget_LabelBMFont()
{

}
//-------------------------------------------------------------------------
ENUM_WidgetTouchModel FKCW_UIWidget_LabelBMFont::onTouchBegan(CCTouch *pTouch)
{
	FKCW_UIWIDGET_LONGCLICK_ONTOUCHBEGAN

	return eWidgetTouchTransient;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_LabelBMFont::onTouchMoved(CCTouch *pTouch, float fDuration)
{
	FKCW_UIWIDGET_LONGCLICK_ONTOUCHMOVED;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_LabelBMFont::onTouchEnded(CCTouch *pTouch, float fDuration)
{
	FKCW_UIWIDGET_LONGCLICK_ONTOUCHENDED

	CCPoint touchPointInView = m_pParent->convertToNodeSpace(pTouch->getLocation());
	if( boundingBox().containsPoint(touchPointInView) )
	{
		executeClickHandler(this);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_LabelBMFont::onTouchCancelled(CCTouch *pTouch, float fDuration)
{
	FKCW_UIWIDGET_LONGCLICK_ONTOUCHCANCELLED;
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_LabelBMFont::init()
{
	bool bRet = CCSpriteBatchNode::init();

	setTouchEnabled(false);
	setAnchorPoint(FKCW_UIWIDGET_BASIC_DEFAULT_ANCHOR_POINT);

	return bRet;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_LabelBMFont* FKCW_UIWidget_LabelBMFont::create()
{
	FKCW_UIWidget_LabelBMFont* pRet = new FKCW_UIWidget_LabelBMFont();
	if( pRet && pRet->init() )
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_LabelBMFont * FKCW_UIWidget_LabelBMFont::create(const char *pString, const char *fntFile, float width, CCTextAlignment alignment)
{
	return FKCW_UIWidget_LabelBMFont::create(pString, fntFile, width, alignment, CCPointZero);
}
//-------------------------------------------------------------------------
FKCW_UIWidget_LabelBMFont * FKCW_UIWidget_LabelBMFont::create(const char *pString, const char *fntFile, float width)
{
	return FKCW_UIWidget_LabelBMFont::create(pString, fntFile, width, kCCTextAlignmentLeft, CCPointZero);
}
//-------------------------------------------------------------------------
FKCW_UIWidget_LabelBMFont * FKCW_UIWidget_LabelBMFont::create(const char *pString, const char *fntFile)
{
	return FKCW_UIWidget_LabelBMFont::create(pString, fntFile, kCCLabelAutomaticWidth, kCCTextAlignmentLeft, CCPointZero);
}
//-------------------------------------------------------------------------
FKCW_UIWidget_LabelBMFont *FKCW_UIWidget_LabelBMFont::create(const char *pString, const char *fntFile, float width/* = kCCLabelAutomaticWidth*/, CCTextAlignment alignment/* = kCCTextAlignmentLeft*/, CCPoint imageOffset/* = CCPointZero*/)
{
	FKCW_UIWidget_LabelBMFont *pRet = new FKCW_UIWidget_LabelBMFont();
	if( pRet && pRet->initWithString(pString, fntFile, width, alignment, imageOffset) )
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