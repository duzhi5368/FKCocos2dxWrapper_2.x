//*************************************************************************
//	创建日期:	2014-11-12
//	文件名称:	FKCW_SceneExMgr_SceneExTransitionTurnOffTiles.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_SceneExMgr_SceneExTransition.h"
//-------------------------------------------------------------------------
// 以随机方格替换方式切换场景（将原场景和新场景切换为多个小方格，随机方格内图像切换为新场景）
class  FKCW_SceneExMgr_SceneExTransitionTurnOffTiles : public FKCW_SceneExMgr_SceneExTransition ,public FKCW_SceneExMgr_SceneExTransitionEase
{
public :
    FKCW_SceneExMgr_SceneExTransitionTurnOffTiles();
    virtual ~FKCW_SceneExMgr_SceneExTransitionTurnOffTiles();

    virtual void onEnter();
    virtual CCActionInterval * easeActionWithAction(CCActionInterval * action);

public:
    static FKCW_SceneExMgr_SceneExTransitionTurnOffTiles* create(float t, FKCW_SceneExMgr_SceneEx* scene);
protected:
    virtual void sceneOrder();
};
