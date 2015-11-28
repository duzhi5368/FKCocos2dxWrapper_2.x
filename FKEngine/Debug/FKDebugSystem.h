//*************************************************************************
//	创建日期:	2015-1-16
//	文件名称:	FKDebugSystem.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../FKEngineCommonHead.h"
//-------------------------------------------------------------------------
class FKDebugSystem : public IDebugSystem
{
public:
	FKDebugSystem();
	~FKDebugSystem();
public:
	// 销毁释放
	virtual void	Release();
	// 关闭全部Debug行为
	virtual void	CloseDebugSystem();

	// 显示/隐藏游戏中控制台面板
	virtual bool	EnableGameConsole( bool bIsEnable );
	// 开启/关闭 Log 日志记录
	virtual bool	EnableLogFile( bool bIsEnable );
	// 开启/关闭 WinConsole
	virtual bool	EnableWinConsole( bool bIsEnable );
	// 开启/关闭 FPS 和渲染批次显示
	virtual bool	EnableFPS( bool bIsEnable );

	// 输出游戏中内存占用状况
	virtual void	DumpMemoryUse();
	// 输出游戏中内存泄露状况
	virtual void	DumpMemoryLeak();
	// 输出游戏中Node信息
	virtual void	DumpNodeInfo();
	// 输出游戏中Touch信息
	virtual void	DumpTouchInfo();
	// 输出游戏中纹理使用信息
	virtual void	DumpTextureInfo();
	// 输出硬件状况信息
	virtual void	DumpConfigurationInfo();
	// 输出全部Dump
	virtual void	DumpAll();

	// 添加普通调试性Log
	virtual void	LogDebug( const char* pszFormat, ... );
	// 添加信息式Log
	virtual void	LogInfo( const char* pszFormat, ... );
	// 添加错误Log
	virtual void	LogError( const char* pszFormat, ... );
private:
	bool			m_bIsUseLogFile;		// 是否使用LogFile
};
//-------------------------------------------------------------------------
extern FKDebugSystem gs_FKDebugSystem;
//-------------------------------------------------------------------------