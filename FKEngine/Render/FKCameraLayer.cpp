//-------------------------------------------------------------------------
#include "FKCameraLayer.h"
//-------------------------------------------------------------------------
FKCameraLayer::FKCameraLayer( unsigned int id )
	: FKMapLayer( s_CameraLayerIDBegin + id )
{
	SetLayerDepth( s_CameraLayerIDBegin + id );
}
//-------------------------------------------------------------------------
FKCameraLayer::~FKCameraLayer()
{

}
//-------------------------------------------------------------------------
// 获取地图层类型( 返回 ENUM_MapImageLayerType )
int FKCameraLayer::GetLayerType()
{
	return eMILT_Camera;
}
//-------------------------------------------------------------------------