//*************************************************************************
//	创建日期:	2014-11-14
//	文件名称:	FKCW_SimpleNet_Macro.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "cocos2d.h"
#include "../../FKCW_Base/FKCW_Base.h"
#include <string>
#include <list>
//-------------------------------------------------------------------------
USING_NS_CC;
using std::list;
using std::string;
//-------------------------------------------------------------------------
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
typedef unsigned int          SOCKET;
#ifndef INVALID_SOCKET
#define INVALID_SOCKET  (SOCKET)(~0)
#endif
#ifndef SOCKET_ERROR
#define SOCKET_ERROR            (-1)
#endif
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <windows.h>
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#endif

// 是否文件头一定加文件长度
#ifndef USING_PACKAGE_HEAD_LENGTH
#define USING_PACKAGE_HEAD_LENGTH 0
#endif

// Socket连接超时时间，默认30秒
#ifndef SOCKET_SOTIMEOUT
#define SOCKET_SOTIMEOUT 30.0f
#endif

#ifndef SOCKET_READ_BUFFER_SIZE
#define SOCKET_READ_BUFFER_SIZE 8192
#endif

// 是否内存反转处理
#ifndef MEMORYTYPE_REVERSE
#define MEMORYTYPE_REVERSE 0
#endif

#ifndef HANDLE_ON_SINGLE_FRAME
#define HANDLE_ON_SINGLE_FRAME 1
#endif

#ifndef CCBUFFER_ORGINAL_SIZE
#define CCBUFFER_ORGINAL_SIZE 512
#endif

#ifndef SINGLE_DELEGATE_INSTANCE_FUNC
#define SINGLE_DELEGATE_INSTANCE_FUNC(_CLASS_) \
	static _CLASS_* sharedDelegate() \
{ \
	static _CLASS_* pRet = NULL; \
	if( !pRet ) pRet = new _CLASS_(); \
	return pRet; \
}
#endif
//-------------------------------------------------------------------------