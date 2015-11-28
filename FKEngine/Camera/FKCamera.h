//*************************************************************************
//	创建日期:	2015-2-9
//	文件名称:	FKCamera.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../FKEngineCommonHead.h"
//-------------------------------------------------------------------------
class FKCameraSystem :public ICameraSystem
{
public:
	FKCameraSystem();
	~FKCameraSystem();
public:
	// 获取当前视口中心坐标
	virtual void		GetViewport( CCPoint& pt );
	// 设置当前视口中心坐标
	virtual void		SetViewport( CCPoint& pt );
	// 获取当前视口可视范围
	virtual void		GetViewSize( CCSize& size );
	// 设置当前视口可视范围
	virtual void		SetViewSize( CCSize& size );
	// 获取当前摄像机所视区域
	virtual void		GetViewRect( CCRect& rect );

	// 震动视口
	// 参数：fMinRange 震动最小范围
	// 参数：fMaxRange 震动最大范围
	// 参数：fTime 持续事件
	virtual void		ShakeCamera( float fMinRange, float fMaxRange, float fTime );
	// 设置视口跟随
	// 参数：pFollower 跟随对象
	virtual void		TrackCamera( CCNode* pFollower );
	// 设置视口路径，摄像机将按照指定路线移动
	// 参数：strPointListFile 点集文件（文件内包括点列表和时间）
	virtual void		TrackCamera( const string& strPointListFile );
	// 设置视口拉近，拉远
	// 参数：fScaleDis 摄像机距离渲染面的距离
	virtual void		StrechCamera( float fScaleDis );
};
//-------------------------------------------------------------------------
extern FKCameraSystem gs_FKCameraSystem;
//-------------------------------------------------------------------------