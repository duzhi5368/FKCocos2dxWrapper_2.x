//-------------------------------------------------------------------------
#include "FKCamera.h"
//-------------------------------------------------------------------------
FKCameraSystem gs_FKCameraSystem;
ICameraSystem* gs_pCameraSystem;
//-------------------------------------------------------------------------
ICameraSystem* GetCameraSystem()
{
	return gs_pCameraSystem;
}
//-------------------------------------------------------------------------
FKCameraSystem::FKCameraSystem()
{

	gs_pCameraSystem = this;
}
//-------------------------------------------------------------------------
FKCameraSystem::~FKCameraSystem()
{
	gs_pCameraSystem = NULL;
}
//-------------------------------------------------------------------------
// 获取当前视口中心坐标
void FKCameraSystem::GetViewport( CCPoint& pt )
{
	
}
//-------------------------------------------------------------------------
// 设置当前视口中心坐标
void FKCameraSystem::SetViewport( CCPoint& pt )
{

}
//-------------------------------------------------------------------------
// 获取当前视口可视范围
void FKCameraSystem::GetViewSize( CCSize& size )
{
	if( CCDirector::sharedDirector() == NULL )
		return;
	size = CCDirector::sharedDirector()->getVisibleSize();
}
//-------------------------------------------------------------------------
// 设置当前视口可视范围
void FKCameraSystem::SetViewSize( CCSize& size )
{
	if( CCDirector::sharedDirector() == NULL )
		return;
	CCDirector::sharedDirector()->setViewport();
}
//-------------------------------------------------------------------------
// 获取当前摄像机所视区域
void FKCameraSystem::GetViewRect( CCRect& rect )
{

}
//-------------------------------------------------------------------------
// 震动视口
// 参数：fMinRange 震动最小范围
// 参数：fMaxRange 震动最大范围
// 参数：fTime 持续事件
void FKCameraSystem::ShakeCamera( float fMinRange, float fMaxRange, float fTime )
{

}
//-------------------------------------------------------------------------
// 设置视口跟随
// 参数：pFollower 跟随对象
void FKCameraSystem::TrackCamera( CCNode* pFollower )
{

}
//-------------------------------------------------------------------------
// 设置视口路径，摄像机将按照指定路线移动
// 参数：strPointListFile 点集文件（文件内包括点列表和时间）
void FKCameraSystem::TrackCamera( const string& strPointListFile )
{

}
//-------------------------------------------------------------------------
// 设置视口拉近，拉远
// 参数：fScaleDis 摄像机距离渲染面的距离
void FKCameraSystem::StrechCamera( float fScaleDis )
{

}
//-------------------------------------------------------------------------