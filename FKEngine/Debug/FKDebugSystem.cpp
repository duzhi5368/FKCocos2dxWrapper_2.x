//*************************************************************************
//	创建日期:	2015-1-16
//	文件名称:	FKDebugSystem.cpp
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKDebugSystem.h"
//-------------------------------------------------------------------------
#ifndef MAX_LOG_LEN
#define MAX_LOG_LEN         (cocos2d::kMaxLogLen + 1)
#endif
//-------------------------------------------------------------------------
IDebugSystem* gs_pDebugSystem;
FKDebugSystem gs_FKDebugSystem;
//-------------------------------------------------------------------------
// 获取调试管理器指针
IDebugSystem* GetDebugSystem()
{
	return gs_pDebugSystem;
}
//-------------------------------------------------------------------------
FKDebugSystem::FKDebugSystem()
	: m_bIsUseLogFile( true )
{
	FKCW_Base_Utils::CreateWinConsole();

	gs_pDebugSystem = this;
}
//-------------------------------------------------------------------------
FKDebugSystem::~FKDebugSystem()
{
	gs_pDebugSystem = NULL;
}
//-------------------------------------------------------------------------
// 销毁释放
void FKDebugSystem::Release()
{
	CloseDebugSystem();

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	FreeConsole();
#endif
}
//-------------------------------------------------------------------------
// 关闭全部Debug行为
void FKDebugSystem::CloseDebugSystem()
{
	EnableGameConsole( false );
	EnableLogFile( false );
	EnableWinConsole( false );
	EnableFPS( false );
}
//-------------------------------------------------------------------------
// 显示/隐藏游戏中控制台面板
bool FKDebugSystem::EnableGameConsole( bool bIsEnable )
{
	return true;
}
//-------------------------------------------------------------------------
// 开启/关闭 Log 日志记录
bool FKDebugSystem::EnableLogFile( bool bIsEnable )
{
	m_bIsUseLogFile = bIsEnable;
	return true;
}
//-------------------------------------------------------------------------
// 开启/关闭 WinConsole
bool FKDebugSystem::EnableWinConsole( bool bIsEnable )
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	if( bIsEnable )
	{
		ShowWindow(GetConsoleWindow(), SW_SHOW);
	}
	else
	{
		ShowWindow(GetConsoleWindow(), SW_HIDE);
	}
#endif
	return true;
}
//-------------------------------------------------------------------------
// 开启/关闭 FPS 和渲染批次显示
bool FKDebugSystem::EnableFPS( bool bIsEnable )
{
	CCDirector *pDirector = CCDirector::sharedDirector();
	if( pDirector == NULL )
	{
		return false;
	}
	pDirector->setDisplayStats( bIsEnable );
	return true;
}
//-------------------------------------------------------------------------
// 输出游戏中内存占用状况
void FKDebugSystem::DumpMemoryUse()
{
	FKCW_Base_Memory::PrintUasge();
}
//-------------------------------------------------------------------------
// 输出游戏中内存泄露状况
void FKDebugSystem::DumpMemoryLeak()
{
	FKCW_Base_Memory::PrintDump();
}
//-------------------------------------------------------------------------
// 输出游戏中Node信息
void FKDebugSystem::DumpNodeInfo()
{
	CCScene* p = CCDirector::sharedDirector()->getRunningScene();
	if( p )
		p->Dump();
}
//-------------------------------------------------------------------------
// 输出游戏中Touch信息
void FKDebugSystem::DumpTouchInfo()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->Dump();
}
//-------------------------------------------------------------------------
// 输出游戏中纹理使用信息
void FKDebugSystem::DumpTextureInfo()
{
	CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
}
//-------------------------------------------------------------------------
// 输出硬件状况信息
void FKDebugSystem::DumpConfigurationInfo()
{
	CCConfiguration::sharedConfiguration()->dumpInfo();
}
//-------------------------------------------------------------------------
// 输出全部Dump
void FKDebugSystem::DumpAll()
{
	DumpMemoryUse();
	DumpMemoryLeak();
	DumpNodeInfo();
	DumpTouchInfo();
	DumpTextureInfo();
	DumpConfigurationInfo();
}
//-------------------------------------------------------------------------
// 添加普通调试性Log
void FKDebugSystem::LogDebug( const char* pszFormat, ... )
{
	char szBuf[MAX_LOG_LEN - 20] = {0};
	char szShowBuf[MAX_LOG_LEN] = { 0 };

	struct tm *tm;  
	time_t timep;  
	time(&timep);  
	tm = localtime(&timep);  
	int hour=tm->tm_hour;  
	int minute=tm->tm_min;  
	int second=tm->tm_sec;
	char szTime[10] = { 0 };
	sprintf( szTime, "%02d:%02d:%02d",  hour, minute, second );

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	
	va_list ap;
	va_start(ap, pszFormat);
	vsnprintf_s(szBuf, MAX_LOG_LEN - 20, MAX_LOG_LEN - 20, pszFormat, ap);
	va_end(ap);

	sprintf( szShowBuf, "%s - %s", szTime, szBuf );

	OutputDebugStringA(szShowBuf);
	OutputDebugStringA("\n");
	puts(szShowBuf);

#else
	va_list ap;
	va_start(ap, pszFormat);
	vsnprintf(szBuf, MAX_LOG_LEN - 20, pszFormat, ap);//MAX_LOG_LEN - 20, 
	va_end(ap);
	sprintf( szShowBuf, "%s - %s", szTime, szBuf );
	CCLOG(szBuf);
#endif

	if( m_bIsUseLogFile )
	{
		FKCW_Base_Utils::FKFileLog( szShowBuf );
	}
	
}
//-------------------------------------------------------------------------
// 添加信息式Log
void FKDebugSystem::LogInfo( const char* pszFormat, ... )
{
	char szBuf[MAX_LOG_LEN - 20] = {0};
	char szShowBuf[MAX_LOG_LEN] = { 0 };

	struct tm *tm;  
	time_t timep;  
	time(&timep);  
	tm = localtime(&timep);  
	int hour=tm->tm_hour;  
	int minute=tm->tm_min;  
	int second=tm->tm_sec;
	char szTime[10] = { 0 };
	sprintf( szTime, "%02d:%02d:%02d",  hour, minute, second );

	

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	va_list ap;
	va_start(ap, pszFormat);
	vsnprintf_s(szBuf, MAX_LOG_LEN - 20, MAX_LOG_LEN - 20, pszFormat, ap);
	va_end(ap);

	sprintf( szShowBuf, "%s - %s [L]%d - %s", szTime, __FILE__, __LINE__, szBuf );

	OutputDebugStringA(szShowBuf);
	OutputDebugStringA("\n");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole != INVALID_HANDLE_VALUE)
	{
		SetConsoleTextAttribute(hConsole, eConsolBKColor_Gray | eConsolTextColor_Ching );
	}
	puts(szShowBuf);
