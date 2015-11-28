//*************************************************************************
//	创建日期:	2014-12-5
//	文件名称:	FKCW_UIWidget_ProgressBar.h
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
#include "FKCW_UIWidget_GradientView.h"
//-------------------------------------------------------------------------
USING_NS_CC;
//-------------------------------------------------------------------------
enum ENUM_ProgressBarDirection
{
	eProgressBarDirectionLeftToRight,
	eProgressBarDirectionRightToLeft,
	eProgressBarDirectionBottomToTop,
	eProgressBarDirectionTopToBottom
};
//-------------------------------------------------------------------------
class FKCW_UIWidget_ProgressBar : public CCNodeRGBA
, public FKCW_UIWidget_ProgressEndedProtocol
, public FKCW_UIWidget_ValueChangeableProtocol
{
public:
	FKCW_UIWidget_ProgressBar();
	virtual ~FKCW_UIWidget_ProgressBar();
	virtual bool init();
	virtual bool initWithFile(const char* pProgress);
	static FKCW_UIWidget_ProgressBar* create();
	static FKCW_UIWidget_ProgressBar* create(const char* pProgress);

	virtual void setContentSize(const CCSize& tSize);
	void setValue(int nValue);
	int getValue() const;
	void setMaxValue(int nMaxValue);
	int getMaxValue() const;
	void setMinValue(int nMinValue);
	int getMinValue() const;
	void setDirection(ENUM_ProgressBarDirection eDirection);
	ENUM_ProgressBarDirection getDirection() const;
	void startProgress(int nValue, float fDuration);
	void startProgressFromTo(int nFromValue, int nToValue, float fDuration);
	void stopProgress();
	float getPercentage() const;
	bool isProgressEnded() const;

	virtual void setProgressImage(const char* pFile);
	virtual void setProgressTexture(CCTexture2D* pTexture);
	virtual void setProgressSpriteFrame(CCSpriteFrame* pFrame);
	virtual void setProgressSpriteFrameName(const char* pSpriteName);

	FKCW_UIWIDGET_BACKGROUND;
	
protected:
	void performProgressing();
	void onProgressing(float dt);
	void stoppedProgressing();

	void getScissorRectByPercentage(CCRect& tRect);
	virtual void changeValueAndExecuteEvent(int nValue, bool bExeEvent);	

protected:
	int m_nMaxValue;
	int m_nMinValue;
	int m_nValue;

	bool m_bProgressing;

	bool m_bFirstTick;
	float m_fLapsed;
	float m_fDuration;
	int m_nToValue;
	int m_nFromValue;

	CCSize m_tProgressSize;
	CCPoint m_tCenterPoint;

	CCSprite* m_pProgressSprite;
	ENUM_ProgressBarDirection m_eDirection;
};
