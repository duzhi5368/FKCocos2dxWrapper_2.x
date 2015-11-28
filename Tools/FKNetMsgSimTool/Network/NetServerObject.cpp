#include "NetServerObject.h"

NetServerObject::NetServerObject()
{
	init();
}

bool NetServerObject::init()
{
	m_isConnect = false;
	m_isClose = false;

	WSADATA wsad;
	WORD wVersionRequested; 
	wVersionRequested = MAKEWORD( 2, 0 );  
	if (WSAStartup(wVersionRequested , &wsad)!=0) 
	{
		return false;
	}
	return true;
}
//设置监听端口
void NetServerObject::setListenPost( short post )
{
	m_post = post;
}

void NetServerObject::setListenIp(const char* ip)
{
	m_ip = ip;
}

void NetServerObject::close()
{
	::closesocket(s_sockConn);		//关闭当前连接
	::closesocket(s_serverSocket);	//关闭服务器连接
	m_isClose = true;
}

void NetServerObject::send( FKCW_SimpleNet_Buffer* pBuffer )
{
	if(s_sockConn != NULL)
		::send(s_sockConn, pBuffer->data(), pBuffer->length(), 0);
	else
		FKLOG("m_sockConn is null");
}

DWORD WINAPI threadFunc( LPVOID pM )
{
	NetServerParams params = *(NetServerParams*)pM;
	delete pM;

	do
	{
		bool isContinue = true;
	
		//构造服务器SOCKET
		NetServerObject::s_serverSocket = socket(AF_INET, SOCK_STREAM, 0);

		//配置监听地址和端口
		SOCKADDR_IN serverAddr;
		ZeroMemory((char *)&serverAddr,sizeof(serverAddr));
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(params.post);					//本地监听端口
		serverAddr.sin_addr.S_un.S_addr = inet_addr(params.ip.c_str());
		//serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		
		//绑定监听SOCKET//
		::bind(NetServerObject::s_serverSocket,(struct sockaddr *)&serverAddr, sizeof(serverAddr));

		int error = listen(NetServerObject::s_serverSocket, 1);
		if (error != SOCKET_ERROR)			//监听SCOKET不出错
		{
			SOCKADDR_IN addrClient;
			int len = sizeof(SOCKADDR);

			//开始阻塞监听 
			NetServerObject::s_sockConn = accept(NetServerObject::s_serverSocket, (SOCKADDR*)&addrClient, &len);

			params.netServerObject->onConnected();	//连接成功回调

			while (isContinue)
			{
				char recvBuf[4096];
				int erro = recv(NetServerObject::s_sockConn, recvBuf, 4096, 0);	//等待客户端消息
				if (erro < 0)
				{
					params.netServerObject->onExceptionCaught();	//连接出错回调
					isContinue = false;
					break;
				}
				else if(erro == 0)
				{
					params.netServerObject->onDisconnected();	//连接关闭
					isContinue = false;
					break;
				}
				else
				{
					FKCW_SimpleNet_Buffer buffer(recvBuf, 4096);
					params.netServerObject->onMessageReceived(buffer);	//得到消息回调
				}
			}
		}
		else
			MessageBox(NULL, L"服务端端口被占用/IP错误", L"", 1);

		params.netServerObject->onConnectedEnded();	//连接结束回调
		
		closesocket(NetServerObject::s_sockConn);
		closesocket(NetServerObject::s_serverSocket);

	} while (!NetServerObject::m_isClose);

	WSACleanup();	//清除WSAD

	params.netServerObject->setConnected(true);	//设置连接完成标识

	delete params.netServerObject;
	params.netServerObject = NULL;

	return 0;
}

void NetServerObject::listen()
{
	NetServerParams* params = new NetServerParams;
	params->post = m_post;
	params->netServerObject = this;
	params->ip = m_ip;

	HANDLE buf = CreateThread(NULL, 0, threadFunc, params, 0, NULL);
}

void NetServerObject::setConnected( bool isConnect )
{
	m_isConnect = isConnect;
}

bool NetServerObject::isConnected()
{
	return m_isConnect;
}

NetServerObject::~NetServerObject()
{
}

bool NetServerObject::m_isClose;

SOCKET NetServerObject::s_serverSocket;

bool NetServerObject::m_isConnect = false;

SOCKET NetServerObject::s_sockConn;
