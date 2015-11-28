/********************************************************************
	created:	2014/10/22
	created:	2014:10:22   23:08
	file base:	FKCocos2dxWrapper
	author:		FreeKnight
	
	purpose:	对外唯一头包
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
/*
	设计初衷：对cocos2dx-2.x的扩展。
	1：避免污染原生cocos2dx源代码。
	2：避免使用第三方库，部分轻量级允许使用
	设计要求：
	1：文件必须有足够注释说明（中英文均可）。
	2：保持一致的编码规范。
	TODO:
	1: 动画方面对于SpriteX, Motion welder等格式的支持
	2：TMX地图格式支持
	3：故事版的封装和对故事版的Lua脚本注册
	4: 模拟服务器
	5：DargonBones的DebugDraw
*/
//--------------------------------------------------------------------
#pragma warning ( disable : 4251 )
//--------------------------------------------------------------------
#include "FKCW_Base/FKCW_Base.h"
#include "FKCW_IO/FKCW_IO.h"

#include "FKCW_Shader/FKCW_Shader.h"
#include "FKCW_Database/FKCW_Database.h"
#include "FKCW_Json/FKCW_Json.h"
#include "FKCW_Network/FKCW_Network.h"
#include "FKCW_SimpleNet/FKCW_SimpleNet.h"
#include "FKCW_Action/FKCW_Action.h"
#include "FKCW_RenderEx/FKCW_RenderEx.h"
#include "FKCW_UI/FKCW_UI.h"
#include "FKCW_GR/FKCW_GR.h"
#include "FKCW_SceneExMgr/FKCW_SceneExMgr.h"
#include "FKCW_EffectNode/FKCW_EffectNode.h"
#include "FKCW_UIWidget/FKCW_UIWidget.h"
#include "FKCW_3D/FKCW_3D.h"
#include "FKCW_TMX/FKCW_TMX.h"
#include "FKCW_Font/FKCW_Font.h"
#include "FKCW_RichHtml/FKCW_RichHtml.h"
#include "FKCW_Story/FKCW_Story.h"
#include "FKCW_Pixel/FKCW_Pixel.h"
//--------------------------------------------------------------------
// 下面是一些全局的限制设置
//--------------------------------------------------------------------
/*
// 如果该值为非0，则LOG记录全部的内存分配。
g_bISFlagAllocationLog = true;
// 是否开启FONT_debug
g_bIsFKCWFontDebug = true;
// 是否显示像素Pixel包围盒
g_isShowBoundingBox = true;
// 是否输出Pixel的DEBUG片段颜色矩阵
g_isPrintFragMat = false;
// 是否开启RichHtml的Debug模式
g_bIsFKCWRichHtmlDebug = true;

// 是否使用RichHtml的Lua消息通知
#define  FKCW_RICHHTML_USING_LUA
// 是否开启DEBUG内存跟踪
#define FKCW_MEMORY_TRACKING
*/