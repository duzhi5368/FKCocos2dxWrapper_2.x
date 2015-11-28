//*************************************************************************
//	创建日期:	2014-12-29
//	文件名称:	FKCW_TMX_ObjectDebugRender.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_TMX_Macros.h"
//-------------------------------------------------------------------------
class FKCW_TMX_Map;
//-------------------------------------------------------------------------
class FKCW_TMX_ObjectDebugRender : public CCNode
{
protected:
	FKCW_TMX_ObjectDebugRender();

public:
	virtual ~FKCW_TMX_ObjectDebugRender();
	static FKCW_TMX_ObjectDebugRender* create(FKCW_TMX_Map* m);

	virtual bool initWithMap(FKCW_TMX_Map* map);
	virtual void draw();

	CC_SYNTHESIZE(FKCW_TMX_Map*, m_map, Map);					// tmx的Map
	CC_SYNTHESIZE(FKCW_Base_PointList, m_vertices, Vertices);	// 顶点Buffer列表
};