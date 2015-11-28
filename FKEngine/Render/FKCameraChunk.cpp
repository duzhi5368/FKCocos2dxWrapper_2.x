//-------------------------------------------------------------------------
#include "FKCameraChunk.h"
//-------------------------------------------------------------------------
FKCameraChunk::FKCameraChunk( RENDER_MAP_CHUNK_ID id )
	: FKMapChunk( id )
{
	m_mapCameraGrids.clear();
}
//-------------------------------------------------------------------------
FKCameraChunk::~FKCameraChunk()
{
	CameraGridsMap::iterator Ite = m_mapCameraGrids.begin();
	for( ; Ite != m_mapCameraGrids.end(); ++Ite )
	{
		if( Ite->second )
		{
			delete Ite->second;
			Ite->second = NULL;
		}
	}
	m_mapCameraGrids.clear();
}
//-------------------------------------------------------------------------
void FKCameraChunk::draw()
{
	if( !m_bIsUseDebugRender )
		return;
	if( !m_bIsVisible )
		return;

	CCPoint tagOriginPos;
	CCPoint tagDestPos;
	CCRect tagRect;
	CameraGridsMap::iterator Ite = m_mapCameraGrids.begin();
	for( ; Ite != m_mapCameraGrids.end(); ++Ite )
	{
		if( Ite->second )
		{
			Ite->second->GetRectInChunk( tagRect );
			tagOriginPos = ccp( tagRect.getMinX(), tagRect.getMinY() );
			tagDestPos = ccp( tagRect.getMaxX(), tagRect.getMaxY() );
			FKCW_Base_DrawingPrimitives::DrawSolidRoundRect( 
				tagOriginPos, tagDestPos, 4, ccc4BFromccc4F(s_CameraGridDebugColor) );
		}
	}
}
//-------------------------------------------------------------------------
// 添加子格
bool FKCameraChunk::_SubAddGrid( IMapGridInfo* pGridInfo )
{
	if( pGridInfo == NULL )
		return true;
	SCameraGridInfo* p = dynamic_cast<SCameraGridInfo*>( pGridInfo );
	if( p == NULL )
		return false;
	unsigned short id = p->GetGridID();

	CameraGridsMap::iterator Ite = m_mapCameraGrids.find( id );
	if( Ite != m_mapCameraGrids.end() )
	{
		if( Ite->second )
		{
			// 这里返回false，外界应当手动release pGridInfo
			return false;
		}
	}

	m_mapCameraGrids[id] = p;
	return true;
}
//-------------------------------------------------------------------------
// 移除子格
void FKCameraChunk::_SubDelGrid( CCPoint& tagPos )
{
	unsigned short id = ((((unsigned short)tagPos.x / s_CameraGridSize )<< 8) 
		| ((unsigned short)tagPos.y / s_CameraGridSize ));

	CameraGridsMap::iterator Ite = m_mapCameraGrids.find( id );
	if( Ite != m_mapCameraGrids.end() )
	{
		if( Ite->second )
		{
			delete Ite->second;
			Ite->second = NULL;
		}
	}
	m_mapCameraGrids[id] = NULL;
	m_mapCameraGrids.erase( Ite );
}
//-------------------------------------------------------------------------