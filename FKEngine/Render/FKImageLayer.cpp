//-------------------------------------------------------------------------
#include "FKImageLayer.h"
#include "FKSpriteEx.h"
//-------------------------------------------------------------------------
FKImageLayer::FKImageLayer( RENDER_MAP_LAYER_ID id )
	: FKMapLayer( id )
{
	// 自己类的特殊init，即特性
	if( m_pMapLayerInfo )
	{
		m_pMapLayerInfo->fHeightParallax = 0.0f;
		m_pMapLayerInfo->fWidthParallax = 0.0f;
	}
}
//-------------------------------------------------------------------------
FKImageLayer::FKImageLayer( RENDER_MAP_LAYER_ID id, vector<string>& vecImage )
	: FKMapLayer( id )
{
	InitImageLayer( vecImage );
}
//-------------------------------------------------------------------------
FKImageLayer::~FKImageLayer()
{
	removeAllChildrenWithCleanup( true );
}
//-------------------------------------------------------------------------
// 获取层横向视差
float FKImageLayer::GetLayerHeightParallax()
{
	return 0.0f;
}
//-------------------------------------------------------------------------
// 获取层纵向视差
float FKImageLayer::GetLayerWidthParallax()
{
	return 0.0f;
}
//-------------------------------------------------------------------------
// 设置层横向视差
void FKImageLayer::SetLayerHeightParallax( float fParallax )
{
	
}
//-------------------------------------------------------------------------
// 设置层纵向视差
void FKImageLayer::GetLayerHeightParallax( float fParallax )
{

}
//-------------------------------------------------------------------------
// 初始化创建
bool FKImageLayer::InitImageLayer( vector<string>& vecImage )
{
	vector< FKSpriteEx* >		vecSprite;
	int nWidth = 0;

	unsigned int unImageNum = vecImage.size();
	if( !vecImage.empty() )
	{
		for( unsigned int i = 0; i < unImageNum; ++i )
		{
			FKSpriteEx* p = FKSpriteEx::Create( vecImage[i].c_str() );
			if( p == NULL )
				continue;

			nWidth = static_cast<int>(p->getContentSize().width);
			vecSprite.push_back( p );
			addChild( p );
		}
	}

	int k = unImageNum / 2;
	int nHalfWidth = nWidth / 2;
	if( unImageNum % 2 == 0 )
	{
		int nIndex = 0;
		for( int i = -k; i < k; ++i )
		{
			vecSprite[nIndex++]->setPosition( ccp((i*2+1)*nHalfWidth, 0) );
		}
	}
	else
	{
		int nIndex = 0;
		for( int i = -k; i <= k; ++i )
		{
			vecSprite[nIndex++]->setPosition(ccp(i*nWidth,0));
		}	
	}
	return true;
}
//-------------------------------------------------------------------------