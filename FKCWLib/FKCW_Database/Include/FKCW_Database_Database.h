/********************************************************************
	created:	2014/10/29
	file base:	FKCW_Database_Database
	author:		FreeKnight
	
	purpose:	Sqlite3的C++封装
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_Database_Macros.h"
#include "FKCW_Database_ResultSet.h"
#include "FKCW_Database_Statement.h"
//--------------------------------------------------------------------
struct sqlite3;
//--------------------------------------------------------------------
class FKCW_Database_Database : public CCObject
{
	friend class FKCW_Database_ResultSet;
private:
	bool			m_bIsShouldCacheStatements;		// 是否需要缓存statement

	typedef map<string, FKCW_Database_Statement*>	StatementMap;
	StatementMap	m_CachedStatements;				// 被缓存的编译过的statement
public:
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(string, m_strDatabasePath, DatabasePath);		// 数据库DB文件路径
	CC_SYNTHESIZE_READONLY(sqlite3*, m_pDB, Sqlite3Handle);								// 数据库对象
	CC_SYNTHESIZE(int, m_nBusyRetryTimeout, BusyRetryTimeout);							// 如果数据库在忙，那将间隔20毫秒进行一次操作尝试。该值为最大尝试次数
	CC_SYNTHESIZE_READONLY_BOOL(m_bIsInTransaction, InTransaction);						// 是否数据库正在处理事务中
	CC_SYNTHESIZE_BOOL(m_bIsInUse, InUse);												// 是否数据库正在执行某操作
private:
	// 输出一个警告，当前DB正在被使用中
	void						__WarnInUse();
	// 获取缓存的statement
	FKCW_Database_Statement*	__GetCachedStatement( const char* p_szSql );
	// 缓存statement
	void						__SetCachedStatement( const char* p_szSql, FKCW_Database_Statement* p_pSM );
	// 执行一个sql查询
	// 返回值：如果为空，则查询失败。
	FKCW_Database_ResultSet*	__ExecuteQuery( const char* p_szSql );
	// 执行一个非查询的sql语句
	bool						__ExecuteUpdate( const char* p_szSql );
	// 当结果集需要关闭时，由FKCW_Database_ResultSet调用
	void						__PostResultSetClosed( FKCW_Database_ResultSet* p_RS );
protected:
	FKCW_Database_Database( string p_strPath );
public:
	virtual			~FKCW_Database_Database();
	// 创建一个DB对象
	// 参数：p_strPath 一个平台无关的database文件路径
	static FKCW_Database_Database* Create( string p_strPath );
	// 是否线程安全
	static bool		IsThreadSafe();
	// 获取sqlite版本
	static string	SqliteLibVersion();

	// 获取sqlite handler
	sqlite3*		SqliteHandler();
	// 打开DB
	bool			Open( int p_nFlags = 0 );
	// 关闭DB
	// 注：该函数在析构函数中被调用，故可以无需手动调用。另外关闭一个DB后，可以调用Open()直接再次打开
	bool			Close();
	// 检查DB是否打开
	bool			IsDatabaseOpened();
	// 检查DB链接是否正常
	bool			IsGoodConnection();
	// 清除缓存的statements
	void			ClearCachedStatements();
	// 执行一个查询SQL
	FKCW_Database_ResultSet*	ExecuteQuery( string p_strSql, ... );
	// 执行一次非查询SQL
	bool			ExecuteUpdate( string p_strSql, ... );
	// 获取最后一个错误
	// 返回值：若没有错误，则会返回空字符串
	string			LastErrorMessage();
	// 获取最后一个错误
	// 返回值：若没有错误，则会返回 SQLITE_OK
	int				LastErrorCode();
	// 是否上一个操作有错误
	// 返回值：true则表示上一个操作有错误
	bool			IsHadError();
	// 获取最后插入的数据行ID
	int64_t			LastInsertRowID();
	// 是否缓存了statements
	bool			IsCachedStatements();
	// 设置是否缓存statements
	void			SetCachedStatements( bool p_bValue );
	// 获取上一次操作影响的数据行数
	int				Changes();
	// 回滚上一个事务
	bool			Rollback();
	// 提交一个事物
	bool			Commit();
	// 开始一个延期事务
	// 注：延期事务意思是所有修改都保存在一个缓存内，当全部修改都完成之后，再一次性将缓存同步到数据库中
	bool			BeginDeferredTransaction();
	// 开始一个事务
	bool			BeginTransaction();
	// 检查一个表是否存在
	bool			IsTableExists( string p_strTableName );

	// 通过一个查询，得到一个int值
	int				IntFromQuery( string p_strSql, ... );
	// 通过一个查询，得到一个long值
	long			LongFromQuery( string p_strSql, ... );
	// 通过一个查询，得到一个int64_t值
	int64_t			Int64FromQuery( string p_strSql, ... );
	// 通过一个查询，得到一个bool值
	bool			BoolFromQuery( string p_strSql, ... );
	// 通过一个查询，得到一个double值
	double			DoubleFromQuery( string p_strSql, ... );
	// 通过一个查询，得到一个string值
	string			StringFromQuery( string p_strSql, ... );
	// 通过一个查询，得到一个blob值
	// 注: 返回值是从源数据中拷贝的，所以调用者需要手动删除
	const void*		DataFromQuery( string p_strSql, size_t* p_pOutLen, ... );
	// 通过一个查询，得到一个blob值
	// 注: 返回值是就是原数据，所以调用者无需手动删除
	const void*		DataNoCopyFromQuery( string p_strSql, size_t* p_pOutLen, ... );

	// 检查一个SQLStatement
	// 返回值：如果有效则返回空字符串
	string			ValidateSQL( string p_strSql, ... );
	// 获取DB框架
	FKCW_Database_ResultSet*	GetDBSchema();
	// 获取表框架
	FKCW_Database_ResultSet*	GetTableSchema( string p_strTableName );
	// 检查一个表字段是否存在
	bool			IsColumnExists( string p_strTableName, string p_strColumnName );
	// 获取数据库版本
	int				GetVersion();
	// 设置数据库版本
	// 注：默认创建时版本为1
	void			SetVersion( int p_nVersion );
	// 在事务中执行一个sql文件
	// 参数：p_strPath SQL文件 p_DeFunc 解密函数，允许sql文件被加密
	bool			ExecuteSQL( string p_strPath, DECRYPT_FUNC p_DeFunc = NULL );
	// 在事务中执行一个sql文件
	bool			ExecuteSQL(const void* p_szData, size_t p_unLength );
};