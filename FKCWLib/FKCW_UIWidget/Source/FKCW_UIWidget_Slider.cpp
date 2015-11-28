//-------------------------------------------------------------------------
#include "../Include/FKCW_UIWidget_Slider.h"
//-------------------------------------------------------------------------
FKCW_UIWidget_Slider::FKCW_UIWidget_Slider()
	: m_pSlider(NULL)
	, m_bDrag(false)
{
	setThisObject(this);
}
//-------------------------------------------------------------------------
FKCW_UIWidget_Slider::~FKCW_UIWidget_Slider()
{

}
//-------------------------------------------------------------------------
FKCW_UIWidget_Slider* FKCW_UIWidget_Slider::create()
{
	FKCW_UIWidget_Slider* pRet = new FKCW_UIWidget_Slider();
	if( pRet && pRet->init() )
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_Slider* FKCW_UIWidget_Slider::create(const char* pSlider, const char* pProgress)
{
	FKCW_UIWidget_Slider* pRet = new FKCW_UIWidget_Slider();
	if( pRet && pRet->initWithSlider(pSlider, pProgress) )
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_Slider::initWithSlider(const char* pSlider, const char* pProgress)
{
	setSliderImage(pSlider);

	if( initWithFile(pProgress) )
	{
		return true;
	}

	return false;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Slider::setContentSize(const CCSize& tSize)
{
	if( m_pSlider && m_pProgressSprite )
	{
		const CCSize& tSliderSize = m_pSlider->getContentSize();

		CCSize tTargetSize;
		tTargetSize.width = m_tProgressSize.width + tSliderSize.width;
		tTargetSize.height = m_tProgressSize.height + tSliderSize.height;

		FKCW_UIWidget_ProgressBar::setContentSize(tTargetSize);
		return;
	}

	FKCW_UIWidget_ProgressBar::setContentSize(tSize);
}
//-------------------------------------------------------------------------
int FKCW_UIWidget_Slider::valueFromPercent(float fPercentage)
{
	return (int)(fPercentage * m_nMaxValue);
}
//-------------------------------------------------------------------------
int FKCW_UIWidget_Slider::valueFromPoint(const CCPoint& tPoint)
{
	int nRet = 0;
	switch( m_eDirection )
	{
	case eProgressBarDirectionLeftToRight:
		{
			float fHalfWidth = m_tProgressSize.width / 2;
			if( tPoint.x < m_tCenterPoint.x - fHalfWidth )
			{
				nRet = m_nMinValue;
				break;
			}
			if( tPoint.x > m_tCenterPoint.x + fHalfWidth )
			{
				nRet = m_nMaxValue;
				break;
			}

			float fStartPoint = tPoint.x - (m_tCenterPoint.x - fHalfWidth);
			float fPercentage = fStartPoint / m_tProgressSize.width;
			nRet = valueFromPercent(fPercentage);
		}
		break;
	case eProgressBarDirectionRightToLeft:
		{
			float fHalfWidth = m_tProgressSize.width / 2;
			if( tPoint.x < m_tCenterPoint.x - fHalfWidth )
			{
				nRet = m_nMaxValue;
				break;
			}
			if( tPoint.x > m_tCenterPoint.x + fHalfWidth )
			{
				nRet = m_nMinValue;
				break;
			}

			float fStartPoint = tPoint.x - (m_tCenterPoint.x - fHalfWidth);
			float fPercentage = (m_tProgressSize.width - fStartPoint) / m_tProgressSize.width;
			nRet = valueFromPercent(fPercentage);
		}
		break;
	case eProgressBarDirectionBottomToTop:
		{
			float fHalfHeight = m_tProgressSize.height / 2;
			if( tPoint.y < m_tCenterPoint.y - fHalfHeight )
			{
				nRet = m_nMinValue;
				break;
			}
			if( tPoint.y > m_tCenterPoint.y + fHalfHeight )
			{
				nRet = m_nMaxValue;
				break;
			}

			float fStartPoint = tPoint.y - (m_tCenterPoint.y - fHalfHeight);
			float fPercentage = fStartPoint / m_tProgressSize.height;
			nRet = valueFromPercent(fPercentage);
		}
		break;
	case eProgressBarDirectionTopToBottom:
		{
			float fHalfHeight = m_tProgressSize.height / 2;
			if( tPoint.y < m_tCenterPoint.y - fHalfHeight )
			{
				nRet = m_nMaxValue;
				break;
			}
			if( tPoint.y > m_tCenterPoint.y + fHalfHeight )
			{
				nRet = m_nMinValue;
				break;
			}

			float fStartPoint = tPoint.y - (m_tCenterPoint.y - fHalfHeight);
			float fPercentage = (m_tProgressSize.height - fStartPoint) / m_tProgressSize.height;
			nRet = valueFromPercent(fPercentage);
		}
		break;
	default:
		break;
	}
	return nRet;
}
//-------------------------------------------------------------------------
ENUM_WidgetTouchModel FKCW_UIWidget_Slider::onTouchBegan(CCTouch *pTouch)
{
	m_bDrag = m_pSlider->boundingBox().containsPoint(
		convertToNodeSpace(pTouch->getLocation())
		);

	if( m_bDrag )
	{
		changeValueAndExecuteEvent(valueFromPoint(convertToNodeSpace(pTouch->getLocation())), true);
		return eWidgetTouchSustained;
	}

	return eWidgetTouchNone;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Slider::onTouchMoved(CCTouch *pTouch, float fDuration)
{
	if( m_bDrag )
	{
		changeValueAndExecuteEvent(valueFromPoint(convertToNodeSpace(pTouch->getLocation())), true);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Slider::onTouchEnded(CCTouch *pTouch, float fDuration)
{
	if( m_bDrag )
	{
		changeValueAndExecuteEvent(valueFromPoint(convertToNodeSpace(pTouch->getLocation())), true);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Slider::onTouchCancelled(CCTouch *pTouch, float fDuration)
{
	if( m_bDrag )
	{
		changeValueAndExecuteEvent(valueFromPoint(convertToNodeSpace(pTouch->getLocation())), true);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Slider::pointFromValue(int nValue, CCPoint& tOutPoint)
{
	float fPercentage = getPercentage();
	switch( m_eDirection )
	{
	case eProgressBarDirectionLeftToRight:
		{
			tOutPoint.x = m_tProgressSize.width * fPercentage + (m_tCenterPoint.x - m_tProgressSize.width / 2);
			tOutPoint.y = m_tCenterPoint.y;
		}
		break;
	case eProgressBarDirectionRightToLeft:
		{
			tOutPoint.x = m_tProgressSize.width - (m_tProgressSize.width * fPercentage) + (m_tCenterPoint.x - m_tProgressSize.width / 2);
			tOutPoint.y = m_tCenterPoint.y;
		}
		break;
	case eProgressBarDirectionBottomToTop:
		{
			tOutPoint.x = m_tCenterPoint.x;
			tOutPoint.y = m_tProgressSize.height * fPercentage + (m_tCenterPoint.y - m_tProgressSize.height / 2);
		}
		break;
	case eProgressBarDirectionTopToBottom:
		{
			tOutPoint.x = m_tCenterPoint.x;
			tOutPoint.y = m_tProgressSize.height - (m_tProgressSize.height * fPercentage) + (m_tCenterPoint.y - m_tProgressSize.height / 2);
		}
		break;
	default:
		break;
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Slider::changeValueAndExecuteEvent(int nValue, bool bExeEvent)
{
	FKCW_UIWidget_ProgressBar::changeValueAndExecuteEvent(nValue, bExeEvent);

	if( m_pSlider )
	{
		CCPoint tOutPoint;
		pointFromValue(m_nValue, tOutPoint);

		m_pSlider->setPosition(tOutPoint);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Slider::setSliderImage(const char* pFile)
{
	if( pFile && strlen(pFile) )
	{
		CCTexture2D* pTexture = CCTextureCache::sharedTextureCache()->addImage(pFile);
		setSliderTexture(pTexture);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Slider::setSliderTexture(CCTexture2D* pTexture)
{
	if( m_pSlider )
	{
		m_pSlider->setTexture(pTexture);

		CCRect tRect = CCRectZero;
		tRect.size = pTexture->getContentSize();
		m_pSlider->setTextureRect(tRect);
	}
	else
	{
		m_pSlider = CCSprite::createWithTexture(pTexture);
		addChild(m_pSlider, 2);
	}
	setContentSize(m_obContentSize);
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Slider::setSliderSpriteFrame(CCSpriteFrame* pFrame)
{
	if( pFrame )
	{
		if( m_pSlider )
		{
			m_pSlider->setDisplayFrame(pFrame);
		}
		else
		{
			m_pSlider = CCSprite::createWithSpriteFrame(pFrame);
			addChild(m_pSlider);
		}
	}
	setContentSize(m_obContentSize);
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_Slider::setSliderSpriteFrameName(const char* pSpriteName)
{
	CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pSpriteName);

#if COCOS2D_DEBUG > 0
	char msg[256] = {0};
	sprintf(msg, "Invalid spriteFrameName: %s", pSpriteName);
	CCAssert(pFrame != NULL, msg);
#endif

	return setSliderSpriteFrame(pFrame);
}
//-------------------------------------------------------------------------