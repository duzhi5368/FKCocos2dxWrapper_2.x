//-------------------------------------------------------------------------
#include "../Include/FKCW_UIWidget_ProgressBar.h"
//-------------------------------------------------------------------------
FKCW_UIWidget_ProgressBar::FKCW_UIWidget_ProgressBar()
	: m_pProgressSprite(NULL)
	, m_eDirection(eProgressBarDirectionLeftToRight)
	, m_nMaxValue(100)
	, m_nMinValue(0)
	, m_nValue(0)
	, m_bFirstTick(true)
	, m_fLapsed(0.0f)
	, m_fDuration(0.0f)
	, m_nToValue(0)
	, m_nFromValue(0)
	, m_bProgressing(false)
	, m_tProgressSize(CCSizeZero)
	, m_tCenterPoint(CCPointZero)
	, m_pBackgroundImage(NULL)
	, m_pBackgroundColor(NULL)
	, m_pBackgroundGradient(NULL)
{

}
//-------------------------------------------------------------------------
FKCW_UIWidget_ProgressBar::~FKCW_UIWidget_ProgressBar()
{

}
//-------------------------------------------------------------------------
FKCW_UIWidget_ProgressBar* FKCW_UIWidget_ProgressBar::create()
{
	FKCW_UIWidget_ProgressBar* pRet = new FKCW_UIWidget_ProgressBar();
	if( pRet && pRet->init() )
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ProgressBar* FKCW_UIWidget_ProgressBar::create(const char* pProgress)
{
	FKCW_UIWidget_ProgressBar* pRet = new FKCW_UIWidget_ProgressBar();
	if( pRet && pRet->initWithFile(pProgress) )
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_ProgressBar::init()
{
	setCascadeColorEnabled(true);
	setCascadeOpacityEnabled(true);

	setAnchorPoint(FKCW_UIWIDGET_BASIC_DEFAULT_ANCHOR_POINT);
	setContentSize(FKCW_UIWIDGET_BASIC_DEFAULT_CONTENT_SIZE);

	return true;
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_ProgressBar::initWithFile(const char* pProgress)
{
	if( init() )
	{
		setProgressImage(pProgress);
		return true;
	}
	return false;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ProgressBar::setValue(int nValue)
{
	changeValueAndExecuteEvent(nValue, true);
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ProgressBar::changeValueAndExecuteEvent(int nValue, bool bExeEvent)
{
	CCAssert(m_nMinValue >= 0 && m_nMaxValue >= 0 && m_nMinValue < m_nMaxValue, "err");
	int nOldValue = m_nValue;

	nValue = MAX(nValue, m_nMinValue);
	nValue = MIN(nValue, m_nMaxValue);
	m_nValue = nValue;

	CCRect tScissorRect;
	getScissorRectByPercentage(tScissorRect);

	switch( m_eDirection )
	{
	case eProgressBarDirectionLeftToRight:
		{
			m_pProgressSprite->setAnchorPoint(CCPoint(0.0f, 0.5f));
			m_pProgressSprite->setPosition(CCPoint(m_tCenterPoint.x - m_tProgressSize.width / 2, m_tCenterPoint.y));
		}
		break;
	case eProgressBarDirectionBottomToTop:
		{
			m_pProgressSprite->setAnchorPoint(CCPoint(0.5f, 0.0f));
			m_pProgressSprite->setPosition(CCPoint(m_tCenterPoint.x, m_tCenterPoint.y - m_tProgressSize.height / 2));
		}
		break;
	case eProgressBarDirectionRightToLeft:
		{
			m_pProgressSprite->setAnchorPoint(CCPoint(1.0f, 0.5f));
			m_pProgressSprite->setPosition(CCPoint(m_tCenterPoint.x + m_tProgressSize.width / 2, m_tCenterPoint.y));
		}
		break;
	case eProgressBarDirectionTopToBottom:
		{
			m_pProgressSprite->setAnchorPoint(CCPoint(0.5f, 1.0f));
			m_pProgressSprite->setPosition(CCPoint(m_tCenterPoint.x, m_tCenterPoint.y + m_tProgressSize.height / 2));
		}
		break;
	default:
		break;
	}
	m_pProgressSprite->setTextureRect(tScissorRect);

	if( bExeEvent && nOldValue != nValue )
	{
		executeValueChangedHandler(this, nValue);
	}
}
//-------------------------------------------------------------------------
int FKCW_UIWidget_ProgressBar::getValue() const
{
	return m_nValue;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ProgressBar::setMaxValue(int nMaxValue)
{
	m_nMaxValue = nMaxValue;
}
//-------------------------------------------------------------------------
int FKCW_UIWidget_ProgressBar::getMaxValue() const
{
	return m_nMaxValue;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ProgressBar::setMinValue(int nMinValue)
{
	m_nMinValue = nMinValue;
}
//-------------------------------------------------------------------------
int FKCW_UIWidget_ProgressBar::getMinValue() const
{
	return m_nMinValue;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ProgressBar::setDirection(ENUM_ProgressBarDirection eDirection)
{
	if( m_eDirection != eDirection )
	{
		m_eDirection = eDirection;
		changeValueAndExecuteEvent(m_nValue, false);
	}
}
//-------------------------------------------------------------------------
ENUM_ProgressBarDirection FKCW_UIWidget_ProgressBar::getDirection() const
{
	return m_eDirection;
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_ProgressBar::isProgressEnded() const
{
	return m_fLapsed >= m_fDuration;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ProgressBar::startProgress(int nValue, float fDuration)
{
	CCAssert(nValue >= 0, "nValue >= 0");
	if( nValue >= m_nMaxValue && nValue <= m_nMinValue )
	{
		return;
	}

	m_fLapsed = 0.0f;
	m_bFirstTick = true;
	m_fDuration = fDuration;

	if( m_fDuration == 0 )
	{
		m_fDuration = FLT_EPSILON;
	}

	m_nToValue = nValue;
	m_nFromValue = m_nValue;

	performProgressing();
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ProgressBar::startProgressFromTo(int nFromValue, int nToValue, float fDuration)
{
	changeValueAndExecuteEvent(nFromValue, true);
	startProgress(nToValue, fDuration);
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ProgressBar::stopProgress()
{
	stoppedProgressing();
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ProgressBar::performProgressing()
{
	if( !m_bProgressing )
	{
		schedule(schedule_selector(FKCW_UIWidget_ProgressBar::onProgressing));
		m_bProgressing = true;
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ProgressBar::onProgressing(float dt)
{
	if( isProgressEnded() )
	{
		stoppedProgressing();
		executeProgressEndedHandler(this);
	}
	else
	{
		if( m_bFirstTick )
		{
			m_bFirstTick = false;
			m_fLapsed = 0.0f;
		}
		else
		{
			m_fLapsed += CCDirector::sharedDirector()->getDeltaTime();
		}

		float fTime = MAX(0, MIN(1, m_fLapsed / MAX(m_fDuration, FLT_EPSILON)));
		changeValueAndExecuteEvent( static_cast<int>(m_nFromValue + (m_nToValue - m_nFromValue) * fTime), true);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ProgressBar::stoppedProgressing()
{
	if( m_bProgressing )
	{
		unschedule(schedule_selector(FKCW_UIWidget_ProgressBar::onProgressing));
		m_bProgressing = false;
	}
}
//-------------------------------------------------------------------------
float FKCW_UIWidget_ProgressBar::getPercentage() const
{
	return (float)(m_nValue - m_nMinValue) / (float)(m_nMaxValue - m_nMinValue);
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ProgressBar::getScissorRectByPercentage(CCRect& tRect)
{
	float fPercentage = getPercentage();
	switch( m_eDirection )
	{
	case eProgressBarDirectionLeftToRight:
		{
			tRect.origin = CCPointZero;
			tRect.size.width = m_tProgressSize.width * fPercentage;
			tRect.size.height = m_tProgressSize.height;
		}
		break;
	case eProgressBarDirectionRightToLeft:
		{
			tRect.origin.x = m_tProgressSize.width - m_tProgressSize.width * fPercentage;
			tRect.size.width = m_tProgressSize.width * fPercentage;
			tRect.origin.y = 0;
			tRect.size.height = m_tProgressSize.height;
		}
		break;
	case eProgressBarDirectionBottomToTop:
		{
			tRect.origin.x = 0;
			tRect.origin.y = m_tProgressSize.height - m_tProgressSize.height * fPercentage;
			tRect.size.width = m_tProgressSize.width;
			tRect.size.height = m_tProgressSize.height * fPercentage;
		}
		break;
	case eProgressBarDirectionTopToBottom:
		{
			tRect.origin = CCPointZero;
			tRect.size.width = m_tProgressSize.width;
			tRect.size.height = m_tProgressSize.height * fPercentage;
		}
		break;
	default:
		break;
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ProgressBar::setContentSize(const CCSize& tSize)
{
	CCNodeRGBA::setContentSize(tSize);

	m_tCenterPoint.x = m_obContentSize.width / 2;
	m_tCenterPoint.y = m_obContentSize.height / 2;

	if( m_pProgressSprite )
	{
		changeValueAndExecuteEvent(m_nValue, false);
	}

	FKCW_UIWIDGET_UPDATE_BACKGROUND_POS;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ProgressBar::setProgressImage(const char* pFile)
{
	if( pFile && strlen(pFile) )
	{
		CCTexture2D* pTexture = CCTextureCache::sharedTextureCache()->addImage(pFile);
		setProgressTexture(pTexture);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ProgressBar::setProgressTexture(CCTexture2D* pTexture)
{
	if( m_pProgressSprite )
	{
		m_pProgressSprite->setTexture(pTexture);

		CCRect tRect = CCRectZero;
		tRect.size = pTexture->getContentSize();
		m_pProgressSprite->setTextureRect(tRect);

		m_tProgressSize = pTexture->getContentSize();
		setContentSize(m_tProgressSize);
	}
	else
	{
		m_pProgressSprite = CCSprite::createWithTexture(pTexture);
		m_pProgressSprite->setZOrder(1);

		m_tProgressSize = m_pProgressSprite->getContentSize();
		setContentSize(m_tProgressSize);
		addChild(m_pProgressSprite);
	}
	changeValueAndExecuteEvent(m_nValue, false);
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ProgressBar::setProgressSpriteFrame(CCSpriteFrame* pFrame)
{
	if( pFrame )
	{
		if( m_pProgressSprite )
		{
			m_pProgressSprite->setDisplayFrame(pFrame);
			m_tProgressSize = m_pProgressSprite->getContentSize();
			setContentSize(m_tProgressSize);
		}
		else
		{
			m_pProgressSprite = CCSprite::createWithSpriteFrame(pFrame);
			m_pProgressSprite->setZOrder(1);

			m_tProgressSize = m_pProgressSprite->getContentSize();
			setContentSize(m_tProgressSize);
			addChild(m_pProgressSprite);
		}
		changeValueAndExecuteEvent(m_nValue, false);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ProgressBar::setProgressSpriteFrameName(const char* pSpriteName)
{
	CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pSpriteName);

#if COCOS2D_DEBUG > 0
	char msg[256] = {0};
	sprintf(msg, "Invalid spriteFrameName: %s", pSpriteName);
	CCAssert(pFrame != NULL, msg);
#endif

	return setProgressSpriteFrame(pFrame);	
}
//-------------------------------------------------------------------------