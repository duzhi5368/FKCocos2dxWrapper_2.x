//-------------------------------------------------------------------------
#include "../Include/FKCW_UI_PanZoomLayer.h"
#include "../../FKCW_Base/Include/FKCW_Base_Macro.h"
#include "../../FKCW_Base/Include/FKCW_Base_Utils.h"
//-------------------------------------------------------------------------
bool FKCW_UI_LayerPanZoom::init()
{
	if( !CCLayer::init() )
		return false;

	m_fMaxScale = 3.0f;
	m_fMinScale	= 0.7f;
	m_pTouches = CCArray::createWithCapacity( 10 );
	m_pTouches->retain();
	m_tagPanBoundsRect = CCRectZero;
	m_fTouchDistance = 0.0f;
	m_fMaxTouchDistanceToClick = 315.0f;
	m_eMode = eLayerPanZoomModeSheet;
	m_fMinSpeed = 100.0f;
	m_fMaxSpeed = 100.0f;
	m_fTopFrameMargin = 100.0f;
	m_fBottomFrameMargin = 100.0f;
	m_fLeftFrameMargin = 100.0f;
	m_fRightFrameMargin = 100.0f;
	m_fRubberEffectRatio = 0.0f;
	m_fRubberEffectRecoveryTime = 0.2f;
	m_bRubberEffectRecovering = false;
	m_bRubberEffectZooming = false;
	m_dSingleTouchTimestamp = 0;
	m_bTouchMoveBegan = false;
	 
	return true;
}
//-------------------------------------------------------------------------
void FKCW_UI_LayerPanZoom::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
	CCTouch *pTouch;
	CCSetIterator setIter;
	for (setIter = pTouches->begin(); setIter != pTouches->end(); ++setIter)
	{
		pTouch = (CCTouch *)(*setIter);
		m_pTouches->addObject(pTouch);
	}


	if (m_pTouches->count() == 1)
	{
		m_bTouchMoveBegan = false;
		time_t seconds;

		seconds = time (NULL);
		m_dSingleTouchTimestamp = seconds/60.0;
	}
	else
		m_dSingleTouchTimestamp = MAX_FLOAT;
}
//-------------------------------------------------------------------------
void FKCW_UI_LayerPanZoom::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
	bool multitouch = m_pTouches->count() > 1;
	if (multitouch)
	{
		// 获得前面俩触电
		CCTouch *touch1 = (CCTouch*)m_pTouches->objectAtIndex(0);
		CCTouch *touch2 = (CCTouch*)m_pTouches->objectAtIndex(1);
		// 获取两个点当前和上一次位置
		CCPoint curPosTouch1 = CCDirector::sharedDirector()->convertToGL(touch1->getLocationInView());
		CCPoint curPosTouch2 = CCDirector::sharedDirector()->convertToGL(touch2->getLocationInView());

		CCPoint prevPosTouch1 = CCDirector::sharedDirector()->convertToGL(touch1->getPreviousLocationInView());
		CCPoint prevPosTouch2 = CCDirector::sharedDirector()->convertToGL(touch2->getPreviousLocationInView());


		// 计算当前点和上一个点相对本层的相对位置
		CCPoint curPosLayer = ccpMidpoint(curPosTouch1, curPosTouch2);
		CCPoint prevPosLayer = ccpMidpoint(prevPosTouch1, prevPosTouch2);

		// 计算新的缩放率
		float prevScale = this->getScale();
		float curScale = this->getScale() * ccpDistance(curPosTouch1, curPosTouch2) / ccpDistance(prevPosTouch1, prevPosTouch2);
		this->setScale( curScale);

		// 如果没有开启橡胶效果，避免 panBoundsRect 扩展
		if (!m_fRubberEffectRatio)
		{
			this->setScale( MAX(this->getScale(), this->MinPossibleScale())); 
		}

		// 如果缩放被修改了，那么设置新的缩放率，并且调整新的位置坐标以适应新的缩放比
		if (this->getScale() != prevScale)
		{
			if (m_fRubberEffectRatio)
			{
				m_bRubberEffectZooming = true;
			}
			CCPoint realCurPosLayer = this->convertToNodeSpace(curPosLayer);
			float deltaX = (realCurPosLayer.x - this->getAnchorPoint().x * this->getContentSize().width) * (this->getScale() - prevScale);
			float deltaY = (realCurPosLayer.y - this->getAnchorPoint().y * this->getContentSize().height) * (this->getScale() - prevScale);
			this->setPosition(ccp(this->getPosition().x - deltaX, this->getPosition().y - deltaY));
			m_bRubberEffectZooming = false;
		}

		// 若多点触控的当前中心点和之前中心点位置不同，则需要调整Layer位置
		if (! prevPosLayer.equals(curPosLayer) )
		{            
			this->setPosition(ccp(this->getPosition().x + curPosLayer.x - prevPosLayer.x,
				this->getPosition().y + curPosLayer.y - prevPosLayer.y));
		}
		// 多点触碰不处理触碰距离的问题
		m_fTouchDistance = MAX_FLOAT;
	}
	else  // 单点触碰
	{	        
		// 获取单点触碰的前一位置和当前位置
		CCTouch *touch = (CCTouch*)m_pTouches->objectAtIndex(0);
		CCPoint curTouchPosition = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
		CCPoint prevTouchPosition = CCDirector::sharedDirector()->convertToGL(touch->getPreviousLocationInView());

		// 表单模式则始终滚动
		if ( m_eMode == eLayerPanZoomModeSheet )
		{
			// 设置Layer新位置
			this->setPosition(ccp(this->getPosition().x + curTouchPosition.x - prevTouchPosition.x,
				this->getPosition().y + curTouchPosition.y - prevTouchPosition.y));
		}

		// 叠加累计滑动距离
		m_fTouchDistance += ccpDistance(curTouchPosition, prevTouchPosition);

		// 启动新一次触摸
		if ( m_eMode == eLayerPanZoomModeFrame )
		{
			if ( m_fTouchDistance > m_fMaxTouchDistanceToClick && !m_bTouchMoveBegan)
			{
				// TODO：这里可以添加回调通知
				m_bTouchMoveBegan = true;
			}
		}
	}	
}
//-------------------------------------------------------------------------
void FKCW_UI_LayerPanZoom::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
	 m_dSingleTouchTimestamp = MAX_FLOAT;
    

	// 处理单触点的点击事件
    if (  (m_fTouchDistance < m_fMaxTouchDistanceToClick)
        && (m_pTouches->count() == 1))
    {
        CCTouch *touch = (CCTouch*)m_pTouches->objectAtIndex(0);       
        CCPoint curPos = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
		
		// TODO：回调通知
    }
    
    CCTouch *pTouch;
    CCSetIterator setIter;
    for (setIter = pTouches->begin(); setIter != pTouches->end(); ++setIter)
    {
        pTouch = (CCTouch *)(*setIter);
        m_pTouches->removeObject(pTouch);
    }
    
	if (m_pTouches->count() == 0)
	{
		m_fTouchDistance = 0.0f;
	}
    
    if (!m_pTouches->count() && !m_bRubberEffectRecovering)
    {
        this->recoverPositionAndScale();
    }
}
//-------------------------------------------------------------------------
void FKCW_UI_LayerPanZoom::ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
	CCTouch *pTouch;
	CCSetIterator setIter;
	for (setIter = pTouches->begin(); setIter != pTouches->end(); ++setIter)
	{
		pTouch = (CCTouch *)(*setIter);
		m_pTouches->removeObject(pTouch);
	}

	if (m_pTouches->count() == 0)
	{
		m_fTouchDistance = 0.0f;
	}
}
//-------------------------------------------------------------------------
void FKCW_UI_LayerPanZoom::update(float dt)
{
	// 仅仅Frame模式，单触碰时有该update需求
	if ( m_eMode == eLayerPanZoomModeFrame && m_pTouches->count() == 1 )
	{
		// 如果点击依然有效，则不更新位置
		if (m_fTouchDistance <= m_fMaxTouchDistanceToClick)
			return;

		// 如果拿捏手势依然有效，则不更新位置
		time_t seconds;

		seconds = time (NULL);
		seconds /= 60;
		if (seconds - m_dSingleTouchTimestamp < kCCLayerPanZoomMultitouchGesturesDetectionDelay)
			return;

		// 否则更新位置。获取当前触摸位置。
		CCTouch *touch = (CCTouch*)m_pTouches->objectAtIndex(0);
		CCPoint curPos = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());

		// 如果手指靠近了边缘滚动区域，则进行滚动
		if (this->FrameEdgeWithPoint(curPos) != eLayerPanZoomFrameEdgeNone)
		{
			this->setPosition(ccp(this->getPosition().x + dt * this->HorSpeedWithPosition( curPos), 
				this->getPosition().y + dt * this->VertSpeedWithPosition(curPos)));
		}

		// 如果触摸位置相对层的位置修改了
		CCPoint touchPositionInLayer = this->convertToNodeSpace(curPos);
		if (!m_tagPrevSingleTouchPositionInLayer.equals(touchPositionInLayer))
		{
			m_tagPrevSingleTouchPositionInLayer = touchPositionInLayer;
			// TODO: 回调函数
		}
	}
}
//-------------------------------------------------------------------------
void FKCW_UI_LayerPanZoom::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}
//-------------------------------------------------------------------------
void FKCW_UI_LayerPanZoom::onExit()
{
	CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
	CCLayer::onExit();
}
//-------------------------------------------------------------------------
void FKCW_UI_LayerPanZoom::setMaxScale(float maxScale)
{
	m_fMaxScale = maxScale;
	this->setScale( MIN(getScale(), m_fMaxScale) );
}
//-------------------------------------------------------------------------
float FKCW_UI_LayerPanZoom::getMaxScale()
{
	return m_fMaxScale;
}
//-------------------------------------------------------------------------
void FKCW_UI_LayerPanZoom::setMinScale(float minScale)
{
	m_fMinScale = minScale;
	this->setScale( MAX(getScale(), m_fMinScale) );
}
//-------------------------------------------------------------------------
float FKCW_UI_LayerPanZoom::getMinScale()
{
	return m_fMinScale;
}
//-------------------------------------------------------------------------
void FKCW_UI_LayerPanZoom::setRubberEffectRatio(float rubberEffectRatio)
{
	m_fRubberEffectRatio	= rubberEffectRatio;
	if( m_eMode == eLayerPanZoomModeFrame )
	{
		// 该模式不支持ratio
		m_fRubberEffectRatio = 0.0f;
	}
}
//-------------------------------------------------------------------------
float FKCW_UI_LayerPanZoom::getRubberEffectRatio()
{
	return m_fRubberEffectRatio;
}
//-------------------------------------------------------------------------
void FKCW_UI_LayerPanZoom::setPanBoundsRect(CCRect m_tagRect)
{
	m_tagPanBoundsRect = m_tagRect;
	this->setScale(this->MinPossibleScale());
	this->setPosition(this->getPosition());
}
//-------------------------------------------------------------------------
void FKCW_UI_LayerPanZoom::setPosition(CCPoint m_tagPosition)
{
	CCPoint prevPosition = this->getPosition();
	CCNode::setPosition(m_tagPosition);

	if ( !m_tagPanBoundsRect.equals(CCRectZero) && !m_bRubberEffectZooming)
	{
		if ( m_fRubberEffectRatio && m_eMode == eLayerPanZoomModeSheet )
		{
			if (!m_bRubberEffectRecovering)
			{
				float topDistance = this->TopEdgeDistance();
				float bottomDistance = this->BottomEdgeDistance();
				float leftDistance = this->LeftEdgeDistance();
				float rightDistance = this->RightEdgeDistance();
				float dx = this->getPosition().x - prevPosition.x;
				float dy = this->getPosition().y - prevPosition.y;
				if (bottomDistance || topDistance)
				{
					CCNode::setPosition(ccp(this->getPosition().x, 
						prevPosition.y + dy * m_fRubberEffectRatio));                    
				}
				if (leftDistance || rightDistance)
				{
					CCNode::setPosition(ccp(prevPosition.x + dx * m_fRubberEffectRatio, 
						this->getPosition().y));                    
				}
			}
		}
		else
		{
			CCRect boundBox = this->boundingBox();
			if (this->getPosition().x - boundBox.size.width * this->getAnchorPoint().x > m_tagPanBoundsRect.origin.x)
			{
				CCNode::setPosition(ccp(boundBox.size.width * this->getAnchorPoint().x + m_tagPanBoundsRect.origin.x, 
					this->getPosition().y));
			}	
			if (this->getPosition().y - boundBox.size.height * this->getAnchorPoint().y > m_tagPanBoundsRect.origin.y)
			{
				CCNode::setPosition(ccp(this->getPosition().x, boundBox.size.height * this->getAnchorPoint().y + 
					m_tagPanBoundsRect.origin.y));
			}
			if (this->getPosition().x + boundBox.size.width * (1 - this->getAnchorPoint().x) < m_tagPanBoundsRect.size.width +
				m_tagPanBoundsRect.origin.x)
			{
				CCNode::setPosition(ccp(m_tagPanBoundsRect.size.width + m_tagPanBoundsRect.origin.x - 
					boundBox.size.width * (1 - this->getAnchorPoint().x), this->getPosition().y));
			}
			if (this->getPosition().y + boundBox.size.height * (1 - this->getAnchorPoint().y) < m_tagPanBoundsRect.size.height + 
				m_tagPanBoundsRect.origin.y)
			{
				CCNode::setPosition(ccp(this->getPosition().x, m_tagPanBoundsRect.size.height + m_tagPanBoundsRect.origin.y - 
					boundBox.size.height * (1 - this->getAnchorPoint().y)));
			}	
		}
	}
}
//-------------------------------------------------------------------------
void FKCW_UI_LayerPanZoom::setScale(float m_fScale)
{
	 CCLayer::setScale( MIN(MAX(m_fScale, m_fMinScale), m_fMaxScale));
}
//-------------------------------------------------------------------------
void FKCW_UI_LayerPanZoom::recoverPositionAndScale()
{
	if (!m_tagPanBoundsRect.equals(CCRectZero))
	{    
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		float rightEdgeDistance = this->RightEdgeDistance();
		float leftEdgeDistance = this->LeftEdgeDistance();
		float topEdgeDistance = this->TopEdgeDistance();
		float bottomEdgeDistance = this->BottomEdgeDistance();
		float scale = this->MinPossibleScale();

		if (!rightEdgeDistance && !leftEdgeDistance && !topEdgeDistance && !bottomEdgeDistance)
		{
			return;
		}

		if (this->getScale() < scale)
		{
			m_bRubberEffectRecovering = true;
			CCPoint newPosition = CCPointZero;
			if (rightEdgeDistance && leftEdgeDistance && topEdgeDistance && bottomEdgeDistance)
			{
				float dx = scale * this->getContentSize().width * (this->getAnchorPoint().x - 0.5f);
				float dy = scale * this->getContentSize().height * (this->getAnchorPoint().y - 0.5f);
				newPosition = ccp(winSize.width * 0.5f + dx, winSize.height * 0.5f + dy);
			}
			else if (rightEdgeDistance && leftEdgeDistance && topEdgeDistance)
			{
				float dx = scale * this->getContentSize().width * (this->getAnchorPoint().x - 0.5f);
				float dy = scale * this->getContentSize().height * (1.0f - this->getAnchorPoint().y);            
				newPosition = ccp(winSize.width * 0.5f + dx, winSize.height - dy);
			}
			else if (rightEdgeDistance && leftEdgeDistance && bottomEdgeDistance)
			{
				float dx = scale * this->getContentSize().width * (this->getAnchorPoint().x - 0.5f);
				float dy = scale * this->getContentSize().height * this->getAnchorPoint().y;            
				newPosition = ccp(winSize.width * 0.5f + dx, dy);
			}
			else if (rightEdgeDistance && topEdgeDistance && bottomEdgeDistance)
			{
				float dx = scale * this->getContentSize().width * (1.0f - this->getAnchorPoint().x);
				float dy = scale * this->getContentSize().height * (this->getAnchorPoint().y - 0.5f);            
				newPosition = ccp(winSize.width  - dx, winSize.height  * 0.5f + dy);
			}
			else if (leftEdgeDistance && topEdgeDistance && bottomEdgeDistance)
			{
				float dx = scale * this->getContentSize().width * this->getAnchorPoint().x;
				float dy = scale * this->getContentSize().height * (this->getAnchorPoint().y - 0.5f);            
				newPosition = ccp(dx, winSize.height * 0.5f + dy);
			}
			else if (leftEdgeDistance && topEdgeDistance)
			{
				float dx = scale * this->getContentSize().width * this->getAnchorPoint().x;
				float dy = scale * this->getContentSize().height * (1.0f - this->getAnchorPoint().y);            
				newPosition = ccp(dx, winSize.height - dy);
			} 
			else if (leftEdgeDistance && bottomEdgeDistance)
			{
				float dx = scale * this->getContentSize().width * this->getAnchorPoint().x;
				float dy = scale * this->getContentSize().height * this->getAnchorPoint().y;            
				newPosition = ccp(dx, dy);
			} 
			else if (rightEdgeDistance && topEdgeDistance)
			{
				float dx = scale * this->getContentSize().width * (1.0f - this->getAnchorPoint().x);
				float dy = scale * this->getContentSize().height * (1.0f - this->getAnchorPoint().y);            
				newPosition = ccp(winSize.width - dx, winSize.height - dy);
			} 
			else if (rightEdgeDistance && bottomEdgeDistance)
			{
				float dx = scale * this->getContentSize().width * (1.0f - this->getAnchorPoint().x);
				float dy = scale * this->getContentSize().height * this->getAnchorPoint().y;            
				newPosition = ccp(winSize.width - dx, dy);
			} 
			else if (topEdgeDistance || bottomEdgeDistance)
			{
				float dy = scale * this->getContentSize().height * (this->getAnchorPoint().y - 0.5f);            
				newPosition = ccp(this->getPosition().x, winSize.height * 0.5f + dy);
			}
			else if (leftEdgeDistance || rightEdgeDistance)
			{
				float dx = scale * this->getContentSize().width * (this->getAnchorPoint().x - 0.5f);
				newPosition = ccp(winSize.width * 0.5f + dx, this->getPosition().y);
			} 

			CCMoveTo *moveToPosition = CCMoveTo::create( m_fRubberEffectRecoveryTime,newPosition);
			CCScaleTo *scaleToPosition = CCScaleTo::create( m_fRubberEffectRecoveryTime,scale);
			CCFiniteTimeAction *sequence = CCSpawn::create(scaleToPosition, moveToPosition, CCCallFunc::create( this, callfunc_selector(FKCW_UI_LayerPanZoom::recoverEnded)), NULL);
			this->runAction(sequence);

		}
		else
		{
			m_bRubberEffectRecovering = false;
			CCMoveTo *moveToPosition = CCMoveTo::create(m_fRubberEffectRecoveryTime,ccp(this->getPosition().x + rightEdgeDistance - leftEdgeDistance, 
				this->getPosition().y + topEdgeDistance - bottomEdgeDistance));
			CCFiniteTimeAction *sequence = CCSpawn::create(moveToPosition, CCCallFunc::create( this, callfunc_selector(FKCW_UI_LayerPanZoom::recoverEnded)), NULL);
			this->runAction(sequence);

		}
	}
}
//-------------------------------------------------------------------------
void FKCW_UI_LayerPanZoom::recoverEnded()
{
	m_bRubberEffectRecovering = false;
}
//-------------------------------------------------------------------------
float FKCW_UI_LayerPanZoom::TopEdgeDistance()
{
	CCRect boundBox = this->boundingBox();
	return FKCW_Base_Utils::Round(MAX(m_tagPanBoundsRect.size.height + m_tagPanBoundsRect.origin.y - this->getPosition().y - 
		boundBox.size.height * (1 - this->getAnchorPoint().y), 0));
}
//-------------------------------------------------------------------------
float FKCW_UI_LayerPanZoom::LeftEdgeDistance()
{
	CCRect boundBox = this->boundingBox();
	return FKCW_Base_Utils::Round(MAX(this->getPosition().x - boundBox.size.width * this->getAnchorPoint().x
		- m_tagPanBoundsRect.origin.x, 0));
}
//-------------------------------------------------------------------------
float FKCW_UI_LayerPanZoom::BottomEdgeDistance()
{
	CCRect boundBox = this->boundingBox();
	return FKCW_Base_Utils::Round(MAX(this->getPosition().y - boundBox.size.height * this->getAnchorPoint().y 
		- m_tagPanBoundsRect.origin.y, 0));
}
//-------------------------------------------------------------------------
float FKCW_UI_LayerPanZoom::RightEdgeDistance()
{
	CCRect boundBox = this->boundingBox();
	return FKCW_Base_Utils::Round(MAX(m_tagPanBoundsRect.size.width + m_tagPanBoundsRect.origin.x - this->getPosition().x - 
		boundBox.size.width * (1 - this->getAnchorPoint().x), 0));
}
//-------------------------------------------------------------------------
float FKCW_UI_LayerPanZoom::MinPossibleScale()
{
	if (! m_tagPanBoundsRect.equals(CCRectZero))
	{
		return MAX(m_tagPanBoundsRect.size.width / this->getContentSize().width,
			m_tagPanBoundsRect.size.height /this->getContentSize().height);
	}
	else 
	{
		return m_fMinScale;
	}
}
//-------------------------------------------------------------------------
ENUM_LayerPanZoomFrameEdge FKCW_UI_LayerPanZoom::FrameEdgeWithPoint( cocos2d::CCPoint point)
{
	bool isLeft		= point.x <= m_tagPanBoundsRect.origin.x + m_fLeftFrameMargin;
	bool isRight	= point.x >= m_tagPanBoundsRect.origin.x + m_tagPanBoundsRect.size.width - m_fRightFrameMargin;
	bool isBottom	= point.y <= m_tagPanBoundsRect.origin.y + m_fBottomFrameMargin;
	bool isTop		= point.y >= m_tagPanBoundsRect.origin.y + m_tagPanBoundsRect.size.height - m_fTopFrameMargin;

	if (isLeft && isBottom)
	{
		return eLayerPanZoomFrameEdgeBottomLeft;
	}
	if (isLeft && isTop)
	{
		return eLayerPanZoomFrameEdgeTopLeft;
	}
	if (isRight && isBottom)
	{
		return eLayerPanZoomFrameEdgeBottomRight;
	}
	if (isRight && isTop)
	{
		return eLayerPanZoomFrameEdgeTopRight;
	}

	if (isLeft)
	{
		return eLayerPanZoomFrameEdgeLeft;
	}
	if (isTop)
	{
		return eLayerPanZoomFrameEdgeTop;
	}
	if (isRight)
	{
		return eLayerPanZoomFrameEdgeRight;
	}
	if (isBottom)
	{
		return eLayerPanZoomFrameEdgeBottom;
	}

	return eLayerPanZoomFrameEdgeNone;
}
//-------------------------------------------------------------------------
float FKCW_UI_LayerPanZoom::HorSpeedWithPosition(CCPoint pos)
{
	ENUM_LayerPanZoomFrameEdge edge = this->FrameEdgeWithPoint(pos);
	float speed = 0.0f;
	if (edge == eLayerPanZoomFrameEdgeLeft)
	{
		speed = m_fMinSpeed + (m_fMaxSpeed - m_fMinSpeed) * 
			(m_tagPanBoundsRect.origin.x + m_fLeftFrameMargin - pos.x) / m_fLeftFrameMargin;
	}
	if (edge == eLayerPanZoomFrameEdgeBottomLeft || edge == eLayerPanZoomFrameEdgeTopLeft)
	{
		speed = m_fMinSpeed + (m_fMaxSpeed - m_fMinSpeed) * 
			(m_tagPanBoundsRect.origin.x + m_fLeftFrameMargin - pos.x) / (m_fLeftFrameMargin * sqrt(2.0f));
	}
	if (edge == eLayerPanZoomFrameEdgeRight)
	{
		speed = - (m_fMinSpeed + (m_fMaxSpeed - m_fMinSpeed) * 
			(pos.x - m_tagPanBoundsRect.origin.x - m_tagPanBoundsRect.size.width + 
			m_fRightFrameMargin) / m_fRightFrameMargin);
	}
	if (edge == eLayerPanZoomFrameEdgeBottomRight || edge == eLayerPanZoomFrameEdgeTopRight)
	{
		speed = - (m_fMinSpeed + (m_fMaxSpeed - m_fMinSpeed) * 
			(pos.x - m_tagPanBoundsRect.origin.x - m_tagPanBoundsRect.size.width + 
			m_fRightFrameMargin) / (m_fRightFrameMargin * sqrt(2.0f)));
	}
	return speed;
}
//-------------------------------------------------------------------------
float FKCW_UI_LayerPanZoom::VertSpeedWithPosition(CCPoint pos)
{
	ENUM_LayerPanZoomFrameEdge edge = this->FrameEdgeWithPoint(pos);
	float speed = 0.0f;
	if (edge == eLayerPanZoomFrameEdgeBottom)
	{
		speed = m_fMinSpeed + (m_fMaxSpeed - m_fMinSpeed) * 
			(m_tagPanBoundsRect.origin.y + m_fBottomFrameMargin - pos.y) / m_fBottomFrameMargin;
	}
	if (edge == eLayerPanZoomFrameEdgeBottomLeft || edge == eLayerPanZoomFrameEdgeBottomRight)
	{
		speed = m_fMinSpeed + (m_fMaxSpeed - m_fMinSpeed) * 
			(m_tagPanBoundsRect.origin.y + m_fBottomFrameMargin - pos.y) / (m_fBottomFrameMargin * sqrt(2.0f));
	}
	if (edge == eLayerPanZoomFrameEdgeTop)
	{
		speed = - (m_fMinSpeed + (m_fMaxSpeed - m_fMinSpeed) * 
			(pos.y - m_tagPanBoundsRect.origin.y - m_tagPanBoundsRect.size.height + 
			m_fTopFrameMargin) / m_fTopFrameMargin);
	}
	if (edge == eLayerPanZoomFrameEdgeTopLeft || edge == eLayerPanZoomFrameEdgeTopRight)
	{
		speed = - (m_fMinSpeed + (m_fMaxSpeed - m_fMinSpeed) * 
			(pos.y - m_tagPanBoundsRect.origin.y - m_tagPanBoundsRect.size.height + 
			m_fTopFrameMargin) / (m_fTopFrameMargin * sqrt(2.0f)));
	}
	return speed;
}
//-------------------------------------------------------------------------