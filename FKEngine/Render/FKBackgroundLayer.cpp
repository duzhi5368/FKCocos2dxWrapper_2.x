//-------------------------------------------------------------------------
#include "FKBackgroundLayer.h"
//-------------------------------------------------------------------------
FKBackgroundLayer::FKBackgroundLayer()
	: FKImageLayer( s_BackgroundLayerID )
{
	SetLayerDepth( s_BackgroundLayerID );
}
//-------------------------------------------------------------------------
FKBackgroundLayer::FKBackgroundLayer( vector<string>& vecImage )
	: FKImageLayer( s_BackgroundLayerID, vecImage )
{

}
//-------------------------------------------------------------------------
FKBackgroundLayer::~FKBackgroundLayer()
{

}
//-------------------------------------------------------------------------
// 获取地图层类型( 返回 ENUM_MapImageLayerType )
int FKBackgroundLayer::GetLayerType()
{
	return eMILT_Background;
}
//-------------------------------------------------------------------------
// 对区域进行视野裁剪
void FKBackgroundLayer::CullVisiableChunks()
{
	return;
}
//-------------------------------------------------------------------------