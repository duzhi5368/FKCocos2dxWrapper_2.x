//*************************************************************************
//	创建日期:	2014-11-12
//	文件名称:	FKCW_SceneExMgr_SceneExTransition.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_SceneExMgr_SceneEx.h"
#include "FKCW_SceneExMgr.h"
//-------------------------------------------------------------------------
// 纯虚函数，继承本类则意味着场景将执行一个【变速动画】
class FKCW_SceneExMgr_SceneExTransitionEase
{
public:
    virtual CCActionInterval * easeActionWithAction(CCActionInterval * action) = 0;
};
//-------------------------------------------------------------------------
// 过渡场景基类
class FKCW_SceneExMgr_SceneExTransition : public FKCW_SceneExMgr_SceneEx
{

protected:
	FKCW_SceneExMgr_SceneEx*	m_pInScene;					// 即将被显示的场景（新场景）
    FKCW_SceneExMgr_SceneEx*	m_pOutScene;				// 即将被清除的场景（老场景）
	float						m_fDuration;				// 切换动画效果市场
	bool						m_bIsInSceneOnTop;			// 是否进行场景渲染排序，以保证新场景在最上面
	bool						m_bIsSendCleanupToScene;	// 是否在切换完成之后清空旧场景

public:
    FKCW_SceneExMgr_SceneExTransition();
    virtual ~FKCW_SceneExMgr_SceneExTransition();
    virtual void draw();
    virtual void onEnter();
    virtual void onExit();
    virtual void cleanup();

	virtual FKCW_SceneExMgr_SceneEx* getInScene();

	// 创建一个基本过渡场景
    static FKCW_SceneExMgr_SceneExTransition* create(float t, FKCW_SceneExMgr_SceneEx *scene);
	// 初始化一个基本过渡场景
    virtual bool initWithDuration(float t,FKCW_SceneExMgr_SceneEx* scene);
	// 过渡结束后调用
    void finish(void);
	// 部分过渡场景子类需要本函数用以隐藏老场景
    void hideOutShowIn(void);

protected:
	// 设置对场景进行排序。
    virtual void sceneOrder();
private:
	// 私有函数，用于在场景切换动画完成后，设置新场景为当前游戏场景。
    void setNewScene(float dt);
};
//-------------------------------------------------------------------------