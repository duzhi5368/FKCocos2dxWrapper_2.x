//*************************************************************************
//	创建日期:	2014-12-29
//	文件名称:	FKCW_TMX_Loader.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_TMX_Macros.h"
//-------------------------------------------------------------------------
class FKCW_TMX_MapInfo;
//-------------------------------------------------------------------------
class FKCW_TMX_Loader : public CCObject, public CCSAXDelegator
{
private:
	// tmx文件标签
	enum TMXTag{
		UNKNOWN,
		MAP,
		TILESET,
		TILE,
		IMAGE,
		LAYER,
		DATA,
		PROPERTIES,
		PROPERTY,
		OBJECTGROUP,
		OBJECT,
		POLYLINE,
		POLYGON
	};

	// TMX文件标签属性
	enum TMXAttr{
		ATTR_UNKNOWN,
		ATTR_VERSION,
		ATTR_ORIENTATION,
		ATTR_WIDTH,
		ATTR_HEIGHT,
		ATTR_TILEWIDTH,
		ATTR_TILEHEIGHT,
		ATTR_NAME,
		ATTR_FIRSTGID,
		ATTR_SPACING,
		ATTR_MARGIN,
		ATTR_VISIBLE,
		ATTR_OPACITY,
		ATTR_X,
		ATTR_Y,
		ATTR_TYPE,
		ATTR_COLOR,
		ATTR_POINTS
	};

private:
	string m_tmxDir;			// tmx目录
	FKCW_TMX_MapInfo* m_map;	// 地图数据
	vector<TMXTag> m_tags;		// 标签集
	int m_lastGid;				// 最后的GridID
	bool m_compressed;			// 是否压缩

private:
	// 文件读取
	// 若失败返回 false
	bool load(const char* data, int length);

	// 标签操作
	void pushTag(TMXTag tag);
	void popTag();
	TMXTag topTag();
	TMXTag parentTag();
	TMXTag grandpaTag();
	TMXTag getTag(const char* name);
	TMXAttr getAttr(const char* name);

	// 查找一个属性值
	const char* valueForKey(const char** atts, const char* name);

protected:
	FKCW_TMX_Loader();

public:
	virtual ~FKCW_TMX_Loader();
	static FKCW_TMX_Loader* create();

	virtual void startElement(void* ctx, const char* name, const char** atts);
	virtual void endElement(void* ctx, const char* name);
	virtual void textHandler(void* ctx, const char* s, int len);

	// 加载一个 tmx 文件
	// 参数：tmx相对路径
	FKCW_TMX_MapInfo* load(const string& tmxFile);
	// 加载一段 tmx 数据。但需要调用者给予文件路径 
	FKCW_TMX_MapInfo* load(const char* tmxData, int length, const string& resourcePath);
};