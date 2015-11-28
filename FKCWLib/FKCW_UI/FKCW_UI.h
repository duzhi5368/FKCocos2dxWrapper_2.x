//*************************************************************************
//	创建日期:	2014-11-5
//	文件名称:	FKCW_UI.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
/* 
====================================
模块说明：	
====================================
	对Cocos2dx的UI扩展库，添加了一些UI控件
====================================
内部文件功能说明：
====================================
	FKCW_UI_AutoRenderMenuItemSprite 
		封装的菜单选项精灵。选中和取消选中状态可以通过内部render进行自动亮暗变色，一张图片即可。
	FKCW_UI_MenuItemColor
		封装的菜单选项精灵。纯颜色的，不需要纹理。
	FKCW_UI_ToastLayer
		类似dialog的信息窗口，但不占有焦点，且过一段时间会自动消失。有些类似Tips。
	FKCW_UI_RookieGuideLayer
		新手引导层。就是在游戏画面之上加了一层，层的指定区域为透明可以看到下方，并可以接受点击消息进行切换下一个区域。
	FKCW_UI_SimpleRookieGuiderLayer
		新手引导层。继承于FKCW_UI_RookieGuideLayer，它添加了一个箭头和一个Lable协助进行用户提示，并添加了渐入渐出效果。
	FKCW_UI_ProgressIndicator
		一个进度提示器（和进度条不同的是，该进度提示器是无法确认进度长度时间的，该进度提示是无限循环的），自带一个信息提示label。
	FKCW_UI_LayerPanZoom
		一个自识别并支持缩放滑动的层。
	FKCW_UI_TableView
		带滚动条的TableView（文字标签纵向列表）
	FKCW_UI_MenuItemProgressTimer
		带进度时间的菜单选项（多用于带自动CD的技能图标）
	FKCW_UI_ScrollableLayer
		允许进行滑动，并显示其中一部分的Layer
	FKCW_UI_Table
		表格式UI
	FKCW_UI_DynamicNumLabel
		动态更换数字Label
====================================
依赖项目/库：
====================================
	FKCW_Base
	FKCW_Action
====================================
todo:
====================================
	RichEdit
*/ 
//-------------------------------------------------------------------------
#include "Include/FKCW_UI_AutoRenderMenuItemSprite.h"
#include "Include/FKCW_UI_MenuItemColor.h"
#include "Include/FKCW_UI_ToastLayer.h"
#include "Include/FKCW_UI_RookieGuideLayer.h"
#include "Include/FKCW_UI_SimpleRookieGuiderLayer.h"
#include "Include/FKCW_UI_ProgressIndicator.h"
#include "Include/FKCW_UI_PanZoomLayer.h"
#include "Include/FKCW_UI_TableView.h"
#include "Include/FKCW_UI_MenuItemProgressTimer.h"
#include "Include/FKCW_UI_ScrollableLayer.h"
#include "Include/FKCW_UI_Table.h"
#include "Include/FKCW_UI_DynamicNumLabel.h"
//-------------------------------------------------------------------------