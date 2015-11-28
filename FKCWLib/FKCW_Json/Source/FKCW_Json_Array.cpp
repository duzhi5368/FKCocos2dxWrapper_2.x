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
FKCW_Json_Array::FKCW_Json_Array()
{

}
//-------------------------------------------------------------------------
FKCW_Json_Array::~FKCW_Json_Array()
{
	for(vector<FKCW_Json_Object::SKeyValue>::iterator iter = m_ArrayKeyValue.begin(); 
		iter != m_ArrayKeyValue.end(); iter++) 
	{
		ReleaseKeyValue(*iter);
	}
}
//-------------------------------------------------------------------------
// 创建一个空的json数组对象
FKCW_Json_Array* FKCW_Json_Array::Create()
{
	FKCW_Json_Array* a = new FKCW_Json_Array();
	return (FKCW_Json_Array*)a->autorelease();
}
//-------------------------------------------------------------------------
// 解析一段json格式内存
// 注：若CCObject是FKCW_Json_Object类型，则该值将作为返回数组的第一个元素。
FKCW_Json_Array* FKCW_Json_Array::Create( const char* p_szJson, size_t p_unLength )
{
	CCObject* obj = FKCW_Json_Parser::Load(p_szJson, p_unLength);
	return SafeCast(obj);
}
//-------------------------------------------------------------------------
// 解析一个Json格式文件
FKCW_Json_Array* FKCW_Json_Array::Create( const string& p_strPath )
{
	CCObject* obj = FKCW_Json_Parser::Load(p_strPath);
	return SafeCast(obj);
}
//-------------------------------------------------------------------------
// 转换CCObject为FKCW_Json_Array。
// 注：若CCObject是FKCW_Json_Object类型，则该值将作为返回数组的第一个元素。
FKCW_Json_Array* FKCW_Json_Array::SafeCast( CCObject* p_pObj )
{
	FKCW_Json_Array* ja = dynamic_cast<FKCW_Json_Array*>(p_pObj);
	if(ja) 
	{
		return ja;
	} 
	else 
	{
		FKCW_Json_Object* jo = dynamic_cast<FKCW_Json_Object*>(p_pObj);
		if(jo) 
		{
			FKCW_Json_Array* ja = FKCW_Json_Array::Create();
			ja->AddObject(jo);
			return ja;
		} 
		else 
		{
			return NULL;
		}
	}
}
//-------------------------------------------------------------------------
//  移除一个键值
void FKCW_Json_Array::ReleaseKeyValue( FKCW_Json_Object::SKeyValue& p_KV )
{
	switch(p_KV.t) {
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
// 返回json数组的元素个数
int FKCW_Json_Array::GetLength()
{
	return m_ArrayKeyValue.size();
}
//-------------------------------------------------------------------------
// 输出json对象全部数组值
// 参数：p_pAOS 输出流
void FKCW_Json_Array::Output( FKCW_IO_AssetOutputStream* p_pAOS, int p_nLevel )
{
	// 数组支架
	s_szArrayBuf[0] = '[';
	s_szArrayBuf[1] = '\n';
	p_pAOS->Write(s_szArrayBuf, 2);

	for(vector<FKCW_Json_Object::SKeyValue>::iterator iter = m_ArrayKeyValue.begin();
		iter != m_ArrayKeyValue.end(); iter++) 
	{
		// 输出空白格
		for(int i = 0; i < p_nLevel; i++) {
			s_szArrayBuf[i] = '\t';
		}
		p_pAOS->Write(s_szArrayBuf, p_nLevel);

		switch(iter->t) 
		{
		case FKCW_Json_Object::eJsonValueType_Object:
			iter->v.jo->Output(p_pAOS, p_nLevel + 1);

			// 逗号
			if(m_ArrayKeyValue.end() - iter != 1)
				p_pAOS->Write(",", 1);

			break;
		case FKCW_Json_Object::eJsonValueType_Array:
			iter->v.ja->Output(p_pAOS, p_nLevel + 1);

			// 逗号
			if(m_ArrayKeyValue.end() - iter != 1)
				p_pAOS->Write(",", 1);

			break;
		case FKCW_Json_Object::eJsonValueType_Boolean:
			if(m_ArrayKeyValue.end() - iter == 1)
				sprintf(s_szArrayBuf, "%s\n", iter->v.b ? "true" : "false");
			else
				sprintf(s_szArrayBuf, "%s,\n", iter->v.b ? "true" : "false");
			p_pAOS->Write(s_szArrayBuf, strlen(s_szArrayBuf));
			break;
		default:
			if(m_ArrayKeyValue.end() - iter == 1)
				sprintf(s_szArrayBuf, "\"%s\"\n", FKCW_Json_Value::CastToString(*iter));
			else
				sprintf(s_szArrayBuf, "\"%s\",\n", FKCW_Json_Value::CastToString(*iter));
			p_pAOS->Write(s_szArrayBuf, strlen(s_szArrayBuf));
			break;
		}
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
string FKCW_Json_Array::ToString()
{
	FKCW_IO_MemoryOutputStream mos;
	Output(&mos);
	return string(mos.GetBuffer(), mos.GetLength());
}
//-------------------------------------------------------------------------
// 在数组中添加一个空值
void FKCW_Json_Array::AddNull()
{
	FKCW_Json_Object::Value v;
	memset(&v, 0, sizeof(FKCW_Json_Object::Value));
	FKCW_Json_Object::SKeyValue kv = {
		NULL,
		FKCW_Json_Object::eJsonValueType_Nil,
		v
	};
	m_ArrayKeyValue.push_back(kv);
}
//-------------------------------------------------------------------------
// 在数组中添加一个bool值
void FKCW_Json_Array::AddBool( bool p_bValue )
{
	FKCW_Json_Object::Value v;
	v.b = p_bValue;
	FKCW_Json_Object::SKeyValue kv = {
		NULL,
		FKCW_Json_Object::eJsonValueType_Boolean,
		v
	};
	m_ArrayKeyValue.push_back(kv);
}
//-------------------------------------------------------------------------
// 在数组中添加一个int值
void FKCW_Json_Array::AddInt( int p_nValue )
{
	char buf[32];
	sprintf(buf, "%d", p_nValue);
	FKCW_Json_Object::Value v;
	v.s = FKCW_Base_Utils::Copy(buf);
	FKCW_Json_Object::SKeyValue kv = {
		NULL,
		FKCW_Json_Object::eJsonValueType_String,
		v
	};
	m_ArrayKeyValue.push_back(kv);
}
//-------------------------------------------------------------------------
// 在数组中添加一个long值
void FKCW_Json_Array::AddLong( long p_lValue )
{
	char buf[32];
	sprintf(buf, "%ld", p_lValue);
	FKCW_Json_Object::Value v;
	v.s = FKCW_Base_Utils::Copy(buf);
	FKCW_Json_Object::SKeyValue kv = {
		NULL,
		FKCW_Json_Object::eJsonValueType_String,
		v
	};
	m_ArrayKeyValue.push_back(kv);
}
//-------------------------------------------------------------------------
// 在数组中添加一个float值
void FKCW_Json_Array::AddFloat( float p_fValue )
{
	char buf[32];
	sprintf(buf, "%f", p_fValue);
	FKCW_Json_Object::Value v;
	v.s = FKCW_Base_Utils::Copy(buf);
	FKCW_Json_Object::SKeyValue kv = {
		NULL,
		FKCW_Json_Object::eJsonValueType_String,
		v
	};
	m_ArrayKeyValue.push_back(kv);
}
//-------------------------------------------------------------------------
// 在数组中添加一个double值
void FKCW_Json_Array::AddDouble( double p_dValue )
{
	char buf[32];
	sprintf(buf, "%lf", p_dValue);
	FKCW_Json_Object::Value v;
	v.s = FKCW_Base_Utils::Copy(buf);
	FKCW_Json_Object::SKeyValue kv = {
		NULL,
		FKCW_Json_Object::eJsonValueType_String,
		v
	};
	m_ArrayKeyValue.push_back(kv);
}
//-------------------------------------------------------------------------
// 在数组中添加一个string值
void FKCW_Json_Array::AddString( const char* p_szValue )
{
	FKCW_Json_Object::Value v;
	v.s = FKCW_Base_Utils::Copy(p_szValue);
	FKCW_Json_Object::SKeyValue kv = {
		NULL,
		FKCW_Json_Object::eJsonValueType_String,
		v
	};
	m_ArrayKeyValue.push_back(kv);
}
//-------------------------------------------------------------------------
// 在数组中添加一个object值
void FKCW_Json_Array::AddObject( FKCW_Json_Object* p_pJO )
{
	FKCW_Json_Object::Value v;
	v.jo = p_pJO;
	FKCW_Json_Object::SKeyValue kv = {
		NULL,
		FKCW_Json_Object::eJsonValueType_Object,
		v
	};
	CC_SAFE_RETAIN(p_pJO);
	m_ArrayKeyValue.push_back(kv);
}
//-------------------------------------------------------------------------
// 在数组中添加一个array值
void FKCW_Json_Array::AddArray( FKCW_Json_Array* p_pJA )
{
	FKCW_Json_Object::Value v;
	v.ja = p_pJA;
	FKCW_Json_Object::SKeyValue kv = {
		NULL,
		FKCW_Json_Object::eJsonValueType_Array,
		v
	};
	CC_SAFE_RETAIN(p_pJA);
	m_ArrayKeyValue.push_back(kv);
}
//-------------------------------------------------------------------------
// 获取指定编号对应的bool值
bool FKCW_Json_Array::GetBool( int p_nIndex, bool p_bDefault )
{
	if( p_nIndex < 0 || p_nIndex >= m_ArrayKeyValue.size()) 
	{
		return p_bDefault;
	}

	return FKCW_Json_Value::CastToBool(m_ArrayKeyValue.at(p_nIndex));
}
//-------------------------------------------------------------------------
// 获取指定编号对应的int值
int FKCW_Json_Array::GetInt( int p_nIndex, int p_nDefault )
{
	if( p_nIndex < 0 || p_nIndex >= m_ArrayKeyValue.size()) 
	{
		return p_nDefault;
	}

	return FKCW_Json_Value::CastToInt(m_ArrayKeyValue.at(p_nIndex));
}
//-------------------------------------------------------------------------
// 获取指定编号对应的long值
long FKCW_Json_Array::GetLong( int p_nIndex, long p_lDefault )
{
	if( p_nIndex < 0 || p_nIndex >= m_ArrayKeyValue.size()) 
	{
		return p_lDefault;
	}

	return FKCW_Json_Value::CastToLong(m_ArrayKeyValue.at(p_nIndex));
}
//-------------------------------------------------------------------------
// 获取指定编号对应的float值
float FKCW_Json_Array::GetFloat( int p_nIndex, bool p_fDefault )
{
	if( p_nIndex < 0 || p_nIndex >= m_ArrayKeyValue.size()) 
	{
		return p_fDefault;
	}

	return FKCW_Json_Value::CastToFloat(m_ArrayKeyValue.at(p_nIndex));
}
//-------------------------------------------------------------------------
// 获取指定编号对应的double值
double FKCW_Json_Array::GetDouble( int p_nIndex, bool p_dDefault )
{
	if( p_nIndex < 0 || p_nIndex >= m_ArrayKeyValue.size()) 
	{
		return p_dDefault;
	}

	return FKCW_Json_Value::CastToDouble(m_ArrayKeyValue.at(p_nIndex));
}
//-------------------------------------------------------------------------
// 获取指定编号对应的JsonObject值
FKCW_Json_Object* FKCW_Json_Array::GetObject( int p_nIndex )
{
	if( p_nIndex < 0 || p_nIndex >= m_ArrayKeyValue.size()) 
	{
		return NULL;
	}

	return FKCW_Json_Value::CastToObject(m_ArrayKeyValue.at(p_nIndex));
}
//-------------------------------------------------------------------------
// 获取指定编号对应的JsonArray值
FKCW_Json_Array* FKCW_Json_Array::GetArray( int p_nIndex )
{
	if( p_nIndex < 0 || p_nIndex >= m_ArrayKeyValue.size()) 
	{
		return NULL;
	}

	return FKCW_Json_Value::CastToArray(m_ArrayKeyValue.at(p_nIndex));
}
//-------------------------------------------------------------------------
// 获取指定编号对应的string值
string FKCW_Json_Array::GetString( int p_nIndex, const char* p_szDefault )
{
	if( p_nIndex < 0 || p_nIndex >= m_ArrayKeyValue.size()) 
	{
		return p_szDefault;
	}

	return FKCW_Json_Value::CastToString(m_ArrayKeyValue.at(p_nIndex));
}
//-------------------------------------------------------------------------