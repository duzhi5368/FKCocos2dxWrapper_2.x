#ifndef _NETWORK_CLIENT_H_
#define _NETWORK_CLIENT_H_

//-------------------------------------------------------------------------
#include "FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------

class NetworkClient;

typedef void (CCObject::*NetClientBackCallFunc)();
#define netclient_backcall_selector(_SELECTOR_) (NetClientBackCallFunc)(&_SELECTOR_)

typedef void (CCObject::*NetClientBackCallReceivedFunc)(NetworkClient* client, FKCW_SimpleNet_Buffer& oBuffer);
#define netclient_backcall_received_selector(_SELECTOR_) (NetClientBackCallReceivedFunc)(&_SELECTOR_)

class NetworkClient : public FKCW_SimpleNet_NetObject
{
public:
	SINGLE_DELEGATE_INSTANCE_FUNC(NetworkClient);
	NetworkClient()
	{
		onMessageReceivedFunc = NULL;
		onConnectedFunc = NULL;
		onDisconnectedFunc = NULL;
		onConnectedEndedFunc = NULL;
		onExceptionCaughtFunc = NULL;
		onConnectTimeoutFunc = NULL;
	}
	~NetworkClient()
	{
	}
public:
	// 收到消息包的回调
	virtual void onMessageReceived( FKCW_SimpleNet_Buffer& oBuffer );
	// 连接成功时的回调
	virtual void onConnected();
	// 连接超时时的回调
	virtual void onConnectTimeout();
	// 连接失败时的回调
	virtual void onDisconnected();
	// 出错时的回调
	virtual void onExceptionCaught( ENUM_SocketStatus eStatus );
public:
	void registerOnMessageReceived(CCObject* object, NetClientBackCallReceivedFunc func);
	void registerOnConnected(CCObject* object, NetClientBackCallFunc func);
	void registerOnDisconnected(CCObject* object, NetClientBackCallFunc func);
	void registerOnConnectedEnded(CCObject* object, NetClientBackCallFunc func);
	void registerOnExceptionCaught(CCObject* object, NetClientBackCallFunc func);
	void registerOnConnectTimeout(CCObject* object, NetClientBackCallFunc func);
private:
	NetClientBackCallReceivedFunc onMessageReceivedFunc;
	CCObject* onMessageReceivedObject;
	NetClientBackCallFunc onConnectedFunc;
	CCObject* onConnectedObject;
	NetClientBackCallFunc onDisconnectedFunc;
	CCObject* onDisconnectedObject;
	NetClientBackCallFunc onConnectedEndedFunc;
	CCObject* onConnectedEndedObject;
	NetClientBackCallFunc onExceptionCaughtFunc;
	CCObject* onExceptionCaughtObject;
	NetClientBackCallFunc onConnectTimeoutFunc;
	CCObject* onConnectTimeoutObject;
};

#endif