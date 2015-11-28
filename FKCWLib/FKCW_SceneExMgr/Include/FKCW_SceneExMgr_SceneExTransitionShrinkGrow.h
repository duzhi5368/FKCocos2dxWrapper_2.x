//*************************************************************************
//	创建日期:	2014-11-12
//	文件名称:	FKCW_SceneExMgr_SceneExTransitionShrinkGrow.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_SceneExMgr_SceneExTransition.h"
//-------------------------------------------------------------------------
// 环绕切换场景效果
class  FKCW_SceneExMgr_SceneExTransitionShrinkGrow : public FKCW_SceneExMgr_SceneExTransition , public FKCW_SceneExMgr_SceneExTransitionEase
{
public:
    FKCW_SceneExMgr_SceneExTransitionShrinkGrow();
    virtual ~FKCW_SceneExMgr_SceneExTransitionShrinkGrow();

    virtual void onEnter();
    virtual CCActionInterval* easeActionWithAction(CCActionInterval * action);

    static FKCW_SceneExMgr_SceneExTransitionShrinkGrow* create(float t, FKCW_SceneExMgr_SceneEx* scene);
};
