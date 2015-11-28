/********************************************************************
	created:	2014/10/29
	file base:	FKCW_Database_ResultSet
	author:		FreeKnight
	
	purpose:	结果集
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_Database_Macros.h"
//--------------------------------------------------------------------
class FKCW_Database_Statement;
class FKCW_Database_Database;
//--------------------------------------------------------------------
class FKCW_Database_ResultSet : public CCObject
{
	friend class FKCW_Database_Database;

private:
	typedef vector<string>		StringList;

	StringList		m_ColumnNames;		// 结果集的列名称
	string			m_strSQL;			// 生成该结果集的SQL语句
	CC_SYNTHESIZE_READONLY(FKCW_Database_Database*, m_pDatabase, Database);
	CC_SYNTHESIZE_READONLY(FKCW_Database_Statement*, m_pStatement, Statement);

protected:
	FKCW_Database_ResultSet( FKCW_Database_Database* p_pDB, FKCW_Database_Statement* p_pSM );
	static FKCW_Database_ResultSet* Create( FKCW_Database_Database* p_pDB, FKCW_Database_Statement* p_pSM );
	// 关闭一个结果集
	// 注：关闭后，该结果集不允许被使用。
	// 注：析构函数会调用该函数
	void			Close();
public:
	virtual ~FKCW_Database_ResultSet();
	// 移动当前指针到下一行
	// 返回值：若为false则表示已没有数据。true表示成功。
	bool			Next();
	// 是否还有一行数据
	bool			IsHasAnotherRow();
	// 获取结果集的列数
	int				ColumnCount();
	// 判断指定列是否为空
	bool			IsColumnIndexIsNull( int p_nColumnIndex );
	bool			IsColumnIsNull( string p_strColumnName );
	// 根据列名获取指定列的索引号
	int				ColumnIndexByName( string p_strColumnName );
	// 获取指定索引号的列名
	string			ColumnNameByIndex( int p_nColumnIndex );

	// 获取当前指针所在行 的 指定列内int数据
	int				IntFromColumnName( string p_strColumnName );
	int				IntFromColumnIndex( int p_nColumnIndex );
	// 获取当前指针所在行 的 指定列内long数据
	long			LongFromColumnName( string p_strColumnName );
	long			LongFromColumnIndex( int p_nColumnIndex );
	// 获取当前指针所在行 的 指定列内int64_t数据
	int64_t			Int64FromColumnName( string p_strColumnName );
	int64_t			Int64FromColumnIndex( int p_nColumnIndex );
	// 获取当前指针所在行 的 指定列内bool数据
	bool			BoolFromColumnName( string p_strColumnName );
	bool			BoolFromColumnIndex( int p_nColumnIndex );
	// 获取当前指针所在行 的 指定列内double数据
	double			DoubleFromColumnName( string p_strColumnName );
	double			DoubleFromColumnIndex( int p_nColumnIndex );
	// 获取当前指针所在行 的 指定列内string数据
	string			StringFromColumnName( string p_strColumnName );
	string			StringFromColumnIndex( int p_nColumnIndex );
	// 获取当前指针所在行 的 指定列内Blob数据
	// 注: 返回值是从源数据中拷贝的，所以调用者需要手动删除
	const void*		DataFromColumnName( string p_strColumnName, size_t* p_pOutLen );
	const void*		DataFromColumnIndex( int p_nColumnIndex, size_t* p_pOutLen );
	// 获取当前指针所在行 的 指定列内Blob数据
	// 注: 返回值是就是原数据，所以调用者无需手动删除
	const void*		DataNoCopyFromColumnName( string p_strColumnName, size_t* p_pOutLen );
	const void*		DataNoCopyFromColumnIndex( int p_nColumnIndex, size_t* p_pOutLen );
};