//*************************************************************************
//	创建日期:	2014-11-5
//	文件名称:	FKCW_RenderEx.h
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
	在渲染部分，对cocos2dx的加强库，同时增加了一些效果
====================================
内部文件功能说明：
====================================
	FKCW_RenderEx_PredrawSprite
		加强版的Sprite，支持预渲染和渲染模式的记录
	FKCW_RenderEx_PredrawArmature
		加强版的Armature，支持预渲染和渲染模式的记录
	FKCW_RenderEx_GradientSprite
		可自动渐变色的精灵
	FKCW_RenderEx_TiledSprite
		自动平铺精灵
	FKCW_RenderEx_CatmullRomSprite
		笛卡尔差值扭曲精灵
	FKCW_RenderEx_LaserSprite
		镭射激光效果精灵
	FKCW_RenderEx_Texture2DMutable
		加强版的纹理，可以实现后期渲染处理，例如取得某点的颜色，设置某些点的颜色等，注意，这里的数据保存在内存中，将额外占用内存。
	FKCW_RenderEX_RenderTextureMutable
		加强版的渲染纹理，实现 FKCW_RenderEx_Texture2DMutable 的封装。自身带有保存为图片文件等功能。
	FKCW_RenderEx_TurnCardSprite
		翻卡效果精灵
	FKCW_RenderEx_EraserSprite
		橡皮擦擦除精灵
	FKCW_RenderEx_ScrollingBackgroundNode
		可四项滚动的背景节点，上面可挂接各种子节点。可无尽循环
	FKCW_RenderEx_NumberCountNode
		可变速跳转的数字翻动精灵
	FKCW_RenderEx_JoystickNode
		虚拟摇杆类，可自定义事件处理函数
	FKCW_RenderEx_SimpleShaderColorPainter
		shader进行简单的node颜色修正涂色器，单件对象
	FKCW_RenderEx_SwayNode
		可以上部摇晃的精灵，通常做一些草，影子等对象
	FKCW_RenderEx_BlurTextureFilter
		纹理模糊器
	FKCW_RenderEx_SpotLightSprite
		聚光灯效果
====================================
依赖项目/库：
====================================
	FKCW_Base
	FKCW_Shader
====================================
todo:
====================================

*/
//-------------------------------------------------------------------------
#include "Include/FKCW_RenderEx_PredrawSprite.h"
#include "Include/FKCW_RenderEx_PredrawArmature.h"
#include "Include/FKCW_RenderEx_GradientSprite.h"
#include "Include/FKCW_RenderEx_TiledSprite.h"
#include "Include/FKCW_RenderEx_CatmullRomSprite.h"
#include "Include/FKCW_RenderEx_LaserSprite.h"
#include "Include/FKCW_RenderEx_Texture2DMutable.h"
#include "Include/FKCW_RenderEx_RenderTextureMutable.h"
#include "Include/FKCW_RenderEx_TurnCardSprite.h"
#include "Include/FKCW_RenderEx_EraserSprite.h"
#include "Include/FKCW_RenderEx_ScrollingBackgroundNode.h"
#include "Include/FKCW_RenderEx_NumberCountNode.h"
#include "Include/FKCW_RenderEx_JoystickNode.h"
#include "Include/FKCW_RenderEx_SimpleShaderColorPainter.h"
#include "Include/FKCW_RenderEx_SwayNode.h"
#include "Include/FKCW_RenderEx_BlurTextureFilter.h"
#include "Include/FKCW_RenderEx_SpotLightSprite.h"