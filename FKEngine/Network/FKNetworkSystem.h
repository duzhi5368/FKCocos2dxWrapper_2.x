//*************************************************************************
//	创建日期:	2015-1-10
//	文件名称:	FKNetworkSystem.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../FKEngineCommonHead.h"
#include "../Common/TSwitchList.h"
#include "FKNetworkCommand.h"
//-------------------------------------------------------------------------
class FKNetworkSystem : public INetworkSystem
{
public:
	FKNetworkSystem();
	~FKNetworkSystem();
public:
	// 清理网络系统
	virtual void			Release();
	// 创建客户端连接socket
	virtual INetworkSocket*	CreateSocket( INetworkMsgHandler* pMsgHandler );
	// 清理客户端指定连接
	virtual void			ClearSocketByID( int nSocketID );
	// 设置指定连接的回调
	virtual void			SetMsgHandler( int nSocketID, INetworkMsgHandler* pMsgHandler );
	// 逻辑更新
	virtual void			UpdateEvent();
public:
	// 连接成功处理
	void	PushConnectSuccessMsg( unsigned int unIndex, INetworkSocket* pClientSocket );
	// 连接错误处理
	void	PushConnectErrorMsg( unsigned int unIndex, int nError, const char* lpErrorMsg );
	// 接收数据处理
	void	PushRecvMsg( unsigned int unIndex, const char* szBuf, int nLen );
	// 连接关闭处理
	void	PushCloseMsg( unsigned int unIndex );
private:
	void	__HandleConnectSuccess( unsigned int unIndex, INetworkSocket* pClientSocket );
	void	__HandleConnectErrorMsg( unsigned int unIndex, int nError, const char* lpErrorMsg );
	void	__HandleRecvMsg( unsigned int unIndex, const char* szBuf, int nLen );
	void	__HandleCloseMsg( unsigned int unIndex );
private:
	unsigned int					m_unCurSocketID;		// 分配给socket的序号
	vector<INetworkMsgHandler*>		m_vecMsgHandlers;		// 消息回调处理列表
	TSwitchList<FKNetworkCommand>	m_listCommandList;		// 网络状态消息队列
};