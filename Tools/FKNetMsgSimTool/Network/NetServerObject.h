#ifndef _NET_SERVER_OBJECT_H_
#define _NET_SERVER_OBJECT_H_
//-------------------------------------------------------------------------
#include "FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
class NetServerObject;
//-------------------------------------------------------------------------
struct NetServerParams
{
	NetServerObject* netServerObject;
	short post;
	string ip;
};
//-------------------------------------------------------------------------
DWORD WINAPI threadFunc(LPVOID pM);
//-------------------------------------------------------------------------
//注意 close方法会关闭当前连接、当前监听、delete本类的实例
//要销毁NetServerObject极其子类，close掉即可
class NetServerObject
{
public:
	NetServerObject();
	virtual ~NetServerObject();
public:
	// 收到消息包的回调
	virtual void onMessageReceived(FKCW_SimpleNet_Buffer& oBuffer) = 0;
	// 连接成功时的回调
	virtual void onConnected() = 0;
	// 连接关闭时的回调
	virtual void onDisconnected() = 0;
	// 连接结束时的回调
	virtual void onConnectedEnded() = 0;
	// 连接出错的回调
	virtual void onExceptionCaught() = 0;

	// 设置连接状态
	static void setConnected(bool isConnect);
	// 得到连接状态
	static bool isConnected();

	static SOCKET s_sockConn;			// 当前连接的Socket
	static SOCKET s_serverSocket;		// 服务器socket
	static bool m_isClose;				// 是否已经手动调用关闭连接
public:
	// 初始化
	virtual bool init();
	// 设置监听IP
	virtual void setListenPost(short post);
	// 设置监听IP
	virtual void setListenIp(const char* ip);
	// 连接目标地址
	virtual void listen();
	// 关闭当前连接
	virtual void close();
	// 发送消息包给目标
	virtual void send(FKCW_SimpleNet_Buffer* pBuffer);
protected:
	static bool m_isConnect;		//当前是否完成连接
	short		m_post;
	string		m_ip;
	SOCKET		m_ServerSocket;		//服务端Socket
};
//-------------------------------------------------------------------------
#endif