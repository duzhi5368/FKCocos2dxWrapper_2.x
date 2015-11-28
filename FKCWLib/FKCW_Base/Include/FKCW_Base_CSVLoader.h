//*************************************************************************
//	创建日期:	2014-11-11
//	文件名称:	FKCW_Base_CSVLoader.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_Base_Macro.h"
//-------------------------------------------------------------------------
class FKCW_Base_CSVLoader
{
public:
	FKCW_Base_CSVLoader();
	~FKCW_Base_CSVLoader();

	bool OpenAndResolveFile(const char *fileName);
	const char *GetData(unsigned int rowIndex, unsigned int colIndex);
	inline int GetColLength() { return m_nColLength; }
	inline int GetRowLength() { return m_Datas.size(); }
	void Clear();

private:
	const std::string						m_strSeperator;
	std::vector<std::vector<std::string> >	m_Datas;
	int										m_nColLength;

private:
	void __RowSplit(std::vector<std::string> &rows, const std::string &content, const char &rowSeperator);
	void __FieldSplit(std::vector<std::string> &fields, std::string line);
	//获取带引号的字段
	int __GetFieldWithQuoted(const std::string &line, std::string& field, int index);
	//获取无引号的字段
	int __GetFieldNoQuoted(const std::string &line, std::string &field, int index);
};