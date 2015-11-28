//*************************************************************************
//	创建日期:	2015-1-13
//	文件名称:	IConfigFileSystem.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
/*
	注意：本系统仅处理配置文件相关，文件将保证在App初始化时被加载，
		  且将仅仅在程序退出时才被释放的配置资源才放置这里。

	当前管理的有以下数据文件格式类型：
	1：本地化语言所需的文字集【指定个数的固定xml格式】 string-en.xml string-zn.xml 等
	2：本地用户数据配置文件【单个固定xml格式】 UserDefault.xml
	3：策划配置表数据文件【多个非固定csv格式】 一系列的csv格式配置文件
	4：用户大数据记录文件【单个固定sqlite3数据库格式】 UserDefault.db
	5：复杂数据配置文件【多个非固定json格式】一系列的json格式配置文件，用于多层级关系数据表达
	注意，游戏中美术相关的动作文件，地图文件，角色配置文件，UI信息文件均不属于本类管理范畴。
	
	以后可能需要添加的基础支持包括【TODO】
	1：两种格式：简易自定义非固定ini配置文件管理；XML配置文件管理，多层级关系数据管理
	2：部分额外功能：加密，zip包，pak包中加载
	3：多线程加载支持
*/
//-------------------------------------------------------------------------
#include <string>
//-------------------------------------------------------------------------
// Csv配置文件ID
typedef int		CSV_ID;
//-------------------------------------------------------------------------
// 配置文件类型
enum ENUM_ConfigFileType
{
	eCFT_Localization			= 0,	// 本地化语言文件
	eCFT_UserDefaultXML,				// 系统级用户基本数据xml文件
	eCFT_UserDefaultDB,					// 用户基本数据db文件
	eCFT_CSVConfig,						// 策划CSV配置文件
	eCFT_JsonConfig,					// Json复杂层级配置文件
	eCFT_IniConfig,						// 标准配置文件
};
//-------------------------------------------------------------------------
// 配置文件系统
class IConfigFileSystem
{
public:
	// 释放本系统
	virtual void		Release() = 0;
	// 加载一个配置文件
	// 参数：szConfigFile 配置文件路径和文件名
	// 参数：eType 配置文件类型
	// 参数：nID 本配置文件唯一ID
	virtual bool		LoadConfig( const char* szConfigFile, ENUM_ConfigFileType eType, CSV_ID nID ) = 0;
	// 更新修改一个配置文件
	// 参数：nID 本配置文件唯一ID
	virtual bool		SaveConfig( CSV_ID nID ) = 0;
	// 获取一个配置文件ID
	virtual CSV_ID		GetConfigID( const char* szConfigFile, ENUM_ConfigFileType eType ) = 0;
	// 读取数据
	// 参数：nID 配置文件唯一ID
	// 参数：szCol 若配置表为 eCFT_UserDefaultDB 和 eCFT_CSVConfig 格式时，这里为字段名
	//			   若配置表为 eCFT_Localization 格式时，这里为ISO639-1双字母代码，如"zh","en"等
	//			   若配置表为 eCFT_UserDefaultXML 格式时，此处无效
	//			   若配置表为 eCFT_JsonConfig 格式时，此处为父对象的FKCW_Json_Object指针，最上层则填NULL
	// 参数：szKey 键值
	// 参数：outValue 读取到的数据（若读取不到，则被填充cDefaultValue ）
	// 参数：cDefaultValue 若读取不到，获得的默认值
	// 返回值：读取全过程中出于任何情况的失败，都将返回false；成功则返回true
	virtual bool		GetCharByKey( CSV_ID nID, const char* szCol, const char* szKey,
		char& outValue, char cDefaultValue = 0 ) = 0;
	virtual bool		GetShortByKey( CSV_ID nID, const char* szCol, const char* szKey,
		short& outValue, short sDefaultValue = 0 ) = 0;
	virtual bool		GetIntByKey( CSV_ID nID, const char* szCol, const char* szKey,
		int& outValue, int nDefaultValue = 0 ) = 0;
	virtual bool		GetFloatByKey( CSV_ID nID, const char* szCol, const char* szKey,
		float& outValue, float fDefaultValue = 0.0f ) = 0;
	virtual bool		GetStringByKey( CSV_ID nID, const char* szCol, const char* szKey,
		std::string& outValue, std::string szDefaultValue = "" ) = 0;
	virtual bool		GetBoolByKey( CSV_ID nID, const char* szCol, const char* szKey,
		bool& outValue, bool bDefaultValue = false ) = 0;

	// 写入数据
	// 注意： eCFT_Localization  eCFT_CSVConfig 这俩种格式禁止写入。故下列写入函数必将失败。
	// 注意：若写入的文件键值已有数据，则自动进行覆盖更;若没有，则创建添加。
	// 参数：szCol 若配置表为 eCFT_UserDefaultDB 和 eCFT_CSVConfig 格式时，这里为字段名
	//			   若配置表为 eCFT_Localization 格式时，这里为ISO639-1双字母代码，如"zh","en"等
	//			   若配置表为 eCFT_UserDefaultXML 格式时，此处无效
	//			   若配置表为 eCFT_JsonConfig 格式时，此处为父对象的FKCW_Json_Object指针，最上层则填NULL
	// 参数：szKey 键值
	// 参数：Value 写入或修改的数据
	// 返回值：写入全过程中出于任何情况的失败，都将返回false；成功则返回true
	virtual bool		SetCharByKey( CSV_ID nID, const char* szCol, const char* szKey, char cValue ) = 0;
	virtual bool		SetShortByKey( CSV_ID nID, const char* szCol, const char* szKey, short sValue ) = 0;
	virtual bool		SetIntByKey( CSV_ID nID, const char* szCol, const char* szKey, int nValue ) = 0;
	virtual bool		SetFloatByKey( CSV_ID nID, const char* szCol, const char* szKey, float fValue ) = 0;
	virtual bool		SetStringByKey( CSV_ID nID, const char* szCol, const char* szKey, std::string strValue ) = 0;
	virtual bool		SetBoolByKey( CSV_ID nID, const char* szCol, const char* szKey, bool bValue ) = 0;
};
//-------------------------------------------------------------------------