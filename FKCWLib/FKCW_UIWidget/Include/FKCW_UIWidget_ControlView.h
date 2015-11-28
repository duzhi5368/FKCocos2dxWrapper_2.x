//*************************************************************************
//	创建日期:	2014-12-5
//	文件名称:	FKCW_UIWidget_ControlView.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "cocos2d.h"
#include "FKCW_UIWidget_Widget.h"
#include "FKCW_UIWidget_Protocol.h"
//-------------------------------------------------------------------------
USING_NS_CC;
//-------------------------------------------------------------------------
class FKCW_UIWidget_ControlView : public CCNodeRGBA, 
	public FKCW_UIWidget_Widget, public FKCW_UIWidget_ControlableProtocol
{
public:
	FKCW_UIWidget_ControlView();
	virtual ~FKCW_UIWidget_ControlView();
	virtual bool init();
	virtual bool initWithFile(const char* pBaseBoard, const char* pJoystick);
	static FKCW_UIWidget_ControlView* create();
	static FKCW_UIWidget_ControlView* create(const char* pBaseBoard, const char* pJoystick);

	virtual void setContentSize(const CCSize& tContentSize);
	void setRadius(float fRadius);
	float getRadius() const;
	void setRelocateWithAnimation(bool bAni);
	bool isRelocateWithAnimation() const;

	virtual void setBaseBoardImage(const char* pFile);
	virtual void setJoystickImage(const char* pFile);
	virtual void setBaseBoardSpriteFrame(CCSpriteFrame* pFrame);
	virtual void setJoystickSpriteFrame(CCSpriteFrame* pFrame);
	virtual void setBaseBoardTexture(CCTexture2D *pTexture);
	virtual void setJoystickTexture(CCTexture2D *pTexture);
	virtual void setBaseBoardSpriteFrameName(const char* pSpriteName);
	virtual void setJoystickSpriteFrameName(const char* pSpriteName);

public:
	virtual ENUM_WidgetTouchModel onTouchBegan(CCTouch *pTouch);
	virtual void onTouchMoved(CCTouch *pTouch, float fDuration);
	virtual void onTouchEnded(CCTouch *pTouch, float fDuration);
	virtual void onTouchCancelled(CCTouch *pTouch, float fDuration);

protected:
	void relocateJoystick(bool bAni);
	virtual void onAnimateUpdate(float dt);
	void performAnimateUpdate();
	void stopAnimateUpdate();
	virtual void onExecuteEventUpdate(float dt);
	void performExecuteUpdate();
	void stopExecuteUpdate();

protected:
	CCSprite* m_pBaseBoard;
	CCSprite* m_pJoystick;
	float m_fRadius;

	bool m_bExecuteEventUpdate;
	bool m_bAnimationUpdate;
	bool m_bRelocateWithAnimation;
	CCPoint m_tCenterPoint;
	CCPoint m_tLastPoint;
};
