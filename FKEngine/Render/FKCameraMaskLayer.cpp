//-------------------------------------------------------------------------
#include "FKCameraMaskLayer.h"
//-------------------------------------------------------------------------
FKCameraMaskLayer::FKCameraMaskLayer()
	: FKImageLayer( s_CameraMaskLayerID )
{
	SetLayerDepth( s_CameraMaskLayerID );
}
//-------------------------------------------------------------------------
FKCameraMaskLayer::FKCameraMaskLayer( vector<string>& vecImage )
	: FKImageLayer( s_CameraMaskLayerID, vecImage )
{

}
//-------------------------------------------------------------------------
FKCameraMaskLayer::~FKCameraMaskLayer()
{

}
//-------------------------------------------------------------------------
// 获取地图层类型( 返回 ENUM_MapImageLayerType )
int FKCameraMaskLayer::GetLayerType()
{
	return eMILT_CameraMask;
}
//-------------------------------------------------------------------------
// 对区域进行视野裁剪
void FKCameraMaskLayer::CullVisiableChunks()
{
	return;
}
//-------------------------------------------------------------------------