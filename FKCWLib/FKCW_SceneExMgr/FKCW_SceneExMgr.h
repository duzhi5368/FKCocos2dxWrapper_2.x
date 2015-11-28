//*************************************************************************
//	创建日期:	2014-11-12
//	文件名称:	FKCW_SceneExMgr.h
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
	自创建了一个场景管理对象，提供了一系列自定义场景过渡切换和消息通知机制
	使用方式如下：
	定义自定义场景
		class CMyScene : public FKCW_SceneExMgr_SceneEx
		{
			// your code
		}
	在AppDalagate::applicationDidFinishLaunching() 函数中添加
		REGISTER_SCENE_FUNC(CMyScene);
		CCSceneManager::sharedManager()->runWithScene(LoadScene("CMyScene"));
	之后进行场景切换时可
		CCSceneManager::sharedManager()->popScene(this);
		CCSceneManager::sharedManager()->runScene(LoadScene("CMyScene2"));
		或
		CCSceneManager::sharedManager()->popUIScene(this);
		CCSceneManager::sharedManager()->runUIScene(LoadScene("CMyUIScene2"));
	等操作
	使用FKCW_SceneExMgr_SceneExTransition系列做场景过渡时，可创建一个新的FKCW_SceneExMgr_SceneExTransitionOriented，
	然后执行CCSceneManager::sharedManager()->replaceScene( FKCW_SceneExMgr_SceneExTransition::create(...) );即可
====================================
内部文件功能说明：
====================================
	FKCW_SceneExMgr_SceneEx
		类似CCScene，是一个自定义场景类基类。它相对于CCScene，增加了资源加载通知，同/异步资源加载，缓存，过渡切换动作
		等一系列实用函数，并添加了自己的消息机制。
	FKCW_SceneExMgr_SceneExTransition
		一个进行过渡动作的场景，需要有入场景和出场景配合，播放一系列动作进行场景切换的过渡。
	FKCW_SceneExMgr_SceneExMgr
		全部场景的管理器，单例。可以缓存场景对象，以及场景对象的展示，销毁等控制操作。
		同时控制整个场景切换的基础消息机制。

	子类过渡场景系列
		FKCW_SceneExMgr_SceneExTransitionRotoZoom
			旋转并缩放的场景切换效果（涡轮效果）
		FKCW_SceneExMgr_SceneExTransitionJumpZoom
			跳跃缩放场景切换动画
		FKCW_SceneExMgr_SceneExTransitionMoveInL
		FKCW_SceneExMgr_SceneExTransitionMoveInR
		FKCW_SceneExMgr_SceneExTransitionMoveInT
		FKCW_SceneExMgr_SceneExTransitionMoveInB
			覆盖切换效果（新场景从L/R/T/B向另一方向移动覆盖原场景，原场景不动）
		FKCW_SceneExMgr_SceneExTransitionSlideInL
		FKCW_SceneExMgr_SceneExTransitionSlideInR
		FKCW_SceneExMgr_SceneExTransitionSlideInT
		FKCW_SceneExMgr_SceneExTransitionSlideInB
			推动切换效果（新场景从L/R/T/B向另一方向将原场景推出屏幕）
		FKCW_SceneExMgr_SceneExTransitionShrinkGrow
			环绕切换场景效果
		FKCW_SceneExMgr_SceneExTransitionFlipX
		FKCW_SceneExMgr_SceneExTransitionFlipY
		FKCW_SceneExMgr_SceneExTransitionFlipAngular
			绕 X轴/Y轴/左上右下对角线 旋转场景切换效果
		FKCW_SceneExMgr_SceneExTransitionZoomFlipX
		FKCW_SceneExMgr_SceneExTransitionZoomFlipY
		FKCW_SceneExMgr_SceneExTransitionZoomFlipAngular
			绕 X轴/Y轴/左上右下对角线 旋转并缩放场景切换效果
		FKCW_SceneExMgr_SceneExTransitionFadeColor
			渐变色效果淡入淡出（原场景逐渐加深颜色至指定色，再由指定色淡出为新场景，默认黑色）
		FKCW_SceneExMgr_SceneExTransitionCrossFade
			融合，混合方式切换场景（原场景逐渐变淡，新场景逐渐变深）
		FKCW_SceneExMgr_SceneExTransitionTurnOffTiles
			以随机方格替换方式切换场景（将原场景和新场景切换为多个小方格，随机方格内图像切换为新场景）
		FKCW_SceneExMgr_SceneExTransitionSplitCols
		FKCW_SceneExMgr_SceneExTransitionSplitRows
			纵向/横向将原场景切为多条，岔开滑动至两端并替换为新场景
		FKCW_SceneExMgr_SceneExTransitionFadeTR
		FKCW_SceneExMgr_SceneExTransitionFadeBL
		FKCW_SceneExMgr_SceneExTransitionFadeUp
		FKCW_SceneExMgr_SceneExTransitionFadeDown
			自左下/右上/上/下 向对称方向以方格渐隐方式逐渐替换
		FKCW_SceneExMgr_SceneExTransitionPageTurn
			前/后 翻页方式切换场景
		FKCW_SceneExMgr_SceneExTransitionProgressRadialCCW
		FKCW_SceneExMgr_SceneExTransitionProgressRadialCW
		FKCW_SceneExMgr_SceneExTransitionProgressHorizontal
		FKCW_SceneExMgr_SceneExTransitionProgressVertical
		FKCW_SceneExMgr_SceneExTransitionProgressInOut
		FKCW_SceneExMgr_SceneExTransitionProgressOutIn
			以进度条方式进行场景切换（顺/逆时针钟表样式进度条，垂直/横向进度条，中心向四周扩散，四周向中心扩散）
====================================
依赖项目/库：
====================================
	无
====================================
todo:
====================================

*/
//-------------------------------------------------------------------------
#include "Include/FKCW_SceneExMgr_SceneEx.h"
#include "Include/FKCW_SceneExMgr_SceneExMgr.h"

#include "Include/FKCW_SceneExMgr_SceneExTransitionCrossFade.h"
#include "Include/FKCW_SceneExMgr_SceneExTransitionFadeColor.h"
#include "Include/FKCW_SceneExMgr_SceneExTransitionFadeDir.h"
#include "Include/FKCW_SceneExMgr_SceneExTransitionFlip.h"
#include "Include/FKCW_SceneExMgr_SceneExTransitionJumpZoom.h"
#include "Include/FKCW_SceneExMgr_SceneExTransitionMoveIn.h"
#include "Include/FKCW_SceneExMgr_SceneExTransitionOriented.h"
#include "Include/FKCW_SceneExMgr_SceneExTransitionPageTurn.h"
#include "Include/FKCW_SceneExMgr_SceneExTransitionProgress.h"
#include "Include/FKCW_SceneExMgr_SceneExTransitionRotoZoom.h"
#include "Include/FKCW_SceneExMgr_SceneExTransitionShrinkGrow.h"
#include "Include/FKCW_SceneExMgr_SceneExTransitionSlideIn.h"
#include "Include/FKCW_SceneExMgr_SceneExTransitionSplit.h"
#include "Include/FKCW_SceneExMgr_SceneExTransitionTurnOffTiles.h"
//-------------------------------------------------------------------------