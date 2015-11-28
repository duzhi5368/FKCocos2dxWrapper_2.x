//*************************************************************************
//	创建日期:	2015-1-21
//	文件名称:	IMapObj.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
// 地图块ID
typedef unsigned int	RENDER_MAP_CHUNK_ID;
// 地图层ID
typedef unsigned int	RENDER_MAP_LAYER_ID;
// 地图ID
typedef unsigned int	RENDER_MAP_ID;
// 地图中的渲染对象ID
typedef unsigned int	RENDER_OBJ_MAP_ID;
//-------------------------------------------------------------------------
class IMapLayer;
struct IMapGridInfo
{
	// 获取格子ID，该ID应当在本Chunk内唯一
	virtual unsigned short		GetGridID()	= 0;
	// 获取格子大小
	virtual void				GetSize( CCSize& tagSize ) = 0;
	// 获取格子区域
	virtual void				GetRectInChunk( CCRect& tagRect ) = 0;
};
//-------------------------------------------------------------------------
class IMapChunk
{
public:
	// 释放全部信息
	virtual void			Release() = 0;
	// 帧更新
	virtual void			Update( float dt ) = 0;

	// 获取ChunkID
	virtual unsigned int	GetChunkID() const = 0;
	// 设置ChunkID
	virtual void			SetChunkID( unsigned int unChunkID ) = 0;
	// 获取当前区域格在大地图中所在位置
	virtual void 			GetGlobalPosition( CCPoint& tagPos ) = 0;
	// 设置当前区域格所在大地图中的位置
	virtual void			SetGlobalPosition( const CCPoint& tagPos ) = 0;
	// 获取当前区域大小
	virtual void			GetSize( CCSize& tagSize ) = 0;
	// 获取当前区域范围
	virtual void			GetRect( CCRect& rect ) = 0;
	// 获取地图格类型( 返回 ENUM_MapChunkType )
	virtual int				GetType() = 0;
	// 开启/关闭DEBUG渲染
	virtual void			SetDebugRender( bool bIsUseDebugRender ) = 0;

	// 获取区域所在层
	virtual IMapLayer*		GetLayer() = 0;
	// 设置区域所在层
	virtual void			SetLayer( IMapLayer* pLayer ) = 0;
	// 是否可见
	virtual bool			IsVisiable() = 0;
	// 设置是否可见
	virtual void			SetIsVisiable( bool bIsVisiable ) = 0;
	// 检查一个点是否在本区域内
	virtual bool			IsPosInChunk( const CCPoint& pos ) = 0;

	// 添加子格
	virtual bool			AddGrid( IMapGridInfo* pGridInfo ) = 0;
	// 移除子格
	virtual void			DelGrid( CCPoint& tagPos ) = 0;
	// 添加渲染对象
	virtual bool			AddRenderObj( RENDER_OBJ_MAP_ID id, const string& strTerrainObjName, CCRect& rc,
		CCPoint& tagOffset, CCPoint& tagScale, ccColor4B& tagColor ) = 0;
	// 添加渲染对象
	virtual bool			AddRenderObj( RENDER_OBJ_MAP_ID id, IRenderObj* pRenderObj ) = 0;
	// 获取渲染对象
	virtual IRenderObj*		GetRenderObj( RENDER_OBJ_MAP_ID id ) = 0;
	// 移除渲染对象
	virtual void			DelRenderObj( RENDER_OBJ_MAP_ID id ) = 0;

	// 转换世界坐标到chunk中的坐标
	virtual void			ConvertGlobalPos2ChunkPos( CCPoint& GlobalPos, CCPoint& ChunkPos ) = 0;
	// 转换chunk中的坐标到世界坐标
	virtual void			ConvertChunkPos2GlobalPos( CCPoint& ChunkPos, CCPoint& GlobalPos ) = 0;
};
//-------------------------------------------------------------------------
class IMapLayer
{
public:
	// 释放
	virtual void			Release() = 0;
	// 帧更新
	virtual void			Update( float dt ) = 0;

