//*************************************************************************
//	创建日期:	2014-12-23
//	文件名称:	Test_Network_HttpGet.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
class CTest_Network_HttpGetLayer : public CCLayer
{
public:
	virtual bool init()
	{
		CCMenuItemLabel* item1 = CCMenuItemLabel::create(
			CCLabelTTF::create( "Cancel", "Helvetica", 40 / CC_CONTENT_SCALE_FACTOR()),
			this,
			menu_selector(CTest_Network_HttpGetLayer::onCancelClicked));
		item1->setPosition( SCREEN_CENTER_POS_X, SCREEN_CENTER_POS_Y - 100 / CC_CONTENT_SCALE_FACTOR());
		item1->setColor( ccRED );
		CCMenu* menu = CCMenu::create(item1, NULL);
		menu->setAnchorPoint(CCPointZero);
		menu->setPosition(CCPointZero);
		addChild(menu,1);

		CCLabelTTF* pLabel = CCLabelTTF::create( A2U("准备下载http://mirrors.cnnic.cn/apache/ant/binaries/apache-ant-1.9.4-bin.zip").c_str(), "Helvetica", 26 / CC_CONTENT_SCALE_FACTOR());
		pLabel->setPosition( SCREEN_CENTER_POS + ccp(0, 100) );
		addChild(pLabel);

		m_pLabel = CCLabelTTF::create( A2U("正在get").c_str(), "Helvetica", 36 / CC_CONTENT_SCALE_FACTOR());
		m_pLabel->setPosition( SCREEN_CENTER_POS );
		addChild(m_pLabel);

		return true;
	}
	virtual void onEnter()
	{
		CCLayer::onEnter();

		// 开始异步请求
		FKCW_Network_HttpClient* client = FKCW_Network_HttpClient::Create();
		FKCW_Network_HttpRequest* req = FKCW_Network_HttpRequest::Create();
		req->setTag(1);
		req->setUrl("http://mirrors.cnnic.cn/apache/ant/binaries/apache-ant-1.9.4-bin.zip");
		req->setMethod(FKCW_Network_HttpRequest::eHttpMethod_Get);
		client->AsyncExecute(req);
		m_pClient = client;
		CC_SAFE_RETAIN(m_pClient);

		// 监听器
		CCNotificationCenter::sharedNotificationCenter()->addObserver(this, 
			callfuncO_selector(CTest_Network_HttpGetLayer::onHttpDone), NotificationHttpRequestCompleted, NULL);
		CCNotificationCenter::sharedNotificationCenter()->addObserver(this, 
			callfuncO_selector(CTest_Network_HttpGetLayer::onHttpData), NotificationHttpDataReceived, NULL);
		CCNotificationCenter::sharedNotificationCenter()->addObserver(this, 
			callfuncO_selector(CTest_Network_HttpGetLayer::onHttpHeaders), NotificationHttpDidReceiveResponse, NULL);
	}
	void onExit()
	{
		CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NotificationHttpRequestCompleted);
		CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NotificationHttpDataReceived);
		CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NotificationHttpDidReceiveResponse);

		CCLayer::onExit();
	}
public:
	void onCancelClicked(CCObject* sender) 
	{
		m_pClient->Cancel(1);
		m_pLabel->setString( A2U("文件httpGet被取消").c_str() );
	}
	void onHttpHeaders(CCObject* p) 
	{
		FKCW_Network_HttpResponse* response = (FKCW_Network_HttpResponse*)p;
		string contentLength = response->GetHeader("Content-Length");
		m_nFileLen = atoi(contentLength.c_str());
		m_nRecvLen = 0;

		char buf[64];
		sprintf(buf, "文件长度 : %d/%d", m_nRecvLen, m_nFileLen);
		m_pLabel->setString( A2U(buf).c_str() );
	}
	void onHttpData(CCObject* p) 
	{
		FKCW_Network_HttpResponse* response = (FKCW_Network_HttpResponse*)p;
		m_nRecvLen += response->getData()->getSize();
		char buf[64];
		sprintf(buf, "文件长度 : %d/%d", m_nRecvLen, m_nFileLen);
		m_pLabel->setString( A2U(buf).c_str() );
	}

	void onHttpDone(CCObject* p) 
	{
		FKCW_Network_HttpResponse* response = (FKCW_Network_HttpResponse*)p;
		char buf[128];
		sprintf(buf, "是否完成 : %s", response->isSuccess() ? "是" : "否");
		m_pLabel->setString( A2U(buf).c_str() );
	}
public:
	CREATE_FUNC( CTest_Network_HttpGetLayer );
private:
	int							m_nFileLen;
	int							m_nRecvLen;
	CCLabelTTF*					m_pLabel;
	FKCW_Network_HttpClient*	m_pClient;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(Network, HttpGet, HttpGet方式测试)
//-------------------------------------------------------------------------