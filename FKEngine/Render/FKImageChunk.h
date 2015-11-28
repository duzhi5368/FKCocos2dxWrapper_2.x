//*************************************************************************
//	创建日期:	2015-1-21
//	文件名称:	FKImageChunk.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKMapChunk.h"
//-------------------------------------------------------------------------
// 场景中的物件
struct STerrainObj
{
	RENDER_OBJ_ID			m_unID;				// 对象唯一ID
	RENDER_MAP_CHUNK_ID		m_unChunkID;		// 所在Chunk ID
	RENDER_MAP_LAYER_ID		m_unLayerID;		// 所在Layer ID
	CCPoint					m_tagScale;			// 缩放值
	CCPoint					m_tagOffset;		// 偏移值
	ccColor4B				m_tagColor;			// 颜色值
	CCRect					m_tagRc;			// 显示区
	IRenderObj*				m_pRenderObj;		// 渲染对象
	string					m_strFileName;		// 物件文件名
	unsigned int			m_unTime;			// 在场景中创建的时间
	STerrainObj()
		: m_unID( 0 )
		, m_unChunkID( 0 )
		, m_unLayerID( 0 )
		, m_unTime( 0 )
		, m_pRenderObj( NULL )
	{
		m_tagScale = ccp( 1.0f, 1.0f );
		m_tagOffset = CCPointZero;
		m_tagColor = ccc4( 255, 255, 255, 255 );
		m_tagRc = CCRectZero;
		m_strFileName.clear();
	}
};
//-------------------------------------------------------------------------
// 图片区
class FKImageChunk : public FKMapChunk
{
public:
	FKImageChunk( RENDER_MAP_CHUNK_ID id );
	~FKImageChunk();
public:
	virtual void			draw();
protected:
	// 帧更新
	virtual void			_SubUpdate( float dt );
	// 添加渲染对象
	virtual bool			_SubAddRenderObj( RENDER_OBJ_MAP_ID id, const string& strTerrainObjName, CCRect& rc,
		CCPoint& tagOffset, CCPoint& tagScale, ccColor4B& tagColor );
	// 添加渲染对象
	virtual bool			_SubAddRenderObj( RENDER_OBJ_MAP_ID id, IRenderObj* pRenderObj );
	// 移除渲染对象
	virtual void			_SubDelRenderObj( RENDER_OBJ_MAP_ID id );
	// 获取渲染对象
	virtual IRenderObj*		_SubGetRenderObj( RENDER_OBJ_MAP_ID id );
private:
	// 更新可视对象集
	void					__UpdateVisibleObject();
private:
	typedef map< RENDER_OBJ_MAP_ID, STerrainObj >	TerrainObjsMap;
	typedef map< RENDER_OBJ_ID, RENDER_OBJ_MAP_ID >	RenderObjID2TerrainObjIDMap;
	typedef set< RENDER_OBJ_MAP_ID >				TerrainRenderObjsSet;
	TerrainObjsMap				m_mapTerrainObjs;			// 需要场景渲染物件对象
	bool						m_bIsNeedUpdateTerrainObj;	// 是否需要更新地形对象
	RenderObjID2TerrainObjIDMap	m_mapRenderObj2TerrainObj;	// 渲染对象ID->场景对象ID对应表
	TerrainRenderObjsSet		m_setTerrainRenderObjs;		// 场景中正在被渲染的物件对象
};