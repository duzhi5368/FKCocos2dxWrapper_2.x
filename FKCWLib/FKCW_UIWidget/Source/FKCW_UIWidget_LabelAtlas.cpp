//-------------------------------------------------------------------------
#include "../Include/FKCW_UIWidget_LabelAtlas.h"
//-------------------------------------------------------------------------
FKCW_UIWidget_LabelAtlas::FKCW_UIWidget_LabelAtlas()
{
	setThisObject(this);
}
//-------------------------------------------------------------------------
FKCW_UIWidget_LabelAtlas::~FKCW_UIWidget_LabelAtlas()
{

}
//-------------------------------------------------------------------------
FKCW_UIWidget_LabelAtlas* FKCW_UIWidget_LabelAtlas::create(const char* pString, const char* charMapFile, unsigned int itemWidth, int unsigned itemHeight, unsigned int startCharMap)
{
	FKCW_UIWidget_LabelAtlas *pRet = new FKCW_UIWidget_LabelAtlas();
	if( pRet && pRet->initWithString(pString, charMapFile, itemWidth, itemHeight, startCharMap) )
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
FKCW_UIWidget_LabelAtlas* FKCW_UIWidget_LabelAtlas::create(const char* pString, const char* fntFile)
{    
	FKCW_UIWidget_LabelAtlas *pRet = new FKCW_UIWidget_LabelAtlas();
	if( pRet && pRet->initWithString(pString, fntFile) )
	{
		pRet->setTouchEnabled(false);
		pRet->setAnchorPoint(FKCW_UIWIDGET_BASIC_DEFAULT_ANCHOR_POINT);
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return pRet;
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_LabelAtlas::init()
{
	bool bRet = CCAtlasNode::init();

	setTouchEnabled(false);
	setAnchorPoint(FKCW_UIWIDGET_BASIC_DEFAULT_ANCHOR_POINT);

	return bRet;
}
//-------------------------------------------------------------------------
ENUM_WidgetTouchModel FKCW_UIWidget_LabelAtlas::onTouchBegan(CCTouch* pTouch)
{
	FKCW_UIWIDGET_LONGCLICK_ONTOUCHBEGAN

	return eWidgetTouchTransient;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_LabelAtlas::onTouchMoved(CCTouch* pTouch, float fDuration)
{
	FKCW_UIWIDGET_LONGCLICK_ONTOUCHMOVED
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_LabelAtlas::onTouchEnded(CCTouch* pTouch, float fDuration)
{
	FKCW_UIWIDGET_LONGCLICK_ONTOUCHENDED

		CCPoint touchPointInView = m_pParent->convertToNodeSpace(pTouch->getLocation());
	if( boundingBox().containsPoint(touchPointInView) )
	{
		executeClickHandler(this);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_LabelAtlas::onTouchCancelled(CCTouch* pTouch, float fDuration)
{
	FKCW_UIWIDGET_LONGCLICK_ONTOUCHCANCELLED
}
//-------------------------------------------------------------------------