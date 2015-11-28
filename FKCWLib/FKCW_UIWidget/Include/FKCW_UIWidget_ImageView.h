//*************************************************************************
//	创建日期:	2014-12-5
//	文件名称:	FKCW_UIWidget_ImageView.h
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
class FKCW_UIWidget_ImageView : public CCSprite
	, public FKCW_UIWidget_Widget
	, public FKCW_UIWidget_ClickableProtocol
	, public FKCW_UIWidget_LongClickableProtocol
{
public:
	FKCW_UIWidget_ImageView();
	virtual bool init();
	static FKCW_UIWidget_ImageView* create();
	static FKCW_UIWidget_ImageView* create(const char *pszFileName);
	static FKCW_UIWidget_ImageView* create(const char *pszFileName, const CCRect& rect);
	static FKCW_UIWidget_ImageView* createWithTexture(CCTexture2D *pTexture);
	static FKCW_UIWidget_ImageView* createWithTexture(CCTexture2D *pTexture, const CCRect& rect);
	static FKCW_UIWidget_ImageView* createWithSpriteFrame(CCSpriteFrame *pSpriteFrame);
	static FKCW_UIWidget_ImageView* createWithSpriteFrameName(const char *pszSpriteFrameName);

public:
	virtual ENUM_WidgetTouchModel onTouchBegan(CCTouch *pTouch);
	virtual void onTouchMoved(CCTouch *pTouch, float fDuration);
	virtual void onTouchEnded(CCTouch *pTouch, float fDuration);
	virtual void onTouchCancelled(CCTouch *pTouch, float fDuration);

	FKCW_UIWIDGET_LONGCLICK_SCHEDULE(FKCW_UIWidget_ImageView);
};