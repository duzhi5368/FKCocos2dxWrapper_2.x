#ifndef _NETWORK_SERVER_EX_H_
#define _NETWORK_SERVER_EX_H_

#include "NetServerObject.h"
#include "FKCWLib/FKCocos2dxWrapper.h"

class NetworkServer;

typedef void (CCObject::*NetServerBackCallFunc)();
#define netserver_backcall_selector(_SELECTOR_) (NetServerBackCallFunc)(&_SELECTOR_)

typedef void (CCObject::*NetServerBackCallReceivedFunc)(NetworkServer* server, FKCW_SimpleNet_Buffer& oBuffer);
#define netserver_backcall_received_selector(_SELECTOR_) (NetServerBackCallReceivedFunc)(&_SELECTOR_)

class NetworkServer : public NetServerObject
{
public:
	NetworkServer();
	virtual ~NetworkServer(){}
public:
	virtual void onMessageReceived( FKCW_SimpleNet_Buffer& oBuffer );
	virtual void onConnected();
	virtual void onDisconnected();
	virtual void onConnectedEnded();
	virtual void onExceptionCaught();

	void registerOnMessageReceived(CCObject* object, NetServerBackCallReceivedFunc func);
	void registerOnConnected(CCObject* object, NetServerBackCallFunc func);
	void registerOnDisconnected(CCObject* object, NetServerBackCallFunc func);
	void registerOnConnectedEnded(CCObject* object, NetServerBackCallFunc func);
	void registerOnExceptionCaught(CCObject* object, NetServerBackCallFunc func);
private:
	NetServerBackCallReceivedFunc onMessageReceivedFunc;
	CCObject* onMessageReceivedObject;
	NetServerBackCallFunc onConnectedFunc;
	CCObject* onConnectedObject;
	NetServerBackCallFunc onDisconnectedFunc;
	CCObject* onDisconnectedObject;
	NetServerBackCallFunc onConnectedEndedFunc;
	CCObject* onConnectedEndedObject;
	NetServerBackCallFunc onExceptionCaughtFunc;
	CCObject* onExceptionCaughtObject;
};

#endif