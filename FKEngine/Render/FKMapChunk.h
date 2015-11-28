//*************************************************************************
//	创建日期:	2015-1-27
//	文件名称:	FKMapChunk.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../FKEngineCommonHead.h"
#include "FKMapSystemMacros.h"
#include "FKMapStruct.h"
//-------------------------------------------------------------------------
struct SMapChunkInfo;
class FKMapChunk : public IMapChunk, public CCNode
{
public:
	FKMapChunk( RENDER_MAP_CHUNK_ID id );
	virtual ~FKMapChunk();
public:
	// 释放全部信息
	virtual void			Release();
	// 帧更新
	virtual void			Update( float dt );

	// 获取ChunkID
	virtual unsigned int	GetChunkID() const;
	// 设置ChunkID
	virtual void			SetChunkID( unsigned int unChunkID );
	// 获取当前区域格在大地图中所在位置
	virtual void 			GetGlobalPosition( CCPoint& tagPos );
	// 设置当前区域格所在大地图中的位置
	virtual void			SetGlobalPosition( const CCPoint& tagPos );
	// 获取当前区域大小
	virtual void			GetSize( CCSize& tagSize );
	// 获取当前区域范围
	virtual void			GetRect( CCRect& rect );
	// 获取地图格类型( 返回 ENUM_MapChunkType )
	virtual int				GetType();
	// 开启/关闭DEBUG渲染
	virtual void			SetDebugRender( bool bIsUseDebugRender );

	// 获取区域所在层
	virtual IMapLayer*		GetLayer();
	// 设置区域所在层
	virtual void			SetLayer( IMapLayer* pLayer );
	// 是否可见
	virtual bool			IsVisiable();
	// 设置是否可见
	virtual void			SetIsVisiable( bool bIsVisiable );
	// 检查一个点是否在本区域内
	virtual bool			IsPosInChunk( const CCPoint& pos );

	// 添加子格
	virtual bool			AddGrid( IMapGridInfo* pGridInfo );
	// 移除子格
	virtual void			DelGrid( CCPoint& tagPos );
	// 添加渲染对象
	virtual bool			AddRenderObj( RENDER_OBJ_MAP_ID id, const string& strTerrainObjName, CCRect& rc,
		CCPoint& tagOffset, CCPoint& tagScale, ccColor4B& tagColor );
	// 添加渲染对象
	virtual bool			AddRenderObj( RENDER_OBJ_MAP_ID id, IRenderObj* pRenderObj );
	// 获取渲染对象
	virtual IRenderObj*		GetRenderObj( RENDER_OBJ_MAP_ID id );
	// 移除渲染对象
	virtual void			DelRenderObj( RENDER_OBJ_MAP_ID id );

	// 转换世界坐标到chunk中的坐标
	virtual void			ConvertGlobalPos2ChunkPos( CCPoint& GlobalPos, CCPoint& ChunkPos );
	// 转换chunk中的坐标到世界坐标
	virtual void			ConvertChunkPos2GlobalPos( CCPoint& ChunkPos, CCPoint& GlobalPos );
protected:
	// 帧更新
	virtual void			_SubUpdate( float dt ){};
	// 添加子格
	virtual bool			_SubAddGrid( IMapGridInfo* pGridInfo ){ return true; }
	// 移除子格
	virtual void			_SubDelGrid( CCPoint& tagPos ){}
	// 添加渲染对象
	virtual bool			_SubAddRenderObj( RENDER_OBJ_MAP_ID id, const string& strTerrainObjName, CCRect& rc,
		CCPoint& tagOffset, CCPoint& tagScale, ccColor4B& tagColor ){ return true; }
	// 添加渲染对象
	virtual bool			_SubAddRenderObj( RENDER_OBJ_MAP_ID id, IRenderObj* pRenderObj ){ return true; }
	// 移除渲染对象
	virtual void			_SubDelRenderObj( RENDER_OBJ_MAP_ID id ){}
	// 移除渲染对象
	virtual void			_SubDelRenderObj( IRenderObj* pRenderObj ){}
	// 获取渲染对象
	virtual IRenderObj*		_SubGetRenderObj( RENDER_OBJ_MAP_ID id ){ return NULL; }
protected:
	IMapLayer*				m_pOwnerLayer;					// 所属的Layer
	bool					m_bIsUseDebugRender;			// 是否开启使用Debug渲染
	SMapChunkInfo*			m_pMapChunkInfo;				// 本区域信息
	bool					m_bIsVisible;					// 是否可见
};