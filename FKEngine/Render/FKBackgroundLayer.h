//*************************************************************************
//	创建日期:	2015-1-29
//	文件名称:	FKBackgroundLayer.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKImageLayer.h"
//-------------------------------------------------------------------------
class FKBackgroundLayer : public FKImageLayer
{
public:
	FKBackgroundLayer();
	FKBackgroundLayer( vector<string>& vecImage );
	~FKBackgroundLayer();
public:
	// 获取地图层类型( 返回 ENUM_MapImageLayerType )
	virtual int				GetLayerType();

	// 对区域进行视野裁剪
	virtual void			CullVisiableChunks();
};