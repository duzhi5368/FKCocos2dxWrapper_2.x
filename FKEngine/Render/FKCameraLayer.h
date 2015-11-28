//*************************************************************************
//	创建日期:	2015-1-29
//	文件名称:	FKCameraLayer.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKMapLayer.h"
//-------------------------------------------------------------------------
class FKCameraLayer : public FKMapLayer
{
public:
	FKCameraLayer( unsigned int id );
	~FKCameraLayer();
public:
	// 获取地图层类型( 返回 ENUM_MapImageLayerType )
	virtual int				GetLayerType();
};