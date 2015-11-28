//-------------------------------------------------------------------------
#include "../Include/FKCW_SceneExMgr_SceneExTransitionOriented.h"
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionOriented::FKCW_SceneExMgr_SceneExTransitionOriented()
{

}
//-------------------------------------------------------------------------
FKCW_SceneExMgr_SceneExTransitionOriented::~FKCW_SceneExMgr_SceneExTransitionOriented()
{

}
//-------------------------------------------------------------------------
// 创建一个单向切换的动画场景
// 参数：t 动画市场， scene 即将要切换到的目标场景， orientation 切换的方向
FKCW_SceneExMgr_SceneExTransitionOriented* FKCW_SceneExMgr_SceneExTransitionOriented::create(float t,FKCW_SceneExMgr_SceneEx* scene,
														  tOrientation orientation)
{
	FKCW_SceneExMgr_SceneExTransitionOriented * pScene = new FKCW_SceneExMgr_SceneExTransitionOriented();
	pScene->initWithDuration(t,scene,orientation);
	pScene->autorelease();
	return pScene;
}
//-------------------------------------------------------------------------
bool FKCW_SceneExMgr_SceneExTransitionOriented::initWithDuration(
	float t,FKCW_SceneExMgr_SceneEx* scene,tOrientation orientation)
{
	if ( FKCW_SceneExMgr_SceneExTransition::initWithDuration(t, scene) )
	{
		m_eOrientation = orientation;
	}
	return true;
}
//-------------------------------------------------------------------------