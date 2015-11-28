//*************************************************************************
//	创建日期:	2014-10-30
//	文件名称:	FKCW_Json_Array.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_Json_Object.h"
#include "FKCW_Json_Macros.h"
//-------------------------------------------------------------------------
class FKCW_Json_Array : public CCObject
{
private:
	typedef vector<FKCW_Json_Object::SKeyValue>		KeyValueList;
	KeyValueList			m_ArrayKeyValue;
protected:
	FKCW_Json_Array();
	// 转换CCObject为FKCW_Json_Array。
	// 注：若CCObject是FKCW_Json_Object类型，则该值将作为返回数组的第一个元素。
	static FKCW_Json_Array* SafeCast( CCObject* p_pObj );
	//  移除一个键值
	static void	ReleaseKeyValue( FKCW_Json_Object::SKeyValue& p_KV );
public:
	virtual ~FKCW_Json_Array();
	// 创建一个空的json数组对象
	static FKCW_Json_Array* Create();
	// 解析一段json格式内存
	// 注：若CCObject是FKCW_Json_Object类型，则该值将作为返回数组的第一个元素。
	static FKCW_Json_Array* Create( const char* p_szJson, size_t p_unLength );
	// 解析一个Json格式文件
	static FKCW_Json_Array*	Create( const string& p_strPath );

	// 返回json数组的元素个数
	int			GetLength();
	// 输出json对象全部数组值
	// 参数：p_pAOS 输出流
	void		Output( FKCW_IO_AssetOutputStream* p_pAOS, int p_nLevel = 1 );
	// 将Json序列化成string
	string		ToString();

	// 在数组中添加一个空值
	void		AddNull();
	// 在数组中添加一个bool值
	void		AddBool( bool p_bValue );
	// 在数组中添加一个int值
	void		AddInt( int p_nValue );
	// 在数组中添加一个long值
	void		AddLong( long p_lValue );
	// 在数组中添加一个float值
	void		AddFloat( float p_fValue );
	// 在数组中添加一个double值
	void		AddDouble( double p_dValue );
	// 在数组中添加一个string值
	void		AddString( const char* p_szValue );
	// 在数组中添加一个object值
	void		AddObject( FKCW_Json_Object* p_pJO );
	// 在数组中添加一个array值
	void		AddArray( FKCW_Json_Array* p_pJA );

	// 获取指定编号对应的bool值
	bool	GetBool( int p_nIndex, bool p_bDefault = false );
	// 获取指定编号对应的int值
	int		GetInt( int p_nIndex, int p_nDefault = 0 );
	// 获取指定编号对应的long值
	long	GetLong( int p_nIndex, long p_lDefault = 0 );
	// 获取指定编号对应的float值
	float	GetFloat( int p_nIndex, bool p_fDefault = 0.0f );
	// 获取指定编号对应的double值
	double	GetDouble( int p_nIndex, bool p_dDefault = 0 );
	// 获取指定编号对应的JsonObject值
	FKCW_Json_Object*	GetObject( int p_nIndex );
	// 获取指定编号对应的JsonArray值
	FKCW_Json_Array*	GetArray( int p_nIndex );
	// 获取指定编号对应的string值
	string	GetString( int p_nIndex, const char* p_szDefault = NULL );
};