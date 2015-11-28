//*************************************************************************
//	创建日期:	2014-11-12
//	文件名称:	FKCW_SceneExMgr_SceneExTransitionJumpZoom.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_SceneExMgr_SceneExTransition.h"
//-------------------------------------------------------------------------
// 跳跃缩放场景切换动画
class  FKCW_SceneExMgr_SceneExTransitionJumpZoom : public FKCW_SceneExMgr_SceneExTransition
{
public:
    FKCW_SceneExMgr_SceneExTransitionJumpZoom();
    virtual ~FKCW_SceneExMgr_SceneExTransitionJumpZoom();

    virtual void onEnter();

    static FKCW_SceneExMgr_SceneExTransitionJumpZoom* create(float t, FKCW_SceneExMgr_SceneEx* scene);
};