#else
	va_list ap;
	va_start(ap, pszFormat);
	vsnprintf(szBuf, MAX_LOG_LEN - 20, pszFormat, ap);// MAX_LOG_LEN - 20,
	va_end(ap);
	sprintf( szShowBuf, "%s - %s", szTime, szBuf );
	CCLOG(szBuf);
#endif

	if( m_bIsUseLogFile )
	{
		FKCW_Base_Utils::FKFileLog( szShowBuf );
	}

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	if (hConsole != INVALID_HANDLE_VALUE)
	{
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
#endif
}
//-------------------------------------------------------------------------
// 添加错误Log
void FKDebugSystem::LogError( const char* pszFormat, ... )
{
	char szBuf[MAX_LOG_LEN - 20] = {0};
	char szShowBuf[MAX_LOG_LEN] = { 0 };

	struct tm *tm;  
	time_t timep;  
	time(&timep);  
	tm = localtime(&timep);  
	int hour=tm->tm_hour;  
	int minute=tm->tm_min;  
	int second=tm->tm_sec;
	char szTime[10] = { 0 };
	sprintf( szTime, "%02d:%02d:%02d",  hour, minute, second );


#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	va_list ap;
	va_start(ap, pszFormat);
	vsnprintf_s(szBuf, MAX_LOG_LEN - 20, MAX_LOG_LEN - 20, pszFormat, ap);
	va_end(ap);

	sprintf( szShowBuf, "%s - %s [L]%d - %s", szTime, __FILE__, __LINE__, szBuf );
	OutputDebugStringA(szShowBuf);
	OutputDebugStringA("\n");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole != INVALID_HANDLE_VALUE)
	{
		SetConsoleTextAttribute(hConsole, eConsolBKColor_Yellow | eConsolTextColor_Red );
	}
	puts(szShowBuf);
#else
	va_list ap;
	va_start(ap, pszFormat);
	vsnprintf(szBuf, MAX_LOG_LEN - 20,pszFormat, ap);// MAX_LOG_LEN - 20, 
	va_end(ap);
	sprintf( szShowBuf, "%s - %s", szTime, szBuf );
	CCLOG(szBuf);
#endif
	if( m_bIsUseLogFile )
	{
		FKCW_Base_Utils::FKFileLog( szShowBuf );
	}
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	if (hConsole != INVALID_HANDLE_VALUE)
	{
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
#endif
}
//-------------------------------------------------------------------------