//-------------------------------------------------------------------------
#include "FKNormalLayer.h"
//-------------------------------------------------------------------------
FKNormalLayer::FKNormalLayer( unsigned int id )
	: FKMapLayer( s_NormalLayerIDBegin + id )
{
	SetLayerDepth( s_NormalLayerIDBegin + id );
}
//-------------------------------------------------------------------------
FKNormalLayer::~FKNormalLayer()
{

}
//-------------------------------------------------------------------------
// 获取地图层类型( 返回 ENUM_MapImageLayerType )
int FKNormalLayer::GetLayerType()
{
	return eMILT_Normal;
}
//-------------------------------------------------------------------------