//*************************************************************************
//	创建日期:	2015-1-21
//	文件名称:	FKImageLayer.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKMapLayer.h"
#include "FKImageChunk.h"
//-------------------------------------------------------------------------
// 连续的静态图片层[ 仅为背景层 和 天气层使用 ]
class FKImageLayer : public FKMapLayer
{
public:
	FKImageLayer( RENDER_MAP_LAYER_ID id );
	FKImageLayer( RENDER_MAP_LAYER_ID id, vector<string>& vecImage );
	~FKImageLayer();
public:
	// 获取层横向视差
	virtual float			GetLayerHeightParallax();
	// 获取层纵向视差
	virtual float			GetLayerWidthParallax();
	// 设置层横向视差
	virtual void			SetLayerHeightParallax( float fParallax );
	// 设置层纵向视差
	virtual void			GetLayerHeightParallax( float fParallax );
public:
	// 初始化创建
	bool					InitImageLayer( vector<string>& vecImage );
};