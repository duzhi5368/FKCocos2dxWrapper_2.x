//--------------------------------------------------------------------
#include "../Include/FKCW_Network_FileDownLoader.h"
#include "../../FKCW_Base/Include/FKCW_Base_Utils.h"
//--------------------------------------------------------------------
// FKCW_Network_DownloadEntry
//--------------------------------------------------------------------
FKCW_Network_DownloadEntry::FKCW_Network_DownloadEntry()
	: m_unSize( 0 )
	, m_bAppend( false )
{

}
//--------------------------------------------------------------------
FKCW_Network_DownloadEntry::~FKCW_Network_DownloadEntry()
{

}
//--------------------------------------------------------------------
FKCW_Network_DownloadEntry*	FKCW_Network_DownloadEntry::Create()
{
	FKCW_Network_DownloadEntry* p = new FKCW_Network_DownloadEntry();
	return ( FKCW_Network_DownloadEntry* )p->autorelease();
}
//--------------------------------------------------------------------
// FKCW_Network_FileDownLoader
//--------------------------------------------------------------------
static FKCW_Network_FileDownLoader* s_Instance = NULL;
//--------------------------------------------------------------------
FKCW_Network_FileDownLoader::FKCW_Network_FileDownLoader()
	: m_bIsDownloading( false )
	, m_nTotalSize( 0 )
	, m_nTotalDownloadedSize( 0 )
	, m_pFOS( NULL )
{
	m_pClient = FKCW_Network_HttpClient::Create();
	CC_SAFE_RETAIN(m_pClient);

	// 监听器
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FKCW_Network_FileDownLoader::_OnHttpDone), NotificationHttpRequestCompleted, NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FKCW_Network_FileDownLoader::_OnHttpData), NotificationHttpDataReceived, NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FKCW_Network_FileDownLoader::_OnHttpHeaders), NotificationHttpDidReceiveResponse, NULL);
}
//--------------------------------------------------------------------
// 初始化
bool FKCW_Network_FileDownLoader::_Init()
{
	return true;
}
//--------------------------------------------------------------------
// 通知
void FKCW_Network_FileDownLoader::_OnHttpDone( FKCW_Network_HttpResponse* p_pResponse )
{
	// 如果没有下载单元，则该消息不是自己的。
	if(!m_pEntry)
		return;

	// 失败了
	if(!p_pResponse->isSuccess()) 
	{
		m_vecFailedEntries.addObject(m_pEntry);
	}

	// 关闭流
	CC_SAFE_RELEASE_NULL(m_pFOS);
	m_pEntry = NULL;

	// 移除第一个下载单元
	m_vecEntries.removeObjectAtIndex(0);

	// 下载下一个
	if(m_vecEntries.count() > 0) 
	{
		_DownloadNext();
	} 
	else 
	{
		m_bIsDownloading = false;
	}
}
//--------------------------------------------------------------------
void FKCW_Network_FileDownLoader::_OnHttpData( FKCW_Network_HttpResponse* p_pResponse )
{
	if( m_pFOS ) 
	{
		FKCW_Base_Data* data = p_pResponse->getData();
		m_pFOS->Write((const char*)data->getBytes(), data->getSize());
		m_nTotalDownloadedSize += data->getSize();
	}
}
//--------------------------------------------------------------------
void FKCW_Network_FileDownLoader::_OnHttpHeaders( CCObject* p )
{
	FKCW_Network_HttpResponse* response = (FKCW_Network_HttpResponse*)p;
	if( m_pEntry ) 
	{
		if(m_pEntry->m_unSize == 0) 
		{
			string contentLength = response->GetHeader("Content-Length");
			m_pEntry->m_unSize = atoi(contentLength.c_str());
		}
	}
}
//--------------------------------------------------------------------
// 下载下一个实体
void FKCW_Network_FileDownLoader::_DownloadNext()
{
	// 获取第一个下载对象
	m_pEntry = (FKCW_Network_DownloadEntry*)m_vecEntries.objectAtIndex(0);

	// 创建流
	string path = m_strFolder + m_pEntry->m_strDstName;
	m_pFOS = FKCW_IO_AssetOutputStream::Create( path, m_pEntry->m_bAppend );
	CC_SAFE_RETAIN(m_pFOS);

	// 开始下载
	FKCW_Network_HttpRequest* request = FKCW_Network_HttpRequest::Create();
	request->setUrl(m_pEntry->m_strUrl);
	request->setMethod(FKCW_Network_HttpRequest::eHttpMethod_Get);
	m_pClient->AsyncExecute(request);
}
//--------------------------------------------------------------------
FKCW_Network_FileDownLoader::~FKCW_Network_FileDownLoader()
{
	CC_SAFE_RELEASE(m_pClient);
	s_Instance = NULL;
}
//--------------------------------------------------------------------
FKCW_Network_FileDownLoader* FKCW_Network_FileDownLoader::GetInstance()
{
	if(!s_Instance) 
	{
		s_Instance = new FKCW_Network_FileDownLoader();
	}
	return s_Instance;
}
//--------------------------------------------------------------------
// 清除文件下载器对象
void FKCW_Network_FileDownLoader::Purge()
{
	if(s_Instance) 
	{
		// 移除监听器
		CCNotificationCenter::sharedNotificationCenter()->removeObserver(s_Instance, NotificationHttpRequestCompleted);
		CCNotificationCenter::sharedNotificationCenter()->removeObserver(s_Instance, NotificationHttpDataReceived);
		CCNotificationCenter::sharedNotificationCenter()->removeObserver(s_Instance, NotificationHttpDidReceiveResponse);

		CC_SAFE_RELEASE(s_Instance);
	}
}
//--------------------------------------------------------------------
// 添加一个需要下载的对象
// 参数：p_strUrl 需要下载的文件url，文件名则是url的最后一段
//		p_bIsAppend 若该值为true，则意味着下载后的文件将添加在本地的同名原文件后面，若为false，则替换
void FKCW_Network_FileDownLoader::AddFile( const string& p_strUrl, bool p_bIsAppend )
{
	string str = p_strUrl;
	string dst = FKCW_Base_Utils::LastPathOfComponent(str);
	AddFile(p_strUrl, dst, p_bIsAppend);
}
//--------------------------------------------------------------------
// 添加一个需要下载的对象
// 参数：p_strUrl 需要下载的文件url
//		p_strFileName 下载文件后，更改为新的文件名
//		p_bIsAppend 若该值为true，则意味着下载后的文件将添加在本地的同名原文件后面，若为false，则替换
void FKCW_Network_FileDownLoader::AddFile( const string& p_strUrl, const string& p_strFileName, bool p_bIsAppend )
{
	AddFile(p_strUrl, p_strFileName, 0, p_bIsAppend);
}
//--------------------------------------------------------------------
// 添加一个需要下载的对象
// 参数：p_strUrl 需要下载的文件url
//		p_strFileName 下载文件后，更改为新的文件名
//		p_unSize 要下载的文件大小
//		p_bIsAppend 若该值为true，则意味着下载后的文件将添加在本地的同名原文件后面，若为false，则替换
void FKCW_Network_FileDownLoader::AddFile( const string& p_strUrl, const string& p_strFileName, size_t p_unSize, bool p_bIsAppend )
{
	FKCW_Network_DownloadEntry* e = FKCW_Network_DownloadEntry::Create();
	e->m_strUrl			= p_strUrl;
	e->m_strDstName		= p_strFileName;
	e->m_unSize			= p_unSize;
	e->m_bAppend		= p_bIsAppend;
	m_nTotalSize		+= p_unSize;
	m_vecEntries.addObject(e);
}
//--------------------------------------------------------------------
// 开始下载
void FKCW_Network_FileDownLoader::Start()
{
	if(!m_bIsDownloading) 
	{
		// 如果有需要下载的实体，则go..
		if(m_vecEntries.count() > 0) 
		{
			m_vecFailedEntries.removeAllObjects();
			_DownloadNext();
			m_bIsDownloading = true;
		}
	}
}
//--------------------------------------------------------------------
// 中断下载
void FKCW_Network_FileDownLoader::Abort()
{
	if(m_bIsDownloading) 
	{
		m_pClient->CancelAll();
		CC_SAFE_RELEASE_NULL(m_pFOS);
		m_pEntry = NULL;
		m_vecEntries.removeAllObjects();
		m_bIsDownloading = false;
	}
}
//--------------------------------------------------------------------
// 获取当前下载文件已下载的大小
size_t FKCW_Network_FileDownLoader::GetCurrentDownloadedSize()
{
	return m_pFOS ? m_pFOS->GetPosition() : 0;
}
//--------------------------------------------------------------------
// 获取当前正在下载的文件名
string FKCW_Network_FileDownLoader::GetCurrentDownloadingFileName()
{
	return m_pEntry ? FKCW_Base_Utils::LastPathOfComponent( m_pEntry->m_strDstName ) : "";
}
//--------------------------------------------------------------------
// 获取当前正在下载的文件名，包括全部路径
string FKCW_Network_FileDownLoader::GetCurrentDownloadingFileFullPath()
{
	return m_pEntry ? m_pEntry->m_strDstName : "";
}
//--------------------------------------------------------------------
// 获取当前下载文件的总大小
// 注：若尚未知道本文件大小，则该值可能为0
size_t FKCW_Network_FileDownLoader::GetCurrentDownloadingFileSize()
{
	return m_pEntry ? m_pEntry->m_unSize : 0;
}
//--------------------------------------------------------------------