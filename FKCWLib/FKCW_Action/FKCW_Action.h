/********************************************************************
	created:	2014/11/04
	file base:	FKCW_Action
	author:		FreeKnight
	
	purpose:	
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
/* 
====================================
模块说明：	
====================================
	封装的一些便利的动作和一些持续更变的效果。
====================================
内部文件功能说明：
====================================
	FKCW_Action_CallfuncT
		一个回调函数封装模板类，继承自CCCallFunc，可允许回调函数传入一个任意类型的参数。
	FKCW_Action_CallfuncNT
		一个回调函数封装模板类，继承自CCCallFunc，可允许回调函数传入一个节点对象和一个任意类型的参数。
	FKCW_Action_ClipIn
		从一个方向逐渐显示一个对象。原理是上面设置一个用户定义的遮罩层，移动该遮罩层。
	FKCW_Action_ClipOut
		从一个方向逐隐藏一个对象。原理是上面设置一个用户定义的遮罩层，移动该遮罩层。
	FKCW_Action_TreeFadeIn
		渐显一个对象。和CCFadeIn动作类似，但建议使用本动作，因为本动作会自动对对象子节点都进行渐显操作。
		但 CCFadeIn动作不会对子节点作用。
	FKCW_Action_TreeFadeOut
		渐隐一个对象。参见 FKCW_Action_TreeFadeIn 描述。
	FKCW_Action_Flash
		闪烁动作。让一个节点单位进行闪烁变色。
	FKCW_Action_Missile
		追击动作。让节点按一定角度的导弹轨迹自动追击另一个节点。
	FKCW_Action_Shake
		震动动作。让节点在一定范围圆内随机位移，造成震动效果。
	FKCW_Action_Shine
		动态光线动作。在一个节点上显示一系列光线效果。光线会成梯度移动，并可被设置为最多三种颜色。
	FKCW_Action_Blur
		模糊动作。对一个节点添加模糊效果，可以动态的更新模糊的模糊度。
	FKCW_Action_Count
		数字翻动动作。该动作只能Label调用。
	FKCW_Action_TrailMoveTo
		带拖尾残影的移动动作。节点移动到一个指定目标，同时身后保留一系列残影效果。
	FKCW_Action_Curl
		螺旋旋转动作。
	FKCW_Action_Suck
		黑洞吸入效果
====================================
依赖项目/库：
====================================
	FKCW_Base
	FKCW_Shader
	FKCW_RenderEx
====================================
todo:
====================================

*/ 
//--------------------------------------------------------------------
#include "Include/FKCW_Action_CallfuncT.h"
#include "Include/FKCW_Action_CallfuncNT.h"

#include "Include/FKCW_Action_ClipIn.h"
#include "Include/FKCW_Action_ClipOut.h"
#include "Include/FKCW_Action_TreeFadeIn.h"
#include "Include/FKCW_Action_TreeFadeOut.h"
#include "Include/FKCW_Action_Flash.h"
#include "Include/FKCW_Action_Missile.h"
#include "Include/FKCW_Action_Shake.h"
#include "Include/FKCW_Action_Shine.h"
#include "Include/FKCW_Action_Blur.h"
#include "Include/FKCW_Action_Count.h"
#include "Include/FKCW_Action_TrailMoveTo.h"
#include "Include/FKCW_Action_Curl.h"
#include "Include/FKCW_Action_Suck.h"