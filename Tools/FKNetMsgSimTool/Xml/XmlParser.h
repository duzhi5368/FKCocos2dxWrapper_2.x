#ifndef _XML_PARSER_
#define _XML_PARSER_

//-------------------------------------------------------------------------
#include "support/tinyxml2/tinyxml2.h"
#include "FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
enum ENUM_MSG_DATA_TYPE
{
	EMDT_SHORT,
	EMDT_INT,
	EMDT_FLOAT,
	EMDT_BOOL,
	EMDT_STRING,
};

//消息属性对象
struct MsgAttObject
{
	std::string name;			//名
	ENUM_MSG_DATA_TYPE type;	//数据类型
	std::string data;			//数据
};

struct MsgPack
{
	short mainType;
	short subType;
	std::vector<MsgAttObject> atts;

	short fbm;	//反馈主类型
	short fbs;	//反馈子类型
};

class XmlParser : public cocos2d::CCObject
{
public:
	//获取XmlParser单例
	static XmlParser* getInstance()
	{
		if(m_pInstance == NULL)
		{
			m_pInstance = new XmlParser();
		}
		return m_pInstance;
	}
	
	//载入xml文件
	bool loadFile(const char* pFilePath);
	void newFile(const char* pFilePath);

	//获取带格式的主节点CString集  格式如:"【主类型】描述"
	std::vector<std::string> getMainElementCStrings();
	//获取带格式的子节点Cstring集  格式如:"【子类型】 描述 [反馈主类型](反馈子类型)"
	std::vector<std::string> getSubElementCstrings(short mainType);
	//获取带格式的属性Cstring集  格式如:"【属性名】 (数据类型) 默认值"
	std::vector<std::string> getAttributeCstrings(short mainType, short subType);
	
	//获取某个子消息的描述
	std::string getSubElementName(short mainType, short subType);
	//获取某个子消息的反馈主类型
	short getSubElementFBM(short mainType, short subType);
	//获取某个子消息的反馈子类型
	short getSubElementFBS(short mainType, short subType);

	//获取所有属性对象 todo...
	std::vector<MsgAttObject> getAttributeObjects(short mainType, short subType);
	//新的格式如:"【属性名】 (数据类型) 默认值"
	std::string getAttributeCstring(short mainType, short subType, std::string attName);
	MsgAttObject getAttributeObject(short mainType, short subType, std::string attName);


	//添加主类型
	bool addMainElement(short mainType, std::string name);
	//添加子类型
	bool addSubElement(short mainType, std::string name, short subType, short fbm, short fbs);
	//添加属性
	bool addAttributeElement(short mainType, short subType, std::string attName, std::string dataType, std::string sValue);

	//修改主类型type
	bool setMainElementType(short mainType, short newMainType);
	//修改子类型type
	bool setSubElementType(short mainType, short subType, short newMainType);
	//修改子类型CS
	bool setSubElementCS(short mainType, short subType, std::string newCS);
	//修改属性名
	bool setAttributeElementName(short mainType, short subType, std::string attName, std::string newName);
	//修改主类型描述
	bool setMainElementName(short mainType, std::string newName);
	//修改子类型描述
	bool setSubElementName(short mainType, short subType, std::string newName);
	//设置反馈主类型
	bool setSubElementFBM(short mainType, short subType, short fbm);
	//设置反馈子类型
	bool setSubElementFBS(short mainType, short subType, short fbs);
	//修改属性类型
	bool setAttributeElementDataType(short mainType, short subType, std::string attName, std::string newType);
	//修改属性值
	bool setAttributeElementValue(short mainType, short subType, std::string attName, std::string newValue);

	//删除主类型
	bool removeMainElement(short mainType);
	//删除子类型
	bool removeSubElement(short mainType, short subType);
	//删除属性
	bool removeAttributeElement(short mainType, short subType, std::string attName);

	bool isHaveMainElement(short mainType);
	bool isHaveSubElement(short mainType, short subType);

	//获取消息包
	MsgPack getMsgPack(short mainType, short subType);

public:
	//解析带格式的字符串 "【属性名】 (数据类型) 默认值"
	static ENUM_MSG_DATA_TYPE GetDataType(std::string text);

	ENUM_MSG_DATA_TYPE getDataType(short mainType, short subType, std::string attName);
	static std::string	GetAttName(std::string text);
	static std::string	GetAttData(std::string text);
	static std::string	GetAttType(std::string text);
	//截取成不包括反馈类型的字符串
	static std::string  GetSubString(std::string text);
	//解析带格式的字符串
	static short		GetMainType(std::string text);
	static string		GetMainName(std::string text);
	//解析带格式的字符串
	static short		GetSubType(std::string text);
	static std::string  GetNoFBTypeToSubName(std::string text);
	//解析反馈主类型
	static short		GetSubFBM(std::string text);
	static short		GetSubFBS(std::string text);

	//将std::string 转换成int
	static int			StringToInt(std::string text)
	{
		return atoi(text.c_str());
	}
	//将std::string 转换成float
	static float		StringToFloat(std::string text)
	{
		return (float)atof(text.c_str());
	}
	//将std::string 转换成short
	static short		StringToShort(std::string text)
	{
		return (short)atoi(text.c_str());
	}
	//将std::string 转换成long
	static string   	StringToBool(std::string text)
	{
		if(text.compare("false") == 0 || text.compare("0") == 0)
			return "false";
		return "true";
	}

private:
	//得到主类型的XMLElement
	tinyxml2::XMLElement* _getMainElement(short mainType);
	//得到子类型的XMLElement
	tinyxml2::XMLElement* _getSubElement(short mainType, short subType);
	//得到属性的XMLElement
	tinyxml2::XMLElement* _getAttributeElement(short mainType, short subType, std::string attName);

	//类型转换
	string _typeConversion(string type, string value);

private:
	static XmlParser* m_pInstance;	//XmlParser单例
	std::string m_pFilePath;		//当前配置文件路径
	tinyxml2::XMLDocument* m_pDoc;

private:
	XmlParser();
	~XmlParser();

};

#endif