//*************************************************************************
//	创建日期:	2014-12-29
//	文件名称:	FKCW_TMX_ObjectGroup.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_TMX_Object.h"
//-------------------------------------------------------------------------
class FKCW_TMX_ObjectGroup : public CCObject
{
protected:
	FKCW_TMX_ObjectGroup();
	
public:
	virtual ~FKCW_TMX_ObjectGroup();
	static FKCW_TMX_ObjectGroup* create();
	
	// 添加一个属性
	void addProperty(const string& key, const string& value);
	// 根据Key获取一个属性值
	string getProperty(const string& name);
	
	// 创建一个新的TMXObject
	// 注：该对象已添加到TMXObjectGroup中
	FKCW_TMX_Object* newObject();
	// 根据名字获取一个Object
	// 若该对象不存在，则返回NULL
	FKCW_TMX_Object* getObject(const string& name);
	// 根据位置索引查找一个Object
	// 若该位无对象，则返回NULL
	FKCW_TMX_Object* getObjectAt(int index);
	// 获取对象组中的对象个数
	int getObjectCount() { return m_objects.count(); }
	
public:
	CC_SYNTHESIZE(string, m_name, Name);									// 对象组名
	CC_SYNTHESIZE(float, m_offsetX, OffsetX);								// 对象组偏移X
	CC_SYNTHESIZE(float, m_offsetY, OffsetY);								// 对象组偏移Y
	CC_SYNTHESIZE_PASS_BY_REF(CCArray, m_objects, Objects);					// 对象列表，每个单元是一个FKCW_TMX_Object
	CC_SYNTHESIZE_PASS_BY_REF_NC(CCDictionary, m_properties, Properties);	// 属性词典
	CC_SYNTHESIZE(int, m_color, Color);										// 对象组颜色.argb8888格式的。alpha为0xff
	CC_SYNTHESIZE_SETTER(float, m_opacity, Opacity);						// 对象组透明度【0,1】
};