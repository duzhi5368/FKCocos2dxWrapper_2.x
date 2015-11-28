//*************************************************************************
//	创建日期:	2014-12-5
//	文件名称:	FKCW_UIWidget_ImageViewScale9.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "cocos2d.h"
#include "FKCW_UIWidget_Widget.h"
#include "FKCW_UIWidget_Protocol.h"
#include "FKCW_UIWidget_Scale9Sprite.h"
//-------------------------------------------------------------------------
USING_NS_CC;
//-------------------------------------------------------------------------
class FKCW_UIWidget_ImageViewScale9 : public FKCW_UIWidget_Scale9Sprite
	, public FKCW_UIWidget_Widget
	, public FKCW_UIWidget_ClickableProtocol
	, public FKCW_UIWidget_LongClickableProtocol
{
public:
	FKCW_UIWidget_ImageViewScale9();
	virtual bool init();
	static FKCW_UIWidget_ImageViewScale9* create(const CCSize& size, const char* file);
	static FKCW_UIWidget_ImageViewScale9* create(const char* file, CCRect rect,  CCRect capInsets);
	static FKCW_UIWidget_ImageViewScale9* create(const char* file, CCRect rect);
	static FKCW_UIWidget_ImageViewScale9* create(CCRect capInsets, const char* file);
	static FKCW_UIWidget_ImageViewScale9* create(const char* file);
	static FKCW_UIWidget_ImageViewScale9* createWithSpriteFrame(CCSpriteFrame* spriteFrame, CCRect capInsets);
	static FKCW_UIWidget_ImageViewScale9* createWithSpriteFrame(CCSpriteFrame* spriteFrame); 
	static FKCW_UIWidget_ImageViewScale9* createWithSpriteFrameName(const char*spriteFrameName, CCRect capInsets);
	static FKCW_UIWidget_ImageViewScale9* createWithSpriteFrameName(const char*spriteFrameName);
	static FKCW_UIWidget_ImageViewScale9* createWithTexture(CCTexture2D* pTexture);
	static FKCW_UIWidget_ImageViewScale9* createWithTexture(CCTexture2D* pTexture, CCRect capInsets);
	static FKCW_UIWidget_ImageViewScale9* create();

public:
	virtual ENUM_WidgetTouchModel onTouchBegan(CCTouch *pTouch);
	virtual void onTouchMoved(CCTouch *pTouch, float fDuration);
	virtual void onTouchEnded(CCTouch *pTouch, float fDuration);
	virtual void onTouchCancelled(CCTouch *pTouch, float fDuration);

	FKCW_UIWIDGET_LONGCLICK_SCHEDULE(FKCW_UIWidget_ImageViewScale9);
};