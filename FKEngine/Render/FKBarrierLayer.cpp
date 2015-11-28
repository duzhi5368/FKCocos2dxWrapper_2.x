//-------------------------------------------------------------------------
#include "FKBarrierLayer.h"
//-------------------------------------------------------------------------
FKBarrierLayer::FKBarrierLayer()
	: FKMapLayer( s_BarrierLayerID )
{
	SetLayerDepth( s_BarrierLayerID );
}
//-------------------------------------------------------------------------
FKBarrierLayer::~FKBarrierLayer()
{

}
//-------------------------------------------------------------------------
// 获取地图层类型( 返回 ENUM_MapImageLayerType )
int FKBarrierLayer::GetLayerType()
{
	return eMILT_Barrier;
}
//-------------------------------------------------------------------------