/********************************************************************
	created:	2014/10/27
	file base:	FKCW_Base_PlatformStringParser
	author:		FreeKnight
	
	purpose:	
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_Base_Macro.h"
#include "support/tinyxml2/tinyxml2.h"
//--------------------------------------------------------------------
using namespace tinyxml2;
//--------------------------------------------------------------------
class FKCW_Base_PlatformStringParser : public CCObject, public XMLVisitor
{
private:
	CCDictionary*		m_pDict;			// 词典
public:
	FKCW_Base_PlatformStringParser();
	virtual ~FKCW_Base_PlatformStringParser();
	static FKCW_Base_PlatformStringParser* Create();
public:
	// 解析平台的 xml 并保存在字符串字典中
	// 参数：p_strPath xml文件的路径。该路径是平台无关的，它将被映射到对应的平台格式
	// 例如：Android上的"/sdcard/strings.xml" 将被映射到iOS系统上的"~/Documents/sdcard/strings.xml" 中
	void			Parse( const string& p_strPath, const CCDictionary& p_Dict );
	// 重载XMLVisitor函数
	virtual bool	VisitEnter(const XMLElement& element, const XMLAttribute* firstAttribute);
};
