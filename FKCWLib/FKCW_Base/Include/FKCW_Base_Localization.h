/********************************************************************
	created:	2014/10/27
	file base:	FKCW_Base_Localization
	author:		FreeKnight
	
	purpose:	
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_Base_Macro.h"
//--------------------------------------------------------------------
class FKCW_Base_Localization : public CCObject
{
private:
	static FKCW_Base_Localization* s_Instance;
private:
	// Key 是语言的ISO编号，Value是字符串CCDictionary
	CCDictionary		m_LanguageDicMap;		// 语言表
private:
	// 用 ascii 码替换 \n \t \r
	string				__UnEscape( const string& s );
protected:
	FKCW_Base_Localization();
public:
	virtual ~FKCW_Base_Localization();
	static FKCW_Base_Localization* GetInstance();
public:
	// 读取一个平台的strings.xml文件
	// 注：文件必须是平台xml格式文件
	// 参数：p_strLanguage ISO639-1双字母代码 
	// 参数：p_strPath xml文件的路径。该路径是平台无关的，它将被映射到对应的平台格式
	// 例如：Android上的"/sdcard/strings.xml" 将被映射到iOS系统上的"~/Documents/sdcard/strings.xml" 中
	// 参数：p_bMerge。True表示如果当前字符串已经存在，新字符串将会合并进去。
	//		false表示若当前字符串已经存在，则新字符串会直接替代原字符串。
	void	AddPlatformString( const string& p_strLanguage, const string& p_strPath, bool p_bMerge = false );

	// 通过指定的语言种类，查找key对应的value，如果该语言找不到，则会恢复为默认的 English 语言进行查找。
	// 注：默认使用中文
	string	GetString( const string& p_strKey, const string& p_strLanguage = "zh" );
};
//--------------------------------------------------------------------
#define FKCW_LocStr(k)		(FKCW_Base_Localization::GetInstance()->GetString((k)))
#define FKCW_LocCharPtr(k)	(FKCW_LocStr(k).c_str())