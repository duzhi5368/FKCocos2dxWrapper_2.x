//-------------------------------------------------------------------------
#include "../Include/FKCW_UIWidget_ToggleView.h"
//-------------------------------------------------------------------------
#define CC_SAFE_SET_VISIBLE(__ITEM__, __VAR__) if( __ITEM__ ) __ITEM__->setVisible(__VAR__)
//-------------------------------------------------------------------------
FKCW_UIWidget_ToggleView::FKCW_UIWidget_ToggleView()
{

}
//-------------------------------------------------------------------------
FKCW_UIWidget_ToggleView::~FKCW_UIWidget_ToggleView()
{

}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ToggleView::setEnabled(bool bEnabled)
{
	if( m_bEnabled != bEnabled )
	{
		if( bEnabled )
		{
			CC_SAFE_SET_VISIBLE(m_pNormalImage, true);
			CC_SAFE_SET_VISIBLE(m_pSelectedImage, false);
			CC_SAFE_SET_VISIBLE(m_pDisabledImage, false);
		}
		else
		{
			setChecked(false);
			CC_SAFE_SET_VISIBLE(m_pNormalImage, false);
			CC_SAFE_SET_VISIBLE(m_pSelectedImage, false);
			CC_SAFE_SET_VISIBLE(m_pDisabledImage, true);
		}
		m_bEnabled = bEnabled;
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ToggleView::setChecked(bool bChecked)
{
	if( m_bChecked != bChecked && m_bEnabled )
	{
		if( bChecked )
		{
			CC_SAFE_SET_VISIBLE(m_pNormalImage, false);
			CC_SAFE_SET_VISIBLE(m_pSelectedImage, true);
			CC_SAFE_SET_VISIBLE(m_pDisabledImage, false);

			setUnCheckedForOtherWidgetsByExclusion(m_pParent);
		}
		else
		{
			CC_SAFE_SET_VISIBLE(m_pNormalImage, true);
			CC_SAFE_SET_VISIBLE(m_pSelectedImage, false);
			CC_SAFE_SET_VISIBLE(m_pDisabledImage, false);
		}
		m_bChecked = bChecked;
		executeCheckHandler(this, m_bChecked);
	}
}
//-------------------------------------------------------------------------
ENUM_WidgetTouchModel FKCW_UIWidget_ToggleView::onTouchBegan(CCTouch *pTouch)
{
	FKCW_UIWIDGET_LONGCLICK_ONTOUCHBEGAN;

	return eWidgetTouchTransient;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ToggleView::onTouchMoved(CCTouch *pTouch, float fDuration)
{
	FKCW_UIWIDGET_LONGCLICK_ONTOUCHMOVED;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ToggleView::onTouchEnded(CCTouch *pTouch, float fDuration)
{
	FKCW_UIWIDGET_LONGCLICK_ONTOUCHENDED;

	CCPoint touchPointInView = m_pParent->convertToNodeSpace(pTouch->getLocation());
	if( boundingBox().containsPoint(touchPointInView) )
	{
		setChecked(!m_bChecked);
		executeClickHandler(this);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ToggleView::onTouchCancelled(CCTouch *pTouch, float fDuration)
{
	FKCW_UIWIDGET_LONGCLICK_ONTOUCHCANCELLED;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ToggleView* FKCW_UIWidget_ToggleView::create()
{
	FKCW_UIWidget_ToggleView *pRet = new FKCW_UIWidget_ToggleView();
	if( pRet && pRet->init() )
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ToggleView* FKCW_UIWidget_ToggleView::create(const char* pNormal, const char* pSelected, const char* pDisabled)
{
	FKCW_UIWidget_ToggleView *pRet = new FKCW_UIWidget_ToggleView();
	if( pRet && pRet->initWithFile(pNormal, pSelected, pDisabled) )
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ToggleView* FKCW_UIWidget_ToggleView::createWith9Sprite(const CCSize& tSize, const char* pNormal, const char* pSelected, const char* pDisabled)
{
	FKCW_UIWidget_ToggleView* pRet = new FKCW_UIWidget_ToggleView();
	if( pRet && pRet->initWith9Sprite(tSize, pNormal, pSelected, pDisabled) )
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------