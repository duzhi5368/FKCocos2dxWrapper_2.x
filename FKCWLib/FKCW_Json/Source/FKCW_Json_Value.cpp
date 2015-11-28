//-------------------------------------------------------------------------
#include "../Include/FKCW_Json_Value.h"
//-------------------------------------------------------------------------
static char s_TmpBuffer[64 * 1024];
//-------------------------------------------------------------------------
// 转换为 bool
bool FKCW_Json_Value::CastToBool( FKCW_Json_Object::SKeyValue& p_KV )
{
	switch(p_KV.t) 
	{
	case FKCW_Json_Object::eJsonValueType_Nil:
		return false;
	case FKCW_Json_Object::eJsonValueType_Boolean:
		return p_KV.v.b;
	case FKCW_Json_Object::eJsonValueType_String:
		return !strcmp("true", p_KV.v.s);
	case FKCW_Json_Object::eJsonValueType_Object:
		return p_KV.v.jo != NULL;
	case FKCW_Json_Object::eJsonValueType_Array:
		return p_KV.v.ja != NULL;
	default:
		CCLOGERROR("CCJSONValue::castToInt: unexpected json value type: %d", p_KV.t);
		return false;
	}
}
//-------------------------------------------------------------------------
// 转换为 int
int FKCW_Json_Value::CastToInt( FKCW_Json_Object::SKeyValue& p_KV )
{
	switch(p_KV.t) 
	{
	case FKCW_Json_Object::eJsonValueType_Nil:
		return 0;
	case FKCW_Json_Object::eJsonValueType_Boolean:
		return p_KV.v.b ? 1 : 0;
	case FKCW_Json_Object::eJsonValueType_String:
		return atoi(p_KV.v.s);
	case FKCW_Json_Object::eJsonValueType_Object:
		return *((int*)&p_KV.v.jo);
	case FKCW_Json_Object::eJsonValueType_Array:
		return *((int*)&p_KV.v.ja);
	default:
		CCLOGERROR("CCJSONValue::castToInt: unexpected json value type: %d", p_KV.t);
		return 0;
	}
}
//-------------------------------------------------------------------------
// 转换为 long
long FKCW_Json_Value::CastToLong( FKCW_Json_Object::SKeyValue& p_KV )
{
	switch(p_KV.t) 
	{
	case FKCW_Json_Object::eJsonValueType_Nil:
		return 0;
	case FKCW_Json_Object::eJsonValueType_Boolean:
		return p_KV.v.b ? 1 : 0;
	case FKCW_Json_Object::eJsonValueType_String:
		return atoi(p_KV.v.s);
	case FKCW_Json_Object::eJsonValueType_Object:
		return (long)p_KV.v.jo;
	case FKCW_Json_Object::eJsonValueType_Array:
		return (long)p_KV.v.ja;
	default:
		CCLOGERROR("CCJSONValue::castToInt: unexpected json value type: %d", p_KV.t);
		return 0;
	}
}
//-------------------------------------------------------------------------
// 转换为 float
float FKCW_Json_Value::CastToFloat( FKCW_Json_Object::SKeyValue& p_KV )
{
	switch(p_KV.t) 
	{
	case FKCW_Json_Object::eJsonValueType_Nil:
		return 0;
	case FKCW_Json_Object::eJsonValueType_Boolean:
		return p_KV.v.b ? 1.0f : 0.0f;
	case FKCW_Json_Object::eJsonValueType_String:
		return (float)atof(p_KV.v.s);
	case FKCW_Json_Object::eJsonValueType_Object:
		return (float)*((int*)&p_KV.v.jo);
	case FKCW_Json_Object::eJsonValueType_Array:
		return (float)*((int*)&p_KV.v.ja);
	default:
		CCLOGERROR("CCJSONValue::castToInt: unexpected json value type: %d", p_KV.t);
		return 0;
	}
}
//-------------------------------------------------------------------------
// 转换为 double
double FKCW_Json_Value::CastToDouble( FKCW_Json_Object::SKeyValue& p_KV )
{
	switch(p_KV.t) 
	{
	case FKCW_Json_Object::eJsonValueType_Nil:
		return 0;
	case FKCW_Json_Object::eJsonValueType_Boolean:
		return p_KV.v.b ? 1 : 0;
	case FKCW_Json_Object::eJsonValueType_String:
		return atof(p_KV.v.s);
	case FKCW_Json_Object::eJsonValueType_Object:
		return (double)*((int*)&p_KV.v.jo);
	case FKCW_Json_Object::eJsonValueType_Array:
		return (double)*((int*)&p_KV.v.ja);
	default:
		CCLOGERROR("CCJSONValue::castToInt: unexpected json value type: %d", p_KV.t);
		return 0;
	}
}
//-------------------------------------------------------------------------
// 转换为 json object
FKCW_Json_Object* FKCW_Json_Value::CastToObject( FKCW_Json_Object::SKeyValue& p_KV )
{
	switch(p_KV.t) 
	{
	case FKCW_Json_Object::eJsonValueType_Nil:
	case FKCW_Json_Object::eJsonValueType_Boolean:
	case FKCW_Json_Object::eJsonValueType_String:
	case FKCW_Json_Object::eJsonValueType_Array:
			return NULL;
	case FKCW_Json_Object::eJsonValueType_Object:
		return p_KV.v.jo;
	default:
		CCLOGERROR("CCJSONValue::castToInt: unexpected json value type: %d", p_KV.t);
		return 0;
	}
}
//-------------------------------------------------------------------------
// 转换为 json array
FKCW_Json_Array* FKCW_Json_Value::CastToArray( FKCW_Json_Object::SKeyValue& p_KV )
{
	switch(p_KV.t) 
	{
	case FKCW_Json_Object::eJsonValueType_Nil:
	case FKCW_Json_Object::eJsonValueType_Boolean:
	case FKCW_Json_Object::eJsonValueType_String:
	case FKCW_Json_Object::eJsonValueType_Object:
		return NULL;
	case FKCW_Json_Object::eJsonValueType_Array:
		return p_KV.v.ja;
	default:
		CCLOGERROR("CCJSONValue::castToInt: unexpected json value type: %d", p_KV.t);
		return 0;
	}
}
//-------------------------------------------------------------------------
// 转换为 string
const char* FKCW_Json_Value::CastToString( FKCW_Json_Object::SKeyValue& p_KV )
{
	switch(p_KV.t) 
	{
	case FKCW_Json_Object::eJsonValueType_Nil:
		return NULL;
	case FKCW_Json_Object::eJsonValueType_Boolean:
		if(p_KV.v.b) {
			sprintf(s_TmpBuffer, "true");
		} else {
			sprintf(s_TmpBuffer, "false");
		}
		return s_TmpBuffer;
	case FKCW_Json_Object::eJsonValueType_String:
		return p_KV.v.s;
	case FKCW_Json_Object::eJsonValueType_Object:
		sprintf(s_TmpBuffer, "FKCW_Json_Object");
		return s_TmpBuffer;
	case FKCW_Json_Object::eJsonValueType_Array:
		sprintf(s_TmpBuffer, "FKCW_Json_Array");
		return s_TmpBuffer;
	default:
		CCLOGERROR("CCJSONValue::castToInt: unexpected json value type: %d", p_KV.t);
		return NULL;
	}
}
//-------------------------------------------------------------------------