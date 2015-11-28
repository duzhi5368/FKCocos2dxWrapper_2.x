#include "NetworkServer.h"

NetworkServer::NetworkServer()
{
	onMessageReceivedFunc = NULL;
	onConnectedFunc = NULL;
	onDisconnectedFunc = NULL;
	onConnectedEndedFunc = NULL;
	onExceptionCaughtFunc = NULL;
}

void NetworkServer::onMessageReceived(FKCW_SimpleNet_Buffer& oBuffer )
{
	if(onMessageReceivedFunc != NULL)
		(onMessageReceivedObject->*onMessageReceivedFunc)(this, oBuffer);
}

void NetworkServer::onConnected()
{
	if(onConnectedFunc != NULL)
		(onConnectedObject->*onConnectedFunc)();
}

void NetworkServer::onDisconnected()
{
	if(onDisconnectedFunc != NULL)
		(onDisconnectedObject->*onDisconnectedFunc)();
}

void NetworkServer::onConnectedEnded()
{
	if(onConnectedEndedFunc!=NULL)
		(onConnectedEndedObject->*onConnectedEndedFunc)();
}

void NetworkServer::onExceptionCaught()
{
	if(onExceptionCaughtFunc!=NULL)
		(onExceptionCaughtObject->*onExceptionCaughtFunc)();
}

void NetworkServer::registerOnMessageReceived(CCObject* object,  NetServerBackCallReceivedFunc func )
{
	onMessageReceivedFunc = func;
	onMessageReceivedObject = object;
}

void NetworkServer::registerOnConnected(CCObject* object,  NetServerBackCallFunc func )
{
	onConnectedFunc = func;
	onConnectedObject = object;
}

void NetworkServer::registerOnDisconnected( CCObject* object, NetServerBackCallFunc func )
{
	onDisconnectedFunc = func;
	onDisconnectedObject = object;
}

void NetworkServer::registerOnConnectedEnded( CCObject* object, NetServerBackCallFunc func )
{
	onConnectedEndedFunc = func;
	onConnectedEndedObject = object;
}

void NetworkServer::registerOnExceptionCaught( CCObject* object, NetServerBackCallFunc func )
{
	onExceptionCaughtFunc = func;
	onExceptionCaughtObject = object;
}