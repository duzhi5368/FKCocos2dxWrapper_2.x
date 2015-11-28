//*************************************************************************
//	创建日期:	2014-11-5
//	文件名称:	FKCW_UI_MenuItemColorStateListener.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_UI_Macros.h"
//-------------------------------------------------------------------------
// 用来监听 FKCW_UI_MenuItemColor 控件事件的接口类
// 用户多数情况下不需要使用该类，它被FKCW_UI_MenuItemColor内部使用。
class FKCW_UI_MenuItemColor;
class FKCW_UI_MenuItemColorStateListener
{
public:
	virtual void onMenuItemColorSelected(FKCW_UI_MenuItemColor* item) = 0;
	virtual void onMenuItemColorDeselected(FKCW_UI_MenuItemColor* item) = 0;
	virtual void onMenuItemColorFocused(FKCW_UI_MenuItemColor* item) = 0;
	virtual void onMenuItemColorUnfocused(FKCW_UI_MenuItemColor* item) = 0;
	virtual void onMenuItemColorEnabled(FKCW_UI_MenuItemColor* item) = 0;
	virtual void onMenuItemColorDisabled(FKCW_UI_MenuItemColor* item) = 0;
};