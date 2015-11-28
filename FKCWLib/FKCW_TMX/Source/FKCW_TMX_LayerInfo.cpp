//-------------------------------------------------------------------------
#include "../Include/FKCW_TMX_LayerInfo.h"
//-------------------------------------------------------------------------
FKCW_TMX_LayerInfo::FKCW_TMX_LayerInfo() :
	m_tiles(NULL),
	m_layerHeight(0),
	m_layerWidth(0),
	m_alpha(255),
	m_visible(true),
	m_offsetX(0),
	m_offsetY(0) 
{
}
//-------------------------------------------------------------------------
FKCW_TMX_LayerInfo::~FKCW_TMX_LayerInfo() 
{
	CC_SAFE_FREE(m_tiles);
}
//-------------------------------------------------------------------------
FKCW_TMX_LayerInfo* FKCW_TMX_LayerInfo::create() 
{
	FKCW_TMX_LayerInfo* li = new FKCW_TMX_LayerInfo();
	return (FKCW_TMX_LayerInfo*)li->autorelease();
}
//-------------------------------------------------------------------------
void FKCW_TMX_LayerInfo::addProperty(const string& key, const string& value) 
{
	if(!m_properties.objectForKey(key)) {
		m_properties.setObject(CCString::create(value), key);
	}
}
//-------------------------------------------------------------------------
string FKCW_TMX_LayerInfo::getProperty(const string& key) 
{
	CCString* p = (CCString*)m_properties.objectForKey(key);
	if(p)
		return p->getCString();
	else
		return "";
}
//-------------------------------------------------------------------------