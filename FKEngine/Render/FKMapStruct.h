//*************************************************************************
//	创建日期:	2015-1-27
//	文件名称:	FKMapStruct.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../FKEngineCommonHead.h"
#include "FKMapSystemMacros.h"
//-------------------------------------------------------------------------
// 地图格
struct SMapGridInfo : public IMapGridInfo
{
	CCPoint				tagPos;			// 格子所在位置中心点（相对本区域）
	unsigned short		usMainValue;	// 格子主值
	unsigned short		usSubValue;		// 格子子值
	SMapGridInfo()
		: usMainValue( 0 )
		, usSubValue( 0 )
	{
		tagPos = CCPointZero;
	}

	// 获取格子类型
	virtual ENUM_MapChunkType	GetGridType() = 0;
};
// 障碍格
struct SBarrierGridInfo : public SMapGridInfo
{
	// 判断是否是障碍
	bool		IsBarrier()
	{
		return (usMainValue != 0);
	}
	// 获取格子ID，该ID应当在本Chunk内唯一
	virtual unsigned short		GetGridID()
	{
		return ((((unsigned short)tagPos.x / s_BarrierGridSize )<< 8) 
			| ((unsigned short)tagPos.y / s_BarrierGridSize ));
	}
	// 获取格子大小
	virtual void				GetSize( CCSize& tagSize )
	{
		tagSize = CCSizeMake( s_BarrierGridSize, s_BarrierGridSize );
	}
	// 获取格子区域
	virtual void				GetRectInChunk( CCRect& tagRect )
	{
		tagRect = CCRectMake( tagPos.x - s_BarrierGridSize / 2,
			tagPos.y - s_BarrierGridSize / 2, s_BarrierGridSize, s_BarrierGridSize );
	}
	// 获取格子类型
	virtual ENUM_MapChunkType	GetGridType()
	{
		return eMCT_Barrier;
	}
};
// 摄像机格
struct SCameraGridInfo : public SMapGridInfo
{
	// 判断是否阻止摄像机移动
	bool		IsCameraBarrier()
	{
		return (usMainValue != 0);
	}
	// 获取格子ID，该ID应当在本Chunk内唯一
	virtual unsigned short		GetGridID()
	{
		return ((((unsigned short)tagPos.x / s_CameraGridSize )<< 8) 
			| ((unsigned short)tagPos.y / s_CameraGridSize ));
	}
	// 获取格子大小
	virtual void				GetSize( CCSize& tagSize )
	{
		tagSize = CCSizeMake( s_CameraGridSize, s_CameraGridSize );
	}
	// 获取格子区域
	virtual void				GetRectInChunk( CCRect& tagRect )
	{
		tagRect = CCRectMake( tagPos.x - s_CameraGridSize / 2,
			tagPos.y - s_CameraGridSize / 2, s_CameraGridSize, s_CameraGridSize );
	}
	// 获取格子类型
	virtual ENUM_MapChunkType	GetGridType()
	{
		return eMCT_Camera;
	}
};
// 事件格
struct SEventGridInfo : public SMapGridInfo
{
	// 判断事件类型
	void 		GetEventType( unsigned short& sMainEventType, unsigned short& sSubEventType )
	{
		sMainEventType = usMainValue;
		sSubEventType = usSubValue;
	}
	// 获取格子ID，该ID应当在本Chunk内唯一
	virtual unsigned short		GetGridID()
	{
		return ((((unsigned short)tagPos.x / s_EventGridSize )<< 8) 
			| ((unsigned short)tagPos.y / s_EventGridSize ));
	}
	// 获取格子大小
	virtual void				GetSize( CCSize& tagSize )
	{
		tagSize = CCSizeMake( s_EventGridSize, s_EventGridSize );
	}
	// 获取格子区域
	virtual void				GetRectInChunk( CCRect& tagRect )
	{
		tagRect = CCRectMake( tagPos.x - s_EventGridSize / 2,
			tagPos.y - s_EventGridSize / 2, s_EventGridSize, s_EventGridSize );
	}
	// 获取格子类型
	virtual ENUM_MapChunkType	GetGridType()
	{
		return eMCT_Event;
	}
};
// 蒙版遮罩格
struct SMaskGridInfo : public SMapGridInfo
{
	// 是否蒙版遮罩
	bool		IsMask()
	{
		return (usMainValue != 0);
	}
	// 获取格子ID，该ID应当在本Chunk内唯一
	virtual unsigned short		GetGridID()
	{
		return ((((unsigned short)tagPos.x / s_MaskGridSize )<< 8) 
			| ((unsigned short)tagPos.y / s_MaskGridSize ));
	}
	// 获取格子大小
	virtual void				GetSize( CCSize& tagSize )
	{
		tagSize = CCSizeMake( s_MaskGridSize, s_MaskGridSize );
	}
	// 获取格子区域
	virtual void				GetRectInChunk( CCRect& tagRect )
	{
		tagRect = CCRectMake( tagPos.x - s_MaskGridSize / 2,
			tagPos.y - s_MaskGridSize / 2, s_MaskGridSize, s_MaskGridSize );
	}
	// 获取格子类型
	virtual ENUM_MapChunkType	GetGridType()
	{
		return eMCT_Mask;
	}
};
//-------------------------------------------------------------------------
// 区域
//-------------------------------------------------------------------------
// 地图区域信息
struct SMapChunkInfo
{
	CCPoint				tagPos;		// 区域所在中心位置（相对整个世界）
	RENDER_MAP_CHUNK_ID	unChunkID;	// 区域编号（在所在层应保持唯一性）
	SMapChunkInfo()
		: unChunkID( 0 )
	{
		tagPos = CCPointZero;
	}
	// 获取区域ID，该ID应当在本区域内唯一
	virtual unsigned int		GetChunkID(){ return unChunkID; }
	// 获取区域大小
	virtual void				GetSize( CCSize& tagSize ){ return; };
	// 获取区域范围
	virtual void				GetRect( CCRect& tagRect ){ return; };
	// 获取区域类型
	virtual ENUM_MapChunkType	GetType(){ return eMCT_Max; };
};
// 障碍区域信息
struct SBarrierChunkInfo : public SMapChunkInfo
{
	// 获取区域大小
	virtual void				GetSize( CCSize& tagSize )
	{
		tagSize = CCSizeMake( s_BarrierChunkWidth, s_BarrierChunkHeight );
	}
	// 获取区域范围
	virtual void				GetRect( CCRect& tagRect ) 
	{
		tagRect = CCRectMake( tagPos.x - s_BarrierChunkWidth / 2,
			tagPos.y - s_BarrierChunkHeight / 2,
			s_BarrierChunkWidth, s_BarrierChunkHeight );
	}
	// 获取区域类型
	virtual ENUM_MapChunkType	GetType()
	{
		return eMCT_Barrier;
	}
};
// 摄像机障碍区域信息
struct SCameraChunkInfo : public SMapChunkInfo
{
	// 获取区域大小
	virtual void				GetSize( CCSize& tagSize )
	{
		tagSize = CCSizeMake( s_CameraChunkWidth, s_CameraChunkHeight );
	}
	// 获取区域范围
	virtual void				GetRect( CCRect& tagRect ) 
	{
		tagRect = CCRectMake( tagPos.x - s_CameraChunkWidth / 2,
			tagPos.y - s_CameraChunkHeight / 2,
			s_CameraChunkWidth, s_CameraChunkHeight );
	}
	// 获取区域类型
	virtual ENUM_MapChunkType	GetType()
	{
		return eMCT_Camera;
	}
};
// 事件区域信息
struct SEventChunkInfo : public SMapChunkInfo
{
	// 获取区域大小
	virtual void				GetSize( CCSize& tagSize )
	{
		tagSize = CCSizeMake( s_EventChunkWidth, s_EventChunkHeight );
	}
	// 获取区域范围
	virtual void				GetRect( CCRect& tagRect ) 
	{
		tagRect = CCRectMake( tagPos.x - s_EventChunkWidth / 2,
			tagPos.y - s_EventChunkHeight / 2,
			s_EventChunkWidth, s_EventChunkHeight );
	}
	// 获取区域类型
	virtual ENUM_MapChunkType	GetType()
	{
		return eMCT_Event;
	}
};
// 遮罩区域信息
struct SMaskBarrierChunkInfo : public SMapChunkInfo
{
	// 获取区域大小
	virtual void				GetSize( CCSize& tagSize )
	{
		tagSize = CCSizeMake( s_MaskChunkWidth, s_MaskChunkHeight );
	}
	// 获取区域范围
	virtual void				GetRect( CCRect& tagRect ) 
	{
		tagRect = CCRectMake( tagPos.x - s_MaskChunkWidth / 2,
			tagPos.y - s_MaskChunkHeight / 2,
			s_MaskChunkWidth, s_MaskChunkHeight );
	}
	// 获取区域类型
	virtual ENUM_MapChunkType	GetType()
	{
		return eMCT_Mask;
	}
};
// 图片区域信息
struct SImageChunkInfo : public SMapChunkInfo
{
	// 获取区域大小
	virtual void				GetSize( CCSize& tagSize )
	{
		tagSize = CCSizeMake( s_PicChunkWidth, s_PicChunkHeight );
	}
	// 获取区域范围
	virtual void				GetRect( CCRect& tagRect ) 
	{
		tagRect = CCRectMake( tagPos.x - s_PicChunkWidth / 2,
			tagPos.y - s_PicChunkHeight / 2,
			s_PicChunkWidth, s_PicChunkHeight );
	}
	// 获取区域类型
	virtual ENUM_MapChunkType	GetType()
	{
		return eMCT_Image;
	}
};
//-------------------------------------------------------------------------
// 层
//-------------------------------------------------------------------------
// 层
struct SMapLayerInfo
{
	RENDER_MAP_LAYER_ID		unLayerID;
	float					fHeightParallax;		// 高度视察
	float					fWidthParallax;			// 纵向视察
	int						nDepth;					// Zorder深度

	SMapLayerInfo()
		: unLayerID( 0 )
		, fWidthParallax( 1.0f )
		, fHeightParallax( 0.0f )
	{
	}
};
//-------------------------------------------------------------------------
// 地图
//-------------------------------------------------------------------------
struct SMapInfo
{
	RENDER_MAP_ID			unMapID;
	string					strMapFileName;			// 地图文件名

	SMapInfo()
		: unMapID( 0 )
	{
		strMapFileName.clear();
	}
};