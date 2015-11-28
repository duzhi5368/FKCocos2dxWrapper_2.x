//*************************************************************************
//	创建日期:	2015-1-26
//	文件名称:	FKFrontLayer.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKMapLayer.h"
//-------------------------------------------------------------------------
class FKFrontLayer : public FKMapLayer
{
public:
	FKFrontLayer( unsigned int id );
	~FKFrontLayer();
public:
	// 获取地图层类型( 返回 ENUM_MapImageLayerType )
	virtual int				GetLayerType();
};