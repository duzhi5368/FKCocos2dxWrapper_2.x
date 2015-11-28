//*************************************************************************
//	创建日期:	2015-1-26
//	文件名称:	FKMapSystemMacros.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
// 地图格类型
enum ENUM_MapChunkType
{
	eMCT_Barrier = 0,			// 障碍格
	eMCT_Camera,				// 摄像机区域格
	eMCT_Mask,					// 遮罩格
	eMCT_Event,					// 事件格
	eMCT_Image,					// 普通的图片格

	eMCT_Max,
};
//-------------------------------------------------------------------------
// debug指示格颜色
const static ccColor4F s_BrrierGridDebugColor = { 1.0f, 0.0f, 0.0f, 0.5f };
const static ccColor4F s_CameraGridDebugColor = { 0.0f, 1.0f, 0.0f, 0.5f };
const static ccColor4F s_MaskGridDebugColor =	{ 0.0f, 0.0f, 1.0f, 0.5f };
const static ccColor4F s_EventGridDebugColor =	{ 1.0f, 0.0f, 1.0f, 0.5f };
//-------------------------------------------------------------------------
// 地图图片层类型
enum ENUM_MapImageLayerType
{
	eMILT_Barrier = 0,				// 障碍格
	eMILT_Camera,					// 摄像机区域格
	eMILT_Mask,						// 遮罩格
	eMILT_Event,					// 事件格

	eMILT_CameraMask,				// 摄像机遮罩层（做雨水，樱花等特效专用）
	eMILT_Front,					// 前景层
	eMILT_Normal,					// 中间层
	eMILT_Far,						// 远景层
	eMILT_Background,				// 背景层

	eMILT_Max,
};
//-------------------------------------------------------------------------
// 地图各层ID 兼其 zOrder
const static unsigned int	s_BackgroundLayerID		= 10000;	// 单层

const static unsigned int	s_FarLayerIDBegin		= 11000;
const static unsigned int	s_FarLayerIDEnd			= 12999;
const static unsigned int	s_NormalLayerIDBegin	= 14000;
const static unsigned int	s_NormalLayerIDEnd		= 15999;
const static unsigned int	s_FrontLayerIDBegin		= 17000;
const static unsigned int	s_FrontLayerIDEnd		= 18999;

const static unsigned int	s_CameraMaskLayerID		= 19000;	// 单层

const static unsigned int	s_EventLayerID			= 20000;	// 单层
const static unsigned int	s_MaskLayerID			= 19999;	// 单层
const static unsigned int	s_BarrierLayerID		= 19998;	// 单层

const static unsigned int	s_CameraLayerIDBegin	= 19500;
const static unsigned int	s_CameraLayerIDEnd		= 19600;
//-------------------------------------------------------------------------
// 障碍格宽高
const static int		s_BarrierGridSize = 16;
// 障碍区管理的障碍格横向和纵向个数
const static int		s_BarrierGridRowNumInChunk = 32;
const static int		s_BarrierGridColNumInChunk = 32;
// 障碍区宽高
const static int		s_BarrierChunkWidth	= 512;	// s_BarrierGridSize * s_BarrierGridRowNumInChunk
const static int		s_BarrierChunkHeight = 512;	// s_BarrierGridSize * s_BarrierGridColNumInChunk
// 摄像机格宽高
const static int		s_CameraGridSize = 16;
// 摄像机区管理的摄像机格横向和纵向个数
const static int		s_CameraGridRowNumInChunk = 32;
const static int		s_CameraGridColNumInChunk = 32;
// 摄像机区宽高			
const static int		s_CameraChunkWidth	= 512;	// s_CameraGridSize * s_CameraGridRowNumInChunk
const static int		s_CameraChunkHeight = 512;	// s_CameraGridSize * s_CameraGridColNumInChunk
// 蒙版格宽高
const static int		s_MaskGridSize = 16;
// 蒙版区管理的蒙版格横向和纵向个数
const static int		s_MaskGridRowNumInChunk = 32;
const static int		s_MaskGridColNumInChunk = 32;
// 蒙版区宽高			
const static int		s_MaskChunkWidth	= 512;	// s_MaskGridSize * s_MaskGridRowNumInChunk
const static int		s_MaskChunkHeight	= 512;	// s_MaskGridSize * s_MaskGridColNumInChunk
// 事件格宽高
const static int		s_EventGridSize = 16;
// 事件区管理的事件格横向和纵向个数
const static int		s_EventGridRowNumInChunk = 32;
const static int		s_EventGridColNumInChunk = 32;
// 事件区宽高			
const static int		s_EventChunkWidth	= 512;	// s_EventGridSize * s_EventGridRowNumInChunk
const static int		s_EventChunkHeight	= 512;	// s_EventGridSize * s_EventGridColNumInChunk

// 图片区宽高
const static int		s_PicChunkWidth		= 512;
const static int		s_PicChunkHeight	= 512;
//-------------------------------------------------------------------------