//-------------------------------------------------------------------------
#include "../Include/FKCW_TMX_Object.h"
//-------------------------------------------------------------------------
FKCW_TMX_Object::FKCW_TMX_Object() :
	m_shape(eShape_NORMAL),
	m_pos(CCPointZero),
	m_size(CCSizeZero) 
{
}
//-------------------------------------------------------------------------
FKCW_TMX_Object::~FKCW_TMX_Object() 
{
}
//-------------------------------------------------------------------------
FKCW_TMX_Object* FKCW_TMX_Object::create() 
{
	FKCW_TMX_Object* o = new FKCW_TMX_Object();
	return (FKCW_TMX_Object*)o->autorelease();
}
//-------------------------------------------------------------------------
string FKCW_TMX_Object::getProperty(const string& key) 
{
	CCString* p = (CCString*)m_properties.objectForKey(key);
	if(p)
		return p->getCString();
	else
		return "";
}
//-------------------------------------------------------------------------
void FKCW_TMX_Object::addProperty(const string& key, const string& value) 
{
	if(!m_properties.objectForKey(key)) 
	{
		m_properties.setObject(CCString::create(value), key);
	}
}
//-------------------------------------------------------------------------