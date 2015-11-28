//-------------------------------------------------------------------------
#include "../Include/Cocos2dxDBTextureAtlas.h"
//-------------------------------------------------------------------------
CCocos2dxDBTextureAtlas::CCocos2dxDBTextureAtlas( CTextureAtlasData* p_pData )
	: m_pTextureAtlasData( p_pData )
{
	m_pTexture2D = CCTextureCache::sharedTextureCache()->addImage( p_pData->m_szImagePath.c_str() );
	m_pTextureAtlas = CCTextureAtlas::create( p_pData->m_szImagePath.c_str(), p_pData->m_szRects.size() );

	if( m_pTextureAtlas == NULL )
	{
		//throw std::invalid_argument("create TextureAtlas failed.");
		//android 平台不支持c++11特性
		CCLOG("create TextureAtlas failed.");
	}

	CC_SAFE_RETAIN( m_pTextureAtlas );
	uint unAtlasWidth	= m_pTextureAtlas->getTexture()->getPixelsWide();
	uint unAtlasHeight	= m_pTextureAtlas->getTexture()->getPixelsHigh();
	ccV3F_C4B_T2F_Quad tagQuad;
	uint unIndex = 0;
	map< string, CRect >::iterator Ite = m_pTextureAtlasData->m_szRects.begin();
	for( ; Ite != m_pTextureAtlasData->m_szRects.end(); ++Ite )
	{
		const CRect& tagRect	= Ite->second;
		float fLeft				= tagRect.m_nX / static_cast<float>( unAtlasWidth );
		float fRight			= ( tagRect.m_nX + tagRect.m_nWidth ) / static_cast<float>( unAtlasWidth );
		float fTop				= tagRect.m_nY / static_cast<float>( unAtlasHeight );
		float fBottom			= ( tagRect.m_nY + tagRect.m_nHeight ) / static_cast<float>( unAtlasHeight );

		tagQuad.bl.texCoords.u	= fLeft;
		tagQuad.bl.texCoords.v	= fBottom;
		tagQuad.br.texCoords.u	= fRight;
		tagQuad.br.texCoords.v	= fBottom;
		tagQuad.tl.texCoords.u	= fLeft;
		tagQuad.tl.texCoords.v	= fTop;
		tagQuad.tr.texCoords.u	= fRight;
		tagQuad.tr.texCoords.v	= fTop;

		tagQuad.bl.colors		= ccc4( 255, 255, 255, 255 );
		tagQuad.br.colors		= ccc4( 255, 255, 255, 255 );
		tagQuad.tl.colors		= ccc4( 255, 255, 255, 255 );
		tagQuad.tr.colors		= ccc4( 255, 255, 255, 255 );

		m_mapTextureNames[Ite->first] = unIndex;
		m_pTextureAtlas->insertQuad( &tagQuad, unIndex++ );
	}
}
//-------------------------------------------------------------------------
CCocos2dxDBTextureAtlas::~CCocos2dxDBTextureAtlas()
{
	Clear();
}
//-------------------------------------------------------------------------
// TextureAtlas的名字
const string& CCocos2dxDBTextureAtlas::GetName()
{
	return m_pTextureAtlasData->m_szName;
}
//-------------------------------------------------------------------------
// 获取指定子资源所占面积区域
CRect CCocos2dxDBTextureAtlas::GetRegion( const string& p_szName )
{
	map< string, CRect >::iterator Ite = m_pTextureAtlasData->m_szRects.find( p_szName );
	if( Ite != m_pTextureAtlasData->m_szRects.end() )
	{
		return Ite->second;
	}
	else
	{
		return CRect();
	}
}
//-------------------------------------------------------------------------
// 资源清理
void CCocos2dxDBTextureAtlas::Clear()
{
	CC_SAFE_DELETE( m_pTextureAtlasData );
	m_pTextureAtlasData = NULL;
	CC_SAFE_RELEASE_NULL( m_pTextureAtlas );
}
//-------------------------------------------------------------------------
uint CCocos2dxDBTextureAtlas::GetQuadIndex( const string& p_szName )
{
	map< string, uint >::iterator Ite = m_mapTextureNames.find( p_szName );
	if( Ite != m_mapTextureNames.end() )
	{
		return Ite->second;
	}
	return 0;
}
//-------------------------------------------------------------------------
CCTextureAtlas* CCocos2dxDBTextureAtlas::GetTextureAtlas()
{
	return m_pTextureAtlas;
}
//-------------------------------------------------------------------------
CCTexture2D* CCocos2dxDBTextureAtlas::GetTexture2D()
{
	return m_pTexture2D;
}
//-------------------------------------------------------------------------