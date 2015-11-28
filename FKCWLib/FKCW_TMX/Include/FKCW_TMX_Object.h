//*************************************************************************
//	创建日期:	2014-12-29
//	文件名称:	FKCW_TMX_Object.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_TMX_Macros.h"
//-------------------------------------------------------------------------
class FKCW_TMX_Object : public CCObject 
{
public:
	// 对象类型
	enum ENUM_Shape 
	{
		eShape_NORMAL,		// 普通对象，格式为 Key = value 组
		eShape_POLYGON,		// 多边形对象，可以获取顶点点列表
		eShape_POLYLINE		// 折线对象，首尾节点不连接
	};
	
protected:
	FKCW_TMX_Object();
	
public:
	virtual ~FKCW_TMX_Object();
	static FKCW_TMX_Object* create();
	
	// 根据Key获取一个属性值
	string getProperty(const string& key);
	// 添加一个属性
	void addProperty(const string& key, const string& value);
public:
	CC_SYNTHESIZE_PASS_BY_REF_NC(CCDictionary, m_properties, Properties);	// 属性词典
	CC_SYNTHESIZE_PASS_BY_REF_NC(FKCW_Base_PointList, m_points, Points);	// 点列表（仅用于POLYGON和POLYLINE对象）
	CC_SYNTHESIZE(ENUM_Shape, m_shape, Shape);								// 对象类型
	CC_SYNTHESIZE_PASS_BY_REF_NC(string, m_name, Name);						// 名字
	CC_SYNTHESIZE_PASS_BY_REF_NC(string, m_type, Type);						// 类型
	CC_SYNTHESIZE_PASS_BY_REF_NC(CCPoint, m_pos, Position);					// 位置
	CC_SYNTHESIZE_PASS_BY_REF_NC(CCSize, m_size, Size);						// 大小
};
