//*************************************************************************
//	创建日期:	2015-1-21
//	文件名称:	FKMapLayer.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../FKEngineCommonHead.h"
//-------------------------------------------------------------------------
struct SMapLayerInfo;
class FKMapLayer : public CCLayer, public IMapLayer
{
public:
	FKMapLayer( RENDER_MAP_LAYER_ID id );
	~FKMapLayer();
public:
	// 释放
	virtual void			Release();
	// 帧更新
	virtual void			Update( float dt );

	// 设置层ID
	virtual void			SetLayerID( unsigned int unLayerID );
	// 获取层ID
	virtual unsigned int	GetLayerID() const;
	// 设置层深
	virtual void			SetLayerDepth( int nLayerDepth );
	// 获取层深
	virtual int				GetLayerDepth();
	// 获取地图层类型( 返回 ENUM_MapImageLayerType )
	virtual int				GetLayerType();
	// 获取层横向视差
	virtual float			GetLayerHeightParallax();
	// 获取层纵向视差
	virtual float			GetLayerWidthParallax();
	// 设置层横向视差
	virtual void			SetLayerHeightParallax( float fParallax );
	// 设置层纵向视差
	virtual void			GetLayerHeightParallax( float fParallax );
	// 开启/关闭DEBUG渲染
	virtual void			SetDebugRender( bool bIsUseDebugRender );

	// 添加子Chunk
	virtual RENDER_MAP_CHUNK_ID	AttachChunk( IMapChunk* pChunk );
	// 创建空的子Chunk
	virtual RENDER_MAP_CHUNK_ID CreateEmptyChunk( CCPoint& tagPos );
	// 移除子Chunk
	virtual bool			DeleteChunk( RENDER_MAP_CHUNK_ID unChunkID );
	// 获取子Chunk
	virtual IMapChunk*		GetChunkByID( RENDER_MAP_CHUNK_ID unChunkID );
	// 获取子chunk
	virtual IMapChunk*		GetChunkByPos( const CCPoint& tagPos );

	// 添加一个Obj
	virtual bool			AddObj( RENDER_OBJ_MAP_ID id, IRenderObj* pObj );
	// 动态更新Obj位置
	virtual void			SetObjPos( RENDER_OBJ_MAP_ID id, const CCPoint& tagPos );
	// 获取渲染Obj
	virtual IRenderObj*		GetObjByID( RENDER_OBJ_MAP_ID id );
	// 移除渲染Obj
	virtual void			RemoveObjByID( RENDER_OBJ_MAP_ID id );

	// 对区域进行视野裁剪
	virtual void			CullVisiableChunks();
protected:
	typedef map<RENDER_MAP_CHUNK_ID, IMapChunk*>		ChunksMap;
	typedef set<RENDER_MAP_CHUNK_ID>					ChunksSet;
	SMapLayerInfo*			m_pMapLayerInfo;		// 层自身信息
	ChunksMap				m_mapChunks;			// 块组
	bool					m_bIsOpenDebug;			// 是否开启了DEBUG渲染
	bool					m_bIsNeedUpdateChunk;	// 是否要更新区块可见性
	unsigned int			m_unCurChunkID;			// 当前ChunkID
	ChunksSet				m_setVisiableChunks;	// 可视区域组[临时]
	ChunksSet				m_setRenderedChunks;	// 正在被渲染的区域组
};