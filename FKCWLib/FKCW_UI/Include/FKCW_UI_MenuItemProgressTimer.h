//*************************************************************************
//	创建日期:	2014-11-14
//	文件名称:	FKCW_UI_MenuItemProgressTimer.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	带进度时间的菜单选项（多用于带自动CD的技能图标）
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_UI_Macros.h"
//-------------------------------------------------------------------------
class FKCW_UI_MenuItemProgressTimer : public CCMenuItem
{
	CC_PROPERTY(CCProgressTimer*, mProgressTimer, ProgressTimer);
	CC_PROPERTY(CCSprite*, mNormalImage, NormalImage);
	CC_PROPERTY(CCSprite*, mMaskImage, MaskImage);
	CC_PROPERTY(CCSprite*, mDisabledImage, DisabledImage);

public:
	FKCW_UI_MenuItemProgressTimer();
	virtual ~FKCW_UI_MenuItemProgressTimer();

public:
	static FKCW_UI_MenuItemProgressTimer* Create(CCSprite* mask, CCSprite* normalSprite, float interval, CCProgressTimerType type, CCSprite* disabledSprite = NULL);
	static FKCW_UI_MenuItemProgressTimer* Create(CCSprite* mask, CCSprite* normalSprite, float interval, CCProgressTimerType type, CCObject* target, SEL_MenuHandler selector);
	static FKCW_UI_MenuItemProgressTimer* Create(CCSprite* mask, CCSprite* normalSprite, CCSprite* disabledSprite, float interval, CCProgressTimerType type, CCObject* target, SEL_MenuHandler selector);

public:
	virtual void				setColor(const ccColor3B& color);
	virtual const ccColor3B&	getColor();
	virtual void				setOpacity(GLubyte opacity);
	virtual GLubyte				getOpacity();
	virtual void				setIsOpacityModifyRGB(bool bValue) {CC_UNUSED_PARAM(bValue);}
	virtual bool				getIsOpacityModifyRGB(void) { return false;}

	virtual void				activate();
	virtual void				selected();
	virtual void				unselected();
	virtual void				setIsEnabled(bool bEnabled);

public:
	// 激活计时器后防止其重复激活
	void						PreventTimer();

protected:
	bool						_Init(CCSprite* mask, CCSprite* normalSprite, CCSprite* disabledSprite,
		float interval, CCProgressTimerType type, CCObject* target, SEL_MenuHandler selector);
	void						_OnTimerTick(float dt);

	float						mInterval;
	float						mCurrent;
	bool						mCanBeActivated;
	CCProgressTimerType			mType;
};