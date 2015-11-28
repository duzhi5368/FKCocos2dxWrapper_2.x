//-------------------------------------------------------------------------
#include "../Include/FKCW_UIWidget_ControlView.h"
//-------------------------------------------------------------------------
#define MOVE_DELAY 0.2f
#define PARAM_PRE 10
//-------------------------------------------------------------------------
FKCW_UIWidget_ControlView::FKCW_UIWidget_ControlView()
	: m_pBaseBoard(NULL)
	, m_pJoystick(NULL)
	, m_fRadius(100.0f)
	, m_tCenterPoint(CCPointZero)
	, m_bRelocateWithAnimation(true)
	, m_bAnimationUpdate(false)
	, m_bExecuteEventUpdate(false)
	, m_tLastPoint(CCPointZero)
{
	setThisObject(this);
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ControlView::~FKCW_UIWidget_ControlView()
{

}
//-------------------------------------------------------------------------
FKCW_UIWidget_ControlView* FKCW_UIWidget_ControlView::create()
{
	FKCW_UIWidget_ControlView* pRet = new FKCW_UIWidget_ControlView();
	if( pRet && pRet->init() )
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ControlView* FKCW_UIWidget_ControlView::create(const char* pBaseBoard, const char* pJoystick)
{
	FKCW_UIWidget_ControlView* pRet = new FKCW_UIWidget_ControlView();
	if( pRet && pRet->initWithFile(pBaseBoard, pJoystick) )
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ControlView::setRadius(float fRadius)
{
	m_fRadius = fRadius;
}
//-------------------------------------------------------------------------
float FKCW_UIWidget_ControlView::getRadius() const
{
	return m_fRadius;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ControlView::setRelocateWithAnimation(bool bAni)
{
	m_bRelocateWithAnimation = bAni;
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_ControlView::isRelocateWithAnimation() const
{
	return m_bRelocateWithAnimation;
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_ControlView::init()
{
	setCascadeColorEnabled(true);
	setCascadeOpacityEnabled(true);

	setAnchorPoint(FKCW_UIWIDGET_BASIC_DEFAULT_ANCHOR_POINT);
	setContentSize(FKCW_UIWIDGET_BASIC_DEFAULT_CONTENT_SIZE);

	return true;
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_ControlView::initWithFile(const char* pBaseBoard, const char* pJoystick)
{
	if( init() )
	{
		if( pBaseBoard && pJoystick && strlen(pBaseBoard) && strlen(pJoystick) )
		{
			setBaseBoardImage(pBaseBoard);
			setJoystickImage(pJoystick);
		}
		return true;
	}
	return false;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ControlView::relocateJoystick(bool bAni)
{
	if( bAni )
	{
		CCSequence* pSequence = CCSequence::create(
			CCMoveTo::create(MOVE_DELAY, m_tCenterPoint),
			CCCallFunc::create(this, callfunc_selector(FKCW_UIWidget_ControlView::stopAnimateUpdate)),
			NULL);
		m_pJoystick->runAction(pSequence);
		performAnimateUpdate();
	}
	else
	{
		m_pJoystick->setPosition(m_tCenterPoint);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ControlView::onAnimateUpdate(float dt)
{

}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ControlView::performAnimateUpdate()
{
	if( !m_bAnimationUpdate )
	{
		m_bAnimationUpdate = true;
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ControlView::stopAnimateUpdate()
{
	if( m_bAnimationUpdate )
	{
		if( m_pJoystick )
		{
			m_pJoystick->stopAllActions();
		}
		m_bAnimationUpdate = false;
		stopExecuteUpdate();
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ControlView::onExecuteEventUpdate(float dt)
{
	if( m_pJoystick )
	{
		CCPoint tOffset = m_pJoystick->getPosition() - m_tCenterPoint;
		executeControlHandler(this, tOffset.x / PARAM_PRE, tOffset.y / PARAM_PRE);
	}
	else
	{
		CCPoint tOffset = m_tLastPoint - m_tCenterPoint;
		executeControlHandler(this, tOffset.x / PARAM_PRE, tOffset.y / PARAM_PRE);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ControlView::performExecuteUpdate()
{
	if( !m_bExecuteEventUpdate )
	{
		schedule(schedule_selector(FKCW_UIWidget_ControlView::onExecuteEventUpdate));
		m_bExecuteEventUpdate = true;
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ControlView::stopExecuteUpdate()
{
	if( m_bExecuteEventUpdate )
	{
		unschedule(schedule_selector(FKCW_UIWidget_ControlView::onExecuteEventUpdate));
		m_bExecuteEventUpdate = false;
	}
}
//-------------------------------------------------------------------------
ENUM_WidgetTouchModel FKCW_UIWidget_ControlView::onTouchBegan(CCTouch *pTouch)
{
	stopAnimateUpdate();
	CCPoint tPoint = convertToNodeSpace(pTouch->getLocation());
	if( m_pJoystick )
	{
		if( m_pJoystick->boundingBox().containsPoint(tPoint) )
		{
			performExecuteUpdate();
			return eWidgetTouchSustained;
		}
	}
	else
	{
		m_tLastPoint = tPoint;
		performExecuteUpdate();
		return eWidgetTouchSustained;
	}
	return eWidgetTouchNone;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ControlView::onTouchMoved(CCTouch *pTouch, float fDuration)
{
	if( m_pJoystick )
	{
		CCPoint tA = convertToNodeSpace(pTouch->getLocation());
		CCPoint tN = m_tCenterPoint-tA;
		float fD = sqrtf(tN.x*tN.x+tN.y*tN.y);
		fD < m_fRadius ? m_pJoystick->setPosition(tA) : m_pJoystick->setPosition(CCPoint(
			((tA.x - m_tCenterPoint.x) / fD) * m_fRadius + m_tCenterPoint.x,
			((tA.y - m_tCenterPoint.y) / fD) * m_fRadius + m_tCenterPoint.y
			)
			);
	}
	else
	{
		CCPoint tA = convertToNodeSpace(pTouch->getLocation());
		CCPoint tN = m_tCenterPoint-tA;
		float fD = sqrtf(tN.x*tN.x+tN.y*tN.y);
		fD < m_fRadius ? m_tLastPoint = tA : m_tLastPoint = (CCPoint(
			((tA.x - m_tCenterPoint.x) / fD) * m_fRadius + m_tCenterPoint.x,
			((tA.y - m_tCenterPoint.y) / fD) * m_fRadius + m_tCenterPoint.y
			)
			);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ControlView::onTouchEnded(CCTouch *pTouch, float fDuration)
{
	if( m_pJoystick )
	{
		if( !m_bRelocateWithAnimation )
		{
			stopExecuteUpdate();
		}
		relocateJoystick(m_bRelocateWithAnimation);
	}
	else
	{
		stopExecuteUpdate();
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ControlView::onTouchCancelled(CCTouch *pTouch, float fDuration)
{
	onTouchEnded(pTouch, fDuration);
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ControlView::setContentSize(const CCSize& tContentSize)
{
	CCNodeRGBA::setContentSize(tContentSize);

	m_tCenterPoint = CCPoint(m_obContentSize.width / 2, m_obContentSize.height / 2);

	if( m_pBaseBoard )
	{
		m_pBaseBoard->setPosition(m_tCenterPoint);
	}

	if( m_pJoystick )
	{
		m_pJoystick->setPosition(m_tCenterPoint);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ControlView::setBaseBoardImage(const char* pFile)
{
	if( pFile && strlen(pFile) )
	{
		CCTexture2D* pTexture = CCTextureCache::sharedTextureCache()->addImage(pFile);
		setBaseBoardTexture(pTexture);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ControlView::setJoystickImage(const char* pFile)
{
	if( pFile && strlen(pFile) )
	{
		CCTexture2D* pTexture = CCTextureCache::sharedTextureCache()->addImage(pFile);
		setJoystickTexture(pTexture);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ControlView::setBaseBoardSpriteFrame(CCSpriteFrame* pFrame)
{
	if( pFrame )
	{
		if( m_pBaseBoard )
		{
			m_pBaseBoard->setDisplayFrame(pFrame);
		}
		else
		{
			m_pBaseBoard = CCSprite::createWithSpriteFrame(pFrame);
			addChild(m_pBaseBoard);
		}
		setContentSize(m_pBaseBoard->getContentSize());
		m_pBaseBoard->setPosition(m_tCenterPoint);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ControlView::setJoystickSpriteFrame(CCSpriteFrame* pFrame)
{
	if( pFrame )
	{
		if( m_pJoystick )
		{
			m_pJoystick->setDisplayFrame(pFrame);
		}
		else
		{
			m_pJoystick = CCSprite::createWithSpriteFrame(pFrame);
			addChild(m_pJoystick, 1);
		}
		m_pJoystick->setPosition(m_tCenterPoint);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ControlView::setBaseBoardTexture(CCTexture2D *pTexture)
{
	if( pTexture )
	{
		if( m_pBaseBoard )
		{
			m_pBaseBoard->setTexture(pTexture);

			CCRect tRect = CCRectZero;
			tRect.size = pTexture->getContentSize();
			m_pBaseBoard->setTextureRect(tRect);
		}
		else
		{
			m_pBaseBoard = CCSprite::createWithTexture(pTexture);
			addChild(m_pBaseBoard);
		}
		setContentSize(m_pBaseBoard->getContentSize());
		m_pBaseBoard->setPosition(m_tCenterPoint);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ControlView::setJoystickTexture(CCTexture2D *pTexture)
{
	if( pTexture )
	{
		if( m_pJoystick )
		{
			m_pJoystick->setTexture(pTexture);

			CCRect tRect = CCRectZero;
			tRect.size = pTexture->getContentSize();
			m_pJoystick->setTextureRect(tRect);
		}
		else
		{
			m_pJoystick = CCSprite::createWithTexture(pTexture);
			addChild(m_pJoystick, 1);
		}
		m_pJoystick->setPosition(m_tCenterPoint);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ControlView::setBaseBoardSpriteFrameName(const char* pSpriteName)
{
	CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pSpriteName);

#if COCOS2D_DEBUG > 0
	char msg[256] = {0};
	sprintf(msg, "Invalid spriteFrameName: %s", pSpriteName);
	CCAssert(pFrame != NULL, msg);
#endif

	return setBaseBoardSpriteFrame(pFrame);
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ControlView::setJoystickSpriteFrameName(const char* pSpriteName)
{
	CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pSpriteName);

#if COCOS2D_DEBUG > 0
	char msg[256] = {0};
	sprintf(msg, "Invalid spriteFrameName: %s", pSpriteName);
	CCAssert(pFrame != NULL, msg);
#endif

	return setJoystickSpriteFrame(pFrame);	
}
//-------------------------------------------------------------------------