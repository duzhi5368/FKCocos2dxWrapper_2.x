//--------------------------------------------------------------------
#include "../Include/FKCW_UI_MenuItemProgressTimer.h"
//--------------------------------------------------------------------
FKCW_UI_MenuItemProgressTimer::FKCW_UI_MenuItemProgressTimer():
	mInterval(0),
	mNormalImage(0),
	mDisabledImage(0),
	mCurrent(0),
	mCanBeActivated(true),
	mProgressTimer(0),
	mType(kCCProgressTimerTypeRadial)
{

}
//--------------------------------------------------------------------
FKCW_UI_MenuItemProgressTimer::~FKCW_UI_MenuItemProgressTimer() 
{

}
//--------------------------------------------------------------------
CCSprite* FKCW_UI_MenuItemProgressTimer::getNormalImage() 
{
	return mNormalImage;
}
//--------------------------------------------------------------------
const unsigned int	kNormalTag = 0x1;
const unsigned int	kDisableTag = 0x2;
//--------------------------------------------------------------------
void FKCW_UI_MenuItemProgressTimer::setNormalImage(CCSprite* image) {

	if (image) 
	{
		addChild(image, 0, kNormalTag);
		image->setAnchorPoint(ccp(0, 0));
		image->setVisible(true);
	}

	if (mNormalImage) 
	{
		removeChild(mNormalImage, true);
	}

	mNormalImage = image;
}
//--------------------------------------------------------------------
CCSprite* FKCW_UI_MenuItemProgressTimer::getDisabledImage() 
{
	return mDisabledImage;
}
//--------------------------------------------------------------------
void FKCW_UI_MenuItemProgressTimer::setDisabledImage(CCSprite* image) 
{        
	if (image) 
	{
		addChild(image, 0, kDisableTag);
		image->setAnchorPoint(ccp(0, 0));
		image->setVisible(true);
	}

	if (mDisabledImage) 
	{
		removeChild(mDisabledImage, true);
	}

	mDisabledImage = image;
}
//--------------------------------------------------------------------
void FKCW_UI_MenuItemProgressTimer::setMaskImage(CCSprite* image) 
{
	mMaskImage = image;
	mProgressTimer->initWithSprite(mMaskImage);
}
//--------------------------------------------------------------------
CCSprite* FKCW_UI_MenuItemProgressTimer::getMaskImage()
{
	return mMaskImage;
}
//--------------------------------------------------------------------
CCProgressTimer* FKCW_UI_MenuItemProgressTimer::getProgressTimer() 
{
	return mProgressTimer;
}
//--------------------------------------------------------------------
void FKCW_UI_MenuItemProgressTimer::setProgressTimer(CCProgressTimer* timer) 
{
	if(mProgressTimer)
		removeChild(mProgressTimer, true);
	mProgressTimer = timer;
	this->addChild(mProgressTimer);

	mProgressTimer->setSprite(mNormalImage);
	mProgressTimer->setPercentage(0);
	mProgressTimer->setType(mType);
}
//--------------------------------------------------------------------
FKCW_UI_MenuItemProgressTimer* FKCW_UI_MenuItemProgressTimer::Create(CCSprite* mask, CCSprite* normalSprite, float interval, CCProgressTimerType type, CCSprite* disabledSprite) 
{
	FKCW_UI_MenuItemProgressTimer* ptimer = new FKCW_UI_MenuItemProgressTimer();
	if(ptimer->_Init(mask, normalSprite, 0, interval, type, 0, 0)) 
	{
		ptimer->autorelease();
		return ptimer;
	}
	delete ptimer;
	return 0;
}
//--------------------------------------------------------------------
FKCW_UI_MenuItemProgressTimer* FKCW_UI_MenuItemProgressTimer::Create(CCSprite* mask, CCSprite* normalSprite,
		float interval, CCProgressTimerType type, CCObject* target, SEL_MenuHandler selector) 
{
	FKCW_UI_MenuItemProgressTimer* ptimer = new FKCW_UI_MenuItemProgressTimer();
	if(ptimer->_Init(mask, normalSprite, 0, interval, type, target, selector)) 
	{
		ptimer->autorelease();
		return ptimer;
	}
	delete ptimer;
	return 0;
}
//--------------------------------------------------------------------
FKCW_UI_MenuItemProgressTimer* FKCW_UI_MenuItemProgressTimer::Create(CCSprite* mask, CCSprite* normalSprite,
		CCSprite* disabledSprite, float interval, CCProgressTimerType type, CCObject* target, SEL_MenuHandler selector)
{
	FKCW_UI_MenuItemProgressTimer* ptimer = new FKCW_UI_MenuItemProgressTimer();
	if(ptimer->_Init(mask, normalSprite, disabledSprite, interval, type, target, selector)) 
	{
		ptimer->autorelease();
		return ptimer;
	}
	delete ptimer;
	return 0;
}
//--------------------------------------------------------------------
bool FKCW_UI_MenuItemProgressTimer::_Init(CCSprite* mask, CCSprite* normalSprite, CCSprite* disabledSprite,
			float interval, CCProgressTimerType type, CCObject* target, SEL_MenuHandler selector)
{
	assert(normalSprite);

	CCMenuItem::initWithTarget(target, selector);

	setNormalImage(normalSprite);
	setDisabledImage(disabledSprite);

	mProgressTimer = new CCProgressTimer();
	mInterval = interval;
	setMaskImage(mask);
	mProgressTimer->setPosition( ccp(normalSprite->getContentSize().width / 2,
		normalSprite->getContentSize().height / 2) );
	mProgressTimer->setType(type);
	mProgressTimer->setPercentage(0);

	this->addChild(mProgressTimer);

	mType = type;
	this->setContentSize(normalSprite->getContentSize());

	return true;
}
//--------------------------------------------------------------------
void FKCW_UI_MenuItemProgressTimer::setColor(const ccColor3B& color) 
{
	dynamic_cast<CCRGBAProtocol*>(mNormalImage)->setColor(color);

	if (mDisabledImage)
	{
		dynamic_cast<CCRGBAProtocol*>(mDisabledImage)->setColor(color);
	}

}
//--------------------------------------------------------------------
const ccColor3B& FKCW_UI_MenuItemProgressTimer::getColor()
{
	return dynamic_cast<CCRGBAProtocol*>(mNormalImage)->getColor();

}
//--------------------------------------------------------------------
void FKCW_UI_MenuItemProgressTimer::setOpacity(GLubyte opacity) 
{
	dynamic_cast<CCRGBAProtocol*>(mNormalImage)->setOpacity(opacity);

	if (mDisabledImage)
	{
		dynamic_cast<CCRGBAProtocol*>(mDisabledImage)->setOpacity(opacity);
	}    
}
//--------------------------------------------------------------------
GLubyte FKCW_UI_MenuItemProgressTimer::getOpacity()
{
	return dynamic_cast<CCRGBAProtocol*>(mNormalImage)->getOpacity();
}
//--------------------------------------------------------------------
void FKCW_UI_MenuItemProgressTimer::activate() 
{
	if(mCanBeActivated) {
		mProgressTimer->setPercentage(100);

		mCurrent = 0.f;
		mCanBeActivated = false;

		this->schedule(schedule_selector(FKCW_UI_MenuItemProgressTimer::_OnTimerTick),
			static_cast<float>(CCDirector::sharedDirector()->getAnimationInterval()));
		CCMenuItem::activate();
	}
}
//--------------------------------------------------------------------
void FKCW_UI_MenuItemProgressTimer::PreventTimer() 
{
	this->unschedule(schedule_selector(FKCW_UI_MenuItemProgressTimer::_OnTimerTick));

	mCurrent = 0.f;
	mCanBeActivated = true;
	mProgressTimer->setPercentage(0);
}
//--------------------------------------------------------------------
void FKCW_UI_MenuItemProgressTimer::selected() 
{
	CCMenuItem::selected();
}
//--------------------------------------------------------------------
void FKCW_UI_MenuItemProgressTimer::unselected() 
{
	CCMenuItem::unselected();
}
//--------------------------------------------------------------------
void FKCW_UI_MenuItemProgressTimer::setIsEnabled(bool bEnabled) 
{
	CCMenuItem::setEnabled(bEnabled);

	if(!bEnabled) {
		mProgressTimer->setVisible(false);
	} else {
		mProgressTimer->setVisible(true);
	}

	if (bEnabled)
	{
		mNormalImage->setVisible(true);

		if (mDisabledImage)
		{
			mDisabledImage->setVisible(false);
		}
	}
	else
	{
		if (mDisabledImage)
		{
			mDisabledImage->setVisible(true);
			mNormalImage->setVisible(false);
		}
		else
		{
			mNormalImage->setVisible(true);
		}
	}
}
//--------------------------------------------------------------------
void FKCW_UI_MenuItemProgressTimer::_OnTimerTick(float dt) 
{
	mCurrent += dt;
	if(mCurrent >= mInterval)
	{
		this->unschedule(schedule_selector(FKCW_UI_MenuItemProgressTimer::_OnTimerTick));
		mCanBeActivated = true;
	}
	mProgressTimer->setPercentage((1.f - mCurrent / mInterval) * 100.f);
}
//--------------------------------------------------------------------