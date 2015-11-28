//-------------------------------------------------------------------------
#include "FKEventChunk.h"
//-------------------------------------------------------------------------
FKEventChunk::FKEventChunk( RENDER_MAP_CHUNK_ID id )
	: FKMapChunk( id )
{
	m_mapEventGrids.clear();
}
//-------------------------------------------------------------------------
FKEventChunk::~FKEventChunk()
{
	EventGridsMap::iterator Ite = m_mapEventGrids.begin();
	for( ; Ite != m_mapEventGrids.end(); ++Ite )
	{
		if( Ite->second )
		{
			delete Ite->second;
			Ite->second = NULL;
		}
	}
	m_mapEventGrids.clear();
}
//-------------------------------------------------------------------------
void FKEventChunk::draw()
{
	if( !m_bIsUseDebugRender )
		return;
	if( !m_bIsVisible )
		return;

	CCPoint tagOriginPos;
	CCPoint tagDestPos;
	CCRect tagRect;
	EventGridsMap::iterator Ite = m_mapEventGrids.begin();
	for( ; Ite != m_mapEventGrids.end(); ++Ite )
	{
		if( Ite->second )
		{
			Ite->second->GetRectInChunk( tagRect );
			tagOriginPos = ccp( tagRect.getMinX(), tagRect.getMinY() );
			tagDestPos = ccp( tagRect.getMaxX(), tagRect.getMaxY() );
			FKCW_Base_DrawingPrimitives::DrawSolidRoundRect( 
				tagOriginPos, tagDestPos, 4, ccc4BFromccc4F(s_EventGridDebugColor) );
		}
	}
}
//-------------------------------------------------------------------------
// 添加子格
bool FKEventChunk::_SubAddGrid( IMapGridInfo* pGridInfo )
{
	if( pGridInfo == NULL )
		return true;
	SEventGridInfo* p = dynamic_cast<SEventGridInfo*>( pGridInfo );
	if( p == NULL )
		return false;
	unsigned short id = p->GetGridID();

	EventGridsMap::iterator Ite = m_mapEventGrids.find( id );
	if( Ite != m_mapEventGrids.end() )
	{
		if( Ite->second )
		{
			// 这里返回false，外界应当手动release pGridInfo
			return false;
		}
	}

	m_mapEventGrids[id] = p;
	return true;
}
//-------------------------------------------------------------------------
// 移除子格
void FKEventChunk::_SubDelGrid( CCPoint& tagPos )
{
	unsigned short id = ((((unsigned short)tagPos.x / s_EventGridSize )<< 8) 
		| ((unsigned short)tagPos.y / s_EventGridSize ));

	EventGridsMap::iterator Ite = m_mapEventGrids.find( id );
	if( Ite != m_mapEventGrids.end() )
	{
		if( Ite->second )
		{
			delete Ite->second;
			Ite->second = NULL;
		}
	}
	m_mapEventGrids[id] = NULL;
	m_mapEventGrids.erase( Ite );
}
//-------------------------------------------------------------------------