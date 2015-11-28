/********************************************************************
	created:	2014/10/27
	file base:	FKCW_Base_Memory
	author:		FreeKnight
	
	purpose:	
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "cocos2d.h"
//--------------------------------------------------------------------
USING_NS_CC;
//--------------------------------------------------------------------
// 是否开启内存跟踪系统
#if defined(_DEBUG) || defined(DEBUG)
#define FKCW_MEMORY_TRACKING
#else
#undef FKCW_MEMORY_TRACKING
#endif
//--------------------------------------------------------------------
// 如果该值为true，则LOG记录全部的内存分配
extern bool g_bIsFlagAllocationLog;
//--------------------------------------------------------------------
#ifdef FKCW_MEMORY_TRACKING

extern "C" /*CC_DLL*/ void* _ccMalloc(size_t size, const char* file, int line, const char* logTag);
extern "C" /*CC_DLL*/ void* _ccCalloc(size_t nitems, size_t size, const char* file, int line);
extern "C" /*CC_DLL*/ void* _ccRealloc(void* ptr, size_t size, const char* file, int line);
extern "C" /*CC_DLL*/ void _ccFree(void* ptr, const char* file, int line);

inline void* operator new(size_t n, const char* file, int line) {
	return _ccMalloc(n, file, line, "NEW");
}

inline void* operator new[](size_t n, const char* file, int line) {
	return _ccMalloc(n, file, line, "NEW[]");
}

inline void operator delete(void* p, const char* file, int line) {
	_ccFree(p, file, line);
}

inline void operator delete(void* p) throw() {
	_ccFree(p, __FILE__, __LINE__);
}

inline void operator delete[](void* p) throw() {
	_ccFree(p, __FILE__, __LINE__);
}

#define ccMalloc(size) _ccMalloc(size, __FILE__, __LINE__, "MALLOC")
#define ccCalloc(nitems, size) _ccCalloc(nitems, size, __FILE__, __LINE__)
#define ccRealloc(ptr, size) _ccRealloc(ptr, size, __FILE__, __LINE__)
#define ccFree(ptr) _ccFree(ptr, __FILE__, __LINE__)
#define CCNEW new(__FILE__, __LINE__)
#define CCDELETE(object) delete object
#define CCNEWARR(t, s) new(__FILE__, __LINE__) t[(s)]
#define CCDELETEARR(object) delete[] (object)
#define new CCNEW
#define delete(object) CCDELETE(object)

#else

#define ccMalloc malloc
#define ccCalloc calloc
#define ccRealloc realloc
#define ccFree free
#define CCNEW new
#define CCDELETE(object) delete object
#define CCNEWARR(t, s) new t[(s)]
#define CCDELETEARR(object) delete[] (object)

#endif
//--------------------------------------------------------------------
class FKCW_Base_Memory
{
public:
	// 输出内存使用列表
	static void PrintUasge();
	// 输出内存泄露记录
	static void PrintDump();
};
//--------------------------------------------------------------------