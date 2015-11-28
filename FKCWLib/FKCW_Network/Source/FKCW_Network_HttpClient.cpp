//--------------------------------------------------------------------
#include "../Include/FKCW_Network_HttpClient.h"
#include <queue>
#include <errno.h>
#include "curl/curl.h"
#include <pthread.h>
#include "../../FKCW_Base/Include/FKCW_Base_Data.h"
//--------------------------------------------------------------------
struct SHttpContext
{
	FKCW_Network_HttpClient*		client;
	FKCW_Network_HttpRequest*		request;
	FKCW_Network_HttpResponse*		response;
	float							connectTimeout;
	float							readTimeout;
};
//--------------------------------------------------------------------
// 实际执行一个Http操作
class FKCW_Network_UrlHandler : public CCObject
{
public:
	CURL*			m_pCurl;							// CURL实例
	curl_slist*		m_pHeaders;							// 保存的自定义 头数据
	char			m_szErrorBuffer[CURL_ERROR_SIZE];	// 错误信息缓冲
	SHttpContext*	m_pCtx;								// Http上下文数据
	int32_t			m_nResponseCode;					// 应答码
	pthread_mutex_t m_pMutex;							// 互斥锁
	bool			m_bIsDone;							// 是否完成的标志
	FKCW_Base_Data* m_pData;							// 未释放数据
	bool			m_bIsHttpDidReceiveResponseDelivered;	// NotificationHttpDidReceiveResponse 是否递送
	bool			m_bIsHeaderAllReceived;				// 是否全部头接收完毕
public:
	FKCW_Network_UrlHandler()
		: m_pCtx( NULL )
		, m_pCurl( NULL )
		, m_bIsDone( false )
		, m_nResponseCode( 500 )
		, m_bIsHttpDidReceiveResponseDelivered( false )
		, m_bIsHeaderAllReceived( false )
		, m_pHeaders( NULL )
	{
		m_pData = new FKCW_Base_Data();
		CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(FKCW_Network_UrlHandler::DispatchNotification), this, 0, kCCRepeatForever, 0, false);
	}

	virtual ~FKCW_Network_UrlHandler()
	{
		if(m_pCurl)
			curl_easy_cleanup(m_pCurl);
		if(m_pHeaders)
			curl_slist_free_all(m_pHeaders);
		if(m_pCtx->client) {
			m_pCtx->client->RemoveContext(m_pCtx);
		}
		CC_SAFE_RELEASE(m_pCtx->request);
		CC_SAFE_RELEASE(m_pCtx->response);
		CC_SAFE_FREE(m_pCtx);
		CC_SAFE_RELEASE(m_pData);
		pthread_mutex_destroy(&m_pMutex);
	}

	// libcul收到响应数据的回调函数
	static size_t WriteData(void* ptr, size_t size, size_t nmemb, void* userdata)
	{
		FKCW_Network_UrlHandler* handler = (FKCW_Network_UrlHandler*)userdata;
		size_t sizes = size * nmemb;

		// 在接收缓冲最后添加一份数据
		pthread_mutex_lock(&handler->m_pMutex);
		handler->m_pData->AppendBytes((uint8_t*)ptr, sizes);
		handler->m_bIsHeaderAllReceived = true;
		pthread_mutex_unlock(&handler->m_pMutex);

		// 返回一个值，如果该值与接收大小不同，则终止
		if(handler->m_pCtx->request->isCancel())
			return sizes + 1;
		else
			return sizes;
	}

	// libcul收到头数据的回调函数
	static size_t WriteHeaderData(void* ptr, size_t size, size_t nmemb, void* userdata)
	{
		FKCW_Network_UrlHandler* handler = (FKCW_Network_UrlHandler*)userdata;
		size_t sizes = size * nmemb;

		// 锁
		pthread_mutex_lock(&handler->m_pMutex);

		// 开始解析
		string header((const char*)ptr, sizes);
		CCArray* pair = new CCArray();
		if(!header.empty()) 
		{
			// 移除头部尾部空格，以及括号
			size_t start = 0;
			size_t end = header.length() - 1;
			char c = header[start];
			while(c == '{' || c == '[' || c == '(') 
			{
				start++;
				c = header[start];
			}
			c = header[end];
			while(c == '}' || c == ']' || c == ')') 
			{
				end--;
				c = header[end];
			}

			// 迭代处理字符串
			size_t compStart = start;
			for(size_t i = start; i <= end; i++) 
			{
				c = header[i];
				if(c == ':') 
				{
					CCString* s = new CCString(header.substr(compStart, i - compStart));
					pair->addObject(s);
					s->release();
					compStart = i + 1;
				} 
				else if(c == ' ' || c == '\t' || c == '\r' || c == '\n') 
				{
					if(compStart == i) 
					{
						compStart++;
					}
				}
			}

			// 如果最后一个字符是分隔符，补充空字符串补偿
			if(compStart <= end)
			{
				CCString* s = new CCString(header.substr(compStart, end - compStart + 1));
				pair->addObject(s);
				s->release();
			} 
			else if(header[end] == ':') 
			{
				CCString* s = new CCString("");
				pair->addObject(s);
				s->release();
			}
		}

		// 如果个数为2，则正常
		if(pair->count() == 2) 
		{
			handler->m_pCtx->response->AddHeader(((CCString*)pair->objectAtIndex(0))->getCString(),
				((CCString*)pair->objectAtIndex(1))->getCString());
		}

		pair->release();

		// 解锁
		pthread_mutex_unlock(&handler->m_pMutex);

		// 返回一个值，若该值与接收大小不同，则终止
		if(handler->m_pCtx->request->isCancel())
			return sizes + 1;
		else
			return sizes;
	}

	bool InitWithContext(SHttpContext* ctx) 
	{
		// 保存上下文
		m_pCtx = ctx;

		// 检查
		m_pCurl = curl_easy_init();
		if (!m_pCurl)
			return false;
		if (!ConfigureCURL())
			return false;

		// 创建互斥锁
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
		pthread_mutex_init(&m_pMutex, &attr);
		pthread_mutexattr_destroy(&attr);

		// 获取自定义头数据
		const vector<string>& headers = ctx->request->getHeaders();
		if(!headers.empty()) 
		{
			// 逐个添加自定义头
			for (vector<string>::const_iterator it = headers.begin(); it != headers.end(); ++it)
				m_pHeaders = curl_slist_append(m_pHeaders, it->c_str());

			// 为curl设置自定义头
			if (curl_easy_setopt(m_pCurl, CURLOPT_HTTPHEADER, m_pHeaders) != CURLE_OK)
				return false;
		}

		return curl_easy_setopt(m_pCurl, CURLOPT_URL, ctx->request->getUrl().c_str()) == CURLE_OK &&
			curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, WriteData) == CURLE_OK &&
			curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, this) == CURLE_OK &&
			curl_easy_setopt(m_pCurl, CURLOPT_HEADERFUNCTION, WriteHeaderData) == CURLE_OK &&
			curl_easy_setopt(m_pCurl, CURLOPT_HEADERDATA, this) == CURLE_OK;
	}

	bool ConfigureCURL()
	{
		if(curl_easy_setopt(m_pCurl, CURLOPT_ERRORBUFFER, m_szErrorBuffer) != CURLE_OK) 
		{
			return false;
		}
		if(curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, m_pCtx->readTimeout) != CURLE_OK) 
		{
			return false;
		}
		if(curl_easy_setopt(m_pCurl, CURLOPT_CONNECTTIMEOUT, m_pCtx->connectTimeout) != CURLE_OK) 
		{
			return false;
		}
		curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYHOST, 0L);
		// 如果超时，该子线程将中断主线程
		curl_easy_setopt(m_pCurl, CURLOPT_NOSIGNAL, 1L);

		return true;
	}

	bool ProcessGetTask() 
	{
		curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, true);
		return Perform();
	}

	bool ProcessPostTask() 
	{
		curl_easy_setopt(m_pCurl, CURLOPT_POST, 1);
		FKCW_Base_Data* data = m_pCtx->request->getRequestData();
		if(data) 
		{
			curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDS, data->getBytes());
			curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDSIZE, data->getSize());
		}
		return Perform();
	}

	bool ProcessPutTask() 
	{
		curl_easy_setopt(m_pCurl, CURLOPT_CUSTOMREQUEST, "PUT");
		FKCW_Base_Data* data = m_pCtx->request->getRequestData();
		if(data) {
			curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDS, data->getBytes());
			curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDSIZE, data->getSize());
		}

		return Perform();
	}

	bool ProcessDeleteTask() 
	{
		curl_easy_setopt(m_pCurl, CURLOPT_CUSTOMREQUEST, "DELETE");
		curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, true);
		return Perform();
	}

	bool Perform() 
	{
		CURLcode code = curl_easy_perform(m_pCurl);
		if (code != CURLE_OK)
		{
			CCLOG("curl_easy_perform error: %d, request: %s", code, m_pCtx->request->getUrl().c_str());
			return false;
		}

		// 获取返回码
		code = curl_easy_getinfo(m_pCurl, CURLINFO_RESPONSE_CODE, &m_nResponseCode);
		if (code != CURLE_OK || m_nResponseCode != 200)
			return false;

		return true;
	}
	
	// 请求结束时调用
	void OnRequestDone() 
	{
		pthread_mutex_lock(&m_pMutex);
		m_bIsDone = true;
		pthread_mutex_unlock(&m_pMutex);
	}

	// 消息分发
	void DispatchNotification(float delta) 
	{
		// 消息中心
		CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
		pthread_mutex_lock(&m_pMutex);

		// 头消息通知
		if(!m_bIsHttpDidReceiveResponseDelivered && m_bIsHeaderAllReceived)
		{
			m_bIsHttpDidReceiveResponseDelivered = true;
			nc->postNotification( NotificationHttpDidReceiveResponse, m_pCtx->response);
		}

		// 通知
		if(m_pData->getSize() > 0) 
		{
			m_pCtx->response->setData(m_pData);
			nc->postNotification( NotificationHttpDataReceived, m_pCtx->response);

			// 重建一个新数据对象
			CC_SAFE_RELEASE(m_pData);
			m_pData = new FKCW_Base_Data();
		}

		// 是否结束
		if(m_bIsDone) 
		{
			// 通知
			nc->postNotification( NotificationHttpRequestCompleted, m_pCtx->response);
			// 在HttpThreadEntry做了retain,所以这里要release();
			autorelease();
			// 自我卸载
			CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(FKCW_Network_UrlHandler::DispatchNotification), this);
		}

		pthread_mutex_unlock(&m_pMutex);
	}
};
//--------------------------------------------------------------------
// FKCW_Network_HttpClient
//--------------------------------------------------------------------
// 线程入口
void* FKCW_Network_HttpClient::__HttpThreadEntry( void* p_pArg )
{
	// url处理器，当初始化失败或者在自身的 DispatchNotification() 函数中被释放，请不要手动释放
	SHttpContext* ctx = (SHttpContext*)p_pArg;
	FKCW_Network_UrlHandler* curl = new FKCW_Network_UrlHandler();
	if(!curl->InitWithContext(ctx)) 
	{
		CC_SAFE_RELEASE_NULL(curl);
	}

	if(curl) 
	{
		// 创建一个应答
		FKCW_Network_HttpResponse* response = new FKCW_Network_HttpResponse(ctx->request);
		ctx->response = response;

		// 处理请求
		bool retValue = false;
		switch (ctx->request->getMethod()) {
		case FKCW_Network_HttpRequest::eHttpMethod_Get:
			retValue = curl->ProcessGetTask();
			break;
		case FKCW_Network_HttpRequest::eHttpMethod_Post:
			retValue = curl->ProcessPostTask();
			break;
		case FKCW_Network_HttpRequest::eHttpMethod_Put:
			retValue = curl->ProcessPutTask();
			break;
		case FKCW_Network_HttpRequest::eHttpMethod_Delete:
			retValue = curl->ProcessDeleteTask();
			break;
		default:
			CCAssert(true, "CBHttpClient: unkown request type, only GET and POST are supported");
			break;
		}

		// 保存应答
		response->setResponseCode(curl->m_nResponseCode);
		if (retValue) 
		{
			response->setSuccess(true);
		} else 
		{
			response->setSuccess(false);
			response->SetErrorData(curl->m_szErrorBuffer);
		}

		// 完成
		curl->OnRequestDone();
	}

	// 退出
	pthread_exit(NULL);

	return NULL;
}
//--------------------------------------------------------------------
FKCW_Network_HttpClient::FKCW_Network_HttpClient()
	: m_fConnectTimeout( 30.0f )
	, m_fReadTimeout( 60.0f )
{
	// 创建互斥锁
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&m_Mutex, &attr);
	pthread_mutexattr_destroy(&attr);
}
//--------------------------------------------------------------------
FKCW_Network_HttpClient::~FKCW_Network_HttpClient()
{
	pthread_mutex_lock(&m_Mutex);
	for(vector<void*>::iterator iter = m_vecActiveContexts.begin(); iter != m_vecActiveContexts.end(); iter++)
	{
		SHttpContext* ctx = (SHttpContext*)*iter;
		ctx->client = NULL;
	}
	pthread_mutex_unlock(&m_Mutex);
	pthread_mutex_destroy(&m_Mutex);
}
//--------------------------------------------------------------------
FKCW_Network_HttpClient* FKCW_Network_HttpClient::Create()
{
	FKCW_Network_HttpClient* c = new FKCW_Network_HttpClient();
	if(c->Init()) 
	{
		return (FKCW_Network_HttpClient*)c->autorelease();
	}
	CC_SAFE_RELEASE(c);
	return NULL;
}
//--------------------------------------------------------------------
// 初始化
bool FKCW_Network_HttpClient::Init()
{
	return true;
}
//--------------------------------------------------------------------
// 执行一个Http请求，该请求将在一个独立子线程中执行
// 参数：p_pRequest Http请求对象。包括请求url，响应回调等都在里面
// 注：请保证在执行本函数之前，参数中的 p_pRequest->m_pRequestData 已被清空
void FKCW_Network_HttpClient::AsyncExecute( FKCW_Network_HttpRequest* p_pRequest )
{
	if (!p_pRequest)
	{
		return;
	}

	// 创建上下文
	SHttpContext* ctx = (SHttpContext*)calloc(1, sizeof(SHttpContext));
	ctx->request = p_pRequest;
	ctx->connectTimeout = m_fConnectTimeout;
	ctx->readTimeout = m_fReadTimeout;
	ctx->client = this;
	CC_SAFE_RETAIN(p_pRequest);

	// 添加到cache
	pthread_mutex_lock(&m_Mutex);
	m_vecActiveContexts.push_back(ctx);
	pthread_mutex_unlock(&m_Mutex);

	// 开启网络线程
	pthread_t thread;
	pthread_create(&thread, NULL, __HttpThreadEntry, ctx);
	pthread_detach(thread);
}
//--------------------------------------------------------------------
// 取消一个请求
void FKCW_Network_HttpClient::Cancel( int p_nTag )
{
	pthread_mutex_lock(&m_Mutex);
	for(vector<void*>::iterator iter = m_vecActiveContexts.begin(); iter != m_vecActiveContexts.end(); iter++)
	{
		SHttpContext* ctx = (SHttpContext*)*iter;
		if(ctx->request->getTag() == p_nTag) 
		{
			ctx->request->setCancel(true);
			break;
		}
	}
	pthread_mutex_unlock(&m_Mutex);
}
//--------------------------------------------------------------------
// 取消全部http操作
void FKCW_Network_HttpClient::CancelAll()
{
	pthread_mutex_lock(&m_Mutex);
	for(vector<void*>::iterator iter = m_vecActiveContexts.begin(); iter != m_vecActiveContexts.end(); iter++) 
	{
		SHttpContext* ctx = (SHttpContext*)*iter;
		ctx->request->setCancel(true);
	}
	pthread_mutex_unlock(&m_Mutex);
}
//--------------------------------------------------------------------
// 移除线程上下文
void FKCW_Network_HttpClient::RemoveContext( void* p_pContext )
{
	pthread_mutex_lock(&m_Mutex);
	for(vector<void*>::iterator iter = m_vecActiveContexts.begin(); iter != m_vecActiveContexts.end(); iter++) 
	{
		if(p_pContext == *iter)
		{
			m_vecActiveContexts.erase(iter);
			break;
		}
	}
	pthread_mutex_unlock(&m_Mutex);
}
//--------------------------------------------------------------------