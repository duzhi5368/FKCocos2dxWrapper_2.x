//*************************************************************************
//	创建日期:	2014-10-28
//	文件名称:	FKCW_Base_ResourceLoaderListener.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	资源加载进度监听器
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_Base_Macro.h"
//-------------------------------------------------------------------------
class FKCW_Base_ResourceLoaderListener
{
public:
	// 当资源在加载时的通知函数
	// 参数：p_fProgress 当前进度。值在0-100之间
	// 参数：p_fDelta 和上次调用该通知函数 之间的间隔时间
	virtual void		OnResourceLoadingProgress( float p_fProgress, float p_fDelta ) = 0;
	// 当资源加载完成时的通知
	virtual void		OnResourceLoadingDone() = 0;
};