/********************************************************************
	created:	2014/11/01
	file base:	FKCW_Network_HttpClient
	author:		FreeKnight
	
	purpose:	
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_Network_Macros.h"
#include "FKCW_Network_HttpRequest.h"
#include "FKCW_Network_HttpResponse.h"
//--------------------------------------------------------------------
// 对象是一个Http应答，你需要检查其成功标示
#define NotificationHttpRequestCompleted	"NotificationHttpRequestCompleted"
// 对象是一个Http应答，你可以从中获取数据片段
#define NotificationHttpDataReceived		"NotificationHttpDataReceived"
// 当头部在实际数据之前先接收到时。这是一个Http应答，你可以从中获得头
#define NotificationHttpDidReceiveResponse	"NotificationHttpDidReceiveResponse"
//--------------------------------------------------------------------
/*
	这个与cocos2dx扩展库中的CCHttpClient不同点有：
	1：可以在操作过程中进行回调，而不仅仅在结束时有回调
	2：请求允许被取消。
	3：不再是单键
	说明：
	使用者无需保存一个httpClient实例。Http请求将会在一个线程中执行，
	所以你保存一个httpClient实例，或是retain它都没有什么意义。
*/
//--------------------------------------------------------------------
class FKCW_Network_HttpClient : public CCObject
{
private:
	vector<void*>			m_vecActiveContexts;		// 保存线程表，key是标签
	pthread_mutex_t			m_Mutex;					// 线程互斥量
	CC_SYNTHESIZE(float, m_fConnectTimeout, ConnectTimeout);	// 链接超时时间
	CC_SYNTHESIZE(float, m_fReadTimeout, ReadTimeout);			// 读取超时时间
private:
	// 线程入口
	static void* __HttpThreadEntry( void* p_pArg );
protected:
	FKCW_Network_HttpClient();
public:
	virtual ~FKCW_Network_HttpClient();
	static FKCW_Network_HttpClient* Create();

	// 初始化
	virtual bool	Init();
	// 执行一个Http请求，该请求将在一个独立子线程中执行
	// 参数：p_pRequest Http请求对象。包括请求url，响应回调等都在里面
	// 注：请保证在执行本函数之前，参数中的 p_pRequest->m_pRequestData 已被清空
	void			AsyncExecute( FKCW_Network_HttpRequest* p_pRequest );
	// 取消一个请求
	void			Cancel( int p_nTag );
	// 取消全部http操作
	void			CancelAll();
	// 移除线程上下文
	void			RemoveContext( void* p_pContext );
};