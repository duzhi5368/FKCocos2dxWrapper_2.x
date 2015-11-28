//-------------------------------------------------------------------------
#include "../Include/FKCW_TMX_ObjectDebugRender.h"
#include "../Include/FKCW_TMX_Object.h"
#include "../Include/FKCW_TMX_ObjectGroup.h"
#include "../Include/FKCW_TMX_Map.h"
//-------------------------------------------------------------------------
FKCW_TMX_ObjectDebugRender::FKCW_TMX_ObjectDebugRender() :
	m_map(NULL) 
{
}
//-------------------------------------------------------------------------
FKCW_TMX_ObjectDebugRender::~FKCW_TMX_ObjectDebugRender() 
{
}
//-------------------------------------------------------------------------
FKCW_TMX_ObjectDebugRender* FKCW_TMX_ObjectDebugRender::create(FKCW_TMX_Map* m) 
{
	FKCW_TMX_ObjectDebugRender* l = new FKCW_TMX_ObjectDebugRender();
	if(l->initWithMap(m)) {
		return (FKCW_TMX_ObjectDebugRender*)l->autorelease();
	}
	l->release();
	return NULL;
}
//-------------------------------------------------------------------------
bool FKCW_TMX_ObjectDebugRender::initWithMap(FKCW_TMX_Map* map)
{
	m_map = map;
	return true;
}
//-------------------------------------------------------------------------
void FKCW_TMX_ObjectDebugRender::draw() 
{
	FKCW_TMX_MapInfo* mapInfo = m_map->getMapInfo();
	CCObject* obj;
	CCARRAY_FOREACH(&mapInfo->getObjectGroups(), obj) 
	{
		FKCW_TMX_ObjectGroup* group = (FKCW_TMX_ObjectGroup*)obj;
		int oc = group->getObjectCount();
		for(int i = 0; i < oc; i++)
		{
			FKCW_TMX_Object* obj = group->getObjectAt(i);

			// get object x, y, those x, y is in tmx coordinates
			CCPoint loc = obj->getPosition();

			// is object a polygon?
			bool isNormal = obj->getShape() == FKCW_TMX_Object::eShape_NORMAL;
			bool isClosed = obj->getShape() == FKCW_TMX_Object::eShape_POLYGON;

			// fill vertex buffer
			m_vertices.Clear();
			if(isNormal) {
				// manual add point, remember currently it should be in tmx coordinate space
				CCSize s = obj->getSize();
				m_vertices.AddPoint(0, 0);
				m_vertices.AddPoint(0, s.height);
				m_vertices.AddPoint(s.width, s.height);
				m_vertices.AddPoint(s.width, 0);
				m_vertices.AddPoint(0, 0);
			} else {
				// add points
				m_vertices.AddPoints(obj->getPoints());

				// if closed, add first point
				if(isClosed) {
					m_vertices.AddPoint(obj->getPoints().GetPointAt(0));
				}
			}

			// convert all points from tmx space to node space
			CCPoint* buffer = m_vertices.GetBuffer();
			for(int i = 0; i < m_vertices.GetCount(); i++) 
			{
				CCPoint p = m_map->tmxToNodeSpace(ccp(buffer[i].x + loc.x, buffer[i].y + loc.y));
				buffer[i].x = p.x;
				buffer[i].y = p.y;
			}

			// draw lines
			ccDrawColor4B(0, 255, 0, 255);
			ccDrawPoly(buffer, m_vertices.GetCount(), false);
		}
	}
}