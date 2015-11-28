//-------------------------------------------------------------------------
#include "../Include/FKCW_TMX_Map.h"
#include "../Include/FKCW_TMX_ObjectGroup.h"
#include "../Include/FKCW_TMX_Layer.h"
#include "../Include/FKCW_TMX_LayerInfo.h"
#include "../Include/FKCW_TMX_TileSetInfo.h"
#include "../Include/FKCW_TMX_ObjectDebugRender.h"
#include "../Include/FKCW_TMX_Loader.h"
//-------------------------------------------------------------------------
#define TAG_OBJECT_LAYER 0x7f7f7f7f
//-------------------------------------------------------------------------
FKCW_TMX_Map::~FKCW_TMX_Map() 
{
	CC_SAFE_RELEASE(m_mapInfo);
}
//-------------------------------------------------------------------------
FKCW_TMX_Map::FKCW_TMX_Map() :
	m_mapWidth(0),
	m_mapHeight(0),
	m_tileWidth(0),
	m_tileHeight(0),
	m_debugDrawObjects(false),
	m_mapInfo(NULL) 
{
}
//-------------------------------------------------------------------------
FKCW_TMX_Map* FKCW_TMX_Map::createWithXMLFile(const string& file) 
{
	FKCW_TMX_Map* tmx = new FKCW_TMX_Map();
	if(tmx->initWithXMLFile(file)) {
		return (FKCW_TMX_Map*)tmx->autorelease();
	}

	tmx->release();
	return NULL;
}
//-------------------------------------------------------------------------
bool FKCW_TMX_Map::initWithXMLFile(const string& file) 
{
	if(!CCNodeRGBA::init())
		return false;

	// set anchor
	setAnchorPoint(CCPointZero);
	ignoreAnchorPointForPosition(false);

	// map info
	m_mapInfo = FKCW_TMX_Loader::create()->load(file);
	CC_SAFE_RETAIN(m_mapInfo);

	// save info
	m_tileWidth = m_mapInfo->getTileWidth();
	m_tileHeight = m_mapInfo->getTileHeight();
	m_mapWidth = m_mapInfo->getMapWidth();
	m_mapHeight = m_mapInfo->getMapHeight();

	// append texture to tileset and set tileset size
	CCObject* obj;
	CCARRAY_FOREACH(&m_mapInfo->getTileSets(), obj) 
	{
		FKCW_TMX_TileSetInfo* tileset = (FKCW_TMX_TileSetInfo*)obj;
		CCTexture2D* tex = CCTextureCache::sharedTextureCache()->addImage(tileset->getSourceImagePath().c_str());
		tileset->setTexture(tex);
		tileset->setImageHeight(tex->getContentSize().height);
		tileset->setImageWidth(tex->getContentSize().width);
	}

	// create tmx layer
	int idx = 0;
	CCARRAY_FOREACH(&m_mapInfo->getLayers(), obj)
	{
		FKCW_TMX_Layer* layer = FKCW_TMX_Layer::create(idx, m_mapInfo);
		if(!layer)
			continue;

		// add
		addChild(layer, idx, idx);
		idx++;

		// set map size
		CCSize size = getContentSize();
		CCSize layerSize = layer->getContentSize();
		size.width = MAX(size.width, layerSize.width);
		size.height = MAX(size.height, layerSize.height);
		setContentSize(size);
	}

	return true;
}
//-------------------------------------------------------------------------
FKCW_TMX_Layer* FKCW_TMX_Map::getLayer(const string& name) 
{
	CCArray* children = getChildren();
	int cc = getChildrenCount();
	for(int i = 0; i < cc; i++) {
		CCNode* child = (CCNode*)children->objectAtIndex(i);
		FKCW_TMX_Layer* layer = dynamic_cast<FKCW_TMX_Layer*>(child);
		if(layer != NULL) {
			if(layer->getLayerInfo()->getName() == name)
				return layer;
		}
	}
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_TMX_Layer* FKCW_TMX_Map::getLayerAt(int index)
{
	CCArray* children = getChildren();
	int cc = getChildrenCount();
	for(int i = 0; i < cc; i++) 
	{
		CCNode* child = (CCNode*)children->objectAtIndex(i);
		FKCW_TMX_Layer* layer = dynamic_cast<FKCW_TMX_Layer*>(child);
		if(layer != NULL) 
		{
			index--;
			if(index < 0)
				return layer;
		}
	}
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_TMX_ObjectGroup* FKCW_TMX_Map::getObjectGroup(const string& name) 
{
	CCObject* obj;
	CCARRAY_FOREACH(&m_mapInfo->getObjectGroups(), obj) 
	{
		FKCW_TMX_ObjectGroup* og = (FKCW_TMX_ObjectGroup*)obj;
		if(og->getName() == name)
		{
			return og;
		}
	}
	return NULL;
}
//-------------------------------------------------------------------------
string FKCW_TMX_Map::getProperty(const string& name) 
{
	return m_mapInfo->getProperty(name);
}
//-------------------------------------------------------------------------
string FKCW_TMX_Map::getTileProperty(int gid, const string& name) 
{
	return m_mapInfo->getTileProperty(gid, name);
}
//-------------------------------------------------------------------------
void FKCW_TMX_Map::setDebugDrawObjects(bool flag) 
{
	if(m_debugDrawObjects != flag) 
	{
		m_debugDrawObjects = flag;

		// add object layer or remove it
		if(m_debugDrawObjects)
		{
			FKCW_TMX_ObjectDebugRender* r = FKCW_TMX_ObjectDebugRender::create(this);
			addChild(r, TAG_OBJECT_LAYER, MAX_INT);
		} 
		else 
		{
			removeChildByTag(TAG_OBJECT_LAYER);
		}
	}
}
//-------------------------------------------------------------------------
CCPoint FKCW_TMX_Map::nodeToTMXSpace(CCPoint p)
{
	switch(m_mapInfo->getOrientation()) 
	{
	case eTMXOrientationOrthogonal:
	case eTMXOrientationHexagonal:
		// ortho and hex is simple, just convert origin
		return ccp(p.x, getContentSize().height - p.y);
	case eTMXOrientationIsometric:
		{
			// iso map origin is at top vertex of (0, 0) tile, and x&y axis follows edges of that tile
			float x = m_mapHeight * m_tileHeight - p.y - (m_mapWidth * m_tileWidth / 4 - p.x / 2);
			float y = m_mapHeight * m_tileHeight - p.y + (m_mapWidth * m_tileWidth / 4 - p.x / 2);
			return ccp(x, y);
		}
	default:
		return p;
	}
}
//-------------------------------------------------------------------------
CCPoint FKCW_TMX_Map::tmxToNodeSpace(CCPoint p) 
{
	switch(m_mapInfo->getOrientation()) 
	{
	case eTMXOrientationOrthogonal:
	case eTMXOrientationHexagonal:
		// ortho and hex is simple, just convert origin
		return ccp(p.x, getContentSize().height - p.y);
	case eTMXOrientationIsometric:
		{
			// iso map origin is at top vertex of (0, 0) tile, and x&y axis follows edges of that tile
			float x = m_mapWidth * m_tileWidth / 2 + p.x - p.y;
			float y = (m_mapHeight * m_tileHeight * 2 - p.x - p.y) / 2;
			return ccp(x, y);
		}
	default:
		return p;
	}
}
//-------------------------------------------------------------------------