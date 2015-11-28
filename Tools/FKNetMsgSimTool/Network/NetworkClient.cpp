#include "NetworkClient.h"

USING_NS_CC;

// 收到消息包的回调
void NetworkClient::onMessageReceived( FKCW_SimpleNet_Buffer& oBuffer )
{
	if (onMessageReceivedFunc != NULL)
	{
		(onMessageReceivedObject->*onMessageReceivedFunc)(this, oBuffer);
	}
}
// 连接成功时的回调
void NetworkClient::onConnected()
{
	if(onConnectedFunc != NULL)
	{
		(onConnectedObject->*onConnectedFunc)();
	}
}
// 连接超时时的回调 todo
void NetworkClient::onConnectTimeout()
{
	if (onConnectTimeoutFunc != NULL)
	{
		(onConnectTimeoutObject->*onConnectTimeoutFunc)();
	}
}

// 连接失败时的回调
void NetworkClient::onDisconnected()
{
	if (onDisconnectedFunc != NULL)
	{
		(onDisconnectedObject->*onDisconnectedFunc)();
	}
}
// 出错时的回调
void NetworkClient::onExceptionCaught( ENUM_SocketStatus eStatus )
{
	if(onExceptionCaughtFunc != NULL)
	{
		(onExceptionCaughtObject->*onExceptionCaughtFunc)();
	}
}

void NetworkClient::registerOnMessageReceived( CCObject* object, NetClientBackCallReceivedFunc func )
{
	onMessageReceivedObject = object;
	onMessageReceivedFunc = func;
}

void NetworkClient::registerOnConnected( CCObject* object, NetClientBackCallFunc func )
{
	onConnectedObject = object;
	onConnectedFunc = func;
}

void NetworkClient::registerOnDisconnected( CCObject* object, NetClientBackCallFunc func )
{
	onDisconnectedObject = object;
	onDisconnectedFunc = func;
}

void NetworkClient::registerOnConnectedEnded( CCObject* object, NetClientBackCallFunc func )
{
	onConnectedEndedObject = object;
	onConnectedEndedFunc = func;
}

void NetworkClient::registerOnExceptionCaught( CCObject* object, NetClientBackCallFunc func )
{
	onExceptionCaughtObject = object;
	onExceptionCaughtFunc = func;
}

void NetworkClient::registerOnConnectTimeout( CCObject* object, NetClientBackCallFunc func )
{
	onConnectTimeoutObject = object;
	onConnectTimeoutFunc = func;
}
