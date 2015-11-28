//*************************************************************************
//	创建日期:	2014-11-12
//	文件名称:	FKCW_SceneExMgr_SceneExTransitionMoveIn.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_SceneExMgr_SceneExTransition.h"
//-------------------------------------------------------------------------
// 覆盖切换效果（新场景从左向右移动覆盖原场景，原场景不动）
class  FKCW_SceneExMgr_SceneExTransitionMoveInL : public FKCW_SceneExMgr_SceneExTransition, public FKCW_SceneExMgr_SceneExTransitionEase
{
public:
    FKCW_SceneExMgr_SceneExTransitionMoveInL();
    virtual ~FKCW_SceneExMgr_SceneExTransitionMoveInL();

    virtual void initScenes(void);
    virtual CCActionInterval* action(void);
    virtual CCActionInterval* easeActionWithAction(CCActionInterval * action);
    virtual void onEnter();

    static FKCW_SceneExMgr_SceneExTransitionMoveInL* create(float t, FKCW_SceneExMgr_SceneEx* scene);
};
//-------------------------------------------------------------------------
class  FKCW_SceneExMgr_SceneExTransitionMoveInR : public FKCW_SceneExMgr_SceneExTransitionMoveInL
{
public:
    FKCW_SceneExMgr_SceneExTransitionMoveInR();
    virtual ~FKCW_SceneExMgr_SceneExTransitionMoveInR();
    virtual void initScenes();

    static FKCW_SceneExMgr_SceneExTransitionMoveInR* create(float t, FKCW_SceneExMgr_SceneEx* scene);
};
//-------------------------------------------------------------------------
class  FKCW_SceneExMgr_SceneExTransitionMoveInT : public FKCW_SceneExMgr_SceneExTransitionMoveInL 
{
public:
    FKCW_SceneExMgr_SceneExTransitionMoveInT();
    virtual ~FKCW_SceneExMgr_SceneExTransitionMoveInT();
    virtual void initScenes();

    static FKCW_SceneExMgr_SceneExTransitionMoveInT* create(float t, FKCW_SceneExMgr_SceneEx* scene);
};
//-------------------------------------------------------------------------
class  FKCW_SceneExMgr_SceneExTransitionMoveInB : public FKCW_SceneExMgr_SceneExTransitionMoveInL
{
public:
    FKCW_SceneExMgr_SceneExTransitionMoveInB();
    virtual ~FKCW_SceneExMgr_SceneExTransitionMoveInB();
    virtual void initScenes();

    static FKCW_SceneExMgr_SceneExTransitionMoveInB* create(float t, FKCW_SceneExMgr_SceneEx* scene);
};
//-------------------------------------------------------------------------
