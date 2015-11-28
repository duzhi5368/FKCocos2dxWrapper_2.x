//*************************************************************************
//	创建日期:	2014-12-29
//	文件名称:	FKCW_TMX_LayerInfo.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_TMX_Macros.h"
//-------------------------------------------------------------------------
class FKCW_TMX_LayerInfo : public CCObject
{
protected:
	FKCW_TMX_LayerInfo();
	
public:
	virtual ~FKCW_TMX_LayerInfo();
	static FKCW_TMX_LayerInfo* create();
	
	// 根据Key获取一个属性值
	string getProperty(const string& key);
	// 添加一个属性
	void addProperty(const string& key, const string& value);
	
public:
	CC_SYNTHESIZE(string, m_name, Name);									// 层名
	CC_SYNTHESIZE(int, m_layerWidth, LayerWidth);							// 层中Tile贴片的宽度
	CC_SYNTHESIZE(int, m_layerHeight, LayerHeight);							// 层中Tile贴片的高度
	CC_SYNTHESIZE(int*, m_tiles, Tiles);									// Tile格子组
	CC_SYNTHESIZE_BOOL(m_visible, Visible);									// 层是否可见
	CC_SYNTHESIZE(int, m_alpha, Alpha);										// 层Alpha值
	CC_SYNTHESIZE_PASS_BY_REF_NC(CCDictionary, m_properties, Properties);	// 属性词典
	CC_SYNTHESIZE(float, m_offsetX, OffsetX);								// X偏移
	CC_SYNTHESIZE(float, m_offsetY, OffsetY);								// Y偏移
};