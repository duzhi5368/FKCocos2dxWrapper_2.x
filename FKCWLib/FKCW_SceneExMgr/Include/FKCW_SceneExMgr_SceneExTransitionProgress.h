//*************************************************************************
//	创建日期:	2014-11-12
//	文件名称:	FKCW_SceneExMgr_SceneExTransitionProgress.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_SceneExMgr_SceneExTransition.h"
//-------------------------------------------------------------------------
// 以进度条方式进行场景切换（顺/逆时针钟表样式进度条，垂直/横向进度条，中心向四周扩散，四周向中心扩散）
class FKCW_SceneExMgr_SceneExTransitionProgress : public FKCW_SceneExMgr_SceneExTransition
{
public:
	static FKCW_SceneExMgr_SceneExTransitionProgress* create(float t, FKCW_SceneExMgr_SceneEx* scene);

    FKCW_SceneExMgr_SceneExTransitionProgress();
    virtual void onEnter();
    virtual void onExit();
protected:
    virtual CCProgressTimer* progressTimerNodeWithRenderTexture(CCRenderTexture* texture);
    virtual void setupTransition();
    virtual void sceneOrder();
    float m_fTo;
    float m_fFrom;
    FKCW_SceneExMgr_SceneEx* m_pSceneToBeModified;
};
//-------------------------------------------------------------------------
// 逆时针
class FKCW_SceneExMgr_SceneExTransitionProgressRadialCCW : public FKCW_SceneExMgr_SceneExTransitionProgress
{
public:
	static FKCW_SceneExMgr_SceneExTransitionProgressRadialCCW* create(float t, FKCW_SceneExMgr_SceneEx* scene);
protected:
	virtual CCProgressTimer* progressTimerNodeWithRenderTexture(CCRenderTexture* texture);
};
//-------------------------------------------------------------------------
// 顺时针
class FKCW_SceneExMgr_SceneExTransitionProgressRadialCW : public FKCW_SceneExMgr_SceneExTransitionProgress
{
public:
	static FKCW_SceneExMgr_SceneExTransitionProgressRadialCW* create(float t, FKCW_SceneExMgr_SceneEx* scene);
protected:
	virtual CCProgressTimer* progressTimerNodeWithRenderTexture(CCRenderTexture* texture);
};
//-------------------------------------------------------------------------
// 横向
class FKCW_SceneExMgr_SceneExTransitionProgressHorizontal : public FKCW_SceneExMgr_SceneExTransitionProgress
{
public:
	static FKCW_SceneExMgr_SceneExTransitionProgressHorizontal* create(float t, FKCW_SceneExMgr_SceneEx* scene);
protected:
	virtual CCProgressTimer* progressTimerNodeWithRenderTexture(CCRenderTexture* texture);

};
//-------------------------------------------------------------------------
// 纵向
class FKCW_SceneExMgr_SceneExTransitionProgressVertical : public FKCW_SceneExMgr_SceneExTransitionProgress
{
public:
	static FKCW_SceneExMgr_SceneExTransitionProgressVertical* create(float t, FKCW_SceneExMgr_SceneEx* scene);
protected:
	virtual CCProgressTimer* progressTimerNodeWithRenderTexture(CCRenderTexture* texture);
};
//-------------------------------------------------------------------------
// 中心向四周扩展
class FKCW_SceneExMgr_SceneExTransitionProgressInOut : public FKCW_SceneExMgr_SceneExTransitionProgress
{
public:
	static FKCW_SceneExMgr_SceneExTransitionProgressInOut* create(float t, FKCW_SceneExMgr_SceneEx* scene);
protected:
	virtual CCProgressTimer* progressTimerNodeWithRenderTexture(CCRenderTexture* texture);
	virtual void sceneOrder();
	virtual void setupTransition();
};
//-------------------------------------------------------------------------
// 四周向中心收缩
class FKCW_SceneExMgr_SceneExTransitionProgressOutIn : public FKCW_SceneExMgr_SceneExTransitionProgress
{
public:
	static FKCW_SceneExMgr_SceneExTransitionProgressOutIn* create(float t, FKCW_SceneExMgr_SceneEx* scene);
protected:
	virtual CCProgressTimer* progressTimerNodeWithRenderTexture(CCRenderTexture* texture);
};
//-------------------------------------------------------------------------