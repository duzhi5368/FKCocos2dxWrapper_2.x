/********************************************************************
	created:	2014/11/01
	file base:	FKCW_Network_FileDownLoader
	author:		FreeKnight
	
	purpose:	Http文件下载器
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_Network_Macros.h"
#include "FKCW_Network_HttpClient.h"
//--------------------------------------------------------------------
// 被下载的实体
class FKCW_Network_DownloadEntry : public CCObject
{
public:
	string		m_strUrl;			// 下载Url
	string		m_strDstName;		// 下载保存目录名
	size_t		m_unSize;			// 文件大小
	bool		m_bAppend;			// 是否是添加而非替换
public:
	FKCW_Network_DownloadEntry();
	virtual ~FKCW_Network_DownloadEntry();
	static FKCW_Network_DownloadEntry*		Create();
};
//--------------------------------------------------------------------
// 一个使用Http协议的文件下载器。
// 它管理一个需要下载文件列表，它自动一个接一个的下载全部文件。
// 所有文件最终将保存在本地。
// 在iOS中，文件将被保存在 ~/Documents 目录下。
// 在Android，文件将被保存在内部存储器中。
// 当前你也可以指定文件夹进行文件保存，例如“Data”目录，则在iOS中，文件将被保存在 ~/Documents/Data 目录下
// 这是一个单件对象
class FKCW_Network_FileDownLoader : public CCObject
{
private:
	FKCW_Network_HttpClient*		m_pClient;		// HttpClient
	FKCW_IO_AssetOutputStream*		m_pFOS;			// 当前文件流
	FKCW_Network_DownloadEntry*		m_pEntry;		// 当前下载实体

	CC_SYNTHESIZE_PASS_BY_REF(string, m_strFolder, Folder);					// 文件夹前缀
	CC_SYNTHESIZE_PASS_BY_REF(CCArray, m_vecEntries, DownloadEntries);		// 需要下载的实体列表
	CC_SYNTHESIZE_PASS_BY_REF(CCArray, m_vecFailedEntries, FailedEntries);	// 下载失败的实体列表
	CC_SYNTHESIZE_BOOL(m_bIsDownloading, Downloading);						// 是否正在下载
	CC_SYNTHESIZE_READONLY(size_t, m_nTotalSize, TotalSize);						// 需要下载的总字节数
	CC_SYNTHESIZE_READONLY(size_t, m_nTotalDownloadedSize, TotalDownloadedSize);	// 已下载的总字节数
protected:
	FKCW_Network_FileDownLoader();
	// 初始化
	virtual bool		_Init();

	// 通知
	void				_OnHttpDone( FKCW_Network_HttpResponse* p_pResponse );
	void				_OnHttpData( FKCW_Network_HttpResponse* p_pResponse );
	void				_OnHttpHeaders( CCObject* p );

	// 下载下一个实体
	void				_DownloadNext();
public:
	virtual ~FKCW_Network_FileDownLoader();
	static FKCW_Network_FileDownLoader* GetInstance();
	// 清除文件下载器对象
	static void		Purge();

	// 添加一个需要下载的对象
	// 参数：p_strUrl 需要下载的文件url，文件名则是url的最后一段
	//		p_bIsAppend 若该值为true，则意味着下载后的文件将添加在本地的同名原文件后面，若为false，则替换
	void	AddFile( const string& p_strUrl, bool p_bIsAppend = false );
	// 添加一个需要下载的对象
	// 参数：p_strUrl 需要下载的文件url
	//		p_strFileName 下载文件后，更改为新的文件名
	//		p_bIsAppend 若该值为true，则意味着下载后的文件将添加在本地的同名原文件后面，若为false，则替换
	void	AddFile( const string& p_strUrl, const string& p_strFileName, bool p_bIsAppend = false );
	// 添加一个需要下载的对象
	// 参数：p_strUrl 需要下载的文件url
	//		p_strFileName 下载文件后，更改为新的文件名
	//		p_unSize 要下载的文件大小
	//		p_bIsAppend 若该值为true，则意味着下载后的文件将添加在本地的同名原文件后面，若为false，则替换
	void	AddFile( const string& p_strUrl, const string& p_strFileName, size_t p_unSize, bool p_bIsAppend = false );

	// 开始下载
	void	Start();
	// 中断下载
	void	Abort();

	// 获取当前下载文件已下载的大小
	size_t	GetCurrentDownloadedSize();
	// 获取当前正在下载的文件名
	string	GetCurrentDownloadingFileName();
	// 获取当前正在下载的文件名，包括全部路径
	string	GetCurrentDownloadingFileFullPath();
	// 获取当前下载文件的总大小
	// 注：若尚未知道本文件大小，则该值可能为0
	size_t	GetCurrentDownloadingFileSize();
};