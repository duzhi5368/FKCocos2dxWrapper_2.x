//*************************************************************************
//	创建日期:	2014-11-12
//	文件名称:	FKCW_SceneExMgr_SceneExTransitionCrossFade.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_SceneExMgr_SceneExTransition.h"
//-------------------------------------------------------------------------
// 融合，混合方式切换场景（原场景逐渐变淡，新场景逐渐变深）
class  FKCW_SceneExMgr_SceneExTransitionCrossFade : public FKCW_SceneExMgr_SceneExTransition
{
public :
    FKCW_SceneExMgr_SceneExTransitionCrossFade();
    virtual ~FKCW_SceneExMgr_SceneExTransitionCrossFade();

    virtual void draw();
    virtual void onEnter();
    virtual void onExit();

public:
    static FKCW_SceneExMgr_SceneExTransitionCrossFade* create(float t, FKCW_SceneExMgr_SceneEx* scene);
};
