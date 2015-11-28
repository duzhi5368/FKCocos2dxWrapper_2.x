//-------------------------------------------------------------------------
#include "../Include/FKCW_TMX_TileSetInfo.h"
#include "../Include/FKCW_TMX_MapInfo.h"
//-------------------------------------------------------------------------
FKCW_TMX_TileSetInfo::FKCW_TMX_TileSetInfo() :
	m_texture(NULL),
	m_imageHeight(0),
	m_imageWidth(0),
	m_spacing(0),
	m_margin(0),
	m_tileHeight(0),
	m_tileWidth(0),
	m_firstGid(0) 
{
}
//-------------------------------------------------------------------------
FKCW_TMX_TileSetInfo::~FKCW_TMX_TileSetInfo() 
{
}
//-------------------------------------------------------------------------
FKCW_TMX_TileSetInfo* FKCW_TMX_TileSetInfo::create() 
{
	FKCW_TMX_TileSetInfo* t = new FKCW_TMX_TileSetInfo();
	return (FKCW_TMX_TileSetInfo*)t->autorelease();
}
//-------------------------------------------------------------------------
CCRect FKCW_TMX_TileSetInfo::getRect(int gid) 
{
	CCRect rect = CCRectZero;
	rect.size.width = m_tileWidth;
	rect.size.height = m_tileHeight;

	gid &= eTMXTileFlagFlipMask;
	gid -= m_firstGid;
	int maxX = (int)((m_imageWidth - m_margin * 2 + m_spacing) / (m_tileWidth + m_spacing));
	rect.origin.x = (gid % maxX) * (m_tileWidth + m_spacing) + m_margin;
	rect.origin.y = (gid / maxX) * (m_tileHeight + m_spacing) + m_margin;

	return rect;
}
//-------------------------------------------------------------------------