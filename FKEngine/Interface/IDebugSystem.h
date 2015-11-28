//*************************************************************************
//	创建日期:	2015-1-16
//	文件名称:	IDebugSystem.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#define FKDEBUG( format, ... )	GetDebugSystem()->LogDebug(format, ##__VA_ARGS__)
#define FKINFO( format, ... )	GetDebugSystem()->LogInfo(format, ##__VA_ARGS__)
#define FKERROR( format, ... )	GetDebugSystem()->LogError(format, ##__VA_ARGS__)
//-------------------------------------------------------------------------
// Debug管理器
class IDebugSystem
{
public:
	// 销毁释放
	virtual void	Release() = 0;
	// 关闭全部Debug行为
	virtual void	CloseDebugSystem() = 0;

	// 显示/隐藏游戏中控制台面板
	virtual bool	EnableGameConsole( bool bIsEnable ) = 0;
	// 开启/关闭 Log 日志记录
	virtual bool	EnableLogFile( bool bIsEnable ) = 0;
	// 开启/关闭 WinConsole
	virtual bool	EnableWinConsole( bool bIsEnable ) = 0;
	// 开启/关闭 FPS 和渲染批次显示
	virtual bool	EnableFPS( bool bIsEnable ) = 0;
	
	// 输出游戏中内存占用状况
	virtual void	DumpMemoryUse() = 0;
	// 输出游戏中内存泄露状况
	virtual void	DumpMemoryLeak() = 0;
	// 输出游戏中Node信息
	virtual void	DumpNodeInfo() = 0;
	// 输出游戏中Touch信息
	virtual void	DumpTouchInfo() = 0;
	// 输出游戏中纹理使用信息
	virtual void	DumpTextureInfo() = 0;
	// 输出全部Dump
	virtual void	DumpAll() = 0;

	// 添加普通调试性Log
	virtual void	LogDebug( const char* pszFormat, ... ) = 0;
	// 添加信息式Log
	virtual void	LogInfo( const char* pszFormat, ... ) = 0;
	// 添加错误Log
	virtual void	LogError( const char* pszFormat, ... ) = 0;
};
//-------------------------------------------------------------------------
// 获取调试管理器指针
extern IDebugSystem* GetDebugSystem();
//-------------------------------------------------------------------------