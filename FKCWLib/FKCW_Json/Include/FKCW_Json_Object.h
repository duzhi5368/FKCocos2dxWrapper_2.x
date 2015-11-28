//*************************************************************************
//	创建日期:	2014-10-30
//	文件名称:	FKCW_Json_Object.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_Json_Macros.h"
//-------------------------------------------------------------------------
class FKCW_Json_Array;
//-------------------------------------------------------------------------
class FKCW_Json_Object : public CCObject
{
public:
	// Json值类型
	enum ENUM_JsonValueType
	{
		eJsonValueType_Nil		= 0,
		eJsonValueType_Boolean,
		eJsonValueType_String,
		eJsonValueType_Object,
		eJsonValueType_Array,
	};

	// JsonValue值结构
	union Value
	{
		bool b;
		const char* s;
		FKCW_Json_Object* jo;
		FKCW_Json_Array* ja;
	};

	// JsonKey值结构
	struct SKeyValue
	{
		const char* k;
		ENUM_JsonValueType t;
		Value v;
	};
private:
	typedef map<string, SKeyValue>	PairMap;
	PairMap			m_mapPairs;				// Json键值表
	typedef vector<SKeyValue>		KeyValueList;
	KeyValueList	m_vecKeyValues;			// Json键数组
protected:
	FKCW_Json_Object();
	// 转换一个CCObject为FKCW_Json_Object对象。
	// 若可以转换，则这个对象会被填充到对象中。若CCObject是一个数组，则会作为一个键为“array”的数组填充到json中
	static FKCW_Json_Object*		SafeCast( CCObject* p_pObj );
	// 释放指定键值
	static void						ReleaseKeyValue( const string& p_strKey, SKeyValue& p_KeyValue );
public:
	virtual ~FKCW_Json_Object();
	// 创建一个空json对象
	static FKCW_Json_Object* Create();
	// 解析一个json格式的内存字符串
	// 若这个json内存是一个数组，则这个数组将会作为一个键为"array"的数组填充
	static FKCW_Json_Object* Create( const char* p_szJson, size_t p_unLength );
	// 解析一个json格式的文件
	static FKCW_Json_Object* Create( const string& p_strPath );

	// 获取Json对象中的元素个数
	int		GetLength();
	// 获取指定编号的Key
	const char*		KeyAt( int p_nIndex );
	// 输出json对象全部键值表
	// 参数：p_pAOS 输出流
	void	Output( FKCW_IO_AssetOutputStream* p_pAOS, int p_nLevel = 1 );
	// 将Json序列化成string
	string	ToString();

	// 为一个key添加一个空value
	void	AddNull( const char* p_szKey );
	// 为一个key添加一个bool值
	void	AddBool( const char* p_szKey, bool p_bValue );
	// 为一个key添加一个int值
	void	AddInt( const char* p_szKey, int p_nValue );
	// 为一个key添加一个long值
	void	AddLong( const char* p_szKey, long p_lValue );
	// 为一个key添加一个float值
	void	AddFloat( const char* p_szKey, float p_fValue );
	// 为一个key添加一个double值
	void	AddDouble( const char* p_szKey, double p_dValue );
	// 为一个key添加一个string值
	void	AddString( const char* p_szKey, const char* p_szValue );
	// 为一个key添加一个object值
	void	AddObject( const char* p_szKey, FKCW_Json_Object* p_pJO );
	// 为一个key添加一个array值
	void	AddArray( const char* p_szKey, FKCW_Json_Array* p_pJA );
	
	// 获取一个Key对应的bool值
	bool	GetBool( const char* p_szKey, bool p_bDefault = false );
	bool	GetBool( int p_nIndex, bool p_bDefault = false );
	// 获取一个Key对应的int值
	int		GetInt( const char* p_szKey, int p_nDefault = 0 );
	int		GetInt( int p_nIndex, int p_nDefault = 0 );
	// 获取一个Key对应的long值
	long	GetLong( const char* p_szKey, long p_lDefault = 0 );
	long	GetLong( int p_nIndex, long p_lDefault = 0 );
	// 获取一个Key对应的float值
	float	GetFloat( const char* p_szKey, float p_fDefault = 0.0f );
	float	GetFloat( int p_nIndex, float p_fDefault = 0.0f );
	// 获取一个Key对应的double值
	double	GetDouble( const char* p_szKey, double p_dDefault = 0 );
	double	GetDouble( int p_nIndex, double p_dDefault = 0 );
	// 获取一个Key对应的JsonObject值
	FKCW_Json_Object*	GetObject( const char* p_szKey );
	FKCW_Json_Object*	GetObject( int p_nIndex );
	// 获取一个Key对应的JsonArray值
	FKCW_Json_Array*	GetArray( const char* p_szKey );
	FKCW_Json_Array*	GetArray( int p_nIndex );
	// 获取一个Key对应的string值
	string	GetString( const char* p_szKey, const char* p_szDefault = NULL );
	string	GetString( int p_nIndex, const char* p_szDefault = NULL );
};