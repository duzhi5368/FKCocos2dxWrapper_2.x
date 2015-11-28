//*************************************************************************
//	创建日期:	2015-1-22
//	文件名称:	FKAnimateExResMgr.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../FKEngineCommonHead.h"
//-------------------------------------------------------------------------
class FKAnimateExRes;
class FKAnimateExResMgr
{
protected:
	FKAnimateExResMgr();
	~FKAnimateExResMgr();
public:
	static FKAnimateExResMgr& GetInstance()
	{
		static FKAnimateExResMgr s_Inst;
		return s_Inst;
	}
public:
	// 创建动画资源
	// 参数：szAnimationExResName 动画资源文件名
	// 注：若该资源已经存在，则直接返回原资源指针
	FKAnimateExRes*		GetAnimateExRes( const char* szAnimationExResName );
	// 删除动画资源
	void				ReleaseAnimationExRes( FKAnimateExRes* pAniRes );
	// 释放全部资源
	void				ReleaseAllResource();
	// 释放没有释放的资源
	void				ReleaseUnusedResource();
private:
	typedef map<string, FKAnimateExRes*>	AnimationExResMap;
	AnimationExResMap				m_mapAnimationExRes;	// 动画资源列表
};