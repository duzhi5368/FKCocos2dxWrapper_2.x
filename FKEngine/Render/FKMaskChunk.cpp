//-------------------------------------------------------------------------
#include "FKMaskChunk.h"
//-------------------------------------------------------------------------
FKMaskChunk::FKMaskChunk( RENDER_MAP_CHUNK_ID id )
	: FKMapChunk( id )
{
	m_mapMaskGrids.clear();
}
//-------------------------------------------------------------------------
FKMaskChunk::~FKMaskChunk()
{
	MaskGridsMap::iterator Ite = m_mapMaskGrids.begin();
	for( ; Ite != m_mapMaskGrids.end(); ++Ite )
	{
		if( Ite->second )
		{
			delete Ite->second;
			Ite->second = NULL;
		}
	}
	m_mapMaskGrids.clear();
}
//-------------------------------------------------------------------------
void FKMaskChunk::draw()
{
	if( !m_bIsUseDebugRender )
		return;
	if( !m_bIsVisible )
		return;

	CCPoint tagOriginPos;
	CCPoint tagDestPos;
	CCRect tagRect;
	MaskGridsMap::iterator Ite = m_mapMaskGrids.begin();
	for( ; Ite != m_mapMaskGrids.end(); ++Ite )
	{
		if( Ite->second )
		{
			Ite->second->GetRectInChunk( tagRect );
			tagOriginPos = ccp( tagRect.getMinX(), tagRect.getMinY() );
			tagDestPos = ccp( tagRect.getMaxX(), tagRect.getMaxY() );
			FKCW_Base_DrawingPrimitives::DrawSolidRoundRect( 
				tagOriginPos, tagDestPos, 4, ccc4BFromccc4F(s_MaskGridDebugColor) );
		}
	}
}
//-------------------------------------------------------------------------
// 添加子格
bool FKMaskChunk::_SubAddGrid( IMapGridInfo* pGridInfo )
{
	if( pGridInfo == NULL )
		return true;
	SMaskGridInfo* p = dynamic_cast<SMaskGridInfo*>( pGridInfo );
	if( p == NULL )
		return false;
	unsigned short id = p->GetGridID();

	MaskGridsMap::iterator Ite = m_mapMaskGrids.find( id );
	if( Ite != m_mapMaskGrids.end() )
	{
		if( Ite->second )
		{
			// 这里返回false，外界应当手动release pGridInfo
			return false;
		}
	}

	m_mapMaskGrids[id] = p;
	return true;
}
//-------------------------------------------------------------------------
// 移除子格
void FKMaskChunk::_SubDelGrid( CCPoint& tagPos )
{
	unsigned short id = ((((unsigned short)tagPos.x / s_MaskGridSize )<< 8) 
		| ((unsigned short)tagPos.y / s_MaskGridSize ));

	MaskGridsMap::iterator Ite = m_mapMaskGrids.find( id );
	if( Ite != m_mapMaskGrids.end() )
	{
		if( Ite->second )
		{
			delete Ite->second;
			Ite->second = NULL;
		}
	}
	m_mapMaskGrids[id] = NULL;
	m_mapMaskGrids.erase( Ite );
}
//-------------------------------------------------------------------------