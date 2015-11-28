//*************************************************************************
//	创建日期:	2014-11-12
//	文件名称:	FKCW_SceneExMgr_SceneExTransitionPageTurn.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_SceneExMgr_SceneExTransition.h"
//-------------------------------------------------------------------------
// 前/后 翻页方式切换场景
class FKCW_SceneExMgr_SceneExTransitionPageTurn : public FKCW_SceneExMgr_SceneExTransition
{
protected:
    bool    m_bBack;
    
public:
    FKCW_SceneExMgr_SceneExTransitionPageTurn();
    virtual ~FKCW_SceneExMgr_SceneExTransitionPageTurn();
	static FKCW_SceneExMgr_SceneExTransitionPageTurn* create(float t,FKCW_SceneExMgr_SceneEx* scene,bool backwards);
	virtual bool initWithDuration(float t,FKCW_SceneExMgr_SceneEx* scene,bool backwards);

    CCActionInterval* actionWithSize(const CCSize& vector);
    virtual void onEnter();

protected:
    virtual void sceneOrder();
}; 