	// 获取层ID
	virtual unsigned int	GetLayerID() const = 0;
	// 设置层ID
	virtual void			SetLayerID( unsigned int unLayerID ) = 0;
	// 设置层深
	virtual void			SetLayerDepth( int nLayerDepth ) = 0;
	// 获取层深
	virtual int				GetLayerDepth() = 0;
	// 获取层横向视差
	virtual float			GetLayerHeightParallax() = 0;
	// 获取层纵向视差
	virtual float			GetLayerWidthParallax() = 0;
	// 设置层横向视差
	virtual void			SetLayerHeightParallax( float fParallax ) = 0;
	// 设置层纵向视差
	virtual void			GetLayerHeightParallax( float fParallax ) = 0;
	// 获取地图层类型( 返回 ENUM_MapImageLayerType )
	virtual int				GetLayerType() = 0;
	// 开启/关闭DEBUG渲染
	virtual void			SetDebugRender( bool bIsUseDebugRender ) = 0;

	// 添加子Chunk
	virtual RENDER_MAP_CHUNK_ID	AttachChunk( IMapChunk* pChunk ) = 0;
	// 创建空的子Chunk
	virtual RENDER_MAP_CHUNK_ID CreateEmptyChunk( CCPoint& tagPos ) = 0;
	// 移除子Chunk
	virtual bool			DeleteChunk( RENDER_MAP_CHUNK_ID unChunkID ) = 0;
	// 获取子Chunk
	virtual IMapChunk*		GetChunkByID( RENDER_MAP_CHUNK_ID unChunkID ) = 0;
	// 获取子chunk
	virtual IMapChunk*		GetChunkByPos( const CCPoint& tagPos ) = 0;

	// 添加一个Obj
	virtual bool			AddObj( RENDER_OBJ_MAP_ID id, IRenderObj* pObj ) = 0;
	// 动态更新Obj位置
	virtual void			SetObjPos( RENDER_OBJ_MAP_ID id, const CCPoint& tagPos ) = 0;
	// 获取渲染Obj
	virtual IRenderObj*		GetObjByID( RENDER_OBJ_MAP_ID id ) = 0;
	// 移除渲染Obj
	virtual void			RemoveObjByID( RENDER_OBJ_MAP_ID id ) = 0;

	// 对区域进行视野裁剪
	virtual void			CullVisiableChunks() = 0;
};
//-------------------------------------------------------------------------
class IMapObj
{
public:
	// 是否允许Update
	virtual void			SetEnableUpdate( bool bIsEnable ) = 0;
	// 从一个资源文件创建
	virtual bool			InitFromFile( const char* szFileName ) = 0;
	// 写入到一个资源文件
	virtual bool			WriteToFile( const char* szFileName ) = 0;

	// 添加一个Obj
	virtual unsigned int	AddObj( RENDER_MAP_LAYER_ID id, IRenderObj* pObj ) = 0;
	// 动态更新Obj位置
	virtual void			SetObjPos( RENDER_MAP_CHUNK_ID id, const CCPoint& tagPos ) = 0;
	// 获取渲染Obj
	virtual IRenderObj*		GetObjByID( RENDER_MAP_CHUNK_ID id ) = 0;
	// 移除渲染Obj
	virtual void			RemoveObjByID( RENDER_MAP_CHUNK_ID id ) = 0;

	// 获取指定层
	virtual IMapLayer*		GetLayerByID( RENDER_MAP_LAYER_ID id ) = 0;

	// 转换屏幕坐标到场景坐标
	virtual void			ConvertScreenPos2ScenePos( const CCPoint& screenPos, 
		CCPoint& scenePos ) = 0;

	// 碰撞选取检测
	virtual void			HitTestObj( const CCPoint& pt, list<IRenderObj*>& ObjList ) = 0;

	// 获取场景ID
	virtual RENDER_MAP_ID	GetID() = 0;
	// 获取场景文件名
	virtual void			GetSceneFileName( string& strFileName ) = 0;
	// 开启/关闭DEBUG渲染
	virtual void			SetDebugRender( bool bIsUseDebugRender ) = 0;
};
//-------------------------------------------------------------------------