/********************************************************************
	created:	2014/10/22
	file base:	FKCW_Base_Macro
	author:		FreeKnight
	
	purpose:	常用宏
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "cocos2d.h"
//--------------------------------------------------------------------
USING_NS_CC;
//--------------------------------------------------------------------
// 包含常用标准头
#include <stack>
#include <vector>
#include <map>
#include <math.h>
#include <list>
#include <string>
#include <iostream>
#include <stddef.h>
#include <memory.h>
#include <stdio.h>
#include <cstring>
using namespace std;
//--------------------------------------------------------------------
#include "FKCW_Base_Memory.h"
//--------------------------------------------------------------------
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	#include <sys/stat.h>
	#include <endian.h>
	#include "platform/android/jni/JniHelper.h"
	#include <unistd.h>

	// 路径分隔符
	#define CC_PATH_SEPARATOR '/'

	// 最大浮点型
	#define MAX_FLOAT 3.4028235E38f
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
	#include <sys/stat.h>
	#import <Foundation/Foundation.h>
	#import <CoreFoundation/CoreFoundation.h>
	#define letoh64 CFSwapInt64LittleToHost
	#define letoh32 CFSwapInt32LittleToHost
	#define letoh16 CFSwapInt16LittleToHost
	#define htole64 CFSwapInt64HostToLittle
	#define htole32 CFSwapInt32HostToLittle
	#define htole16 CFSwapInt16HostToLittle
	#define betoh64	CFSwapInt64BigToHost
	#define betoh32	CFSwapInt32BigToHost
	#define betoh16 CFSwapInt16BigToHost
	#define htobe64 CFSwapInt64HostToBig
	#define htobe32 CFSwapInt32HostToBig
	#define htobe16 CFSwapInt16HostToBig

	// 路径分隔符
	#define CC_PATH_SEPARATOR '/'

	// 最大浮点型
	#define MAX_FLOAT 3.4028235E38f
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	#include <direct.h>
	#define mkdir _mkdir
	#include <io.h>

	#define ___my_swap16(x) \
		((uint16_t)( \
		(((uint16_t)(x) & (uint16_t)0x00ffU) << 8) | \
		(((uint16_t)(x) & (uint16_t)0xff00U) >> 8) ))
	#define ___my_swap32(x) \
		((uint32_t)( \
		(((uint32_t)(x) & (uint32_t)0x000000ffUL) << 24) | \
		(((uint32_t)(x) & (uint32_t)0x0000ff00UL) <<  8) | \
		(((uint32_t)(x) & (uint32_t)0x00ff0000UL) >>  8) | \
		(((uint32_t)(x) & (uint32_t)0xff000000UL) >> 24) ))
	#define ___my_swap64(x) \
		((uint64_t)( \
		(uint64_t)(((uint64_t)(x) & (uint64_t)0x00000000000000ffULL) << 56) | \
		(uint64_t)(((uint64_t)(x) & (uint64_t)0x000000000000ff00ULL) << 40) | \
		(uint64_t)(((uint64_t)(x) & (uint64_t)0x0000000000ff0000ULL) << 24) | \
		(uint64_t)(((uint64_t)(x) & (uint64_t)0x00000000ff000000ULL) <<  8) | \
		(uint64_t)(((uint64_t)(x) & (uint64_t)0x000000ff00000000ULL) >>  8) | \
		(uint64_t)(((uint64_t)(x) & (uint64_t)0x0000ff0000000000ULL) >> 24) | \
		(uint64_t)(((uint64_t)(x) & (uint64_t)0x00ff000000000000ULL) >> 40) | \
		(uint64_t)(((uint64_t)(x) & (uint64_t)0xff00000000000000ULL) >> 56) ))
	#define letoh64(x) (x)
	#define letoh32(x) (x)
	#define letoh16(x) (x)
	#define htole64(x) (x)
	#define htole32(x) (x)
	#define htole16(x) (x)
	#define betoh64(x) ___my_swap64(x)
	#define betoh32(x) ___my_swap32(x)
	#define betoh16(x) ___my_swap16(x)
	#define htobe64(x) ___my_swap64(x)
	#define htobe32(x) ___my_swap32(x)
	#define htobe16(x) ___my_swap16(x)

	#define _LITTLE_ENDIAN 1234
	#define _BIG_ENDIAN 4321
	#define LITTLE_ENDIAN _LITTLE_ENDIAN
	#define BIG_ENDIAN _BIG_ENDIAN
	#define _BYTE_ORDER LITTLE_ENDIAN
	#define BYTE_ORDER _BYTE_ORDER

	// 路径分隔符
	#define CC_PATH_SEPARATOR '\\'

	// 最大浮点型
	#define MAX_FLOAT FLT_MAX

	typedef unsigned long in_addr_t;
	typedef int socklen_t;
#endif
//--------------------------------------------------------------------
#if !defined(__GNUC__)
	typedef signed __int64 int64;
	typedef signed __int32 int32;
	typedef signed __int16 int16;
	typedef signed __int8 int8;
	typedef unsigned __int64 uint64;
	typedef unsigned __int32 uint32;
	typedef unsigned __int16 uint16;
	typedef unsigned __int8 uint8;
	typedef float Real;
#else
	typedef int64_t int64;
	typedef int32_t int32;
	typedef int16_t int16;
	typedef int8_t int8;
	typedef uint64_t uint64;
	typedef uint32_t uint32;
	typedef uint16_t uint16;
	typedef uint8_t uint8;
	typedef uint32_t DWORD;
	typedef float Real;
#endif
//--------------------------------------------------------------------
// 时间结构定义
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	struct timeval_t
	{
		long    tv_sec;			// seconds
		long    tv_usec;		// microSeconds
	};
#else
	typedef timeval timeval_t;
#endif // defined(_WIN32)
//--------------------------------------------------------------------
// 类型定义
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	typedef int ssize_t;
#endif

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	typedef long int ssize_t;
#endif


// 最大整形
#define MAX_INT 0x7fffffff

// 定义无穷
#ifndef	INFINITY
#define INFINITY (0.5*(numeric_limits<float>::max)())
#endif

// 简易定义
#ifndef MAX
#define MAX(a, b) ( a > b ? a : b )
#endif
#ifndef MIN
#define MIN(a, b) ( a < b ? a : b )
#endif

// 简易定义
#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)							\
	((uint32)(char)(ch0) | ((uint32)(char)(ch1) << 8) |			\
	((uint32)(char)(ch2) << 16) | ((uint32)(char)(ch3) << 24 ))
#endif

// 取符号
#define SIGN(x) ((x) >= 0 ? 1 : -1)

// CPU休眠
#define SLEEP(x) FKCW_Base_Utils::SystemSleep(x)

// 简单的便捷宏
#define ccz(x, y) CCSizeMake((float)(x), (float)(y))

// 便捷的字符串转换宏
#define A2U(str) FKCW_Base_StringConvert::a2u(str)
#define U2A(str) FKCW_Base_StringConvert::u2a(str)

// 屏幕中心点
#define SCERRN_WIDTH			CCDirector::sharedDirector()->getWinSize().width
#define SCERRN_HEIGHT			CCDirector::sharedDirector()->getWinSize().height
#define SCREEN_CENTER_POS_X		(CCDirector::sharedDirector()->getWinSize().width / 2)
#define SCREEN_CENTER_POS_Y		(CCDirector::sharedDirector()->getWinSize().height / 2)
#define SCREEN_CENTER_POS		(ccp(CCDirector::sharedDirector()->getWinSize().width / 2,CCDirector::sharedDirector()->getWinSize().height/2))
#define SCREEN_RIGHT_UP_POS		(ccp(CCDirector::sharedDirector()->getWinSize().width, CCDirector::sharedDirector()->getWinSize().height))
#define SCREEN_RIGHT_DOWN_POS	(ccp(CCDirector::sharedDirector()->getWinSize().width, 0))
#define SCREEN_LEFT_UP_POS		(ccp(0, CCDirector::sharedDirector()->getWinSize().height))
#define SCREEN_LEFT_DOWN_POS	(ccp(0,0))

// 取随机值
#define RANDOM_0_X_INT(x) ((int)(CCRANDOM_0_1() * (x) * 100) % ((x) + 1))
#define RANDOM_X_Y_INT(x, y) (RANDOM_0_X_INT((y) - (x)) + (x))

//--------------------------------------------------------------------
#define CC_SYNTHESIZE_PASS_BY_REF_NC(varType, varName, funName) \
	protected: varType varName; \
	public: virtual varType& get##funName(void) { return varName; } \
	public: virtual void set##funName(const varType& var){ varName = var; }

#define CC_SYNTHESIZE_PASS_BY_REF_SETTER(varType, varName, funName) \
	protected: varType varName;\
	public: virtual const varType& get##funName(void) const { return varName; } \
	public: virtual void set##funName(const varType& var);

#define CC_SYNTHESIZE_BOOL(varName, funName) \
	protected: bool varName; \
	public: virtual bool is##funName(void) const { return varName; } \
	public: virtual void set##funName(bool var) { varName = var; } \
	public: virtual void mark##funName() { varName = true; } \
	public: virtual void clear##funName() { varName = false; }

#define CC_SYNTHESIZE_READONLY_BOOL(varName, funName)\
	protected: bool varName;\
	public: virtual bool is##funName(void) const { return varName; }

#define CC_SYNTHESIZE_BOOL_SETTER(varName, funName) \
	protected: bool varName; \
	public: virtual bool is##funName(void) const { return varName; } \
	public: virtual void set##funName(bool var);

#define CC_SYNTHESIZE_SETTER(varType, varName, funName) \
	protected: varType varName;\
	public: virtual varType get##funName(void) const { return varName; } \
	public: virtual void set##funName(varType var);

#define CC_SYNTHESIZE_GETTER(varType, varName, funName) \
	protected: varType varName;\
	public: virtual varType get##funName(void) const; \
	public: virtual void set##funName(varType var) { varName = var; }

//--------------------------------------------------------------------