//*************************************************************************
//	创建日期:	2014-12-29
//	文件名称:	FKCW_TMX_MapInfo.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_TMX_Macros.h"
//-------------------------------------------------------------------------
// tmx方向
enum ENUM_TMXOrientation
{
	eTMXOrientationOrthogonal = 1,			// 直角
	eTMXOrientationIsometric,				// 斜45角
	eTMXOrientationHexagonal				// 六角形
};
// tmx格子标示
enum ENUM_TMXTileFlag
{
	eTMXTileFlagFlipH = 0x80000000,			// 水平翻转
	eTMXTileFlagFlipV = 0x40000000,			// 垂直翻转
	eTMXTileFlagFlipDiagonal = 0x20000000,	// 斜线翻转
	eTMXTileFlagFlipAll = (eTMXTileFlagFlipH | eTMXTileFlagFlipV | eTMXTileFlagFlipDiagonal),
	eTMXTileFlagFlipMask = ~(eTMXTileFlagFlipAll)
};
//-------------------------------------------------------------------------
class FKCW_TMX_MapInfo : public CCObject
{
protected:
	FKCW_TMX_MapInfo();
	
public:
	virtual ~FKCW_TMX_MapInfo();
	static FKCW_TMX_MapInfo* create();
	
	// 根据Key获取一个属性值
	string getProperty(const string& key);
	// 添加一个属性
	void addProperty(const string& key, const string& value);

	// 根据GridID获取其TileSet索引
	int getTileSetIndex(int gid);
	
	// 获取一个Tile的属性。
	// 若属性未找到，则返回空字符串
	string getTileProperty(int gid, const string& key);
	// 为一个Tile添加属性
	void addTileProperty(int gid, const string& key, const string& value);

public:
	CC_SYNTHESIZE(string, m_filename, TMXFilename);									// tmx文件名
	CC_SYNTHESIZE(ENUM_TMXOrientation, m_orientation, Orientation);					// Map方向
	CC_SYNTHESIZE(int, m_mapWidth, MapWidth);										// Map宽（Tiles个数）
	CC_SYNTHESIZE(int, m_mapHeight, MapHeight);										// Map高（Tiles个数）
	CC_SYNTHESIZE(float, m_tileWidth, TileWidth);									// Tiles宽（像素）
	CC_SYNTHESIZE(float, m_tileHeight, TileHeight);									// Tiles高（像素）
	CC_SYNTHESIZE_PASS_BY_REF_NC(CCArray, m_layers, Layers);						// 层列表
	CC_SYNTHESIZE_PASS_BY_REF_NC(CCArray, m_tilesets, TileSets);					// TilesSet信息列表
	CC_SYNTHESIZE_PASS_BY_REF_NC(CCArray, m_objectGroups, ObjectGroups);			// 对象组列表
	CC_SYNTHESIZE_PASS_BY_REF_NC(CCDictionary, m_properties, Properties);			// Map的属性词典
	CC_SYNTHESIZE_PASS_BY_REF_NC(CCDictionary, m_tileProperties, TileProperties);	// Tile属性词典，Key是其GridID,Value是一个子词典
};