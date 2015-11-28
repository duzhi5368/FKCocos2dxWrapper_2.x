//*************************************************************************
//	创建日期:	2015-1-29
//	文件名称:	FKEventLayer.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKMapLayer.h"
//-------------------------------------------------------------------------
class FKEventLayer : public FKMapLayer
{
public:
	FKEventLayer();
	~FKEventLayer();
public:
	// 获取地图层类型( 返回 ENUM_MapImageLayerType )
	virtual int				GetLayerType();
};