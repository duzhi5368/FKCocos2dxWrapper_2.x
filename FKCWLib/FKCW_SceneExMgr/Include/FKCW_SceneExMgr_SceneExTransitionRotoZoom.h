//*************************************************************************
//	创建日期:	2014-11-12
//	文件名称:	FKCW_SceneExMgr_SceneExTransitionRotoZoom.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_SceneExMgr_SceneExTransition.h"
//-------------------------------------------------------------------------
// 旋转并缩放的场景切换效果（涡轮效果）
class  FKCW_SceneExMgr_SceneExTransitionRotoZoom : public FKCW_SceneExMgr_SceneExTransition
{
public:
    FKCW_SceneExMgr_SceneExTransitionRotoZoom();
    virtual ~FKCW_SceneExMgr_SceneExTransitionRotoZoom();

    virtual void onEnter();

    static FKCW_SceneExMgr_SceneExTransitionRotoZoom* create(float t, FKCW_SceneExMgr_SceneEx* scene);
};