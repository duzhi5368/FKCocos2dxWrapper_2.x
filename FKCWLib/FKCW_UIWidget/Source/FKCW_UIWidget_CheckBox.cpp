//-------------------------------------------------------------------------
#include "../Include/FKCW_UIWidget_CheckBox.h"
//-------------------------------------------------------------------------
#define SAFE_SET_VISIBLE(_SPRITE_, _FLAG_) if(_SPRITE_) _SPRITE_->setVisible(_FLAG_)
#define SAFE_SET_CENTER(_SPRITE_, _SIZE_) \
	if( _SPRITE_ ) _SPRITE_->setPosition(CCPoint(_SIZE_.width/2, _SIZE_.height/2))
//-------------------------------------------------------------------------
FKCW_UIWidget_CheckBox::FKCW_UIWidget_CheckBox()
	: m_pNormal(NULL)
	, m_pNormalPress(NULL)
	, m_pChecked(NULL)
	, m_pCheckedPress(NULL)
	, m_pDisabledNormal(NULL)
	, m_pDisabledChecked(NULL)
{
	setThisObject(this);
	setCascadeColorEnabled(true);
	setCascadeOpacityEnabled(true);

	setAnchorPoint(FKCW_UIWIDGET_BASIC_DEFAULT_ANCHOR_POINT);
	setContentSize(FKCW_UIWIDGET_BASIC_DEFAULT_CONTENT_SIZE);
}
//-------------------------------------------------------------------------
FKCW_UIWidget_CheckBox::~FKCW_UIWidget_CheckBox()
{

}
//-------------------------------------------------------------------------
FKCW_UIWidget_CheckBox* FKCW_UIWidget_CheckBox::create()
{
	FKCW_UIWidget_CheckBox* pRet = new FKCW_UIWidget_CheckBox();
	if( pRet && pRet->init() )
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_CheckBox::init()
{
	setAnchorPoint(FKCW_UIWIDGET_BASIC_DEFAULT_ANCHOR_POINT);
	setContentSize(FKCW_UIWIDGET_BASIC_DEFAULT_CONTENT_SIZE);

	return true;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckBox::setContentSize(const CCSize& tContentSize)
{
	CCNodeRGBA::setContentSize(tContentSize);
	SAFE_SET_CENTER(m_pNormal, m_obContentSize);
	SAFE_SET_CENTER(m_pNormalPress, m_obContentSize);
	SAFE_SET_CENTER(m_pChecked, m_obContentSize);
	SAFE_SET_CENTER(m_pCheckedPress, m_obContentSize);
	SAFE_SET_CENTER(m_pDisabledNormal, m_obContentSize);
	SAFE_SET_CENTER(m_pDisabledChecked, m_obContentSize);
}
//-------------------------------------------------------------------------
ENUM_WidgetTouchModel FKCW_UIWidget_CheckBox::onTouchBegan(CCTouch* pTouch)
{
	FKCW_UIWIDGET_LONGCLICK_ONTOUCHBEGAN;

	if( isChecked() )
	{
		if( m_pCheckedPress )
		{
			SAFE_SET_VISIBLE(m_pCheckedPress, true);
			SAFE_SET_VISIBLE(m_pChecked, false);
		}
	}
	else
	{
		if( m_pNormalPress )
		{
			SAFE_SET_VISIBLE(m_pNormalPress, true);
			SAFE_SET_VISIBLE(m_pNormal, false);
		}
	}

	return eWidgetTouchTransient;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckBox::onTouchMoved(CCTouch* pTouch, float fDuration)
{
	FKCW_UIWIDGET_LONGCLICK_ONTOUCHMOVED;

	CCPoint tPoint = m_pParent->convertToNodeSpace(pTouch->getLocation());
	if( boundingBox().containsPoint(tPoint) )
	{
		if( isChecked() )
		{
			if( m_pCheckedPress )
			{
				SAFE_SET_VISIBLE(m_pCheckedPress, true);
				SAFE_SET_VISIBLE(m_pChecked, false);
			}
			else
			{
				SAFE_SET_VISIBLE(m_pChecked, true);
			}
		}
		else
		{
			if( m_pNormalPress )
			{
				SAFE_SET_VISIBLE(m_pNormalPress, true);
				SAFE_SET_VISIBLE(m_pNormal, false);
			}
			else
			{
				SAFE_SET_VISIBLE(m_pNormal, true);
			}
		}
	}
	else
	{
		if( isChecked() )
		{
			SAFE_SET_VISIBLE(m_pCheckedPress, false);
			SAFE_SET_VISIBLE(m_pChecked, true);
		}
		else
		{
			SAFE_SET_VISIBLE(m_pNormalPress, false);
			SAFE_SET_VISIBLE(m_pNormal, true);
		}
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckBox::onTouchEnded(CCTouch* pTouch, float fDuration)
{
	FKCW_UIWIDGET_LONGCLICK_ONTOUCHENDED;

	CCPoint tPoint = m_pParent->convertToNodeSpace(pTouch->getLocation());
	if( boundingBox().containsPoint(tPoint) )
	{
		if( isChecked() )
		{
			setChecked(false);
		}
		else
		{
			setChecked(true);
		}
		executeClickHandler(this);
	}
	else
	{
		if( isChecked() )
		{
			SAFE_SET_VISIBLE(m_pNormalPress, false);
			SAFE_SET_VISIBLE(m_pNormal, false);
			SAFE_SET_VISIBLE(m_pCheckedPress, false);
			SAFE_SET_VISIBLE(m_pChecked, true);
		}
		else
		{
			SAFE_SET_VISIBLE(m_pNormalPress, false);
			SAFE_SET_VISIBLE(m_pNormal, true);
			SAFE_SET_VISIBLE(m_pCheckedPress, false);
			SAFE_SET_VISIBLE(m_pChecked, false);
		}
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckBox::onTouchCancelled(CCTouch* pTouch, float fDuration)
{
	FKCW_UIWIDGET_LONGCLICK_ONTOUCHCANCELLED;

	if( isChecked() )
	{
		SAFE_SET_VISIBLE(m_pCheckedPress, false);
		SAFE_SET_VISIBLE(m_pChecked, true);
	}
	else
	{
		SAFE_SET_VISIBLE(m_pNormalPress, false);
		SAFE_SET_VISIBLE(m_pNormal, true);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckBox::setChecked(bool bChecked)
{
	if( m_bChecked != bChecked )
	{
		if( m_bEnabled )
		{
			if( bChecked )
			{
				SAFE_SET_VISIBLE(m_pNormalPress, false);
				SAFE_SET_VISIBLE(m_pNormal, false);
				SAFE_SET_VISIBLE(m_pCheckedPress, false);
				SAFE_SET_VISIBLE(m_pChecked, true);
				setUnCheckedForOtherWidgetsByExclusion(m_pParent);
			}
			else
			{
				SAFE_SET_VISIBLE(m_pNormalPress, false);
				SAFE_SET_VISIBLE(m_pNormal, true);
				SAFE_SET_VISIBLE(m_pCheckedPress, false);
				SAFE_SET_VISIBLE(m_pChecked, false);
			}
		}
		else
		{
			if( bChecked )
			{
				if( m_pDisabledChecked )
				{
					m_pDisabledChecked->setVisible(true);
					SAFE_SET_VISIBLE(m_pNormalPress, false);
					SAFE_SET_VISIBLE(m_pNormal, false);
					SAFE_SET_VISIBLE(m_pCheckedPress, false);
					SAFE_SET_VISIBLE(m_pChecked, false);
					SAFE_SET_VISIBLE(m_pDisabledNormal, false);
				}
				else
				{
					SAFE_SET_VISIBLE(m_pNormalPress, false);
					SAFE_SET_VISIBLE(m_pNormal, false);
					SAFE_SET_VISIBLE(m_pCheckedPress, false);
					SAFE_SET_VISIBLE(m_pChecked, true);
					SAFE_SET_VISIBLE(m_pDisabledNormal, false);
				}
				setUnCheckedForOtherWidgetsByExclusion(m_pParent);
			}
			else
			{
				if( m_pDisabledNormal )
				{
					m_pDisabledNormal->setVisible(true);
					SAFE_SET_VISIBLE(m_pNormalPress, false);
					SAFE_SET_VISIBLE(m_pNormal, false);
					SAFE_SET_VISIBLE(m_pCheckedPress, false);
					SAFE_SET_VISIBLE(m_pChecked, false);
					SAFE_SET_VISIBLE(m_pDisabledChecked, false);
				}
				else
				{
					SAFE_SET_VISIBLE(m_pNormalPress, false);
					SAFE_SET_VISIBLE(m_pNormal, true);
					SAFE_SET_VISIBLE(m_pCheckedPress, false);
					SAFE_SET_VISIBLE(m_pChecked, false);
					SAFE_SET_VISIBLE(m_pDisabledChecked, false);
				}
			}
		}

		m_bChecked = bChecked;
		executeCheckHandler(this, m_bChecked);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckBox::setEnabled(bool bEnabled)
{
	if( m_bEnabled != bEnabled )
	{
		if( bEnabled )
		{
			if( isChecked() )
			{
				SAFE_SET_VISIBLE(m_pNormalPress, false);
				SAFE_SET_VISIBLE(m_pNormal, false);
				SAFE_SET_VISIBLE(m_pCheckedPress, false);
				SAFE_SET_VISIBLE(m_pChecked, true);
				SAFE_SET_VISIBLE(m_pDisabledNormal, false);
				SAFE_SET_VISIBLE(m_pDisabledChecked, false);
			}
			else
			{
				SAFE_SET_VISIBLE(m_pNormalPress, false);
				SAFE_SET_VISIBLE(m_pNormal, true);
				SAFE_SET_VISIBLE(m_pCheckedPress, false);
				SAFE_SET_VISIBLE(m_pChecked, false);
				SAFE_SET_VISIBLE(m_pDisabledNormal, false);
				SAFE_SET_VISIBLE(m_pDisabledChecked, false);
			}
		}
		else
		{
			if( isChecked() )
			{
				if( m_pDisabledChecked )
				{
					SAFE_SET_VISIBLE(m_pNormalPress, false);
					SAFE_SET_VISIBLE(m_pNormal, false);
					SAFE_SET_VISIBLE(m_pCheckedPress, false);
					SAFE_SET_VISIBLE(m_pChecked, false);
					SAFE_SET_VISIBLE(m_pDisabledNormal, false);
					SAFE_SET_VISIBLE(m_pDisabledChecked, true);
				}
				else
				{
					SAFE_SET_VISIBLE(m_pNormalPress, false);
					SAFE_SET_VISIBLE(m_pNormal, false);
					SAFE_SET_VISIBLE(m_pCheckedPress, false);
					SAFE_SET_VISIBLE(m_pChecked, true);
					SAFE_SET_VISIBLE(m_pDisabledNormal, false);
				}
			}
			else
			{
				if( m_pDisabledNormal )
				{
					SAFE_SET_VISIBLE(m_pNormalPress, false);
					SAFE_SET_VISIBLE(m_pNormal, false);
					SAFE_SET_VISIBLE(m_pCheckedPress, false);
					SAFE_SET_VISIBLE(m_pChecked, false);
					SAFE_SET_VISIBLE(m_pDisabledNormal, true);
					SAFE_SET_VISIBLE(m_pDisabledChecked, false);
				}
				else
				{
					SAFE_SET_VISIBLE(m_pNormalPress, true);
					SAFE_SET_VISIBLE(m_pNormal, false);
					SAFE_SET_VISIBLE(m_pCheckedPress, false);
					SAFE_SET_VISIBLE(m_pChecked, false);
					SAFE_SET_VISIBLE(m_pDisabledChecked, false);
				}
			}
		}

		m_bEnabled = bEnabled;
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckBox::setNormalImage(const char* pFile)
{
	if( pFile && strlen(pFile) )
	{
		CCTexture2D* pTexture = CCTextureCache::sharedTextureCache()->addImage(pFile);
		setNormalTexture(pTexture);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckBox::setNormalPressImage(const char* pFile)
{
	if( pFile && strlen(pFile) )
	{
		CCTexture2D* pTexture = CCTextureCache::sharedTextureCache()->addImage(pFile);
		setNormalPressTexture(pTexture);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckBox::setCheckedImage(const char* pFile)
{
	if( pFile && strlen(pFile) )
	{
		CCTexture2D* pTexture = CCTextureCache::sharedTextureCache()->addImage(pFile);
		setCheckedTexture(pTexture);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckBox::setCheckedPressImage(const char* pFile)
{
	if( pFile && strlen(pFile) )
	{
		CCTexture2D* pTexture = CCTextureCache::sharedTextureCache()->addImage(pFile);
		setCheckedPressTexture(pTexture);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckBox::setDisabledNormalImage(const char* pFile)
{
	if( pFile && strlen(pFile) )
	{
		CCTexture2D* pTexture = CCTextureCache::sharedTextureCache()->addImage(pFile);
		setDisabledNormalTexture(pTexture);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckBox::setDisabledCheckedImage(const char* pFile)
{
	if( pFile && strlen(pFile) )
	{
		CCTexture2D* pTexture = CCTextureCache::sharedTextureCache()->addImage(pFile);
		setDisabledCheckedTexture(pTexture);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckBox::setNormalSpriteFrame(CCSpriteFrame* pFrame)
{
	if( pFrame )
	{
		if( m_pNormal )
		{
			m_pNormal->setDisplayFrame(pFrame);
			setContentSize(m_pNormal->getContentSize());
		}
		else
		{
			m_pNormal = CCSprite::createWithSpriteFrame(pFrame);
			setContentSize(m_pNormal->getContentSize());
			addChild(m_pNormal);
		}
		m_pNormal->setPosition(CCPoint(m_obContentSize.width/2, m_obContentSize.height/2));
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckBox::setNormalPressSpriteFrame(CCSpriteFrame* pFrame)
{
	if( pFrame )
	{
		if( m_pNormalPress )
		{
			m_pNormalPress->setDisplayFrame(pFrame);
		}
		else
		{
			m_pNormalPress = CCSprite::createWithSpriteFrame(pFrame);
			addChild(m_pNormalPress);
		}
		m_pNormalPress->setPosition(CCPoint(m_obContentSize.width/2, m_obContentSize.height/2));
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckBox::setCheckedSpriteFrame(CCSpriteFrame* pFrame)
{
	if( pFrame )
	{
		if( m_pChecked )
		{
			m_pChecked->setDisplayFrame(pFrame);
		}
		else
		{
			m_pChecked = CCSprite::createWithSpriteFrame(pFrame);
			addChild(m_pChecked);
		}
		m_pChecked->setPosition(CCPoint(m_obContentSize.width/2, m_obContentSize.height/2));
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckBox::setCheckedPressSpriteFrame(CCSpriteFrame* pFrame)
{
	if( pFrame )
	{
		if( m_pCheckedPress )
		{
			m_pCheckedPress->setDisplayFrame(pFrame);
		}
		else
		{
			m_pCheckedPress = CCSprite::createWithSpriteFrame(pFrame);
			addChild(m_pCheckedPress);
		}
		m_pCheckedPress->setPosition(CCPoint(m_obContentSize.width/2, m_obContentSize.height/2));
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckBox::setDisabledNormalSpriteFrame(CCSpriteFrame* pFrame)
{
	if( pFrame )
	{
		if( m_pDisabledNormal )
		{
			m_pDisabledNormal->setDisplayFrame(pFrame);
		}
		else
		{
			m_pDisabledNormal = CCSprite::createWithSpriteFrame(pFrame);
			addChild(m_pDisabledNormal);
		}
		m_pDisabledNormal->setPosition(CCPoint(m_obContentSize.width/2, m_obContentSize.height/2));
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckBox::setDisabledCheckedSpriteFrame(CCSpriteFrame* pFrame)
{
	if( pFrame )
	{
		if( m_pDisabledChecked )
		{
			m_pDisabledChecked->setDisplayFrame(pFrame);
		}
		else
		{
			m_pDisabledChecked = CCSprite::createWithSpriteFrame(pFrame);
			addChild(m_pDisabledChecked);
		}
		m_pDisabledChecked->setPosition(CCPoint(m_obContentSize.width/2, m_obContentSize.height/2));
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckBox::setNormalTexture(CCTexture2D* pTexture)
{
	if( m_pNormal )
	{
		m_pNormal->setTexture(pTexture);

		CCRect tRect = CCRectZero;
		tRect.size = pTexture->getContentSize();
		m_pNormal->setTextureRect(tRect);

		setContentSize(pTexture->getContentSize());
	}
	else
	{
		m_pNormal = CCSprite::createWithTexture(pTexture);
		setContentSize(m_pNormal->getContentSize());
		addChild(m_pNormal);
	}
	m_pNormal->setPosition(CCPoint(m_obContentSize.width/2, m_obContentSize.height/2));
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckBox::setNormalPressTexture(CCTexture2D* pTexture)
{
	if( m_pNormalPress )
	{
		m_pNormalPress->setTexture(pTexture);

		CCRect tRect = CCRectZero;
		tRect.size = pTexture->getContentSize();
		m_pNormalPress->setTextureRect(tRect);
	}
	else
	{
		m_pNormalPress = CCSprite::createWithTexture(pTexture);
		addChild(m_pNormalPress);
	}
	m_pNormalPress->setVisible(false);
	m_pNormalPress->setPosition(CCPoint(m_obContentSize.width/2, m_obContentSize.height/2));
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckBox::setCheckedTexture(CCTexture2D* pTexture)
{
	if( m_pChecked )
	{
		m_pChecked->setTexture(pTexture);

		CCRect tRect = CCRectZero;
		tRect.size = pTexture->getContentSize();
		m_pChecked->setTextureRect(tRect);
	}
	else
	{
		m_pChecked = CCSprite::createWithTexture(pTexture);
		addChild(m_pChecked);
	}
	m_pChecked->setVisible(false);
	m_pChecked->setPosition(CCPoint(m_obContentSize.width/2, m_obContentSize.height/2));
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckBox::setCheckedPressTexture(CCTexture2D* pTexture)
{
	if( m_pCheckedPress )
	{
		m_pCheckedPress->setTexture(pTexture);

		CCRect tRect = CCRectZero;
		tRect.size = pTexture->getContentSize();
		m_pCheckedPress->setTextureRect(tRect);
	}
	else
	{
		m_pCheckedPress = CCSprite::createWithTexture(pTexture);
		addChild(m_pCheckedPress);
	}
	m_pCheckedPress->setVisible(false);
	m_pCheckedPress->setPosition(CCPoint(m_obContentSize.width/2, m_obContentSize.height/2));
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckBox::setDisabledNormalTexture(CCTexture2D* pTexture)
{
	if( m_pDisabledNormal )
	{
		m_pDisabledNormal->setTexture(pTexture);

		CCRect tRect = CCRectZero;
		tRect.size = pTexture->getContentSize();
		m_pDisabledNormal->setTextureRect(tRect);
	}
	else
	{
		m_pDisabledNormal = CCSprite::createWithTexture(pTexture);
		addChild(m_pDisabledNormal);
	}
	m_pDisabledNormal->setVisible(false);
	m_pDisabledNormal->setPosition(CCPoint(m_obContentSize.width/2, m_obContentSize.height/2));
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckBox::setDisabledCheckedTexture(CCTexture2D* pTexture)
{
	if( m_pDisabledChecked )
	{
		m_pDisabledChecked->setTexture(pTexture);

		CCRect tRect = CCRectZero;
		tRect.size = pTexture->getContentSize();
		m_pDisabledChecked->setTextureRect(tRect);
	}
	else
	{
		m_pDisabledChecked = CCSprite::createWithTexture(pTexture);
		addChild(m_pDisabledChecked);
	}
	m_pDisabledChecked->setVisible(false);
	m_pDisabledChecked->setPosition(CCPoint(m_obContentSize.width/2, m_obContentSize.height/2));
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckBox::setNormalSpriteFrameName(const char* pSpriteName)
{
	CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pSpriteName);

#if COCOS2D_DEBUG > 0
	char msg[256] = {0};
	sprintf(msg, "Invalid spriteFrameName: %s", pSpriteName);
	CCAssert(pFrame != NULL, msg);
#endif

	return setNormalSpriteFrame(pFrame);
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckBox::setNormalPressSpriteFrameName(const char* pSpriteName)
{
	CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pSpriteName);

#if COCOS2D_DEBUG > 0
	char msg[256] = {0};
	sprintf(msg, "Invalid spriteFrameName: %s", pSpriteName);
	CCAssert(pFrame != NULL, msg);
#endif

	return setNormalPressSpriteFrame(pFrame);
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckBox::setCheckedSpriteFrameName(const char* pSpriteName)
{
	CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pSpriteName);

#if COCOS2D_DEBUG > 0
	char msg[256] = {0};
	sprintf(msg, "Invalid spriteFrameName: %s", pSpriteName);
	CCAssert(pFrame != NULL, msg);
#endif

	return setCheckedSpriteFrame(pFrame);
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckBox::setCheckedPressSpriteFrameName(const char* pSpriteName)
{
	CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pSpriteName);

#if COCOS2D_DEBUG > 0
	char msg[256] = {0};
	sprintf(msg, "Invalid spriteFrameName: %s", pSpriteName);
	CCAssert(pFrame != NULL, msg);
#endif

	return setCheckedPressSpriteFrame(pFrame);
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckBox::setDisabledNormalSpriteFrameName(const char* pSpriteName)
{
	CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pSpriteName);

#if COCOS2D_DEBUG > 0
	char msg[256] = {0};
	sprintf(msg, "Invalid spriteFrameName: %s", pSpriteName);
	CCAssert(pFrame != NULL, msg);
#endif

	return setDisabledNormalSpriteFrame(pFrame);
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_CheckBox::setDisabledCheckedSpriteFrameName(const char* pSpriteName)
{
	CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pSpriteName);

#if COCOS2D_DEBUG > 0
	char msg[256] = {0};
	sprintf(msg, "Invalid spriteFrameName: %s", pSpriteName);
	CCAssert(pFrame != NULL, msg);
#endif

	return setDisabledCheckedSpriteFrame(pFrame);
}
//-------------------------------------------------------------------------