//*************************************************************************
//	创建日期:	2014-11-12
//	文件名称:	FKCW_SceneExMgr_SceneExTransitionOriented.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_SceneExMgr_SceneExTransition.h"
//-------------------------------------------------------------------------
// 单方向滑动切换场景
class  FKCW_SceneExMgr_SceneExTransitionOriented : public FKCW_SceneExMgr_SceneExTransition
{
protected:
    tOrientation m_eOrientation;				// 切换动画的方向

public:
    FKCW_SceneExMgr_SceneExTransitionOriented();
    virtual ~FKCW_SceneExMgr_SceneExTransitionOriented();

    // 创建一个单向切换的动画场景
	// 参数：t 动画市场， scene 即将要切换到的目标场景， orientation 切换的方向
    static FKCW_SceneExMgr_SceneExTransitionOriented * create(float t,FKCW_SceneExMgr_SceneEx* scene,
		tOrientation orientation);

    virtual bool initWithDuration(float t,FKCW_SceneExMgr_SceneEx* scene,tOrientation orientation);
};