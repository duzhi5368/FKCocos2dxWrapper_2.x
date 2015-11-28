//*************************************************************************
//	创建日期:	2014-11-12
//	文件名称:	FKCW_SceneExMgr_SceneExTransitionFadeColor.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_SceneExMgr_SceneExTransition.h"
//-------------------------------------------------------------------------
// 渐变色效果淡入淡出（原场景逐渐加深颜色至指定色，再由指定色淡出为新场景, 默认黑色）
class  FKCW_SceneExMgr_SceneExTransitionFadeColor : public FKCW_SceneExMgr_SceneExTransition
{
protected:
    ccColor4B    m_tColor;

public:
    FKCW_SceneExMgr_SceneExTransitionFadeColor();
    virtual ~FKCW_SceneExMgr_SceneExTransitionFadeColor();
    
    static FKCW_SceneExMgr_SceneExTransitionFadeColor* create(float duration,FKCW_SceneExMgr_SceneEx* scene, const ccColor3B& color);
    static FKCW_SceneExMgr_SceneExTransitionFadeColor* create(float duration,FKCW_SceneExMgr_SceneEx* scene);

    virtual bool initWithDuration(float t, FKCW_SceneExMgr_SceneEx* scene ,const ccColor3B& color);
    virtual bool initWithDuration(float t, FKCW_SceneExMgr_SceneEx* scene);
    virtual void onEnter();
    virtual void onExit();
};
//-------------------------------------------------------------------------