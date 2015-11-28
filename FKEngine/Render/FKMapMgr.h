//*************************************************************************
//	创建日期:	2015-2-2
//	文件名称:	FKMapMgr.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../FKEngineCommonHead.h"
//-------------------------------------------------------------------------
class FKBannerReelMapMgr
{
public:
	FKBannerReelMapMgr();
	~FKBannerReelMapMgr();
public:
	// 设置根渲染节点，所有场景全部挂接在本节点上
	void				SetRoot( CCNode* pMapRootNode );
	// 获取当前运行中的地图场景
	IMapObj*			GetCurrentMap();
	// 更新进入新场景
	// 参数：szFKMFileName 地图数据文件
	RENDER_MAP_ID		EnterMap( const char* szFKMFileName );
	// 是否允许Update
	void				SetEnableUpdate( bool bIsEnable );
private:
	CCNode*						m_pMapRootNode;		// 全部场景挂接的唯一根节点
	IMapObj*					m_pCurMapObj;		// 当前运行中的场景
	unsigned int				m_unMapID;			// map id生成器
};