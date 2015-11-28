//-------------------------------------------------------------------------
#include "../Include/FKCW_UIWidget_Layout.h"
//-------------------------------------------------------------------------
FKCW_UIWidget_Layout::FKCW_UIWidget_Layout()
	: m_pBackgroundImage(NULL)
	, m_pBackgroundColor(NULL)
	, m_pBackgroundGradient(NULL)
{
	setThisObject(this);
}
//-------------------------------------------------------------------------
FKCW_UIWidget_Layout::~FKCW_UIWidget_Layout()
{

}
//-------------------------------------------------------------------------
FKCW_UIWidget_Layout* FKCW_UIWidget_Layout::create()
{
	FKCW_UIWidget_Layout* pRet = new FKCW_UIWidget_Layout();
	if( pRet && pRet->init() )
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_Layout* FKCW_UIWidget_Layout::create(const CCSize& tContentSize)
{
	FKCW_UIWidget_Layout* pRet = new FKCW_UIWidget_Layout();
	if( pRet && pRet->init() )
	{
		pRet->setContentSize(tContentSize);
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_Layout::init()
{
	setCascadeColorEnabled(true);
	setCascadeOpacityEnabled(true);

	setContentSize(FKCW_UIWIDGET_LAYOUT_DEFAULT_CONTENT_SIZE);
	setAnchorPoint(FKCW_UIWIDGET_LAYOUT_DEFAULT_ANCHOR_POINT);
	setPosition(CCPointZero);

	return true;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Layout::setContentSize(const CCSize& tContentSize)
{ 
	CCNodeRGBA::setContentSize(tContentSize);

	FKCW_UIWIDGET_UPDATE_BACKGROUND_POS
}
//-------------------------------------------------------------------------
ENUM_WidgetTouchModel FKCW_UIWidget_Layout::onTouchBegan(CCTouch* pTouch)
{
	m_pSelectedWidget = NULL;
	m_eSelectedWidgetTouchModel = eWidgetTouchNone;

	CCPoint tNodePoint = convertToNodeSpace(pTouch->getLocation());
	if( m_pChildren && m_pChildren->count() > 0 )
	{
		CCObject* pObject = NULL;
		CCARRAY_FOREACH_REVERSE( m_pChildren, pObject )
		{
			CCNode* pNode = dynamic_cast<CCNode*>(pObject);
			FKCW_UIWidget_Widget* pWidget = dynamic_cast<FKCW_UIWidget_Widget*>(pObject);
			if( pWidget && pNode->isVisible() && pWidget->isEnabled() && pWidget->isTouchEnabled() )
			{
				if( pNode->boundingBox().containsPoint(tNodePoint) )
				{
					m_eSelectedWidgetTouchModel = pWidget->executeTouchBeganHandler(pTouch);
					if( m_eSelectedWidgetTouchModel == eWidgetTouchNone )
					{
						m_pSelectedWidget = NULL;
						m_eSelectedWidgetTouchModel = eWidgetTouchNone;
					}
					else
					{
						m_pSelectedWidget = pWidget;
						return m_eSelectedWidgetTouchModel;
					}
				}
			}
		}
	}
	return eWidgetTouchNone;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Layout::onTouchMoved(CCTouch* pTouch, float fDuration)
{
	if( m_pSelectedWidget && !m_pSelectedWidget->isTouchInterrupted() )
	{
		m_pSelectedWidget->executeTouchMovedHandler(pTouch, fDuration);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Layout::onTouchEnded(CCTouch* pTouch, float fDuration)
{
	if( m_pSelectedWidget && !m_pSelectedWidget->isTouchInterrupted() )
	{
		m_pSelectedWidget->executeTouchEndedHandler(pTouch, fDuration);
	}
	m_pSelectedWidget = NULL;
	m_eSelectedWidgetTouchModel = eWidgetTouchNone;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Layout::onTouchCancelled(CCTouch* pTouch, float fDuration)
{
	if( m_pSelectedWidget && !m_pSelectedWidget->isTouchInterrupted() )
	{
		m_pSelectedWidget->executeTouchCancelledHandler(pTouch, fDuration);
	}
	m_pSelectedWidget = NULL;
	m_eSelectedWidgetTouchModel = eWidgetTouchNone;
}
//-------------------------------------------------------------------------
CCObject* FKCW_UIWidget_Layout::findWidgetById(const char* id)
{
	CCAssert(id && strlen(id), "should not null");

	return find(m_pChildren, id);
}
//-------------------------------------------------------------------------
CCObject* FKCW_UIWidget_Layout::find(CCArray* pChidren, const char* id)
{
	if( pChidren && pChidren->count() > 0 )
	{
		unsigned int nCount = pChidren->count();
		for(unsigned int i = 0; i < nCount; ++i)
		{
			CCObject* pObject = pChidren->objectAtIndex(i);
			FKCW_UIWidget_Widget* pWidget = dynamic_cast<FKCW_UIWidget_Widget*>(pObject);
			if( pWidget )
			{
				if( strcmp(pWidget->getId(), id) == 0 )
				{
					return pObject;
				}
				else
				{
					CCObject* pRet = find(dynamic_cast<CCNode*>(pObject)->getChildren(), id);
					if( pRet )
					{
						return pRet;
					}

				}
			}
		}
	}
	return NULL;
}
//-------------------------------------------------------------------------