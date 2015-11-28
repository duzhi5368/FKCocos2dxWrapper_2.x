//*************************************************************************
//	创建日期:	2014-12-24
//	文件名称:	Test_SimpleNet_NetObj.h
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
class CMsgNetDelegate : public FKCW_SimpleNet_NetObject
{
public:
	SINGLE_DELEGATE_INSTANCE_FUNC(CMsgNetDelegate);
public:
	CMsgNetDelegate()
	{

	}
	virtual ~CMsgNetDelegate()
	{

	}
	virtual void onMessageReceived(FKCW_SimpleNet_Buffer& oBuffer)
	{
		FKLOG("onMessageReceived");

		switch( oBuffer.readInt() )
		{
		case 10:
			{
				FKLOG("%lld", oBuffer.readLongLong());
				FKLOG("%f", oBuffer.readFloat());
				FKLOG("%lf", oBuffer.readDouble());
				FKLOG("%c", oBuffer.readChar());
				FKLOG("%s", oBuffer.readLengthAndString().c_str());
			}
			break;
		case 5:
			{
				// 大数据
				FKLOG("large data length = %d", oBuffer.length());
			}
			break;
		}
	}
	virtual void onConnected()
	{
		FKLOG("onConnected");
	}
	virtual void onConnectTimeout()
	{
		FKLOG("onConnectTimeout");
	}
	virtual void onDisconnected()
	{
		FKLOG("onDisconnected");
	}
	virtual void onExceptionCaught(ENUM_SocketStatus eStatus)
	{
		FKLOG("onExceptionCaught %d", (int)eStatus);
	}

public:
	void CS_Test()
	{
		FKCW_SimpleNet_Buffer buffer;
		buffer.writeInt(10);
		buffer.writeLongLong(999999999L);
		buffer.writeFloat(77.7f);
		buffer.writeDouble(99.9);
		buffer.writeChar('h');
		buffer.writeLengthAndString("FreeKnightCocos2dxWrapper");

		this->send(&buffer);
	}
	void CS_LargePackage()
	{
		FKCW_SimpleNet_Buffer buffer;
		buffer.writeInt(5);
		char szBuf[10240];
		memset( szBuf, 0, 10240 );
		buffer.writeLengthAndData( szBuf, 10240 );

		this->send(&buffer);
	}
};
//-------------------------------------------------------------------------
class CTest_SimpleNet_NetObjLayer : public CCLayer
{
public:
	virtual void onEnter()
	{
		CCLayer::onEnter();

		CCMenuItemFont* pConnect = CCMenuItemFont::create( A2U("连接").c_str(), this, menu_selector(CTest_SimpleNet_NetObjLayer::onConnectClick));
		pConnect->setFontSize(26);
		pConnect->setPosition(SCREEN_CENTER_POS+ccp(0, 35));

		CCMenuItemFont* pDisconnect = CCMenuItemFont::create( A2U("断开连接").c_str(), this, menu_selector(CTest_SimpleNet_NetObjLayer::onDisconnectClick));
		pDisconnect->setFontSize(26);
		pDisconnect->setPosition(SCREEN_CENTER_POS+ccp(0, 0));

		CCMenuItemFont* pLargeData = CCMenuItemFont::create( A2U("发送大消息包").c_str(), this, menu_selector(CTest_SimpleNet_NetObjLayer::onLargeDataClick));
		pLargeData->setFontSize(26);
		pLargeData->setPosition(SCREEN_CENTER_POS+ccp(0, -35));

		CCMenuItemFont* pTest = CCMenuItemFont::create( A2U("测试发包").c_str(), this, menu_selector(CTest_SimpleNet_NetObjLayer::onTestClick));
		pTest->setFontSize(26);
		pTest->setPosition(SCREEN_CENTER_POS+ccp(0, -70));

		CCMenu* pMenu = CCMenu::create(pConnect, pDisconnect, pTest, pLargeData, NULL);
		pMenu->setPosition(CCPointZero);
		addChild(pMenu);

		CCSprite* picon = CCSprite::create( s_szIcon );
		picon->setPosition(SCREEN_CENTER_POS+ccp(-SCREEN_CENTER_POS_X, 150));

		CCMoveBy* pMoveRight = CCMoveBy::create(1.5f, ccp(SCREEN_CENTER_POS_X*2, 0));
		CCMoveBy* pMoveLeft = CCMoveBy::create(1.5f, ccp(-SCREEN_CENTER_POS_X*2, 0));
		CCSequence* pSeq = CCSequence::create(pMoveRight, pMoveLeft, NULL);
		picon->runAction(CCRepeatForever::create(pSeq));

		addChild(picon);

		m_pOutputLabel = CCLabelTTF::create( A2U("信息提示框").c_str(), "Helvetica", 24);
		m_pOutputLabel->setPosition( SCREEN_CENTER_POS + ccp(0,-200));
		m_pOutputLabel->setColor(ccc3(255,0,0));
		addChild(m_pOutputLabel);
	}

	virtual void onExit()
	{
		CMsgNetDelegate::sharedDelegate()->disconnect();

		CCLayer::onExit();
	}
public:
	void onConnectClick(CCObject* pSender)
	{
		FKCW_SimpleNet_InetAddress oAddres;
		oAddres.setIp("127.0.0.1");
		oAddres.setPort(9999);

		CMsgNetDelegate::sharedDelegate()->setInetAddress(oAddres);
		CMsgNetDelegate::sharedDelegate()->setSoTimeout( 3.0f );
		CMsgNetDelegate::sharedDelegate()->connect();

		if (m_pOutputLabel)
		{
			string str = "连接 127.0.0.1/9999 查看控制台,\n";
			str += "输出:onConnected 表示连接成功\n"; 
			str += "输出:onConnectTimeout 表示连接失败,请确认是否 打开TCPServerSim.exe 并按下 [start]\n";
			str += "输出:onConnected 表示断开连接\n";
			m_pOutputLabel->setString( A2U(str.c_str()).c_str() );
		}
		
	}
	void onDisconnectClick(CCObject* pSender)
	{
		CMsgNetDelegate::sharedDelegate()->disconnect();
	}
	void onTestClick(CCObject* pSender)
	{
		CMsgNetDelegate::sharedDelegate()->CS_Test();
	}
	void onLargeDataClick(CCObject* pSender)
	{
		CMsgNetDelegate::sharedDelegate()->CS_LargePackage();
	}
public:
	CREATE_FUNC( CTest_SimpleNet_NetObjLayer );

private:
	CCLabelTTF*					m_pOutputLabel;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(SimpleNet, NetObj,测试前请手动启动Debug.win32下的TCPServerSim.exe并按下start)
//-------------------------------------------------------------------------