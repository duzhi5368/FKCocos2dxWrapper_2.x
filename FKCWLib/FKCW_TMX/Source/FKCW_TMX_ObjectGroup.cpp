//-------------------------------------------------------------------------
#include "../Include/FKCW_TMX_ObjectGroup.h"
//-------------------------------------------------------------------------
FKCW_TMX_ObjectGroup::FKCW_TMX_ObjectGroup() :
	m_offsetX(0),
	m_offsetY(0),
	m_color(0xffffffff),
	m_opacity(1) 
{
}
//-------------------------------------------------------------------------
FKCW_TMX_ObjectGroup::~FKCW_TMX_ObjectGroup() 
{
}
//-------------------------------------------------------------------------
FKCW_TMX_ObjectGroup* FKCW_TMX_ObjectGroup::create() 
{
	FKCW_TMX_ObjectGroup* g = new FKCW_TMX_ObjectGroup();
	return (FKCW_TMX_ObjectGroup*)g->autorelease();
}
//-------------------------------------------------------------------------
void FKCW_TMX_ObjectGroup::addProperty(const string& key, const string& value) 
{
	if(!m_properties.objectForKey(key)) {
		m_properties.setObject(CCString::create(value), key);
	}
}
//-------------------------------------------------------------------------
string FKCW_TMX_ObjectGroup::getProperty(const string& name) 
{
	CCString* p = (CCString*)m_properties.objectForKey(name);
	if(p)
		return p->getCString();
	else
		return "";
}
//-------------------------------------------------------------------------
FKCW_TMX_Object* FKCW_TMX_ObjectGroup::newObject() 
{
	FKCW_TMX_Object* to = FKCW_TMX_Object::create();
	m_objects.addObject(to);
	return to;
}
//-------------------------------------------------------------------------
FKCW_TMX_Object* FKCW_TMX_ObjectGroup::getObject(const string& name) 
{
	CCObject* obj;
	CCARRAY_FOREACH(&m_objects, obj) 
	{
		FKCW_TMX_Object* to = (FKCW_TMX_Object*)obj;
		if(to->getName() == name)
			return to;
	}

	return NULL;
}
//-------------------------------------------------------------------------
FKCW_TMX_Object* FKCW_TMX_ObjectGroup::getObjectAt(int index) 
{
	int nCount = static_cast<int>(m_objects.count());
	if(index < 0 || index >= nCount)
		return NULL;
	else
		return (FKCW_TMX_Object*)m_objects.objectAtIndex(index);
}
//-------------------------------------------------------------------------
void FKCW_TMX_ObjectGroup::setOpacity(float opacity)
{
	m_opacity = MIN(1, MAX(0, opacity));
}
//-------------------------------------------------------------------------