//-------------------------------------------------------------------------
#include "../Include/FKCW_Json_Array.h"
#include "../Include/FKCW_Json_Object.h"
#include "../Include/FKCW_Json_Value.h"
#include "../Include/FKCW_Json_Parser.h"
#include "../../FKCW_IO/Include/FKCW_IO_MemoryInputStream.h"
#include "../../FKCW_Base/Include/FKCW_Base_Utils.h"
#include "../../FKCW_Depend/yajl/include/yajl_gen.h"
#include "../../FKCW_Depend/yajl/include/yajl_parse.h"
//-------------------------------------------------------------------------
// json解析上下文
typedef struct SJSONContext{
	yajl_gen g;
	char key[512];
	CCObject* root;
	vector<CCObject*>* objStack;
	vector<bool>* flagStack;
}JSON_CONTEXT;
//-------------------------------------------------------------------------
static int __ReformatNull(void* ctx) 
{
	SJSONContext* jc = (SJSONContext*)ctx;

	bool array = *(jc->flagStack->rbegin());
	if(array) 
	{
		FKCW_Json_Array* p = (FKCW_Json_Array*)(*(jc->objStack->rbegin()));
		p->AddNull();
	} 
	else
	{
		FKCW_Json_Object* p = (FKCW_Json_Object*)(*(jc->objStack->rbegin()));
		p->AddNull(jc->key);
	}

	return yajl_gen_status_ok == yajl_gen_null(jc->g);
}
//-------------------------------------------------------------------------
static int __ReformatBoolean(void* ctx, int boolean) 
{
	SJSONContext* jc = (SJSONContext*)ctx;

	bool array = *(jc->flagStack->rbegin());
	if(array) 
	{
		FKCW_Json_Array* p = (FKCW_Json_Array*)(*(jc->objStack->rbegin()));
		p->AddBool( boolean != 0 ? true : false );
	} 
	else 
	{
		FKCW_Json_Object* p = (FKCW_Json_Object*)(*(jc->objStack->rbegin()));
		p->AddBool(jc->key, boolean != 0 ? true : false );
	}

	return yajl_gen_status_ok == yajl_gen_bool(jc->g, boolean);
}
//-------------------------------------------------------------------------
static int __ReformatNumber(void* ctx, const char * stringVal, size_t stringLen) 
{
	SJSONContext* jc = (SJSONContext*)ctx;
	char* s = (char*)stringVal;
	char oldChar = s[stringLen];
	s[stringLen] = 0;

	bool array = *(jc->flagStack->rbegin());
	if(array) 
	{
		FKCW_Json_Array* p = (FKCW_Json_Array*)(*(jc->objStack->rbegin()));
		p->AddString((const char*)stringVal);
	} 
	else 
	{
		FKCW_Json_Object* p = (FKCW_Json_Object*)(*(jc->objStack->rbegin()));
		p->AddString(jc->key, s);
	}

	s[stringLen] = oldChar;

	return yajl_gen_status_ok == yajl_gen_number(jc->g, stringVal, stringLen);
}
//-------------------------------------------------------------------------
static int __ReformatString(void* ctx, const unsigned char* stringVal, size_t stringLen) 
{
	SJSONContext* jc = (SJSONContext*)ctx;

	char* s = (char*)stringVal;
	char oldChar = s[stringLen];
	s[stringLen] = 0;

	bool array = *(jc->flagStack->rbegin());
	if(array) 
	{
		FKCW_Json_Array* p = (FKCW_Json_Array*)(*(jc->objStack->rbegin()));
		p->AddString(s);
	} 
	else 
	{
		FKCW_Json_Object* p = (FKCW_Json_Object*)(*(jc->objStack->rbegin()));
		p->AddString(jc->key, s);
	}

	s[stringLen] = oldChar;

	return yajl_gen_status_ok == yajl_gen_string(jc->g, stringVal, stringLen);
}
//-------------------------------------------------------------------------
static int __ReformatMapKey(void* ctx, const unsigned char* stringVal, size_t stringLen) 
{
	SJSONContext* jc = (SJSONContext*)ctx;
	memcpy(jc->key, stringVal, stringLen * sizeof(unsigned char));
	jc->key[stringLen] = 0;

	return yajl_gen_status_ok == yajl_gen_string(jc->g, stringVal, stringLen);
}
//-------------------------------------------------------------------------
static int __ReformatStartMap(void* ctx) 
{
	SJSONContext* jc = (SJSONContext*)ctx;
	if(jc->root) 
	{
		FKCW_Json_Object* jo = FKCW_Json_Object::Create();
		bool array = *(jc->flagStack->rbegin());
		if(array) 
		{
			FKCW_Json_Array* p = (FKCW_Json_Array*)(*(jc->objStack->rbegin()));
			p->AddObject(jo);
		} 
		else 
		{
			FKCW_Json_Object* p = (FKCW_Json_Object*)(*(jc->objStack->rbegin()));
			p->AddObject(jc->key, jo);
		}

		jc->objStack->push_back(jo);
		jc->flagStack->push_back(false);
	} 
	else 
	{
		jc->root = FKCW_Json_Object::Create();
		jc->objStack->push_back(jc->root);
		jc->flagStack->push_back(false);
	}

	return yajl_gen_status_ok == yajl_gen_map_open(jc->g);
}
//-------------------------------------------------------------------------
static int __ReformatEndMap(void* ctx) 
{
	SJSONContext* jc = (SJSONContext*)ctx;
	jc->objStack->pop_back();
	jc->flagStack->pop_back();

	return yajl_gen_status_ok == yajl_gen_map_close(jc->g);
}
//-------------------------------------------------------------------------
static int __ReformatStartArray(void* ctx) 
{
	SJSONContext* jc = (SJSONContext*)ctx;
	if(jc->root) 
	{
		FKCW_Json_Array* ja = FKCW_Json_Array::Create();
		bool array = *(jc->flagStack->rbegin());
		if(array) 
		{
			FKCW_Json_Array* p = (FKCW_Json_Array*)(*(jc->objStack->rbegin()));
			p->AddArray(ja);
		} 
		else 
		{
			FKCW_Json_Object* p = (FKCW_Json_Object*)(*(jc->objStack->rbegin()));
			p->AddArray(jc->key, ja);
		}

		jc->objStack->push_back(ja);
		jc->flagStack->push_back(true);
	} 
	else 
	{
		jc->root = FKCW_Json_Array::Create();
		jc->objStack->push_back(jc->root);
		jc->flagStack->push_back(true);
	}

	return yajl_gen_status_ok == yajl_gen_array_open(jc->g);
}
//-------------------------------------------------------------------------
static int __ReformatEndArray(void* ctx) 
{
	SJSONContext* jc = (SJSONContext*)ctx;
	jc->objStack->pop_back();
	jc->flagStack->pop_back();

	return yajl_gen_status_ok == yajl_gen_array_close(jc->g);
}
//-------------------------------------------------------------------------
static yajl_callbacks s_CallbackFuncs = 
{
	__ReformatNull,
	__ReformatBoolean,
	NULL,
	NULL,
	__ReformatNumber,
	__ReformatString,
	__ReformatStartMap,
	__ReformatMapKey,
	__ReformatEndMap,
	__ReformatStartArray,
	__ReformatEndArray
};
//-------------------------------------------------------------------------
// 解析一段内存中的json字符串
CCObject* FKCW_Json_Parser::Load( const char* p_szJsonString, size_t p_unLength )
{
	// 使用内存输入流
	FKCW_IO_MemoryInputStream* mis = FKCW_IO_MemoryInputStream::Create((char*)p_szJsonString, p_unLength);

	// yajl状态
	yajl_status stat;

	// 获取yajl_gen实例
	yajl_gen g = yajl_gen_alloc(NULL);

	// 注册回调函数
	SJSONContext ctx;
	ctx.g = g;
	ctx.root = NULL;
	ctx.objStack = new vector<CCObject*>();
	ctx.flagStack = new vector<bool>();
	yajl_handle hand = yajl_alloc(&s_CallbackFuncs, NULL, (void*)&ctx);

	// 配置yajl
	yajl_gen_config(g, yajl_gen_beautify, 1);
	yajl_gen_config(g, yajl_gen_validate_utf8, 1);
	yajl_config(hand, yajl_allow_comments, 1);

	// 解析
	char buf[4096];
	while(true) 
	{
		// 读取数据
		int rd = mis->Read(buf, 4096);
		if (rd == 0)
			break;

		// 解析数据
		stat = yajl_parse(hand, (const unsigned char*)buf, rd);

		// 解析失败，则break
		if (stat != yajl_status_ok)
			break;
	}

	// 解析完毕
	stat = yajl_complete_parse(hand);

	// 检查是否有错误
	if (stat != yajl_status_ok) 
	{
		unsigned char* str = yajl_get_error(hand, 1, (const unsigned char*)p_szJsonString, p_unLength);
		CCLOGWARN("parse json error: %s", str);
		yajl_free_error(hand, str);

		// 错误
		ctx.root = NULL;
	}

	// 释放yajl
	yajl_gen_free(g);
	yajl_free(hand);
	delete ctx.objStack;
	delete ctx.flagStack;

	return ctx.root;
}
//-------------------------------------------------------------------------
// 解析一个json文件
CCObject* FKCW_Json_Parser::Load( const string& p_szPath )
{
	unsigned long len;
	char* data = (char*)CCFileUtils::sharedFileUtils()->getFileData(p_szPath.c_str(), "rb", &len);
	CCObject* ret = Load(data, (size_t)len);
	free(data);
	return ret;
}
//-------------------------------------------------------------------------