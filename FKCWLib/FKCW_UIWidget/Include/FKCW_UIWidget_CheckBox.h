//*************************************************************************
//	创建日期:	2014-12-5
//	文件名称:	FKCW_UIWidget_CheckBox.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "cocos2d.h"
#include "FKCW_UIWidget_Widget.h"
#include "FKCW_UIWidget_Protocol.h"
#include "FKCW_UIWidget_Button.h"
//-------------------------------------------------------------------------
USING_NS_CC;
//-------------------------------------------------------------------------
class FKCW_UIWidget_CheckBox : public CCNodeRGBA
	, public FKCW_UIWidget_Widget
	, public FKCW_UIWidget_CheckableProtocol
	, public FKCW_UIWidget_ClickableProtocol
	, public FKCW_UIWidget_LongClickableProtocol
{
public:
	FKCW_UIWidget_CheckBox();
	virtual ~FKCW_UIWidget_CheckBox();
	virtual bool init();
	static FKCW_UIWidget_CheckBox* create();

	virtual void setChecked(bool bChecked);
	virtual void setEnabled(bool bEnabled);
	virtual void setContentSize(const CCSize& tContentSize);

	virtual void setNormalImage(const char* pFile);
	virtual void setNormalPressImage(const char* pFile);
	virtual void setCheckedImage(const char* pFile);
	virtual void setCheckedPressImage(const char* pFile);
	virtual void setDisabledNormalImage(const char* pFile);
	virtual void setDisabledCheckedImage(const char* pFile);

	virtual void setNormalSpriteFrame(CCSpriteFrame* pFrame);
	virtual void setNormalPressSpriteFrame(CCSpriteFrame* pFrame);
	virtual void setCheckedSpriteFrame(CCSpriteFrame* pFrame);
	virtual void setCheckedPressSpriteFrame(CCSpriteFrame* pFrame);
	virtual void setDisabledNormalSpriteFrame(CCSpriteFrame* pFrame);
	virtual void setDisabledCheckedSpriteFrame(CCSpriteFrame* pFrame);

	virtual void setNormalTexture(CCTexture2D* pTexture);
	virtual void setNormalPressTexture(CCTexture2D* pTexture);
	virtual void setCheckedTexture(CCTexture2D* pTexture);
	virtual void setCheckedPressTexture(CCTexture2D* pTexture);
	virtual void setDisabledNormalTexture(CCTexture2D* pTexture);
	virtual void setDisabledCheckedTexture(CCTexture2D* pTexture);

	virtual void setNormalSpriteFrameName(const char* pSpriteName);
	virtual void setNormalPressSpriteFrameName(const char* pSpriteName);
	virtual void setCheckedSpriteFrameName(const char* pSpriteName);
	virtual void setCheckedPressSpriteFrameName(const char* pSpriteName);
	virtual void setDisabledNormalSpriteFrameName(const char* pSpriteName);
	virtual void setDisabledCheckedSpriteFrameName(const char* pSpriteName);

public:
	virtual ENUM_WidgetTouchModel onTouchBegan(CCTouch* pTouch);
	virtual void onTouchMoved(CCTouch* pTouch, float fDuration);
	virtual void onTouchEnded(CCTouch* pTouch, float fDuration);
	virtual void onTouchCancelled(CCTouch* pTouch, float fDuration);

	FKCW_UIWIDGET_LONGCLICK_SCHEDULE(FKCW_UIWidget_CheckBox);

protected:
	CCSprite* m_pNormal;
	CCSprite* m_pNormalPress;
	CCSprite* m_pChecked;
	CCSprite* m_pCheckedPress;
	CCSprite* m_pDisabledNormal;
	CCSprite* m_pDisabledChecked;
};
//-------------------------------------------------------------------------