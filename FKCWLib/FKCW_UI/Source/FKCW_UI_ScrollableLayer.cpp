//-------------------------------------------------------------------------
#include "../Include/FKCW_UI_ScrollableLayer.h"
#include "../../FKCW_Base/Include/FKCW_Base_Geometry.h"
//-------------------------------------------------------------------------
const int	s_nScrollableLayerTouchPriority = -256;
const float s_fScrollableLayerScrollShift = 5.0f;
const int	eScrollableLayerBindingRight = 1;
const int	eScrollableLayerBindingUpper = 2;
//-------------------------------------------------------------------------
FKCW_UI_ScrollableLayer* FKCW_UI_ScrollableLayer::create(const CCSize& tWindowSize, const CCSize& tLayerSize, ENUM_ScrollableLayerBindingType eBinding)
{
	FKCW_UI_ScrollableLayer* pRet = new FKCW_UI_ScrollableLayer();
	return pRet->init(tWindowSize, tLayerSize, eBinding) ? (FKCW_UI_ScrollableLayer*)pRet->autorelease() : (delete pRet, pRet = NULL);
}
//-------------------------------------------------------------------------
FKCW_UI_ScrollableLayer::FKCW_UI_ScrollableLayer() :
	m_eBinding(eScrollableLayerBindingType_BottomLeft),
	m_bScrolling(false)
{
}
//-------------------------------------------------------------------------
bool FKCW_UI_ScrollableLayer::init(const CCSize& tWindowSize, const CCSize& tLayerSize, ENUM_ScrollableLayerBindingType eBinding)
{
	do
	{
		if( !CCLayer::init() )
			break;

		m_eBinding = eBinding;
		float w = static_cast<float>(eBinding & eScrollableLayerBindingRight);
		float h = static_cast<float>((eBinding & eScrollableLayerBindingUpper) >> 1);
		CCSize tBinding( w,h);

		CCSize tShiftSize(cczSub(tLayerSize, tWindowSize));

		m_tMinShift = cczNormalizeTo(
			cczDenormalizeTo(tBinding, cczMin(tShiftSize, CCSizeZero)),
			tWindowSize);
		m_tMaxShift = cczNormalizeTo(
			cczMax(tShiftSize, cczDenormalizeTo(tBinding, tShiftSize)),
			tWindowSize);

		setContentSize(tWindowSize);
		setTouchEnabled(true);
		ignoreAnchorPointForPosition(false);
		setShift(tBinding);

		return true;
	} while (false);

	return false;
}
//-------------------------------------------------------------------------
bool FKCW_UI_ScrollableLayer::isNodeVisibleRecursive(CCNode* pNode)
{
	for (CCNode* pCurrentNode = pNode; pCurrentNode != NULL; pCurrentNode = pCurrentNode->getParent())
	{
		if (!pCurrentNode->isVisible())
		{
			return false;
		}
	}

	return true;
}
//-------------------------------------------------------------------------
void FKCW_UI_ScrollableLayer::setShift(const CCSize& tShift)
{
	setAnchorPoint(ccpShift(
		ccp(0.5f, 0.5f),
		cczAdd(m_tMinShift, cczDenormalizeTo(tShift, cczSub(m_tMaxShift, m_tMinShift)))));
}
//-------------------------------------------------------------------------
void FKCW_UI_ScrollableLayer::setLayerSize(const CCSize& tSize)
{
	float w = static_cast<float>(m_eBinding & eScrollableLayerBindingRight);
	float h = static_cast<float>((m_eBinding & eScrollableLayerBindingUpper) >> 1);

	CCSize tBinding(w,h);

	CCSize tShiftSize(cczSub(tSize, getContentSize()));

	CCSize tOldMaxShift(m_tMaxShift);

	m_tMinShift = cczNormalizeTo(
		cczDenormalizeTo(tBinding, cczMin(tShiftSize, CCSizeZero)),
		getContentSize());

	m_tMaxShift = cczNormalizeTo(
		cczMax(tShiftSize, cczDenormalizeTo(tBinding, tShiftSize)),
		getContentSize());

	CCSize tPreserveShift(cczSub(
		m_tMaxShift,
		tOldMaxShift));

	setAnchorPoint(ccpShift(
		getAnchorPoint(),
		cczDenormalizeTo(tBinding, tPreserveShift)));
}
//-------------------------------------------------------------------------
void FKCW_UI_ScrollableLayer::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, s_nScrollableLayerTouchPriority, false);
}
//-------------------------------------------------------------------------
bool FKCW_UI_ScrollableLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	CC_UNUSED_PARAM(pEvent);

	do
	{
		if( !isNodeVisibleRecursive(this))
			break;

		CCPoint tTouchPoint(convertTouchToNodeSpaceAR(pTouch));

		if( !ccRectMakeWithCenterAndSize(CCPointZero, getContentSize()).containsPoint(tTouchPoint) )
			break;
		m_tBeginTouchPoint = tTouchPoint;

		return true;
	} while (false);

	return false;
}
//-------------------------------------------------------------------------
void FKCW_UI_ScrollableLayer::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent)
{
	CC_UNUSED_PARAM(pEvent);

	CCPoint tTouchPoint(convertTouchToNodeSpaceAR(pTouch));
	CCPoint tPrevTouchPoint(convertToNodeSpaceAR(pTouch->getPreviousLocation()));

	CCSize tShift(cczClamp(
		cczAdd(
		cczNormalizeTo(ccpDiff(tPrevTouchPoint, tTouchPoint), getContentSize()),
		ccpDiff(getAnchorPoint(), ccp(0.5f, 0.5f))),
		m_tMinShift,
		m_tMaxShift));

	if (!m_bScrolling && cczLength(ccpDiff(m_tBeginTouchPoint, tPrevTouchPoint)) >= s_fScrollableLayerScrollShift)
	{
		m_bScrolling = true;
		// TODO find workaround instead of HACK
		CCSet* pSet = new CCSet();
		pSet->addObject(pTouch);
		CCTouchDispatcher* pTouchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		CCTargetedTouchHandler* pHandler = (CCTargetedTouchHandler*)pTouchDispatcher->findHandler(this);
		pHandler->getClaimedTouches()->removeObject(pTouch);
		pTouchDispatcher->touchesCancelled(pSet, NULL);
		pHandler->getClaimedTouches()->addObject(pTouch);
		delete pSet;
	}

	if (m_bScrolling)
	{
		setAnchorPoint(ccpShift(ccp(0.5f, 0.5f), tShift));
	}
}
//-------------------------------------------------------------------------
void FKCW_UI_ScrollableLayer::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{
	CC_UNUSED_PARAM(pTouch);
	CC_UNUSED_PARAM(pEvent);

	m_bScrolling = false;
}
//-------------------------------------------------------------------------
void FKCW_UI_ScrollableLayer::ccTouchCancelled(CCTouch* pTouch, CCEvent* pEvent)
{
	CC_UNUSED_PARAM(pTouch);
	CC_UNUSED_PARAM(pEvent);

	m_bScrolling = false;
}
//-------------------------------------------------------------------------