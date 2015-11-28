/********************************************************************
	created:	2014/11/01
	file base:	FKCW_Network_HttpRequest
	author:		FreeKnight
	
	purpose:	Http请求
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_Network_Macros.h"
//--------------------------------------------------------------------
USING_NS_CC;
using std::string;
using std::vector;
//--------------------------------------------------------------------
class FKCW_Network_HttpRequest : public CCObject
{
public:
	// http方法定义
	enum ENUM_HttpMethod
	{
		eHttpMethod_Get,
		eHttpMethod_Post,
		eHttpMethod_Put,
		eHttpMethod_Delete,
		eHttpMethod_Unknown,
	};

	CC_SYNTHESIZE(ENUM_HttpMethod, m_eMethod, Method);		// HTTP方法
	CC_SYNTHESIZE_PASS_BY_REF(string, m_strUrl, Url);		// Url字符串
	CC_SYNTHESIZE_PASS_BY_REF(int, m_nTag, Tag);			// 确定请求所用的标签
	CC_SYNTHESIZE(void*, m_pUserData, UserData);			// 自定义数据，请求不予保存
	CC_SYNTHESIZE_RETAIN(FKCW_Base_Data*, m_pRequestData, RequestData);	// 对于Post,put的请求数据
	CC_SYNTHESIZE_PASS_BY_REF(vector<string>, m_vecHeaders, Headers);	// 头列表
	CC_SYNTHESIZE_BOOL(m_bIsCancel, Cancel);				// 取消标志，如果设置为true，则http请求可以被中止
public:
	FKCW_Network_HttpRequest()
	{
		m_eMethod		= eHttpMethod_Unknown;
		m_pRequestData	= NULL;
		m_pUserData		= NULL;
		m_nTag			= -1;
		m_bIsCancel		= false;
	}
	virtual ~FKCW_Network_HttpRequest()
	{
		CC_SAFE_RELEASE( m_pRequestData );
	}
	static FKCW_Network_HttpRequest* Create()
	{
		FKCW_Network_HttpRequest* req = new FKCW_Network_HttpRequest();
		return ( FKCW_Network_HttpRequest* )req->autorelease();
	}
};