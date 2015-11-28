//--------------------------------------------------------------------
#include "../Include/FKCW_Database_ResultSet.h"
#include "../Include/FKCW_Database_Statement.h"
#include "../Include/FKCW_Database_Database.h"
#include "sqlite3/include/sqlite3.h"
//--------------------------------------------------------------------
FKCW_Database_ResultSet::FKCW_Database_ResultSet( FKCW_Database_Database* p_pDB, FKCW_Database_Statement* p_pSM )
	: m_pDatabase( p_pDB )
	, m_pStatement( p_pSM )
	, m_strSQL( p_pSM->getQuery() )
{
	// 创建列名
	int nColumnCount = sqlite3_column_count(p_pSM->getStatement());
	for(int i = 0; i < nColumnCount; i++) 
	{
		string szName = sqlite3_column_name(p_pSM->getStatement(), i);
		FKCW_Base_Utils::ToLowerCase( szName );
		m_ColumnNames.push_back( szName );
	}
}
//--------------------------------------------------------------------
FKCW_Database_ResultSet* FKCW_Database_ResultSet::Create( FKCW_Database_Database* p_pDB, FKCW_Database_Statement* p_pSM )
{
	FKCW_Database_ResultSet* rs = new FKCW_Database_ResultSet(p_pDB, p_pSM);
	return (FKCW_Database_ResultSet*)rs->autorelease();
}
//--------------------------------------------------------------------
// 关闭一个结果集
// 注：关闭后，该结果集不允许被使用。
// 注：析构函数会调用该函数
void FKCW_Database_ResultSet::Close()
{
	if( m_pStatement ) 
	{
		m_pStatement->Reset();
		m_pStatement = NULL;

		// 通知关闭
		if( m_pDatabase )
			m_pDatabase->__PostResultSetClosed(this);
	}
}
//--------------------------------------------------------------------
FKCW_Database_ResultSet::~FKCW_Database_ResultSet()
{
	Close();
	m_pDatabase = NULL;
}
//--------------------------------------------------------------------
// 移动当前指针到下一行
// 返回值：若为false则表示已没有数据。true表示成功。
bool FKCW_Database_ResultSet::Next()
{
	int rc = 0;
	if( m_pStatement ) 
	{
		bool retry;
		int numberOfRetries = 0;
		do 
		{
			retry = false;

			rc = sqlite3_step( m_pStatement->getStatement());

			if(SQLITE_BUSY == rc || SQLITE_LOCKED == rc) 
			{
				// 如果DB被锁，则等待10毫秒(该值可修改)左右，继续尝试
				retry = true;
				if(SQLITE_LOCKED == rc) 
				{
					rc = sqlite3_reset( m_pStatement->getStatement());
					if(rc != SQLITE_LOCKED) 
					{
						CCLOGERROR("CCResultSet::next: Unexpected result from sqlite3_reset (%d) rs", rc);
					}
				}
				SLEEP(20);

				if( m_pDatabase->getBusyRetryTimeout() && (numberOfRetries++ > m_pDatabase->getBusyRetryTimeout())) 
				{
					CCLOGWARN("CCResultSet::next: Database busy");
					break;
				}
			} 
			else if(SQLITE_DONE == rc || SQLITE_ROW == rc) 
			{
				// 一切正常
			} 
			else if(SQLITE_ERROR == rc) 
			{
				CCLOGERROR("Error calling sqlite3_step (%d: %s) rs", rc, m_pDatabase->LastErrorMessage().c_str());
				break;
			} 
			else if(SQLITE_MISUSE == rc) 
			{
				CCLOGERROR("Error calling sqlite3_step (%d: %s) rs", rc, m_pDatabase->LastErrorMessage().c_str());
				break;
			} 
			else 
			{
				CCLOGERROR("Unknown error calling sqlite3_step (%d: %s) rs", rc, m_pDatabase->LastErrorMessage().c_str());
				break;
			}
		} while(retry);
	}

	return rc == SQLITE_ROW;
}
//--------------------------------------------------------------------
// 是否还有一行数据
bool FKCW_Database_ResultSet::IsHasAnotherRow()
{
	return sqlite3_errcode(m_pDatabase->SqliteHandler()) == SQLITE_ROW;
}
//--------------------------------------------------------------------
// 获取结果集的列数
int FKCW_Database_ResultSet::ColumnCount()
{
	return sqlite3_column_count( m_pStatement->getStatement() );
}
//--------------------------------------------------------------------
// 判断指定列是否为空
bool FKCW_Database_ResultSet::IsColumnIndexIsNull( int p_nColumnIndex )
{
	return sqlite3_column_type( m_pStatement->getStatement(), p_nColumnIndex ) == SQLITE_NULL;
}
//--------------------------------------------------------------------
bool FKCW_Database_ResultSet::IsColumnIsNull( string p_strColumnName )
{
	return IsColumnIndexIsNull( ColumnIndexByName(p_strColumnName) );
}
//--------------------------------------------------------------------
// 根据列名获取指定列的索引号
int FKCW_Database_ResultSet::ColumnIndexByName( string p_strColumnName )
{
	int index = 0;
	for(StringList::iterator iter = m_ColumnNames.begin(); iter != m_ColumnNames.end(); iter++, index++) 
	{
		string& name = *iter;
		if(name == p_strColumnName) 
		{
			return index;
		}
	}

	CCLOGWARN("Can't find column index for name: %s", p_strColumnName.c_str());
	return -1;
}
//--------------------------------------------------------------------
// 获取指定索引号的列名
string FKCW_Database_ResultSet::ColumnNameByIndex( int p_nColumnIndex )
{
	if( p_nColumnIndex < 0 || (size_t)p_nColumnIndex >= m_ColumnNames.size())
		return NULL;

	return m_ColumnNames.at(p_nColumnIndex);
}
//--------------------------------------------------------------------
// 获取当前指针所在行 的 指定列内int数据
int FKCW_Database_ResultSet::IntFromColumnName( string p_strColumnName )
{
	return IntFromColumnIndex( ColumnIndexByName(p_strColumnName));
}
//--------------------------------------------------------------------
int FKCW_Database_ResultSet::IntFromColumnIndex( int p_nColumnIndex )
{
	return sqlite3_column_int( m_pStatement->getStatement(), p_nColumnIndex );
}
//--------------------------------------------------------------------
// 获取当前指针所在行 的 指定列内long数据
long FKCW_Database_ResultSet::LongFromColumnName( string p_strColumnName )
{
	return LongFromColumnIndex( ColumnIndexByName(p_strColumnName));
}
//--------------------------------------------------------------------
long FKCW_Database_ResultSet::LongFromColumnIndex( int p_nColumnIndex )
{
	return (long)sqlite3_column_int64( m_pStatement->getStatement(), p_nColumnIndex );
}
//--------------------------------------------------------------------
// 获取当前指针所在行 的 指定列内int64_t数据
int64_t FKCW_Database_ResultSet::Int64FromColumnName( string p_strColumnName )
{
	return Int64FromColumnIndex( ColumnIndexByName(p_strColumnName));
}
//--------------------------------------------------------------------
int64_t FKCW_Database_ResultSet::Int64FromColumnIndex( int p_nColumnIndex )
{
	return (int64_t)sqlite3_column_int64( m_pStatement->getStatement(), p_nColumnIndex );
}
//--------------------------------------------------------------------
// 获取当前指针所在行 的 指定列内bool数据
bool FKCW_Database_ResultSet::BoolFromColumnName( string p_strColumnName )
{
	return BoolFromColumnIndex( ColumnIndexByName(p_strColumnName) );
}
//--------------------------------------------------------------------
bool FKCW_Database_ResultSet::BoolFromColumnIndex( int p_nColumnIndex )
{
	return IntFromColumnIndex( p_nColumnIndex ) != 0;
}
//--------------------------------------------------------------------
// 获取当前指针所在行 的 指定列内double数据
double FKCW_Database_ResultSet::DoubleFromColumnName( string p_strColumnName )
{
	return DoubleFromColumnIndex( ColumnIndexByName(p_strColumnName) );
}
//--------------------------------------------------------------------
double FKCW_Database_ResultSet::DoubleFromColumnIndex( int p_nColumnIndex )
{
	return sqlite3_column_double( m_pStatement->getStatement(), p_nColumnIndex );
}
//--------------------------------------------------------------------
// 获取当前指针所在行 的 指定列内string数据
string FKCW_Database_ResultSet::StringFromColumnName( string p_strColumnName )
{
	return StringFromColumnIndex( ColumnIndexByName(p_strColumnName) );
}
//--------------------------------------------------------------------
string FKCW_Database_ResultSet::StringFromColumnIndex( int p_nColumnIndex )
{
	if (sqlite3_column_type( m_pStatement->getStatement(), p_nColumnIndex) == SQLITE_NULL || 
		(p_nColumnIndex < 0)) 
	{
		return NULL;
	}

	return (const char*)sqlite3_column_text( m_pStatement->getStatement(), p_nColumnIndex);
}
//--------------------------------------------------------------------
// 获取当前指针所在行 的 指定列内Blob数据
// 注: 返回值是从源数据中拷贝的，所以调用者需要手动删除
const void* FKCW_Database_ResultSet::DataFromColumnName( string p_strColumnName, size_t* p_pOutLen )
{
	return DataFromColumnIndex( ColumnIndexByName(p_strColumnName), p_pOutLen );
}
//--------------------------------------------------------------------
const void* FKCW_Database_ResultSet::DataFromColumnIndex( int p_nColumnIndex, size_t* p_pOutLen )
{
	// 检查类型
	if (sqlite3_column_type(m_pStatement->getStatement(), p_nColumnIndex) == SQLITE_NULL || 
		(p_nColumnIndex < 0)) 
	{
		*p_pOutLen = 0;
		return NULL;
	}

	// 数据拷贝
	int dataSize = sqlite3_column_bytes(m_pStatement->getStatement(), p_nColumnIndex);
	char* buf = (char*)malloc(dataSize * sizeof(char));
	memcpy(buf, sqlite3_column_blob(m_pStatement->getStatement(), p_nColumnIndex), dataSize);

	// 返回缓冲区
	*p_pOutLen = dataSize;
	return (const char*)buf;
}
//--------------------------------------------------------------------
// 获取当前指针所在行 的 指定列内Blob数据
// 注: 返回值是就是原数据，所以调用者无需手动删除
const void* FKCW_Database_ResultSet::DataNoCopyFromColumnName( string p_strColumnName, size_t* p_pOutLen )
{
	return DataNoCopyFromColumnIndex( ColumnIndexByName(p_strColumnName), p_pOutLen );
}
//--------------------------------------------------------------------
const void* FKCW_Database_ResultSet::DataNoCopyFromColumnIndex( int p_nColumnIndex, size_t* p_pOutLen )
{
	if (sqlite3_column_type(m_pStatement->getStatement(), p_nColumnIndex) == SQLITE_NULL ||
		(p_nColumnIndex < 0)) 
	{
		*p_pOutLen = 0;
		return NULL;
	}

	*p_pOutLen = sqlite3_column_bytes(m_pStatement->getStatement(), p_nColumnIndex);
	return (const char*)sqlite3_column_blob(m_pStatement->getStatement(), p_nColumnIndex);
}
//--------------------------------------------------------------------