//*************************************************************************
//	创建日期:	2014-11-15
//	文件名称:	FKCW_EffectNode.h
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
	一些特殊效果Node的封装支持
====================================
内部文件功能说明：
====================================
	FKCW_EffectNode_IndexVBO
		顶点VBO定义类
	FKCW_EffectNode_GLProgramWithUnifos
		带独立编号的Shader Program
	FKCW_EffectNode_Mesh
		网格对象
	FKCW_EffectNode_MeshWire
		线状网格对象
	FKCW_EffectNode_MeshGeneral
		带顶点索引的网格对象

	子特效效果
		FKCW_EffectNode_2DSoftShadow
			2D软阴影，实时计算。可设置一个光照，多个计算阴影的对象，还有一个软阴影根节点
		FKCW_EffectNode_BreakSprite
			破碎掉落精灵，可以破碎为三角块并进行逐步掉落效果
		FKCW_EffectNode_ColorAdjustSprite
			颜色调整精灵。可调整其HSL，可实现变色，高亮变暗等效果
		FKCW_EffectNode_GhostLikeSprite
			幽灵式样精灵。是依附于一个常规精灵的子精灵，和进行与常规精灵完全一致的行为，但颜色大小可额外调整
		FKCW_EffectNode_Laser
			激光式样精灵。
		FKCW_EffectNode_LightingBoltNode
			闪电样式精灵。
		FKCW_EffectNode_NormalMappedSprite
			带法线贴图可以实现自身阴影渲染的精灵
		FKCW_EffectNode_Ripper2DSprite
			2D垂直水面精灵，受到影响会生成水平面的波浪
		FKCW_EffectNode_RippleSprite
			水面波浪精灵，会产生涟漪波浪效果。
		FKCW_EffectNode_Shatter
			破碎效果精灵，会产生中心向周围碎裂的效果。
		FKCW_EffectNode_TailSprite
			拖尾效果精灵，会根据移动速度在后面生成追尾效果。
			注意：和 FKCW_RenderEx_TiledSprite 不同。Tiled是残生残影，这个是生成刀光拖尾效果。
		FKCW_EffectNode_TinyWingsTerrainSprite
			生成类似游戏【TinyWings】 的地形效果的精灵。
====================================
依赖项目/库：
====================================
	FKCW_Base
====================================
todo:
====================================

*/ 
//-------------------------------------------------------------------------
#include "Include/FKCW_EffectNode_IndexVBO.h"
#include "Include/FKCW_EffectNode_GLProgramWithUnifos.h"
#include "Include/FKCW_EffectNode_Mesh.h"
//-------------------------------------------------------------------------
#include "Include/FKCW_EffectNode_2DSoftShadow.h"
#include "Include/FKCW_EffectNode_BreakSprite.h"
#include "Include/FKCW_EffectNode_ColorAdjustSprite.h"
#include "Include/FKCW_EffectNode_GhostLikeSprite.h"
#include "Include/FKCW_EffectNode_Laser.h"
#include "Include/FKCW_EffectNode_LightingBoltNode.h"
#include "Include/FKCW_EffectNode_NormalMappedSprite.h"
#include "Include/FKCW_EffectNode_Ripper2DSprite.h"
#include "Include/FKCW_EffectNode_RippleSprite.h"
#include "Include/FKCW_EffectNode_Shatter.h"
#include "Include/FKCW_EffectNode_TailSprite.h"
#include "Include/FKCW_EffectNode_TinyWingsTerrainSprite.h"