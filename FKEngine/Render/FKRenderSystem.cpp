//-------------------------------------------------------------------------
#include "FKRenderSystem.h"
//-------------------------------------------------------------------------

FKRenderSystem gs_FKRenderSystem;
IRenderSystem* gs_pRenderSystem;
//-------------------------------------------------------------------------
IRenderSystem* GetRenderSystem()
{
	return gs_pRenderSystem;
}
//-------------------------------------------------------------------------
FKRenderSystem::FKRenderSystem()
{
	gs_pRenderSystem = this;
}
FKRenderSystem::~FKRenderSystem()
{
	gs_pRenderSystem = NULL;
}

//-------------------------------------------------------------------------
// 初始化渲染系统
// 参数：tagDesignSize 渲染窗口大小
void FKRenderSystem::Init( CCSize& tagDesignSize )
{

}
//-------------------------------------------------------------------------
// 设置根渲染节点，所有场景全部挂接在本节点上
void FKRenderSystem::SetRoot( CCNode* pMapRootNode )
{

}
//-------------------------------------------------------------------------
// 获取当前运行中的地图场景
IMapObj* FKRenderSystem::GetCurrentMap()
{
	return NULL;
}
//-------------------------------------------------------------------------
// 更新进入新场景
// 参数：szFKMFileName 地图数据文件
RENDER_MAP_ID FKRenderSystem::EnterMap( const char* szFKMFileName )
{
	return 0;
}
//-------------------------------------------------------------------------
// 是否允许Update
void FKRenderSystem::SetEnableUpdate( bool bIsEnable )
{

}
//-------------------------------------------------------------------------
// 设置场景摄像机视点
// 参数：tagLocPos 摄像机视点位置
void FKRenderSystem::SetViewPoint( const CCPoint& tagLocPos )
{

}
//-------------------------------------------------------------------------
// 获取场景摄像机视点
// 参数：[out]tagLocPos 摄像机视点位置
bool FKRenderSystem::GetViewPoint( const CCPoint& tagLocPos )
{
	return true;
}
//-------------------------------------------------------------------------
// 转换屏幕坐标到场景坐标
// 参数：[in]tagScreenPos 屏幕坐标
// 参数：[out]tagScenePos 场景坐标
bool FKRenderSystem::ConvertScreenPosToScenePos( const CCPoint& tagScreenPos,
									   CCPoint& tagScenePos )
{
	return true;
}
//-------------------------------------------------------------------------

// 获取碰撞对象
// 参数：ObjList 被碰撞的对象列表
void FKRenderSystem::HitRenderObj( const CCPoint& tagScreenPos, list<IRenderObj*>& ObjList )
{

}
//-------------------------------------------------------------------------
// 创建渲染对象
// 参数：szGameObjName 游戏对象名
IRenderObj* FKRenderSystem::CreateRenderObj( const char* szGameObjName )
{
	return NULL;
}
//-------------------------------------------------------------------------
// 删除渲染对象
// 参数: pRenderObj 需要删除的渲染对象
bool FKRenderSystem::RemoveRenderObj( IRenderObj* pRenderObj )
{
	return true;
}
//-------------------------------------------------------------------------
// 删除渲染对象
// 参数: id 需要删除的渲染对象ID
bool FKRenderSystem::RemoveRenderObj( RENDER_OBJ_ID id )
{
	return true;
}
//-------------------------------------------------------------------------
// 获取渲染对象
IRenderObj* FKRenderSystem::GetRenderObj( RENDER_OBJ_ID id )
{
	return NULL;
}
//-------------------------------------------------------------------------
// 添加渲染对象到当前场景
// 参数：pRenderObj 添加渲染对象到当前场景
bool FKRenderSystem::AddRenderObjToScene( IRenderObj* pRenderObj )
{
	return true;
}
//-------------------------------------------------------------------------
// 从当前场景删除渲染对象
// 参数：pRenderObj 需要从当前场景删除的渲染对象
bool FKRenderSystem::RemoveRenderObjFromScene( IRenderObj* pRenderObj )
{
	return true;
}
//-------------------------------------------------------------------------

// 创建渲染文字
// 参数：szString 文字
// 参数：szFontName 字体名称
// 参数：nFontSize 字体大小
// 参数：tagDimentions 字体尺寸
// 参数：bIsBmpFont 是否是BMP文字
IRenderText* FKRenderSystem::CreateRenderText( const char* szString, const char* szFontName, 
							  int nFontSize,  CCSize tagDimentions , bool bIsBmpFont  )
{
	return NULL;
}
//-------------------------------------------------------------------------
// 删除渲染文字
// 参数：pRenderText 需要删除的渲染文字
bool FKRenderSystem::RemoveRenderText( IRenderText* pRenderText )
{
	return true;
}
//-------------------------------------------------------------------------
// 删除渲染文字
// 参数：id 唯一ID
bool FKRenderSystem::RemoveRenderTextByID( RENDER_OBJ_ID id )
{
	return true;
}
//-------------------------------------------------------------------------