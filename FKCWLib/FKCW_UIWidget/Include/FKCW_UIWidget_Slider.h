//*************************************************************************
//	创建日期:	2014-12-5
//	文件名称:	FKCW_UIWidget_Slider.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "cocos2d.h"
#include "FKCW_UIWidget_Widget.h"
#include "FKCW_UIWidget_Protocol.h"
#include "FKCW_UIWidget_ColorView.h"
#include "FKCW_UIWidget_ProgressBar.h"
//-------------------------------------------------------------------------
USING_NS_CC;
//-------------------------------------------------------------------------
class FKCW_UIWidget_Slider : public FKCW_UIWidget_ProgressBar, public FKCW_UIWidget_Widget
{
public:
	FKCW_UIWidget_Slider();
	virtual ~FKCW_UIWidget_Slider();
	virtual bool initWithSlider(const char* pSlider, const char* pProgress);
	static FKCW_UIWidget_Slider* create();
	static FKCW_UIWidget_Slider* create(const char* pSlider, const char* pProgress);

	virtual void setContentSize(const CCSize& tSize);
	virtual void setSliderImage(const char* pFile);
	virtual void setSliderTexture(CCTexture2D* pTexture);
	virtual void setSliderSpriteFrame(CCSpriteFrame* pFrame);
	virtual void setSliderSpriteFrameName(const char* pSpriteName);

public:
	virtual ENUM_WidgetTouchModel onTouchBegan(CCTouch *pTouch);
	virtual void onTouchMoved(CCTouch *pTouch, float fDuration);
	virtual void onTouchEnded(CCTouch *pTouch, float fDuration);
	virtual void onTouchCancelled(CCTouch *pTouch, float fDuration);

protected:
	int valueFromPoint(const CCPoint& tPoint);
	int valueFromPercent(float fPercentage);
	void pointFromValue(int nValue, CCPoint& tOutPoint);

protected:
	virtual void changeValueAndExecuteEvent(int nValue, bool bExeEvent);

protected:
	bool m_bDrag;
	CCSprite* m_pSlider;
};