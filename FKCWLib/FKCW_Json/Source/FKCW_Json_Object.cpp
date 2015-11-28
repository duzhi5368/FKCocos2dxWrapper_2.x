//-------------------------------------------------------------------------
#include "../Include/FKCW_Json_Array.h"
#include "../Include/FKCW_Json_Value.h"
#include "../Include/FKCW_Json_Parser.h"
#include "../../FKCW_IO/Include/FKCW_IO_MemoryOutputStream.h"
#include "../../FKCW_Base/Include/FKCW_Base_Utils.h"
//-------------------------------------------------------------------------
#pragma warning( disable : 4018 )
//-------------------------------------------------------------------------
USING_NS_CC;
//-------------------------------------------------------------------------
// 临时缓冲
static char s_szArrayBuf[4096];
//-------------------------------------------------------------------------
FKCW_Json_Object::FKCW_Json_Object()
{

}
//-------------------------------------------------------------------------
// 转换一个CCObject为FKCW_Json_Object对象。
// 若可以转换，则这个对象会被填充到对象中。若CCObject是一个数组，则会作为一个键为“array”的数组填充到json中
FKCW_Json_Object* FKCW_Json_Object::SafeCast( CCObject* p_pObj )
{
	FKCW_Json_Object* jo = dynamic_cast<FKCW_Json_Object*>(p_pObj);
	if(jo) 
	{
		return jo;
	} 
	else 
	{
		FKCW_Json_Array* ja = dynamic_cast<FKCW_Json_Array*>(p_pObj);
		if(ja) 
		{
			FKCW_Json_Object* jo = FKCW_Json_Object::Create();
			jo->AddArray("array", ja);
			return jo;
		} 
		else 
		{
			return NULL;
		}
	}
}
//-------------------------------------------------------------------------
// 释放指定键值
void FKCW_Json_Object::ReleaseKeyValue( const string& p_strKey, SKeyValue& p_KV )
{
	switch(p_KV.t) 
	{
	case FKCW_Json_Object::eJsonValueType_String:
		free((void*)p_KV.v.s);
		break;
	case FKCW_Json_Object::eJsonValueType_Object:
		CC_SAFE_RELEASE(p_KV.v.jo);
		break;
	case FKCW_Json_Object::eJsonValueType_Array:
		CC_SAFE_RELEASE(p_KV.v.ja);
		break;
	default:
		break;
	}
	if(p_KV.k)
		free((void*)p_KV.k);
}
//-------------------------------------------------------------------------
FKCW_Json_Object::~FKCW_Json_Object()
{
	for(PairMap::iterator iter = m_mapPairs.begin(); iter != m_mapPairs.end(); iter++) 
	{
		ReleaseKeyValue(iter->first, iter->second);
	}
}
//-------------------------------------------------------------------------
// 创建一个空json对象
FKCW_Json_Object* FKCW_Json_Object::Create()
{
	FKCW_Json_Object* jo = new FKCW_Json_Object();
	return ( FKCW_Json_Object* )jo->autorelease();
}
//-------------------------------------------------------------------------
// 解析一个json格式的内存字符串
// 若这个json内存是一个数组，则这个数组将会作为一个键为"array"的数组填充
FKCW_Json_Object* FKCW_Json_Object::Create( const char* p_szJson, size_t p_unLength )
{
	CCObject* obj = FKCW_Json_Parser::Load(p_szJson, p_unLength);
	return SafeCast(obj);
}
//-------------------------------------------------------------------------
// 解析一个json格式的文件
FKCW_Json_Object* FKCW_Json_Object::Create( const string& p_strPath )
{
	CCObject* obj = FKCW_Json_Parser::Load(p_strPath);
	return SafeCast(obj);
}
//-------------------------------------------------------------------------
// 为一个key添加一个空value
void FKCW_Json_Object::AddNull( const char* p_szKey )
{
	if(!p_szKey)
		return;

	const char* k = FKCW_Base_Utils::Copy(p_szKey);
	SKeyValue kv = {
		p_szKey,
		FKCW_Json_Object::eJsonValueType_Nil,
		{ false }
	};
	m_mapPairs[k] = kv;
	m_vecKeyValues.push_back(kv);
}
//-------------------------------------------------------------------------
// 为一个key添加一个bool值
void FKCW_Json_Object::AddBool( const char* p_szKey, bool p_bValue )
{
	if(!p_szKey)
		return;

	Value v;
	v.b = p_bValue;
	const char* k = FKCW_Base_Utils::Copy(p_szKey);
	SKeyValue kv = {
		k,
		FKCW_Json_Object::eJsonValueType_Boolean,
		v
	};
	m_mapPairs[k] = kv;
	m_vecKeyValues.push_back(kv);
}
//-------------------------------------------------------------------------
// 为一个key添加一个int值
void FKCW_Json_Object::AddInt( const char* p_szKey, int p_nValue )
{
	if(!p_szKey)
		return;

	char buf[32];
	sprintf(buf, "%d", p_nValue);
	Value v;
	v.s = FKCW_Base_Utils::Copy(buf);
	const char* k = FKCW_Base_Utils::Copy(p_szKey);
	SKeyValue kv = {
		k,
		FKCW_Json_Object::eJsonValueType_String,
		v
	};
	m_mapPairs[k] = kv;
	m_vecKeyValues.push_back(kv);
}
//-------------------------------------------------------------------------
// 为一个key添加一个long值
void FKCW_Json_Object::AddLong( const char* p_szKey, long p_lValue )
{
	if(!p_szKey)
		return;

	char buf[32];
	sprintf(buf, "%ld", p_lValue);
	Value v;
	v.s = FKCW_Base_Utils::Copy(buf);
	const char* k = FKCW_Base_Utils::Copy(p_szKey);
	SKeyValue kv = {
		k,
		FKCW_Json_Object::eJsonValueType_String,
		v
	};
	m_mapPairs[k] = kv;
	m_vecKeyValues.push_back(kv);
}
//-------------------------------------------------------------------------
// 为一个key添加一个float值
void FKCW_Json_Object::AddFloat( const char* p_szKey, float p_fValue )
{
	if(!p_szKey)
		return;

	char buf[32];
	sprintf(buf, "%f", p_fValue);
	Value v;
	v.s = FKCW_Base_Utils::Copy(buf);
	const char* k = FKCW_Base_Utils::Copy(p_szKey);
	SKeyValue kv = {
		k,
		FKCW_Json_Object::eJsonValueType_String,
		v
	};
	m_mapPairs[k] = kv;
	m_vecKeyValues.push_back(kv);
}
//-------------------------------------------------------------------------
// 为一个key添加一个double值
void FKCW_Json_Object::AddDouble( const char* p_szKey, double p_dValue )
{
	if(!p_szKey)
		return;

	char buf[32];
	sprintf(buf, "%lf", p_dValue);
	Value v;
	v.s = FKCW_Base_Utils::Copy(buf);
	const char* k = FKCW_Base_Utils::Copy(p_szKey);
	SKeyValue kv = {
		k,
		FKCW_Json_Object::eJsonValueType_String,
		v
	};
	m_mapPairs[k] = kv;
	m_vecKeyValues.push_back(kv);
}
//-------------------------------------------------------------------------
// 为一个key添加一个string值
void FKCW_Json_Object::AddString( const char* p_szKey, const char* p_szValue )
{
	if(!p_szKey)
		return;

	Value v;
	v.s = FKCW_Base_Utils::Copy(p_szValue);
	const char* k = FKCW_Base_Utils::Copy(p_szKey);
	SKeyValue kv = {
		k,
		FKCW_Json_Object::eJsonValueType_String,
		v
	};
	m_mapPairs[k] = kv;
	m_vecKeyValues.push_back(kv);
}
//-------------------------------------------------------------------------
// 为一个key添加一个object值
void FKCW_Json_Object::AddObject( const char* p_szKey, FKCW_Json_Object* p_pJO )
{
	if(!p_szKey)
		return;

	Value v;
	v.jo = p_pJO;
	const char* k = FKCW_Base_Utils::Copy(p_szKey);
	SKeyValue kv = {
		k,
		FKCW_Json_Object::eJsonValueType_Object,
		v
	};
	CC_SAFE_RETAIN(p_pJO);
	m_mapPairs[k] = kv;
	m_vecKeyValues.push_back(kv);
}
//-------------------------------------------------------------------------
// 为一个key添加一个array值
void FKCW_Json_Object::AddArray( const char* p_szKey, FKCW_Json_Array* p_pJA )
{
	if(!p_szKey)
		return;

	Value v;
	v.ja = p_pJA;
	const char* k = FKCW_Base_Utils::Copy(p_szKey);
	SKeyValue kv = {
		k,
		FKCW_Json_Object::eJsonValueType_Array,
		v
	};
	CC_SAFE_RETAIN(p_pJA);
	m_mapPairs[k] = kv;
	m_vecKeyValues.push_back(kv);
}
//-------------------------------------------------------------------------
// 获取一个Key对应的bool值
bool FKCW_Json_Object::GetBool( const char* p_szKey, bool p_bDefault )
{
	PairMap::iterator iter = m_mapPairs.find(p_szKey);
	if(iter != m_mapPairs.end()) {
		return FKCW_Json_Value::CastToBool(iter->second);
	} else {
		return p_bDefault;
	}
}
//-------------------------------------------------------------------------
bool FKCW_Json_Object::GetBool( int p_nIndex, bool p_bDefault )
{
	if(p_nIndex < 0 || p_nIndex >= m_vecKeyValues.size()) {
		return p_bDefault;
	}

	return FKCW_Json_Value::CastToBool(m_vecKeyValues.at(p_nIndex));
}
//-------------------------------------------------------------------------
// 获取一个Key对应的int值
int FKCW_Json_Object::GetInt( const char* p_szKey, int p_nDefault )
{
	PairMap::iterator iter = m_mapPairs.find(p_szKey);
	if(iter != m_mapPairs.end()) {
		return FKCW_Json_Value::CastToInt(iter->second);
	} else {
		return p_nDefault;
	}
}
//-------------------------------------------------------------------------
int FKCW_Json_Object::GetInt( int p_nIndex, int p_nDefault )
{
	if(p_nIndex < 0 || p_nIndex >= m_vecKeyValues.size()) {
		return p_nDefault;
	}

	return FKCW_Json_Value::CastToInt(m_vecKeyValues.at(p_nIndex));
}
//-------------------------------------------------------------------------
// 获取一个Key对应的long值
long FKCW_Json_Object::GetLong( const char* p_szKey, long p_lDefault )
{
	PairMap::iterator iter = m_mapPairs.find(p_szKey);
	if(iter != m_mapPairs.end()) {
		return FKCW_Json_Value::CastToLong(iter->second);
	} else {
		return p_lDefault;
	}
}
//-------------------------------------------------------------------------
long FKCW_Json_Object::GetLong( int p_nIndex, long p_lDefault )
{
	if(p_nIndex < 0 || p_nIndex >= m_vecKeyValues.size()) {
		return p_lDefault;
	}

	return FKCW_Json_Value::CastToLong(m_vecKeyValues.at(p_nIndex));
}
//-------------------------------------------------------------------------
// 获取一个Key对应的float值
float FKCW_Json_Object::GetFloat( const char* p_szKey, float p_fDefault )
{
	PairMap::iterator iter = m_mapPairs.find(p_szKey);
	if(iter != m_mapPairs.end()) {
		return FKCW_Json_Value::CastToFloat(iter->second);
	} else {
		return p_fDefault;
	}
}
//-------------------------------------------------------------------------
float FKCW_Json_Object::GetFloat( int p_nIndex, float p_fDefault )
{
	if(p_nIndex < 0 || p_nIndex >= m_vecKeyValues.size()) {
		return p_fDefault;
	}

	return FKCW_Json_Value::CastToFloat(m_vecKeyValues.at(p_nIndex));
}
//-------------------------------------------------------------------------
// 获取一个Key对应的double值
double	FKCW_Json_Object::GetDouble( const char* p_szKey, double p_dDefault )
{
	PairMap::iterator iter = m_mapPairs.find(p_szKey);
	if(iter != m_mapPairs.end()) {
		return FKCW_Json_Value::CastToDouble(iter->second);
	} else {
		return p_dDefault;
	}
}
//-------------------------------------------------------------------------
double FKCW_Json_Object::GetDouble( int p_nIndex, double p_dDefault )
{
	if(p_nIndex < 0 || p_nIndex >= m_vecKeyValues.size()) {
		return p_dDefault;
	}

	return FKCW_Json_Value::CastToDouble(m_vecKeyValues.at(p_nIndex));
}
//-------------------------------------------------------------------------
// 获取一个Key对应的JsonObject值
FKCW_Json_Object* FKCW_Json_Object::GetObject( const char* p_szKey )
{
	PairMap::iterator iter = m_mapPairs.find(p_szKey);
	if(iter != m_mapPairs.end()) {
		return FKCW_Json_Value::CastToObject(iter->second);
	} else {
		return NULL;
	}
}
//-------------------------------------------------------------------------
FKCW_Json_Object* FKCW_Json_Object::GetObject( int p_nIndex )
{
	if(p_nIndex < 0 || p_nIndex >= m_vecKeyValues.size()) {
		return NULL;
	}

	return FKCW_Json_Value::CastToObject(m_vecKeyValues.at(p_nIndex));
}
//-------------------------------------------------------------------------
// 获取一个Key对应的JsonArray值
FKCW_Json_Array* FKCW_Json_Object::GetArray( const char* p_szKey )
{
	PairMap::iterator iter = m_mapPairs.find(p_szKey);
	if(iter != m_mapPairs.end()) {
		return FKCW_Json_Value::CastToArray(iter->second);
	} else {
		return NULL;
	}
}
//-------------------------------------------------------------------------
FKCW_Json_Array* FKCW_Json_Object::GetArray( int p_nIndex )
{
	if(p_nIndex < 0 || p_nIndex >= m_vecKeyValues.size()) {
		return NULL;
	}

	return FKCW_Json_Value::CastToArray(m_vecKeyValues.at(p_nIndex));
}
//-------------------------------------------------------------------------
// 获取一个Key对应的string值
string  FKCW_Json_Object::GetString( const char* p_szKey, const char* p_szDefault )
{
	PairMap::iterator iter = m_mapPairs.find(p_szKey);
	if(iter != m_mapPairs.end()) {
		return FKCW_Json_Value::CastToString(iter->second);
	} else {
		return p_szDefault;
	}
}
//-------------------------------------------------------------------------
string FKCW_Json_Object::GetString( int p_nIndex, const char* p_szDefault )
{
	if(p_nIndex < 0 || p_nIndex >= m_vecKeyValues.size()) {
		return p_szDefault;
	}

	return FKCW_Json_Value::CastToString(m_vecKeyValues.at(p_nIndex));
}
//-------------------------------------------------------------------------
// 获取Json对象中的元素个数
int FKCW_Json_Object::GetLength()
{
	return m_vecKeyValues.size();
}
//-------------------------------------------------------------------------
// 获取指定编号的Key
const char* FKCW_Json_Object::KeyAt( int p_nIndex )
{
	if(p_nIndex < 0 || p_nIndex >= m_vecKeyValues.size()) {
		return NULL;
	}

	return m_vecKeyValues.at(p_nIndex).k;
}
//-------------------------------------------------------------------------
// 输出json对象全部键值表
// 参数：p_pAOS 输出流
void FKCW_Json_Object::Output( FKCW_IO_AssetOutputStream* p_pAOS, int p_nLevel )
{
	// 数组支架
	s_szArrayBuf[0] = '[';
	s_szArrayBuf[1] = '\n';
	p_pAOS->Write(s_szArrayBuf, 2);

	int count = 0;
	for(PairMap::iterator iter = m_mapPairs.begin(); iter != m_mapPairs.end(); iter++)
	{
		// 输出空白格
		for(int i = 0; i < p_nLevel; i++) {
			s_szArrayBuf[i] = '\t';
		}
		p_pAOS->Write(s_szArrayBuf, p_nLevel);

		// 输出Key
		sprintf(s_szArrayBuf, "\"%s\" : ", iter->second.k);
		p_pAOS->Write(s_szArrayBuf, strlen(s_szArrayBuf));

		switch(iter->second.t) 
		{
		case FKCW_Json_Object::eJsonValueType_Object:
			iter->second.v.jo->Output(p_pAOS, p_nLevel + 1);

			// 逗号
			if(count != m_mapPairs.size() - 1)
				p_pAOS->Write(",", 1);

			break;
		case FKCW_Json_Object::eJsonValueType_Array:
			iter->second.v.ja->Output(p_pAOS, p_nLevel + 1);

			// 逗号
			if(count != m_mapPairs.size() - 1)
				p_pAOS->Write(",", 1);

			break;
		case FKCW_Json_Object::eJsonValueType_Boolean:
			if(count != m_mapPairs.size() - 1)
				sprintf(s_szArrayBuf, "%s\n", iter->second.v.b ? "true" : "false");
			else
				sprintf(s_szArrayBuf, "%s,\n", iter->second.v.b ? "true" : "false");
			p_pAOS->Write(s_szArrayBuf, strlen(s_szArrayBuf));
			break;
		default:
			if(count != m_mapPairs.size() - 1)
				sprintf(s_szArrayBuf, "\"%s\"\n", FKCW_Json_Value::CastToString(iter->second));
			else
				sprintf(s_szArrayBuf, "\"%s\",\n", FKCW_Json_Value::CastToString(iter->second));
			p_pAOS->Write(s_szArrayBuf, strlen(s_szArrayBuf));
			break;
		}

		count++;
	}

	// 尾部空格
	for(int i = 0; i < p_nLevel - 1; i++) {
		s_szArrayBuf[i] = '\t';
	}
	p_pAOS->Write(s_szArrayBuf, p_nLevel - 1);

	// 尾格式
	s_szArrayBuf[0] = ']';
	s_szArrayBuf[1] = '\n';
	p_pAOS->Write(s_szArrayBuf, 2);
}
//-------------------------------------------------------------------------
// 将Json序列化成string
string FKCW_Json_Object::ToString()
{
	FKCW_IO_MemoryOutputStream mos;
	Output(&mos);
	return string(mos.GetBuffer(), mos.GetLength());
}
//-------------------------------------------------------------------------