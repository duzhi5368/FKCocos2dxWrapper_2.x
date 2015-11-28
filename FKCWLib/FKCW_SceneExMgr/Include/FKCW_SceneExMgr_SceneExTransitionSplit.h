//*************************************************************************
//	创建日期:	2014-11-12
//	文件名称:	FKCW_SceneExMgr_SceneExTransitionSplit.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_SceneExMgr_SceneExTransition.h"
//-------------------------------------------------------------------------
// 纵向/横向将原场景切为多条，岔开滑动至两端并替换为新场景
class  FKCW_SceneExMgr_SceneExTransitionSplitCols : public FKCW_SceneExMgr_SceneExTransition , public FKCW_SceneExMgr_SceneExTransitionEase
{
public:
    FKCW_SceneExMgr_SceneExTransitionSplitCols();
    virtual ~FKCW_SceneExMgr_SceneExTransitionSplitCols();

    virtual CCActionInterval* action(void);
    virtual void onEnter();
    virtual CCActionInterval * easeActionWithAction(CCActionInterval * action);

public:

    static FKCW_SceneExMgr_SceneExTransitionSplitCols* create(float t, FKCW_SceneExMgr_SceneEx* scene);
};
//-------------------------------------------------------------------------
class  FKCW_SceneExMgr_SceneExTransitionSplitRows : public FKCW_SceneExMgr_SceneExTransitionSplitCols
{
public:
    FKCW_SceneExMgr_SceneExTransitionSplitRows();
    virtual ~FKCW_SceneExMgr_SceneExTransitionSplitRows();

    virtual CCActionInterval* action(void);
public:

    static FKCW_SceneExMgr_SceneExTransitionSplitRows* create(float t, FKCW_SceneExMgr_SceneEx* scene);
};
//-------------------------------------------------------------------------