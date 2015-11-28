//-------------------------------------------------------------------------
#include "../Include/FKCW_TMX_MapInfo.h"
#include "../Include/FKCW_TMX_TileSetInfo.h"
//-------------------------------------------------------------------------
FKCW_TMX_MapInfo::FKCW_TMX_MapInfo() 
{
}
//-------------------------------------------------------------------------
FKCW_TMX_MapInfo::~FKCW_TMX_MapInfo() 
{
}
//-------------------------------------------------------------------------
FKCW_TMX_MapInfo* FKCW_TMX_MapInfo::create() 
{
	FKCW_TMX_MapInfo* m = new FKCW_TMX_MapInfo();
	return (FKCW_TMX_MapInfo*)m->autorelease();
}
//-------------------------------------------------------------------------
string FKCW_TMX_MapInfo::getProperty(const string& key) 
{
	CCString* p = (CCString*)m_properties.objectForKey(key);
	if(p)
		return p->getCString();
	else
		return "";
}
//-------------------------------------------------------------------------
void FKCW_TMX_MapInfo::addProperty(const string& key, const string& value) 
{
	if(!m_properties.objectForKey(key)) {
		m_properties.setObject(CCString::create(value), key);
	}
}
//-------------------------------------------------------------------------
int FKCW_TMX_MapInfo::getTileSetIndex(int gid) 
{
	int pureGid = gid & eTMXTileFlagFlipMask;
	int count = m_tilesets.count();
	for(int i = 0; i < count; i++) 
	{
		FKCW_TMX_TileSetInfo* tileset = (FKCW_TMX_TileSetInfo*)m_tilesets.objectAtIndex(i);
		if(tileset->getFirstGid() > pureGid)
			return i - 1;
	}
	return m_tilesets.count() - 1;
}
//-------------------------------------------------------------------------
string FKCW_TMX_MapInfo::getTileProperty(int gid, const string& key) 
{
	int pureGid = gid & eTMXTileFlagFlipMask;
	char buf[32];
	sprintf(buf, "%d", pureGid);
	CCDictionary* props = (CCDictionary*)m_tileProperties.objectForKey(buf);
	if(props) {
		CCString* p = (CCString*)props->objectForKey(key);
		if(p)
			return p->getCString();
		else
			return "";
	} else {
		return "";
	}
}
//-------------------------------------------------------------------------
void FKCW_TMX_MapInfo::addTileProperty(int gid, const string& key, const string& value) 
{
	int pureGid = gid & eTMXTileFlagFlipMask;
	char buf[32];
	sprintf(buf, "%d", pureGid);
	CCDictionary* props = (CCDictionary*)m_tileProperties.objectForKey(buf);
	if(!props) {
		props = CCDictionary::create();
		m_tileProperties.setObject(props, buf);
	}
	props->setObject(CCString::create(value), key);
}
//-------------------------------------------------------------------------