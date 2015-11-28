//-------------------------------------------------------------------------
#include "FKBarrierChunk.h"
//-------------------------------------------------------------------------
FKBarrierChunk::FKBarrierChunk( RENDER_MAP_CHUNK_ID id )
	: FKMapChunk( id )
{
	m_mapBarrierGrids.clear();
}
//-------------------------------------------------------------------------
FKBarrierChunk::~FKBarrierChunk()
{
	BarrierGridsMap::iterator Ite = m_mapBarrierGrids.begin();
	for( ; Ite != m_mapBarrierGrids.end(); ++Ite )
	{
		if( Ite->second )
		{
			delete Ite->second;
			Ite->second = NULL;
		}
	}
	m_mapBarrierGrids.clear();
}
//-------------------------------------------------------------------------
void FKBarrierChunk::draw()
{
	if( !m_bIsUseDebugRender )
		return;
	if( !m_bIsVisible )
		return;

	CCPoint tagOriginPos;
	CCPoint tagDestPos;
	CCRect tagRect;
	BarrierGridsMap::iterator Ite = m_mapBarrierGrids.begin();
	for( ; Ite != m_mapBarrierGrids.end(); ++Ite )
	{
		if( Ite->second )
		{
			Ite->second->GetRectInChunk( tagRect );
			tagOriginPos = ccp( tagRect.getMinX(), tagRect.getMinY() );
			tagDestPos = ccp( tagRect.getMaxX(), tagRect.getMaxY() );
			FKCW_Base_DrawingPrimitives::DrawSolidRoundRect( 
				tagOriginPos, tagDestPos, 4, ccc4BFromccc4F(s_BrrierGridDebugColor) );
		}
	}
}
//-------------------------------------------------------------------------
// 添加子格
bool FKBarrierChunk::_SubAddGrid( IMapGridInfo* pGridInfo )
{
	if( pGridInfo == NULL )
		return true;
	SBarrierGridInfo* p = dynamic_cast<SBarrierGridInfo*>( pGridInfo );
	if( p == NULL )
		return false;
	unsigned short id = p->GetGridID();

	BarrierGridsMap::iterator Ite = m_mapBarrierGrids.find( id );
	if( Ite != m_mapBarrierGrids.end() )
	{
		if( Ite->second )
		{
			// 这里返回false，外界应当手动release pGridInfo
			return false;
		}
	}

	m_mapBarrierGrids[id] = p;
	return true;
}
//-------------------------------------------------------------------------
// 移除子格
void FKBarrierChunk::_SubDelGrid( CCPoint& tagPos )
{
	unsigned short id = ((((unsigned short)tagPos.x / s_BarrierGridSize )<< 8) 
		| ((unsigned short)tagPos.y / s_BarrierGridSize ));
	
	BarrierGridsMap::iterator Ite = m_mapBarrierGrids.find( id );
	if( Ite != m_mapBarrierGrids.end() )
	{
		if( Ite->second )
		{
			delete Ite->second;
			Ite->second = NULL;
		}
	}
	m_mapBarrierGrids[id] = NULL;
	m_mapBarrierGrids.erase( Ite );
}
//-------------------------------------------------------------------------