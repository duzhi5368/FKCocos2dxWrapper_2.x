//*************************************************************************
//	创建日期:	2014-12-4
//	文件名称:	FKCW_UIWidget_Layout.h
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
class FKCW_UIWidget_Layout : public CCNodeRGBA, public FKCW_UIWidget_Widget, public FKCW_UIWidget_LayoutableProtocol
{
public:
	FKCW_UIWidget_Layout();
	virtual ~FKCW_UIWidget_Layout();
	virtual bool init();
	static FKCW_UIWidget_Layout* create();
	static FKCW_UIWidget_Layout* create(const CCSize& tContentSize);
	virtual void setContentSize(const CCSize& tContentSize);

	CCObject* findWidgetById(const char* id);

	FKCW_UIWIDGET_BACKGROUND;

public:
	virtual ENUM_WidgetTouchModel onTouchBegan(CCTouch* pTouch);
	virtual void onTouchMoved(CCTouch* pTouch, float fDuration);
	virtual void onTouchEnded(CCTouch* pTouch, float fDuration);
	virtual void onTouchCancelled(CCTouch* pTouch, float fDuration);

protected:
	CCObject* find(CCArray* pChidren, const char* id);

};
