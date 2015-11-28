//-------------------------------------------------------------------------
#include "../Include/FKCW_RenderEx_ZoomControllerLayer.h"
//-------------------------------------------------------------------------
// 返回值将在0.0f 到1.0f之间，表示一个旋转角度
static inline float vectorsDeviation(CCPoint v1, CCPoint v2) 
{
	return ccpLength(ccpSub(ccpNormalize(v1), ccpNormalize(v2)))/2.0f;
}
//-------------------------------------------------------------------------
FKCW_RenderEx_ZoomControllerLayer::FKCW_RenderEx_ZoomControllerLayer() 
{
}
//-------------------------------------------------------------------------
FKCW_RenderEx_ZoomControllerLayer::~FKCW_RenderEx_ZoomControllerLayer() 
{
	_touchesDic->release();
}
//-------------------------------------------------------------------------
FKCW_RenderEx_ZoomControllerLayer* FKCW_RenderEx_ZoomControllerLayer::controllerWithNode(CCNode* node) 
{
	FKCW_RenderEx_ZoomControllerLayer* pRet = new FKCW_RenderEx_ZoomControllerLayer();
	if (pRet && pRet->initWithNode(node)) 
	{
		pRet->autorelease();
		return pRet;
	} 
	else 
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}
//-------------------------------------------------------------------------
bool FKCW_RenderEx_ZoomControllerLayer::initWithNode(CCNode* node)
{
	if (!CCLayer::init()) {
		return false;
	}

	_touchesDic = CCDictionary::create();
	_touchesDic->retain();

	_node = node;
	_tr = ccp(0, 0);
	_bl = ccp(node->getContentSize().width, node->getContentSize().height);

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	_winTr.x = winSize.width;
	_winTr.y = winSize.height;
	_winBl.x = 0;
	_winBl.y = 0;

	centerOnPinch = true;
	zoomOnDoubleTap = true;
	zoomRate = 1/500.0f;
	zoomInLimit = 1.0f;
	zoomOutLimit = 0.5f;
	swipeVelocityMultiplier = 400;
	scrollDuration = 0.4f;
	scrollDamping = 0.4f;
	pinchDamping = 0.9f;
	pinchDistanceThreshold = 3.0f;
	doubleTapZoomDuration = 0.2f;

	return true;
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_ZoomControllerLayer::setBoundingRect(CCRect rect) 
{
	_bl = rect.origin;
	_tr = ccpAdd(_bl, ccp(rect.size.width, rect.size.height));
}
//-------------------------------------------------------------------------
CCRect FKCW_RenderEx_ZoomControllerLayer::getBoundingRect() 
{
	CCPoint size = ccpSub(_tr, _bl);
	return CCRectMake(_bl.x, _bl.y, size.x, size.y);
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_ZoomControllerLayer::setWindowRect(CCRect rect) 
{
	_winBl = rect.origin;
	_winTr = ccpAdd(_winBl, ccp(rect.size.width, rect.size.height));
}
//-------------------------------------------------------------------------
CCRect FKCW_RenderEx_ZoomControllerLayer::getWindowRect()
{
	CCPoint size = ccpSub(_winTr, _winBl);
	return CCRectMake(_winBl.x, _winBl.y, size.x, size.y);
}
//-------------------------------------------------------------------------
float FKCW_RenderEx_ZoomControllerLayer::getOptimalZoomOutLimit() 
{
	// 默认缩放比为100%
	float xMaxZoom = 1;
	float yMaxZoom = 1;

	float width = (_tr.x - _bl.x);
	float height = (_tr.y - _bl.y);

	// 不要除以0
	if (width)
		xMaxZoom = (_winTr.x - _winBl.x) / width;
	if (height)
		yMaxZoom = (_winTr.y - _winBl.y) / height;

	// 给出最好的建议缩放比
	return (xMaxZoom > yMaxZoom) ? xMaxZoom : yMaxZoom;
}
//-------------------------------------------------------------------------
CCPoint FKCW_RenderEx_ZoomControllerLayer::boundPos(CCPoint pos) 
{
	float scale = _node->getScale();

	// 纠正锚点
	CCPoint anchor = ccp(_node->getContentSize().width * _node->getAnchorPoint().x,
		_node->getContentSize().height * _node->getAnchorPoint().y);
	anchor = ccpMult(anchor, (1.0f - scale));

	// 计算边角
	CCPoint topRight = ccpAdd(ccpSub(ccpMult(_tr, scale), _winTr), anchor);
	CCPoint bottomLeft = ccpSub(ccpAdd(ccpMult(_bl, scale), _winBl), anchor);

	// 包围
	if (pos.x > bottomLeft.x)
		pos.x = bottomLeft.x;
	else if (pos.x < -topRight.x)
		pos.x = -topRight.x;
	if (pos.y > bottomLeft.y)
		pos.y = bottomLeft.y;
	else if (pos.y < -topRight.y)
		pos.y = -topRight.y;

	return pos;
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_ZoomControllerLayer::updatePosition(CCPoint pos) 
{
	pos = boundPos(pos);
	_node->setPosition(pos);
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_ZoomControllerLayer::enableWithTouchPriority(int priority, bool swallowsTouches) 
{
	setTouchEnabled(true);
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_ZoomControllerLayer::disable() 
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_ZoomControllerLayer::updateTime(float dt) 
{
	// 保存时间
	_time += dt;
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_ZoomControllerLayer::registerWithTouchDispatcher(void)
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 0);
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_ZoomControllerLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator iter = pTouches->begin();
	for (; iter != pTouches->end(); iter++)
	{
		CCTouch* pTouch = (CCTouch*)(*iter);
		_touchesDic->setObject(pTouch, CCString::createWithFormat("%d",pTouch->getID())->getCString());
		CCLog("touc id %s,",CCString::createWithFormat("%d",pTouch->getID())->getCString());
	}

	bool multitouch = _touchesDic->count() > 1;

	if (multitouch)
	{
		_timePointStampCounter = 0;

		endScroll(_firstTouch);

		CCArray* keys = _touchesDic->allKeys();
		CCTouch *touch1 = (CCTouch*)_touchesDic->objectForKey(((CCString*)keys->objectAtIndex(0))->getCString());
		CCTouch *touch2 = (CCTouch*)_touchesDic->objectForKey(((CCString*)keys->objectAtIndex(1))->getCString());

		CCPoint pt = touch1->getLocationInView();
		CCPoint pt2 = touch2->getLocationInView();

		beginZoom(pt, pt2);
	} 
	else 
	{
		CCArray* keys = _touchesDic->allKeys();
		_touchesDic->objectForKey(((CCString*)keys->objectAtIndex(0))->getCString());

		CCTouch *touch = (CCTouch*)_touchesDic->objectForKey(((CCString*)keys->objectAtIndex(0))->getCString());
		recordScrollPoint(touch);
		beginScroll(_node->convertToNodeSpace(touch->getLocation()));
	}
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_ZoomControllerLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	bool multitouch = _touchesDic->count() > 1;
	if (multitouch)
	{
		CCArray* keys = _touchesDic->allKeys();
		CCTouch *touch1 = (CCTouch*)_touchesDic->objectForKey(((CCString*)keys->objectAtIndex(0))->getCString());
		CCTouch *touch2 = (CCTouch*)_touchesDic->objectForKey(((CCString*)keys->objectAtIndex(1))->getCString());

		CCPoint pt1 = touch1->getLocationInView();
		CCPoint pt2 = touch2->getLocationInView();
		moveZoom(pt1, pt2);
	} 
	else 
	{
		CCArray* keys = _touchesDic->allKeys();
		CCTouch *touch = (CCTouch*)_touchesDic->objectForKey(((CCString*)keys->objectAtIndex(0))->getCString());
		recordScrollPoint(touch);
		moveScroll(_node->convertToNodeSpace(touch->getLocation()));
	}
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_ZoomControllerLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	bool multitouch = _touchesDic->count() > 1;
	if (multitouch) 
	{
		CCArray* keys = _touchesDic->allKeys();
		CCTouch *touch1 = (CCTouch*)_touchesDic->objectForKey(((CCString*)keys->objectAtIndex(0))->getCString());
		CCTouch *touch2 = (CCTouch*)_touchesDic->objectForKey(((CCString*)keys->objectAtIndex(1))->getCString());

		CCPoint pt1 = touch1->getLocationInView();
		CCPoint pt2 = touch2->getLocationInView();

		endZoom(pt1, pt2);
		beginScroll(_node->convertToNodeSpace(touch2->getLocation()));
	} 
	else 
	{
		CCArray* keys = _touchesDic->allKeys();
		CCTouch *touch = (CCTouch*)_touchesDic->objectForKey(((CCString*)keys->objectAtIndex(0))->getCString());
		recordScrollPoint(touch);

		CCPoint pt = _node->convertToNodeSpace(touch->getLocation());
		endScroll(pt);
	}

	CCSetIterator iter = pTouches->begin();
	for (; iter != pTouches->end(); iter++)
	{
		CCTouch* pTouch = (CCTouch*)(*iter);
		_touchesDic->removeObjectForKey(CCString::createWithFormat("%d",pTouch->getID())->getCString());
	}
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_ZoomControllerLayer::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
	ccTouchesEnded(pTouches, pEvent);
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_ZoomControllerLayer::handleDoubleTapAt(CCPoint pt) 
{
	float mid = (zoomInLimit + zoomOutLimit)/2;

	if (_node->getScale() < mid)
		zoomInOnPoint(pt, doubleTapZoomDuration);
	else
		zoomOutOnPoint(pt, doubleTapZoomDuration);
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_ZoomControllerLayer::zoomInOnPoint(CCPoint pt, float duration)
{
	zoomOnPoint(pt, duration, zoomInLimit);
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_ZoomControllerLayer::zoomOutOnPoint(CCPoint pt, float duration) 
{
	zoomOnPoint(pt, duration, zoomOutLimit);
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_ZoomControllerLayer::zoomOnPoint(CCPoint pt, float duration, float scale) 
{
	_node->runAction(FKCW_RenderEx_ZoomControllerLayerScaleAction::actionWithDuration(duration, scale, this, pt));
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_ZoomControllerLayer::recordScrollPoint(CCTouch* touch) 
{
	CCPoint pt = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
	CCPanZoomTimePointStamp* record = &_history[_timePointStampCounter++ % MaxZoomControllerLayerHistoryCount];
	record->time = _time;
	record->pt = pt;
}
//-------------------------------------------------------------------------
CCPoint FKCW_RenderEx_ZoomControllerLayer::getHistoricSpeed() 
{
	CCPoint lastPt;
	CCPoint tPt = ccp(0,0);
	CCPoint speed = ccp(0,0);
	float   lastTime;
	int     count = 0;

	for (int i = 0; i < MaxZoomControllerLayerHistoryCount && i < _timePointStampCounter; i++) 
	{
		CCPanZoomTimePointStamp *record = &_history[(_timePointStampCounter-i-1) % MaxZoomControllerLayerHistoryCount];

		CCPoint pt = record->pt;
		float time = record->time;

		if (i != 0) 
		{
			// 检查哨兵：如果我们有大量的时间块则退出
			if ((lastTime-time) > .25)
				break;

			if (i > 3 && vectorsDeviation(lastPt, pt) > .1)
				break;

			// 计算两次触摸之间的向量差。
			// 该向量的长度权重受到时间影响，这样可以避免快速迅捷的小触摸动作
			tPt = ccpAdd(tPt, ccpMult(ccpSub(lastPt, pt), (lastTime-time)));
			count++;
		}

		lastPt = pt;
		lastTime = time;
	}

	// 计算速度
	if (count)
		speed = ccpMult(tPt, 1.0f/count);

	CCLog("tPt %f,%f",tPt.x, tPt.y);
	CCLog("count %d",count);
	CCLog("speed %f,%f",speed.x, speed.y);
	return speed;
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_ZoomControllerLayer::beginScroll(CCPoint pos) 
{
	_time = 0;
	_timePointStampCounter = 0;
	_firstTouch = pos;

	schedule(schedule_selector(FKCW_RenderEx_ZoomControllerLayer::updateTime));
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_ZoomControllerLayer::moveScroll(CCPoint pos)
{
	// 值限制
	pos = ccpSub(pos, _firstTouch);
	pos = ccpMult(pos, scrollDamping * _node->getScale());

	updatePosition(ccpAdd(_node->getPosition(), pos));
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_ZoomControllerLayer::endScroll(CCPoint pos)
{
	unschedule(schedule_selector(FKCW_RenderEx_ZoomControllerLayer::updateTime));

	if (_timePointStampCounter > 3) 
	{
		// 计算速度
		CCPoint velocity = ccpMult(getHistoricSpeed(), swipeVelocityMultiplier * _node->getScale());
		// 保证我们每次移动是有效易见的（大于5像素）
		if (ccpLengthSQ(velocity) > 5*5)
		{
			// 计算重击动作位置
			CCPoint newPos = ccpAdd(_node->getPosition(), velocity);
			newPos = boundPos(newPos);

			// 创建动作
			CCMoveTo* moveTo = CCMoveTo::create(scrollDuration, newPos);
			CCEaseOut* ease = CCEaseOut::create(moveTo, 3);
			// 无条件中断动作
			_node->stopAction(_lastScrollAction);
			_node->runAction(ease);

			if (_lastScrollAction) 
			{
				_lastScrollAction->release();
				_lastScrollAction = NULL;
			}

			_lastScrollAction = ease;
			_lastScrollAction->retain();
		}
	}
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_ZoomControllerLayer::beginZoom(CCPoint pt, CCPoint pt2) 
{
	_firstLength = ccpDistance(pt, pt2);
	_oldScale = _node->getScale();

	// 计算触点中心
	_firstTouch = _node->convertToNodeSpace(ccpMidpoint(pt, pt2));
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_ZoomControllerLayer::moveZoom(CCPoint pt, CCPoint pt2)
{
	float length = ccpDistance(pt, pt2);
	float diff = (length-_firstLength);

	if (fabs(diff) < pinchDistanceThreshold)
		return;

	// 計算新縮放比
	float factor = diff * zoomRate * pinchDamping;
	float newScale = _oldScale + factor;

	if (newScale > zoomInLimit)
		newScale = zoomInLimit;
	else if (newScale < zoomOutLimit)
		newScale = zoomOutLimit;

	// 設置新縮放比
	_node->setScale(newScale);

	// 調整中心點
	if (centerOnPinch)
		centerOnPoint(_firstTouch, scrollDamping);
	else
		updatePosition(_node->getPosition());
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_ZoomControllerLayer::endZoom(CCPoint pt, CCPoint pt2) 
{
	//moveZoom(pt, pt2);
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_ZoomControllerLayer::centerOnPoint(CCPoint pt) 
{
	centerOnPoint(pt, 1.0f);
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_ZoomControllerLayer::centerOnPoint(CCPoint pt, float damping)
{
	CCPoint mid = _node->convertToNodeSpace(ccpMidpoint(_winTr, _winBl));
	CCPoint diff = ccpMult(ccpSub(mid, pt), damping);

	CCLog("_winTr %f,%f",_winTr.x,_winTr.y);
	CCLog("_winBl %f,%f",_winBl.x,_winBl.y);
	CCLog("mid %f,%f",mid.x,mid.y);
	CCLog("diff %f,%f",diff.x,diff.y);
	CCLog("_node->getPosition() %f,%f",_node->getPosition().x,_node->getPosition().y);

	updatePosition(ccpAdd(_node->getPosition(), diff));
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_ZoomControllerLayer::centerOnPoint(CCPoint pt, float duration, float rate)
{
	CCPoint mid = _node->convertToNodeSpace(ccpMidpoint(_winTr, _winBl));
	CCPoint diff = ccpSub(mid, pt);

	CCPoint final = boundPos(ccpAdd(_node->getPosition(), diff));

	CCMoveTo* moveTo = CCMoveTo::create(duration, final);
	CCEaseOut* ease = CCEaseOut::create(moveTo, rate);

	_node->runAction(ease);
}
//-------------------------------------------------------------------------
// FKCW_RenderEx_ZoomControllerLayerScaleAction
//-------------------------------------------------------------------------
FKCW_RenderEx_ZoomControllerLayerScaleAction::FKCW_RenderEx_ZoomControllerLayerScaleAction() 
{
}
//-------------------------------------------------------------------------
FKCW_RenderEx_ZoomControllerLayerScaleAction::~FKCW_RenderEx_ZoomControllerLayerScaleAction() 
{
	m_pControllerLayer->release();
}
//-------------------------------------------------------------------------
FKCW_RenderEx_ZoomControllerLayerScaleAction* FKCW_RenderEx_ZoomControllerLayerScaleAction::actionWithDuration(float duration, float s,
											FKCW_RenderEx_ZoomControllerLayer* controller, CCPoint pt) 
{
	FKCW_RenderEx_ZoomControllerLayerScaleAction* pRet = new FKCW_RenderEx_ZoomControllerLayerScaleAction();
	if (pRet && pRet->initWithDuration(duration, s, controller, pt)) 
	{
		pRet->autorelease();
		return pRet;
	} 
	else 
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}
//-------------------------------------------------------------------------
bool FKCW_RenderEx_ZoomControllerLayerScaleAction::initWithDuration(float duration, float s, FKCW_RenderEx_ZoomControllerLayer* controller, CCPoint pt) 
{
	if (!CCScaleTo::initWithDuration(duration, s)) 
	{
		return false;
	}

	this->m_pControllerLayer = controller;
	this->m_pControllerLayer->retain();
	m_tagCenterPoint = pt;

	return true;
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_ZoomControllerLayerScaleAction::update(float t) 
{
	CCScaleTo::update(t);

	// 使用阻尼，逐渐到达目的点
	if (t < 1.0f)
		m_pControllerLayer->centerOnPoint(m_tagCenterPoint, m_pControllerLayer->scrollDamping);
	else
		m_pControllerLayer->centerOnPoint(m_tagCenterPoint);
}
//-------------------------------------------------------------------------