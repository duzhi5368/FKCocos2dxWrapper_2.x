//*************************************************************************
//	创建日期:	2014-12-23
//	文件名称:	Test_Network_TCP.h
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
class CTest_Network_TCPLayer : public CCLayer
{
public:
	virtual void onEnter()
	{
		CCLayer::onEnter();

		CCMenuItemLabel* item1 = CCMenuItemLabel::create(CCLabelTTF::create(A2U("点我往 [127.0.0.1/9999] 发送hello消息").c_str(), "Helvetica", 26 / CC_CONTENT_SCALE_FACTOR()),
			this,
			menu_selector(CTest_Network_TCPLayer::onSendClicked));
		item1->setPosition(SCREEN_CENTER_POS);
		item1->setColor( ccc3FromInt( 0xffd93a49 ) );

		CCMenu* menu = CCMenu::create(item1, NULL);
		menu->setAnchorPoint(CCPointZero);
		menu->setPosition(CCPointZero);
		addChild(menu);

		m_recv = CCLabelTTF::create("", "Helvetica", 26 / CC_CONTENT_SCALE_FACTOR());
		m_recv->setPosition(SCREEN_CENTER_POS+ccp(0, -200));
		addChild(m_recv);

		CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
		nc->addObserver(this, callfuncO_selector(CTest_Network_TCPLayer::onTCPSocketConnected),		NotificationTCPSocketConnected, NULL);
		nc->addObserver(this, callfuncO_selector(CTest_Network_TCPLayer::onTCPSocketDisonnected),	NotificationTCPSocketDisconnected, NULL);
		nc->addObserver(this, callfuncO_selector(CTest_Network_TCPLayer::onPacketReceived),			NotificationPacketReceived, NULL);

		// 这里自己修改IP和端口
		m_hub = FKCW_Network_TCPSocketHub::Create();
		m_hub->setRawPolicy(true);
		if( m_hub->CreateSocket("127.0.0.1", 9999, 1, 3, true ) == NULL )
		{
			m_recv->setString( A2U("创建socket失败").c_str() );
			m_hub->StopAll();
		}
		else
		{
			m_recv->setString( A2U("创建socket成功").c_str() );
		}
	}
	void onExit()
	{
		CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
		nc->removeObserver(this, NotificationTCPSocketConnected);
		nc->removeObserver(this, NotificationTCPSocketDisconnected);
		nc->removeObserver(this, NotificationPacketReceived);

		m_hub->StopAll();
		CCLayer::onExit();
	}

	void onSendClicked(CCObject* sender) 
	{
		static int index = 0;
		char buf[32];
		sprintf(buf, "Hello: %d\r\n", index++);
		FKCW_Network_Packet* p = FKCW_Network_Packet::CreateRawPacket(buf, strlen(buf));
		m_hub->SendPacket(1, p);
	}

	void onTCPSocketConnected(CCObject* obj)
	{
		FKCW_Network_TCPSocket* s = (FKCW_Network_TCPSocket*)obj;
		CCLOG("connected: %d", s->getTag());
	}

	void onTCPSocketDisonnected(CCObject* obj)
	{
		FKCW_Network_TCPSocket* s = (FKCW_Network_TCPSocket*)obj;
		CCLOG("disconnected: %d", s->getTag());
	}

	void onPacketReceived(CCObject* obj) 
	{
		FKCW_Network_Packet* p = (FKCW_Network_Packet*)obj;
		string ret;
		FKCW_Base_ByteBuffer bb(p->getBuffer(), p->getPacketLength(), p->getPacketLength());
		bb.ReadLine(ret);
		if(!ret.empty()) 
		{
			char buf[65535];
			sprintf(buf, "Client get: %s", ret.c_str());
			m_recv->setString(buf);
		}
	}
public:
	CREATE_FUNC( CTest_Network_TCPLayer );
private:
	FKCW_Network_TCPSocketHub*	m_hub;
	CCLabelTTF*					m_recv;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(Network, TCP, TCP连接测试)
//-------------------------------------------------------------------------