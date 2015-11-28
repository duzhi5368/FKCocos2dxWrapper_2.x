//*************************************************************************
//	创建日期:	2015-1-28
//	文件名称:	ICamera.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
class ICameraSystem
{
public:
	// 获取当前视口中心坐标
	virtual void		GetViewport( CCPoint& pt ) = 0;
	// 设置当前视口中心坐标
	virtual void		SetViewport( CCPoint& pt ) = 0;
	// 获取当前视口可视范围
	virtual void		GetViewSize( CCSize& size ) = 0;
	// 设置当前视口可视范围
	virtual void		SetViewSize( CCSize& size ) = 0;
	// 获取当前摄像机所视区域
	virtual void		GetViewRect( CCRect& rect ) = 0;

	// 震动视口
	// 参数：fMinRange 震动最小范围
	// 参数：fMaxRange 震动最大范围
	// 参数：fTime 持续事件
	virtual void		ShakeCamera( float fMinRange, float fMaxRange, float fTime ) = 0;
	// 设置视口跟随
	// 参数：pFollower 跟随对象
	virtual void		TrackCamera( CCNode* pFollower ) = 0;
	// 设置视口路径，摄像机将按照指定路线移动
	// 参数：strPointListFile 点集文件（文件内包括点列表和时间）
	virtual void		TrackCamera( const string& strPointListFile ) = 0;
	// 设置视口拉近，拉远
	// 参数：fScaleDis 摄像机距离渲染面的距离
	virtual void		StrechCamera( float fScaleDis ) = 0;
};
//-------------------------------------------------------------------------
// 获取摄像机系统
extern ICameraSystem*	GetCameraSystem();
//-------------------------------------------------------------------------