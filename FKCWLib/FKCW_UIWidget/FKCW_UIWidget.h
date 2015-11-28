//*************************************************************************
//	创建日期:	2014-11-28
//	文件名称:	FKCW_UIWidget.h
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
	对Cocos2dx的UI扩展库，添加了一系列UI控件组
====================================
内部文件功能说明：
====================================
	FKCW_UIWidget_Protocol
		组件消息处理基类
	FKCW_UIWidget_Widget
		组件基类
	FKCW_UIWidget_Scale9Sprite
		九向连续精灵支持
	FKCW_UIWidget_ColorView
		色块
	FKCW_UIWidget_GradientView
		渐变色块

	子组件
		FKCW_UIWidget_WidgetWindow
			组件窗口类
		FKCW_UIWidget_Label
			静态标签类
		FKCW_UIWidget_LabelAtlas
			静态标签集，减少消耗
		FKCW_UIWidget_LabelBMFont
			使用贴图式的静态标签
		FKCW_UIWidget_Button
			按钮类
		FKCW_UIWidget_ToggleView
			开关按钮类
		FKCW_UIWidget_Layout
			容器层，控件树根控件
		FKCW_UIWidget_ScrollView
			滑动容器基类
		FKCW_UIWidget_TableView
			垂直表单
		FKCW_UIWidget_GridPageView
			网格页容器表单（横纵向）
		FKCW_UIWidget_GridView
			网格容器控件
		FKCW_UIWidget_ImageView
			图片控件
		FKCW_UIWidget_ImageViewScale9
			九宫图片控件
		FKCW_UIWidget_PageView
			页面滑动的容器控件
		FKCW_UIWidget_ListView
			链表滑动容器
		FKCW_UIWidget_CheckBox
			选择框
		FKCW_UIWidget_ControlView
			游戏操作杆
		FKCW_UIWidget_ProgressBar
			进度条
		FKCW_UIWidget_Slider
			滑动块
		FKCW_UIWidget_TextRich
			富文本
		FKCW_UIWidget_ExpandableListView
			可展开搜索的列表容器
====================================
依赖项目/库：
====================================
	FKCW_Base
====================================
todo:
====================================
	Lua支持，使用ToLua++
*/ 
//-------------------------------------------------------------------------
#include "Include/FKCW_UIWidget_Protocol.h"
#include "Include/FKCW_UIWidget_Widget.h"
#include "Include/FKCW_UIWidget_Scale9Sprite.h"
#include "Include/FKCW_UIWidget_ColorView.h"
#include "Include/FKCW_UIWidget_GradientView.h"

#include "Include/FKCW_UIWidget_WidgetWindow.h"
#include "Include/FKCW_UIWidget_Label.h"
#include "Include/FKCW_UIWidget_LabelAtlas.h"
#include "Include/FKCW_UIWidget_LabelBMFont.h"
#include "Include/FKCW_UIWidget_Button.h"
#include "Include/FKCW_UIWidget_ToggleView.h"
#include "Include/FKCW_UIWidget_Layout.h"
#include "Include/FKCW_UIWidget_ScrollView.h"
#include "Include/FKCW_UIWidget_TableView.h"
#include "Include/FKCW_UIWidget_GridPageView.h"
#include "Include/FKCW_UIWidget_GridView.h"
#include "Include/FKCW_UIWidget_ImageView.h"
#include "Include/FKCW_UIWidget_ImageViewScale9.h"
#include "Include/FKCW_UIWidget_PageView.h"
#include "Include/FKCW_UIWidget_ListView.h"
#include "Include/FKCW_UIWidget_CheckBox.h"
#include "Include/FKCW_UIWidget_ControlView.h"
#include "Include/FKCW_UIWidget_ProgressBar.h"
#include "Include/FKCW_UIWidget_Slider.h"
#include "Include/FKCW_UIWidget_TextRich.h"
#include "Include/FKCW_UIWidget_ExpandableListView.h"