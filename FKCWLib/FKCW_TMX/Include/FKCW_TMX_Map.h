//*************************************************************************
//	创建日期:	2014-12-29
//	文件名称:	FKCW_TMX_Map.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_TMX_MapInfo.h"
//-------------------------------------------------------------------------
class FKCW_TMX_Layer;
class FKCW_TMX_ObjectGroup;
//-------------------------------------------------------------------------
class FKCW_TMX_Map : public CCNodeRGBA
{
protected:
	FKCW_TMX_Map();
	
public:
	virtual ~FKCW_TMX_Map();

	static FKCW_TMX_Map* createWithXMLFile(const string& file);
	
	virtual bool initWithXMLFile(const string& file);

	// 通过名字获取一个Layer
	FKCW_TMX_Layer* getLayer(const string& name);
	// 通过索引获取一个Layer
	FKCW_TMX_Layer* getLayerAt(int index);
	// 通过名字获取一个ObjectGroup
	FKCW_TMX_ObjectGroup* getObjectGroup(const string& name);
	// 获取一个属性
	string getProperty(const string& name);
	// 为Tile设置一个属性
	string getTileProperty(int gid, const string& name);
	// 获取TMX方向类型
	ENUM_TMXOrientation getOrientation() { return m_mapInfo->getOrientation(); }
	// 将一个点转换为TMX场景空间坐标
	CCPoint nodeToTMXSpace(CCPoint p);
	// 将一个坐标点从TMX场景空间坐标转换为当前坐标
	CCPoint tmxToNodeSpace(CCPoint p);
	
public:
	CC_SYNTHESIZE(int, m_mapWidth, MapWidth);				// 地图宽（Tiles个数）
	CC_SYNTHESIZE(int, m_mapHeight, MapHeight);				// 地图高（Tiles个数）
	CC_SYNTHESIZE(float, m_tileWidth, TileWidth);			// 地图宽（像素）
	CC_SYNTHESIZE(float, m_tileHeight, TileHeight);			// 地图高（像素）
    CC_SYNTHESIZE(FKCW_TMX_MapInfo*, m_mapInfo, MapInfo);	// 地图信息
    CC_SYNTHESIZE_BOOL_SETTER(m_debugDrawObjects, DebugDrawObjects);	// 是否开启DugDraw
};