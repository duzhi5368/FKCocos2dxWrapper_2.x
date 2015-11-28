//*************************************************************************
//	创建日期:	2015-1-19
//	文件名称:	FKRenderObjMgr.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../FKEngineCommonHead.h"
//-------------------------------------------------------------------------
class FKRenderObj;
class FKRenderObjModel;
class FKRenderObjMgr
{
public:
	FKRenderObjMgr();
	~FKRenderObjMgr();
public:
	// 创建渲染对象
	// 参数：szGameObjName 游戏对象名
	FKRenderObj*	CreateRenderObj( const char* szFKOFileName );
	// 删除渲染对象
	// 参数: id 需要删除的渲染对象ID
	bool			RemoveRenderObj( RENDER_OBJ_ID id );
	// 获取渲染对象
	FKRenderObj*	GetRenderObj( RENDER_OBJ_ID id );
	// 释放未使用的模型对象数据
	void			ReleaseUnusedObjModel();
	// 是否允许Update
	void			SetEnableUpdate( bool bIsEnable );
private:
	typedef map< RENDER_OBJ_ID, FKRenderObj* >	RenderObjMap;
	typedef map< string, FKRenderObjModel* >	RenderObjModelMap;
	RenderObjMap		m_mapRenderObjs;		// 渲染对象管理器
	RenderObjModelMap	m_mapRenderObjModels;	// 渲染对象模板管理器
};