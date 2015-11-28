/********************************************************************
	created:	2014/11/01
	file base:	FKCW_Network
	author:		FreeKnight
	
	purpose:	
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
/* 
====================================
模块说明：	
====================================
	网络支持库
====================================
内部文件功能说明：
====================================
	FKCW_Network_NetworkCommon
		网络支持模块使用的部分基本宏和类型定义
	FKCW_Network_Packet
		网络消息包和包头格式定义和常用函数类

	FKCW_Network_TCPSocket
		TCP连接套接字封装
	FKCW_Network_TCPSocketHub
		TCP连接管理器

	FKCW_Network_UDPSocket
		UDP连接套接字封装
	FKCW_Network_UDPSocketHub
		UDP连接管理器

	FKCW_Network_HttpRequest
		Http请求类
	FKCW_Network_HttpResponse
		Http应答类
	FKCW_Network_HttpClient
		类似CCHttpClient。但做了一定程序的加强。

	FKCW_Network_FileDownLoader
		一个使用Http协议的文件下载器。它管理一个需要下载文件列表，它自动一个接一个的下载全部文件到本地指定空间。
====================================
依赖项目/库：
====================================
	FKCW_Base
====================================
todo:
====================================

*/
//--------------------------------------------------------------------
#include "Include/FKCW_Network_NetworkCommon.h"
#include "Include/FKCW_Network_Packet.h"

#include "Include/FKCW_Network_TCPSocket.h"
#include "Include/FKCW_Network_TCPSocketHub.h"

#include "Include/FKCW_Network_UDPSocket.h"
#include "Include/FKCW_Network_UDPSocketHub.h"

#include "Include/FKCW_Network_HttpRequest.h"
#include "Include/FKCW_Network_HttpResponse.h"
#include "Include/FKCW_Network_HttpClient.h"
#include "Include/FKCW_Network_FileDownLoader.h"