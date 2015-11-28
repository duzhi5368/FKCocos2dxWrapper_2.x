/********************************************************************
	created:	2014/10/29
	file base:	FKCW_Database_Statement
	author:		FreeKnight
	
	purpose:	数据库statement封装
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_Database_Macros.h"
//--------------------------------------------------------------------
struct sqlite3_stmt;
class FKCW_Database_Database;
//--------------------------------------------------------------------
class FKCW_Database_Statement : public CCObject
{
	friend class FKCW_Database_Database;
private:
	int				m_nUseCount;			// 引用计数
private:
	FKCW_Database_Statement();
public:
	virtual ~FKCW_Database_Statement();
	// 关闭statement
	void	Close();
	// 重置statement
	void	Reset();
	// 设置statement
	void	SetStatement( sqlite3_stmt* s );
public:
	CC_SYNTHESIZE_PASS_BY_REF(string, m_strQuery, Query);
	CC_SYNTHESIZE_READONLY(sqlite3_stmt*, m_pStatement, Statement);
};