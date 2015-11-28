//-------------------------------------------------------------------------
#include "../Include/FKCW_UIWidget_ScrollView.h"
//-------------------------------------------------------------------------
#define CSCORLLVIEW_RELOCATE_DURATION				0.2f
#define CSCROLLVIEW_DEACCELERATE_VA					2000.0f
#define CSCROLLVIEW_DEACCELERATE_MAX				400.0f
#define CSCROLLVIEW_DEACCELERATE_INTERVAL			0.245f
#define CSCROLLVIEW_DEACCELERATE_PERCENTAGE			0.955f
#define CSCROLLVIEW_MOVE_INCH						7.0f/160.0f
#define CSCROLLVIEW_MOVE_ACTION_TAG					1
#define CSCROLLVIEW_MOVE_EASEIN_RATE				0.5f
//-------------------------------------------------------------------------
float __ScrollconvertPointToInch(float pointDis)
{
	float factor = ( CCEGLView::sharedOpenGLView()->getScaleX() + CCEGLView::sharedOpenGLView()->getScaleY() ) / 2;
	return pointDis * factor / CCDevice::getDPI();
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollViewContainer::reset()
{
	removeAllChildrenWithCleanup(true);
	ignoreAnchorPointForPosition(true);
	setAnchorPoint(CCPointZero);
	setPosition(CCPointZero);
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ScrollView::FKCW_UIWidget_ScrollView()
	: m_pContainer(NULL)
	, m_eDirection(eScrollViewDirectionBoth)
	, m_tLastMovePoint(CCPointZero)
	, m_bBounceable(true)
	, m_bDragging(false)
	, m_bDeaccelerateable(true)
	, m_fDragSpeed(0.0f)
	, m_tScrollDistance(CCPointZero)
	, m_tTouchBeganPoint(CCPointZero)
	, m_bDragable(true)
	, m_bTouchMoved(false)
	, m_tMaxOffset(CCPointZero)
	, m_tMinOffset(CCPointZero)
	, m_bDeaccelerateScrolling(false)
	, m_bAnimatedScrolling(false)
{
	m_pContainer = new FKCW_UIWidget_ScrollViewContainer();
	m_pContainer->init();
	m_pContainer->setAnchorPoint(CCPointZero);
	m_pContainer->ignoreAnchorPointForPosition(true);
	m_pContainer->setContentSize(FKCW_UIWIDGET_LAYOUT_DEFAULT_CONTENT_SIZE);
	addChild(m_pContainer);
	m_pContainer->release();
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ScrollView::~FKCW_UIWidget_ScrollView()
{

}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollView::setContainerSize(CCSize tSize)
{
	tSize.width  = MAX(tSize.width, m_obContentSize.width);
	tSize.height = MAX(tSize.height, m_obContentSize.height);

	m_pContainer->setContentSize(tSize);
	updateLimitOffset();
}
//-------------------------------------------------------------------------
const CCSize& FKCW_UIWidget_ScrollView::getContainerSize() const
{
	return m_pContainer->getContentSize();
}
//-------------------------------------------------------------------------
ENUM_ScrollViewDirection FKCW_UIWidget_ScrollView::getDirection()
{
	return m_eDirection;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollView::setDirection(ENUM_ScrollViewDirection eDirection)
{
	m_eDirection = eDirection;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ScrollViewContainer* FKCW_UIWidget_ScrollView::getContainer()
{
	return m_pContainer;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollView::setContentSize(const CCSize& contentSize)
{
	FKCW_UIWidget_Layout::setContentSize(contentSize);
	updateLimitOffset();
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_ScrollView::init()
{
	if( FKCW_UIWidget_Layout::init() )
	{
		setContentSize(FKCW_UIWIDGET_LAYOUT_DEFAULT_CONTENT_SIZE);
		setAnchorPoint(FKCW_UIWIDGET_LAYOUT_DEFAULT_ANCHOR_POINT);
		return true;
	}
	return false;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollView::onExit()
{
	stoppedAnimatedScroll();
	stoppedDeaccelerateScroll();
	FKCW_UIWidget_Layout::onExit();
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_ScrollView::initWithSize(const CCSize& tSize)
{
	if( init() )
	{
		setContentSize(tSize);
		setContainerSize(tSize);
		m_pContainer->setPosition(CCPointZero);
		updateLimitOffset();
		return true;
	}
	return false;
}
//-------------------------------------------------------------------------
ENUM_WidgetTouchModel FKCW_UIWidget_ScrollView::onTouchBegan(CCTouch *pTouch)
{
	CCPoint tNodePoint = convertToNodeSpace(pTouch->getLocation());

	if( m_pContainer->boundingBox().containsPoint(tNodePoint) )
	{
		m_pSelectedWidget = m_pContainer;
	}
	else
	{
		m_pSelectedWidget = NULL;
	}

	if( m_pSelectedWidget )
	{
		m_eSelectedWidgetTouchModel = m_pSelectedWidget->executeTouchBeganHandler(pTouch);
		if( m_eSelectedWidgetTouchModel == eWidgetTouchNone )
		{
			m_pSelectedWidget = NULL;
		}
	}

	m_tTouchBeganPoint = tNodePoint;

	if( m_eSelectedWidgetTouchModel == eWidgetTouchNone && m_bDragable )
	{
		m_tLastMovePoint = tNodePoint;
		m_bDragging = true;
		stoppedDeaccelerateScroll();
		stoppedAnimatedScroll();
	}

	return eWidgetTouchSustained;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollView::onTouchMoved(CCTouch *pTouch, float fDuration)
{
	CCPoint tNodePoint = convertToNodeSpace(pTouch->getLocation());

	if( m_pSelectedWidget )
	{
		if( m_eSelectedWidgetTouchModel == eWidgetTouchTransient && !m_bTouchMoved )
		{
			float fDistance = ccpDistance(tNodePoint, m_tTouchBeganPoint);   

			if(!m_bTouchMoved && fabs(__ScrollconvertPointToInch(fDistance)) < CSCROLLVIEW_MOVE_INCH)
			{
				m_pSelectedWidget->executeTouchMovedHandler(pTouch, fDuration);
				return;
			}

			m_bTouchMoved = true;
			m_pSelectedWidget->interruptTouch(pTouch, fDuration);
		}

		if(!m_pSelectedWidget->isTouchInterrupted())
		{
			m_pSelectedWidget->executeTouchMovedHandler(pTouch, fDuration);

			if(!m_pSelectedWidget->isTouchInterrupted())
			{
				return;
			}
		}
	}

	if( !m_bDragging )
	{
		m_tTouchBeganPoint = tNodePoint;
		m_tLastMovePoint = tNodePoint;
		m_bDragging = true;
		stoppedDeaccelerateScroll();
		stoppedAnimatedScroll();
	}

	if( m_bDragable )
	{
		m_tScrollDistance = tNodePoint - m_tLastMovePoint;
		m_tLastMovePoint = tNodePoint;

		switch( m_eDirection )
		{
		case eScrollViewDirectionHorizontal:
			m_tScrollDistance.y = 0;
			break;
		case eScrollViewDirectionVertical:
			m_tScrollDistance.x = 0;
			break;
		default:
			break;
		}

		setContentOffset( getContentOffset() + m_tScrollDistance );
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollView::onTouchEnded(CCTouch *pTouch, float fDuration)
{
	if( m_pSelectedWidget && !m_bTouchMoved && !m_pSelectedWidget->isTouchInterrupted() )
	{
		m_pSelectedWidget->executeTouchEndedHandler(pTouch, fDuration);
		m_bTouchMoved = false;
		m_bDragging = false;
		return;
	}

	if( m_bDragable )
	{
		if( m_bDeaccelerateable && m_eDirection != eScrollViewDirectionBoth && fDuration < CSCROLLVIEW_DEACCELERATE_INTERVAL )
		{
			CCPoint tEndPoint = convertToNodeSpace(pTouch->getLocation());
			switch( m_eDirection )
			{
			case eScrollViewDirectionHorizontal:
				m_fDragSpeed = fabs( m_tTouchBeganPoint.x - tEndPoint.x ) / fDuration;
				break;
			default:
				m_fDragSpeed = fabs( m_tTouchBeganPoint.y - tEndPoint.y ) / fDuration;
				break;
			}
			perpareDeaccelerateScroll();
		}
		else
		{
			CCPoint tOffset = getContentOffset();
			if( validateOffset(tOffset) )
			{
				relocateContainerWithoutCheck(tOffset);
			}
			else
			{
				onDraggingScrollEnded();
			}
		}
	}

	m_bTouchMoved = false;
	m_bDragging = false;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollView::onTouchCancelled(CCTouch *pTouch, float fDuration)
{
	if( m_pSelectedWidget && !m_pSelectedWidget->isTouchInterrupted() )
	{
		m_pSelectedWidget->executeTouchCancelledHandler(pTouch, fDuration);
	}

	m_bTouchMoved = false;
	m_bDragging = false;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollView::setDeaccelerateable(bool bDeaccelerateable)
{
	m_bDeaccelerateable = bDeaccelerateable;
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_ScrollView::isDeaccelerateable()
{
	return m_bDeaccelerateable;
}
//-------------------------------------------------------------------------
const CCPoint& FKCW_UIWidget_ScrollView::getContentOffset() const
{
	return m_pContainer->getPosition();
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollView::setBounceable(bool bBounceable)
{
	m_bBounceable = bBounceable;
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_ScrollView::isBounceable()
{
	return m_bBounceable;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollView::setDragable(bool bDragable)
{
	m_bDragable = bDragable;
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_ScrollView::isDragable()
{
	return m_bDragable;
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_ScrollView::isTouchMoved() 
{
	return m_bTouchMoved; 
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollView::stopContainerAnimation()
{
	if( m_pContainer )
	{
		m_pContainer->stopActionByTag(CSCROLLVIEW_MOVE_ACTION_TAG);
	}
}
//-------------------------------------------------------------------------
const CCPoint& FKCW_UIWidget_ScrollView::getMaxOffset() const
{
	return m_tMaxOffset;
}
//-------------------------------------------------------------------------
const CCPoint& FKCW_UIWidget_ScrollView::getMinOffset() const
{
	return m_tMinOffset;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollView::visit()
{
	glEnable(GL_SCISSOR_TEST);

	CCRect rect = CCRectMake(0, 0, m_obContentSize.width, m_obContentSize.height);
	rect = CCRectApplyAffineTransform(rect, nodeToWorldTransform());

	CCEGLView::sharedOpenGLView()->setScissorInPoints(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
	FKCW_UIWidget_Layout::visit();

	glDisable(GL_SCISSOR_TEST);
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ScrollView* FKCW_UIWidget_ScrollView::create(const CCSize& contentSize)
{
	FKCW_UIWidget_ScrollView * pRet = new FKCW_UIWidget_ScrollView();
	if( pRet && pRet->initWithSize(contentSize) )
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollView::perpareDeaccelerateScroll()
{
	if(!m_bDeaccelerateScrolling && m_bDeaccelerateable)
	{
		schedule(schedule_selector(FKCW_UIWidget_ScrollView::performedDeaccelerateScrolling));
		m_bDeaccelerateScrolling = true;
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollView::performedDeaccelerateScrolling(float dt)
{
	if( m_bDragging )
	{
		stoppedDeaccelerateScroll();
		return;
	}

	CCPoint tOldOffset;
	CCPoint tNewOffset;
	float fDistance = m_fDragSpeed * dt;
	switch( m_eDirection )
	{
	case eScrollViewDirectionHorizontal:
		fDistance  = m_tScrollDistance.x < 0 ? -fDistance : fDistance;
		tNewOffset = getContentOffset() + CCPoint(fDistance, 0);
		break;
	default:
		fDistance  = m_tScrollDistance.y < 0 ? -fDistance : fDistance;
		tNewOffset = getContentOffset() + CCPoint(0, fDistance);
		break;
	}
	tOldOffset = tNewOffset;
	bool bOutOfView = validateOffset(tNewOffset);

	if( bOutOfView && !m_bBounceable )
	{
		setContentOffsetWithoutCheck(tNewOffset);
		stoppedDeaccelerateScroll();
		return;
	}

	setContentOffsetWithoutCheck(tOldOffset);

	if( bOutOfView )
	{
		m_fDragSpeed = MIN(CSCROLLVIEW_DEACCELERATE_MAX, m_fDragSpeed);
		m_fDragSpeed = m_fDragSpeed - CSCROLLVIEW_DEACCELERATE_VA * dt;
	}
	else
	{
		m_fDragSpeed = CSCROLLVIEW_DEACCELERATE_PERCENTAGE * m_fDragSpeed - CSCROLLVIEW_DEACCELERATE_VA * dt;
	}

	if( m_fDragSpeed <= 0 )
	{
		stoppedDeaccelerateScroll();

		if( bOutOfView )
		{
			relocateContainerWithoutCheck(tNewOffset);
		}
		else
		{
			onDeaccelerateScrollEnded();
		}
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollView::stoppedDeaccelerateScroll()
{
	if( m_bDeaccelerateScrolling )
	{
		unschedule(schedule_selector(FKCW_UIWidget_ScrollView::performedDeaccelerateScrolling));
		m_bDeaccelerateScrolling = false;
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollView::perpareAnimatedScroll()
{
	if( !m_bAnimatedScrolling )
	{
		schedule(schedule_selector(FKCW_UIWidget_ScrollView::performedAnimatedScrolling));
		m_bAnimatedScrolling = true;
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollView::performedAnimatedScrolling(float dt)
{
	if( m_bDragging )
	{
		m_pContainer->stopActionByTag(CSCROLLVIEW_MOVE_ACTION_TAG);
		stoppedAnimatedScroll();
		return;
	}
	this->onScrolling();
	this->executeScrollingHandler(this);
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollView::stoppedAnimatedScroll()
{
	if( m_bAnimatedScrolling )
	{
		unschedule(schedule_selector(FKCW_UIWidget_ScrollView::performedAnimatedScrolling));
		m_bAnimatedScrolling = false;

		this->onScrolling();
		this->executeScrollingHandler(this);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollView::relocateContainerWithoutCheck(const CCPoint& tOffset)
{
	setContentOffsetEaseInWithoutCheck(tOffset, CSCORLLVIEW_RELOCATE_DURATION, CSCROLLVIEW_MOVE_EASEIN_RATE);
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollView::relocateContainer()
{
	CCPoint tOffset = getContentOffset();
	if( validateOffset(tOffset) )
	{
		setContentOffsetEaseInWithoutCheck(tOffset, CSCORLLVIEW_RELOCATE_DURATION, CSCROLLVIEW_MOVE_EASEIN_RATE);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollView::setContentOffsetToTop()
{
	if( m_eDirection == eScrollViewDirectionVertical )
	{
		CCPoint tPoint(0, -(m_pContainer->getContentSize().height - m_obContentSize.height));
		setContentOffset(tPoint);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollView::setContentOffsetToTopInDuration(float fDuration)
{
	if( m_eDirection == eScrollViewDirectionVertical )
	{
		CCPoint tPoint(0, -(m_pContainer->getContentSize().height - m_obContentSize.height));
		setContentOffsetInDuration(tPoint, fDuration);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollView::setContentOffsetToTopEaseIn(float fDuration, float fRate)
{
	if( m_eDirection == eScrollViewDirectionVertical )
	{
		CCPoint tPoint(0, -(m_pContainer->getContentSize().height - m_obContentSize.height));
		setContentOffsetEaseIn(tPoint, fDuration, fRate);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollView::setContentOffsetToBottom()
{
	if( m_eDirection == eScrollViewDirectionVertical )
	{
		setContentOffset(m_tMaxOffset);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollView::setContentOffsetToRight()
{
	if( m_eDirection == eScrollViewDirectionHorizontal )
	{
		setContentOffset(m_tMinOffset);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollView::setContentOffsetToLeft()
{
	if( m_eDirection == eScrollViewDirectionHorizontal )
	{
		setContentOffset(m_tMaxOffset);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollView::setContentOffsetWithoutCheck(const CCPoint& tOffset)
{
	m_pContainer->setPosition(tOffset);
	this->onScrolling();
	this->executeScrollingHandler(this);
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollView::setContentOffset(CCPoint tOffset)
{
	if( !m_bBounceable )
	{
		validateOffset(tOffset);
	}
	m_pContainer->stopActionByTag(CSCROLLVIEW_MOVE_ACTION_TAG);
	m_pContainer->setPosition(tOffset);
	this->onScrolling();
	this->executeScrollingHandler(this);
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollView::setContentOffsetInDurationWithoutCheck(const CCPoint& tOffset, float fDuration)
{
	m_pContainer->stopActionByTag(CSCROLLVIEW_MOVE_ACTION_TAG);
	CCSequence* pSequence = CCSequence::create(
		CCMoveTo::create(fDuration, tOffset),
		CCCallFunc::create(this, callfunc_selector(FKCW_UIWidget_ScrollView::stoppedAnimatedScroll)),
		NULL);
	pSequence->setTag(CSCROLLVIEW_MOVE_ACTION_TAG);
	m_pContainer->runAction(pSequence);

	perpareAnimatedScroll();
	this->onScrolling();
	this->executeScrollingHandler(this);
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollView::setContentOffsetInDuration(CCPoint tOffset, float fDuration)
{
	if( !m_bBounceable )
	{
		validateOffset(tOffset);
	}
	setContentOffsetInDurationWithoutCheck(tOffset, fDuration);
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollView::setContentOffsetEaseInWithoutCheck(const CCPoint& tOffset, float fDuration, float fRate)
{
	m_pContainer->stopActionByTag(CSCROLLVIEW_MOVE_ACTION_TAG);
	CCSequence* pSequence = CCSequence::create(
		CCEaseIn::create(
		CCMoveTo::create(fDuration, tOffset),
		fRate
		),
		CCCallFunc::create(this, callfunc_selector(FKCW_UIWidget_ScrollView::stoppedAnimatedScroll)), NULL
		);
	pSequence->setTag(CSCROLLVIEW_MOVE_ACTION_TAG);
	m_pContainer->runAction(pSequence);

	perpareAnimatedScroll();
	this->onScrolling();
	this->executeScrollingHandler(this);
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollView::setContentOffsetEaseIn(CCPoint tOffset, float fDuration, float fRate)
{
	if( !m_bBounceable )
	{
		validateOffset(tOffset);
	}
	setContentOffsetEaseInWithoutCheck(tOffset, fDuration, fRate);
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ScrollView::updateLimitOffset()
{
	m_tMaxOffset.x = 0;
	m_tMinOffset.x = m_obContentSize.width - m_pContainer->m_obContentSize.width;

	m_tMaxOffset.y = 0;
	m_tMinOffset.y = m_obContentSize.height - m_pContainer->m_obContentSize.height;

	if( m_eDirection == eScrollViewDirectionHorizontal )
	{
		m_tMinOffset.y = 0;
	}
	else if( m_eDirection == eScrollViewDirectionVertical )
	{
		m_tMinOffset.x = 0;
	}
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_ScrollView::validateOffset(CCPoint& tPoint)
{
	float x = tPoint.x, y = tPoint.y;
	x = MAX(x, m_tMinOffset.x);
	x = MIN(x, m_tMaxOffset.x);
	y = MAX(y, m_tMinOffset.y);
	y = MIN(y, m_tMaxOffset.y);

	if( tPoint.x != x || tPoint.y != y )
	{
		tPoint.x = x;
		tPoint.y = y;
		return true;
	}

	tPoint.x = x;
	tPoint.y = y;
	return false;
}
//-------------------------------------------------------------------------