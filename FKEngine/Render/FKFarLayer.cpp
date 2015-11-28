//-------------------------------------------------------------------------
#include "FKFarLayer.h"
//-------------------------------------------------------------------------
FKFarLayer::FKFarLayer( unsigned int id )
	: FKMapLayer( s_FarLayerIDBegin + id )
{
	SetLayerDepth( s_FarLayerIDBegin + id );
}
//-------------------------------------------------------------------------
FKFarLayer::~FKFarLayer()
{

}
//-------------------------------------------------------------------------
// 获取地图层类型( 返回 ENUM_MapImageLayerType )
int FKFarLayer::GetLayerType()
{
	return eMILT_Far;
}
//-------------------------------------------------------------------------