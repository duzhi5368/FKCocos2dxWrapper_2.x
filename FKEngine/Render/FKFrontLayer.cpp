//-------------------------------------------------------------------------
#include "FKFrontLayer.h"
//-------------------------------------------------------------------------
FKFrontLayer::FKFrontLayer( unsigned int id )
	: FKMapLayer( s_FrontLayerIDBegin + id )
{
	SetLayerDepth( s_FrontLayerIDBegin + id );
}
//-------------------------------------------------------------------------
FKFrontLayer::~FKFrontLayer()
{

}
//-------------------------------------------------------------------------
// 获取地图层类型( 返回 ENUM_MapImageLayerType )
int FKFrontLayer::GetLayerType()
{
	return eMILT_Front;
}
//-------------------------------------------------------------------------