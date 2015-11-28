//--------------------------------------------------------------------
#include "../Include/FKCW_Database_Database.h"
#include "sqlite3/include/sqlite3.h"
//--------------------------------------------------------------------
#pragma warning( disable:4018 )
//--------------------------------------------------------------------
FKCW_Database_Database::FKCW_Database_Database( string p_strPath )
	: m_pDB( NULL )
	, m_strDatabasePath( p_strPath )
	, m_bIsInUse( false )
	, m_bIsInTransaction( false )
	, m_bIsShouldCacheStatements( false )
	, m_nBusyRetryTimeout( 0 )
{

}
//--------------------------------------------------------------------
FKCW_Database_Database::~FKCW_Database_Database()
{
	Close();

	// 释放statements
	for(StatementMap::iterator iter = m_CachedStatements.begin(); iter != m_CachedStatements.end(); iter++) 
	{
		iter->second->release();
	}
}
//--------------------------------------------------------------------
// 创建一个DB对象
// 参数：p_strPath 一个平台无关的database文件路径
FKCW_Database_Database* FKCW_Database_Database::Create( string p_strPath )
{
	FKCW_Database_Database* d = new FKCW_Database_Database(p_strPath);
	return (FKCW_Database_Database*)d->autorelease();
}
//--------------------------------------------------------------------
// 是否线程安全
bool FKCW_Database_Database::IsThreadSafe()
{
	return sqlite3_threadsafe() != SQLITE_CONFIG_SINGLETHREAD;
}
//--------------------------------------------------------------------
// 获取sqlite版本
string FKCW_Database_Database::SqliteLibVersion()
{
	return sqlite3_libversion();
}
//--------------------------------------------------------------------
// 获取sqlite handler
sqlite3* FKCW_Database_Database::SqliteHandler()
{
	return m_pDB;
}
//--------------------------------------------------------------------
// 打开DB
bool FKCW_Database_Database::Open( int p_nFlags )
{
	if( m_pDB ) 
	{
		return true;
	}

	string path = m_strDatabasePath;

	// 如果路径不存在，则创建到内存中，创建到指定路径
	if(path.empty())
	{
		path = ":memory:";
	}
	else 
	{
		// 创建文件夹路径
		//if(!CCUtils::CreateIntermediateFolders(path)) {
		//	CCLOGERROR("failed to create containing directory for database");
		//	return false;
		//}
	}

	// 创建DB库
	int err = SQLITE_OK;
#if SQLITE_VERSION_NUMBER >= 3005000
	if(p_nFlags != 0)
		err = sqlite3_open_v2(path.c_str(), &m_pDB, p_nFlags, NULL);
	else
#endif
		err = sqlite3_open(path.c_str(), &m_pDB);

	// 检查错误
	if(err != SQLITE_OK) 
	{
		CCLOGERROR("CCDatabase:open: error opening: %d", err);
		m_pDB = NULL;
		return false;
	}

	return true;
}
//--------------------------------------------------------------------
// 关闭DB
// 注：该函数在析构函数中被调用，故可以无需手动调用。另外关闭一个DB后，可以调用Open()直接再次打开
bool FKCW_Database_Database::Close()
{
	ClearCachedStatements();

	// 检查DB
	if( !m_pDB ) 
	{
		return true;
	}

	int rc;
	bool retry;
	int numberOfRetries = 0;

	// 尝试去关闭DB
	// 如果DB正常繁忙或被锁定，则间隔一段时间后继续尝试（会有次数限制，避免死循环）
	do 
	{
		retry = false;
		rc = sqlite3_close( m_pDB );
		if(SQLITE_BUSY == rc || SQLITE_LOCKED == rc) 
		{
			retry = true;
			SLEEP(20);
			if( m_nBusyRetryTimeout && (numberOfRetries++ > m_nBusyRetryTimeout)) 
			{
				CCLOGWARN("CCDatabase:close: Database busy, unable to close");
				return false;
			}
		} 
		else if(SQLITE_OK != rc) 
		{
			CCLOGWARN("CCDatabase:close: error closing!: %d", rc);
		}
	} while(retry);

	m_pDB = NULL;

	return true;
}
//--------------------------------------------------------------------
// 检查DB是否打开
bool FKCW_Database_Database::IsDatabaseOpened()
{
	if (!m_pDB) 
	{
		CCLOGWARN("The CCDatabase %p is not open.", this);
		return false;
	}

	return true;
}
//--------------------------------------------------------------------
// 检查DB链接是否正常
bool FKCW_Database_Database::IsGoodConnection()
{
	if(!m_pDB) 
	{
		return false;
	}

	// 做个简单的查询以判断链接状况
	FKCW_Database_ResultSet* rs = ExecuteQuery("select name from sqlite_master where type='table'");

	// 如果得到了数据则视为链接正常
	if(rs) 
	{
		rs->release();
		return true;
	}

	return false;
}
//--------------------------------------------------------------------
// 清除缓存的statements
void FKCW_Database_Database::ClearCachedStatements()
{
	for(StatementMap::iterator iter = m_CachedStatements.begin(); iter != m_CachedStatements.end(); iter++) 
	{
		iter->second->release();
	}
	m_CachedStatements.clear();
}
//--------------------------------------------------------------------
// 执行一次非查询SQL
bool FKCW_Database_Database::ExecuteUpdate( string p_strSql, ... )
{
	va_list args;
	va_start(args, p_strSql);
	char buf[512];
	vsprintf(buf, p_strSql.c_str(), args);
	va_end(args);

	// 执行最终sql语句
	return __ExecuteUpdate(buf);
}
//--------------------------------------------------------------------
// 执行一个非查询的sql语句
bool FKCW_Database_Database::__ExecuteUpdate( const char* p_szSql )
{
	// 首先检查数据库
	if ( !IsDatabaseOpened() ) 
	{
        return false;
    }

    // 是否正在使用中
    if ( m_bIsInUse ) 
	{
        __WarnInUse();
        return false;
    }

    // 设置现在在使用状态
    setInUse(true);

    int rc = 0;
	sqlite3_stmt* pStmt = NULL;
	FKCW_Database_Statement* cachedStmt = NULL;

	// 获取被缓存的sqlStatement
	cachedStmt = __GetCachedStatement(p_szSql);
	pStmt = cachedStmt ? cachedStmt->getStatement() : NULL;

	// 如果该sqlStatement确实被缓存着
	if(cachedStmt)
		cachedStmt->Reset();
	
    int numberOfRetries = 0;
	bool retry = false;

	// 编译statement
    if(!pStmt) 
	{
		do 
		{
			// 准备statement
			retry = false;
			rc = sqlite3_prepare_v2( m_pDB, p_szSql, -1, &pStmt, 0);

			// 如果在忙碌状态，则持续等待
			if(SQLITE_BUSY == rc || SQLITE_LOCKED == rc) 
			{
				retry = true;
				SLEEP(20);

				if(m_nBusyRetryTimeout && (numberOfRetries++ > m_nBusyRetryTimeout)) 
				{
					CCLOGWARN("CCDatabase:_executeUpdate: Database busy");
					sqlite3_finalize(pStmt);
					setInUse(false);
					return false;
				}
			} 
			else if(SQLITE_OK != rc) 
			{
				CCLOGERROR("CCDatabase:_executeUpdate: DB Error: %d \"%s\"", LastErrorCode(), LastErrorMessage().c_str());

				sqlite3_finalize(pStmt);

				// 修改使用状态
				setInUse(false);
				return false;
			}
		} while(retry);
	}

	// 调用 sqlite3_step() 执行虚拟机。
	// 因为调用的sql未必是select，我们不保证一定有数据返回。
    numberOfRetries = 0;
	do 
	{
		rc = sqlite3_step(pStmt);
		retry = false;

		if(SQLITE_BUSY == rc || SQLITE_LOCKED == rc) 
		{
			// 如果DB被锁，那么每20毫秒检查再尝试一次
			retry = true;
			if(SQLITE_LOCKED == rc) 
			{
				rc = sqlite3_reset(pStmt);
				if(rc != SQLITE_LOCKED) 
				{
					CCLOGERROR("CCDatabase:_executeUpdate: Unexpected result from sqlite3_reset (%d) eu", rc);
				}
			}
			SLEEP(20);

			if(m_nBusyRetryTimeout && (numberOfRetries++ > m_nBusyRetryTimeout)) 
			{
				CCLOGWARN("CCDatabase:_executeUpdate: Database busy 2");
				retry = false;
			}
		} 
		else if(SQLITE_DONE == rc || SQLITE_ROW == rc) 
		{
			// 一切正常，返回
		} 
		else if(SQLITE_ERROR == rc) 
		{
			CCLOGERROR("Error calling sqlite3_step (%d: %s) SQLITE_ERROR", rc, LastErrorMessage().c_str());
		} 
		else if(SQLITE_MISUSE == rc) 
		{
			CCLOGERROR("Error calling sqlite3_step (%d: %s) SQLITE_MISUSE", rc, LastErrorMessage().c_str());
		} 
		else {
			CCLOGERROR("Unknown error calling sqlite3_step (%d: %s) eu", rc, LastErrorMessage().c_str());
		}
	} while(retry);

	// 缓冲statement
    if(m_bIsShouldCacheStatements && !cachedStmt) 
	{
		cachedStmt = new FKCW_Database_Statement();
		cachedStmt->SetStatement(pStmt);
		cachedStmt->setQuery(p_szSql);
		cachedStmt->m_nUseCount = 1;
		__SetCachedStatement(p_szSql, cachedStmt);
	}

    // 如果statement已经被缓存，注意不要释放它
    if(cachedStmt) 
	{
		rc = sqlite3_reset(pStmt);
	} 
	else 
	{
		// 释放虚拟机，它将释放全部内存以及sqlite3_prepare()调用的资源
		rc = sqlite3_finalize(pStmt);
	}

    // 修改占用状态
    setInUse(false);

    return rc == SQLITE_OK;
}
//--------------------------------------------------------------------
// 执行一个sql查询
// 返回值：如果为空，则查询失败。
FKCW_Database_ResultSet* FKCW_Database_Database::__ExecuteQuery( const char* p_szSql )
{
	// 首先检查数据库
	if ( !IsDatabaseOpened() ) 
	{
		return NULL;
	}

	// 是否正在使用中
	if ( m_bIsInUse ) 
	{
		__WarnInUse();
		return NULL;
	}

	// 设置现在在使用状态
	setInUse(true);

	FKCW_Database_ResultSet* rs = NULL;
	int rc = 0;
	sqlite3_stmt* pStmt = NULL;
	FKCW_Database_Statement* cachedStmt = NULL;

	// 获取被缓存的sqlStatement
	cachedStmt = __GetCachedStatement(p_szSql);
	pStmt = cachedStmt ? cachedStmt->getStatement() : NULL;

	// 如果该sqlStatement确实被缓存着
	if(cachedStmt)
		cachedStmt->Reset();

	int numberOfRetries = 0;
	bool retry = false;

	// 编译statement
	if(!pStmt) 
	{
		do 
		{
			// 预准备statement
			retry = false;
			rc = sqlite3_prepare_v2( m_pDB, p_szSql, -1, &pStmt, 0);

			// 如果DB在忙，则等待
			if(SQLITE_BUSY == rc || SQLITE_LOCKED == rc) 
			{
				retry = true;
				SLEEP(20);

				if(m_nBusyRetryTimeout && (numberOfRetries++ > m_nBusyRetryTimeout)) 
				{
					CCLOGWARN("CCDatabase:_executeQuery: Database busy");
					sqlite3_finalize(pStmt);
					setInUse(false);
					return NULL;
				}
			} 
			else if(SQLITE_OK != rc) 
			{
				CCLOGERROR("CCDatabase:_executeQuery: DB Error: %d \"%s\"", LastErrorCode(), LastErrorMessage().c_str());

				// 释放statement
				sqlite3_finalize(pStmt);

				// 修改use状态
				setInUse(false);
				return NULL;
			}
		} while(retry);
	}

	// 创建 Statement
	if (!cachedStmt) {
		cachedStmt = new FKCW_Database_Statement();
		cachedStmt->SetStatement(pStmt);
		cachedStmt->setQuery(p_szSql);
		cachedStmt->m_nUseCount = 1;
		__SetCachedStatement(p_szSql, cachedStmt);
	} else {
		cachedStmt->m_nUseCount++;
	}

	rs = FKCW_Database_ResultSet::Create(this, cachedStmt);

	// 修改使用状态
	setInUse(false);

	return rs;
}
//--------------------------------------------------------------------
// 执行一个查询SQL
FKCW_Database_ResultSet* FKCW_Database_Database::ExecuteQuery( string p_strSql, ... )
{
	va_list args;
	va_start(args, p_strSql);
	char buf[512];
	vsprintf(buf, p_strSql.c_str(), args);
	va_end(args);

	// 实际执行这个sql查询
	return __ExecuteQuery(buf);
}
//--------------------------------------------------------------------
// 当结果集需要关闭时，由FKCW_Database_ResultSet调用
void FKCW_Database_Database::__PostResultSetClosed( FKCW_Database_ResultSet* p_RS )
{
	StatementMap::iterator iter = m_CachedStatements.find(p_RS->m_strSQL);
	if(iter != m_CachedStatements.end()) 
	{
		// 删除引用计数，如果引用计数为0，则从缓冲statement表中删除
		iter->second->m_nUseCount--;
		if(iter->second->m_nUseCount <= 0 && !m_bIsShouldCacheStatements) 
		{
			iter->second->release();
			m_CachedStatements.erase(iter);
		}
	}
}
//--------------------------------------------------------------------
// 输出一个警告，当前DB正在被使用中
void FKCW_Database_Database::__WarnInUse()
{
	CCLOGWARN("The CCDatabase %p is currently in use.", this);
}
//--------------------------------------------------------------------
// 获取最后一个错误
// 返回值：若没有错误，则会返回空字符串
string FKCW_Database_Database::LastErrorMessage()
{
	if(m_pDB)
		return sqlite3_errmsg(m_pDB);
	else
		return "";
}
//--------------------------------------------------------------------
// 获取最后一个错误
// 返回值：若没有错误，则会返回 SQLITE_OK
int FKCW_Database_Database::LastErrorCode()
{
	if(m_pDB)
		return sqlite3_errcode(m_pDB);
	else
		return SQLITE_OK;
}
//--------------------------------------------------------------------
// 是否上一个操作有错误
// 返回值：true则表示上一个操作有错误
bool FKCW_Database_Database::IsHadError()
{
	int lastErrCode = LastErrorCode();
	return (lastErrCode > SQLITE_OK && lastErrCode < SQLITE_ROW);
}
//--------------------------------------------------------------------
// 获取最后插入的数据行ID
int64_t FKCW_Database_Database::LastInsertRowID()
{
	if(m_bIsInUse) 
	{
		__WarnInUse();
		return false;
	}

	setInUse(true);
	sqlite_int64 ret = sqlite3_last_insert_rowid( m_pDB );
	setInUse(false);

	return (int64_t)ret;
}
//--------------------------------------------------------------------
// 获取缓存的statement
FKCW_Database_Statement* FKCW_Database_Database::__GetCachedStatement( const char* p_szSql )
{
	StatementMap::iterator iter = m_CachedStatements.find(p_szSql);
	if(iter != m_CachedStatements.end()) 
	{
		return iter->second;
	} 
	else 
	{
		return NULL;
	}
}
//--------------------------------------------------------------------
// 缓存statement
void FKCW_Database_Database::__SetCachedStatement( const char* p_szSql, FKCW_Database_Statement* p_pSM )
{
	// 如果有老的，则释放
	StatementMap::iterator iter = m_CachedStatements.find(p_szSql);
	if(iter != m_CachedStatements.end()) 
	{
		iter->second->release();
		m_CachedStatements.erase(iter);
	}

	m_CachedStatements[p_szSql] = p_pSM;
}
//--------------------------------------------------------------------
// 是否缓存了statements
bool FKCW_Database_Database::IsCachedStatements()
{
	return m_bIsShouldCacheStatements;
}
//--------------------------------------------------------------------
// 设置是否缓存statements
void FKCW_Database_Database::SetCachedStatements( bool p_bValue )
{
	m_bIsShouldCacheStatements = p_bValue;

	if(!m_bIsShouldCacheStatements) 
	{
		ClearCachedStatements();
	}
}
//--------------------------------------------------------------------
// 获取上一次操作影响的数据行数
int FKCW_Database_Database::Changes()
{
	if (m_bIsInUse) 
	{
		__WarnInUse();
		return 0;
	}

	setInUse(true);
	int ret = sqlite3_changes( m_pDB );
	setInUse(false);

	return ret;
}
//--------------------------------------------------------------------
// 回滚上一个事务
bool FKCW_Database_Database::Rollback()
{
	bool b = ExecuteUpdate("ROLLBACK TRANSACTION;");
	if (b) 
	{
		m_bIsInTransaction = false;
	}
	return b;
}
//--------------------------------------------------------------------
// 提交一个事物
bool FKCW_Database_Database::Commit()
{
	bool b = ExecuteUpdate("COMMIT TRANSACTION;");
	if (b) 
	{
		m_bIsInTransaction = false;
	}
	return b;
}
//--------------------------------------------------------------------
// 开始一个延期事务
// 注：延期事务意思是所有修改都保存在一个缓存内，当全部修改都完成之后，再一次性将缓存同步到数据库中
bool FKCW_Database_Database::BeginDeferredTransaction()
{
	bool b = ExecuteUpdate("BEGIN DEFERRED TRANSACTION;");
	if (b) 
	{
		m_bIsInTransaction = true;
	}
	return b;
}
//--------------------------------------------------------------------
// 开始一个事务
bool FKCW_Database_Database::BeginTransaction()
{
	bool b = ExecuteUpdate("BEGIN EXCLUSIVE TRANSACTION;");
	if (b) 
	{
		m_bIsInTransaction = true;
	}
	return b;
}
//--------------------------------------------------------------------
// 检查一个表是否存在
bool FKCW_Database_Database::IsTableExists( string p_strTableName )
{
	bool returnBool;

	// 在sqlite_master表中去查该表是否存在
	FKCW_Base_Utils::ToLowerCase(p_strTableName);
	FKCW_Database_ResultSet* rs = ExecuteQuery("select [sql] from sqlite_master where [type] = 'table' and lower(name) = '%s'", p_strTableName.c_str());

	// 检查是否有数据
	returnBool = rs->Next();

	rs->Close();

	return returnBool;
}
//--------------------------------------------------------------------
// 检查一个SQLStatement
// 返回值：如果有效则返回空字符串
string FKCW_Database_Database::ValidateSQL( string p_strSql, ... )
{
	va_list args;
	va_start(args, p_strSql);
	char buf[512];
	vsprintf(buf, p_strSql.c_str(), args);
	va_end(args);

	sqlite3_stmt* pStmt = NULL;
	bool keepTrying = true;
	int numberOfRetries = 0;
	string ret;

	// 设置使用标示
	setInUse(true);

	// 持续尝试
	while(keepTrying) 
	{
		keepTrying = false;
		int rc = sqlite3_prepare_v2( m_pDB, buf, -1, &pStmt, 0);
		if(rc == SQLITE_BUSY || rc == SQLITE_LOCKED) 
		{
			keepTrying = true;
			SLEEP(20);

			if(m_nBusyRetryTimeout && (numberOfRetries++ > m_nBusyRetryTimeout)) 
			{
				CCLOGWARN("CCDatabase::validateSQL: Database busy");
			}
		} 
		else if(rc != SQLITE_OK) 
		{
			ret = LastErrorMessage();
		}
	}

	// 修改使用标示
	setInUse(false);

	// 释放statement
	sqlite3_finalize(pStmt);

	return ret;
}
//--------------------------------------------------------------------
// 获取DB框架
FKCW_Database_ResultSet* FKCW_Database_Database::GetDBSchema()
{
	FKCW_Database_ResultSet* rs = ExecuteQuery("SELECT type, name, tbl_name, rootpage, sql FROM (SELECT * FROM sqlite_master UNION ALL SELECT * FROM sqlite_temp_master) WHERE type != 'meta' AND name NOT LIKE 'sqlite_%' ORDER BY tbl_name, type DESC, name");
	return rs;
}
//--------------------------------------------------------------------
// 获取表框架
FKCW_Database_ResultSet* FKCW_Database_Database::GetTableSchema( string p_strTableName )
{
	FKCW_Database_ResultSet* rs = ExecuteQuery("PRAGMA table_info(%s)", p_strTableName.c_str());
	return rs;
}
//--------------------------------------------------------------------
// 检查一个表字段是否存在
bool FKCW_Database_Database::IsColumnExists( string p_strTableName, string p_strColumnName )
{
	bool existent = false;

	// 转小写
	FKCW_Base_Utils::ToLowerCase(p_strTableName);
	FKCW_Base_Utils::ToLowerCase(p_strColumnName);

	// 获取表框架
	FKCW_Database_ResultSet* rs = GetTableSchema( p_strTableName );

	// 逐行检查是否在表框架中
	while(rs->Next()) 
	{
		string column = rs->StringFromColumnName("name");
		FKCW_Base_Utils::ToLowerCase(column);
		if(column == p_strColumnName) 
		{
			existent = true;
			break;
		}
	}

	return existent;
}
//--------------------------------------------------------------------
// 获取数据库版本
int FKCW_Database_Database::GetVersion()
{
	return IntFromQuery("PRAGMA user_version;");
}
//--------------------------------------------------------------------
// 设置数据库版本
// 注：默认创建时版本为1
void FKCW_Database_Database::SetVersion( int p_nVersion )
{
	ExecuteUpdate("PRAGMA user_version = %d;", p_nVersion);
}
//--------------------------------------------------------------------
// 在事务中执行一个sql文件
// 参数：p_strPath SQL文件 p_DeFunc 解密函数，允许sql文件被加密
bool FKCW_Database_Database::ExecuteSQL( string p_strPath, DECRYPT_FUNC p_DeFunc )
{
	unsigned long len;
	unsigned char* raw = FKCW_Base_ResourceLoader::LoadRow(p_strPath, &len, p_DeFunc);
	bool ret = ExecuteSQL(raw, len);
	free(raw);
	return ret;
}
//--------------------------------------------------------------------
// 在事务中执行一个sql文件
bool FKCW_Database_Database::ExecuteSQL(const void* p_szData, size_t p_unLength )
{
	// 开始事务
	if(!BeginTransaction()) 
	{
		CCLOGERROR("CCDatabase::executeSQL: failed to start transaction");
		return false;
	}

	// 逐行执行
	char* tmp = (char*)p_szData;
	int start = 0;
	for(int i = 0; i < p_unLength; i++) 
	{
		if(isspace(tmp[i]) && i == start) 
		{
			start++;
		} 
		else if(tmp[i] == ';') 
		{
			tmp[i] = 0;
			// 执行
			bool success = ExecuteUpdate(tmp + start);
			tmp[i] = ';';

			// 如果执行成功，则执行下一个，若失败，则退出
			if(success) 
			{
				start = i + 1;
			} else 
			{
				// 失败，则回滚
				if(!Rollback()) 
				{
					CCLOGERROR("CCDatabase::executeSQL: failed to rollback transaction");
				}

				return false;
			}
		}
	}

	// 提交事务
	if(!Commit()) {
		CCLOGERROR("CCDatabase::executeSQL: failed to commit transaction");
		return false;
	}

	return true;
}
//--------------------------------------------------------------------
// 通过一个查询，得到一个int值
int FKCW_Database_Database::IntFromQuery( string p_strSql, ... )
{
	va_list args;
	va_start(args, p_strSql);
	char buf[512];
	vsprintf(buf, p_strSql.c_str(), args);
	va_end(args);

	FKCW_Database_ResultSet* rs = __ExecuteQuery(buf);
	if(rs->Next())
		return rs->IntFromColumnIndex(0);
	else
		return 0;
}
//--------------------------------------------------------------------
// 通过一个查询，得到一个long值
long FKCW_Database_Database::LongFromQuery( string p_strSql, ... )
{
	va_list args;
	va_start(args, p_strSql);
	char buf[512];
	vsprintf(buf, p_strSql.c_str(), args);
	va_end(args);

	FKCW_Database_ResultSet* rs = __ExecuteQuery(buf);
	if(rs->Next())
		return rs->LongFromColumnIndex(0);
	else
		return 0;
}
//--------------------------------------------------------------------
// 通过一个查询，得到一个int64_t值
int64_t FKCW_Database_Database::Int64FromQuery( string p_strSql, ... )
{
	va_list args;
	va_start(args, p_strSql);
	char buf[512];
	vsprintf(buf, p_strSql.c_str(), args);
	va_end(args);

	FKCW_Database_ResultSet* rs = __ExecuteQuery(buf);
	if(rs->Next())
		return rs->Int64FromColumnIndex(0);
	else
		return 0;
}
//--------------------------------------------------------------------
// 通过一个查询，得到一个bool值
bool FKCW_Database_Database::BoolFromQuery( string p_strSql, ... )
{
	va_list args;
	va_start(args, p_strSql);
	char buf[512];
	vsprintf(buf, p_strSql.c_str(), args);
	va_end(args);

	FKCW_Database_ResultSet* rs = __ExecuteQuery(buf);
	if(rs->Next())
		return rs->BoolFromColumnIndex(0);
	else
		return false;
}
//--------------------------------------------------------------------
// 通过一个查询，得到一个double值
double FKCW_Database_Database::DoubleFromQuery( string p_strSql, ... )
{
	va_list args;
	va_start(args, p_strSql);
	char buf[512];
	vsprintf(buf, p_strSql.c_str(), args);
	va_end(args);

	FKCW_Database_ResultSet* rs = __ExecuteQuery(buf);
	if(rs->Next())
		return rs->DoubleFromColumnIndex(0);
	else
		return 0;
}
//--------------------------------------------------------------------
// 通过一个查询，得到一个string值
string FKCW_Database_Database::StringFromQuery( string p_strSql, ... )
{
	va_list args;
	va_start(args, p_strSql);
	char buf[512];
	vsprintf(buf, p_strSql.c_str(), args);
	va_end(args);

	FKCW_Database_ResultSet* rs = __ExecuteQuery(buf);
	if(rs->Next())
		return rs->StringFromColumnIndex(0);
	else
		return "";
}
//--------------------------------------------------------------------
// 通过一个查询，得到一个blob值
// 注: 返回值是从源数据中拷贝的，所以调用者需要手动删除
const void* FKCW_Database_Database::DataFromQuery( string p_strSql, size_t* p_pOutLen, ... )
{
	va_list args;
	va_start(args, p_strSql);
	char buf[512];
	vsprintf(buf, p_strSql.c_str(), args);
	va_end(args);

	FKCW_Database_ResultSet* rs = __ExecuteQuery(buf);
	if(rs->Next())
		return rs->DataFromColumnIndex(0, p_pOutLen);
	else
		return NULL;
}
//--------------------------------------------------------------------
// 通过一个查询，得到一个blob值
// 注: 返回值是就是原数据，所以调用者无需手动删除
const void* FKCW_Database_Database::DataNoCopyFromQuery( string p_strSql, size_t* p_pOutLen, ... )
{
	va_list args;
	va_start(args, p_strSql);
	char buf[512];
	vsprintf(buf, p_strSql.c_str(), args);
	va_end(args);

	FKCW_Database_ResultSet* rs = __ExecuteQuery(buf);
	if(rs->Next())
		return rs->DataNoCopyFromColumnIndex(0, p_pOutLen);
	else
		return NULL;
}
//--------------------------------------------------------------------