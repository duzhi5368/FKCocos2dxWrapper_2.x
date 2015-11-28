//*************************************************************************
//	创建日期:	2014-12-19
//	文件名称:	Test_Network_FileDownLoader.h
//  创 建 人:   WilhanTIan
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"
//-------------------------------------------------------------------------
class CTest_Network_FileDownLoaderLayer : public CCLayer
{
public:
	virtual void onEnter()
	{
		CCLayer::onEnter();

		m_fileLabel = CCLabelTTF::create("Current file", "Helvetica", 36 / CC_CONTENT_SCALE_FACTOR());
		m_fileLabel->setPosition(ccp( SCREEN_CENTER_POS_X,
			SCREEN_CENTER_POS_Y + 40 / CC_CONTENT_SCALE_FACTOR()));
		addChild(m_fileLabel);

		m_label = CCLabelTTF::create("Getting", "Helvetica", 30 / CC_CONTENT_SCALE_FACTOR());
		m_label->setPosition( SCREEN_CENTER_POS );
		addChild(m_label);

		FKCW_Network_FileDownLoader* d = FKCW_Network_FileDownLoader::GetInstance();
		d->AddFile("http://123.125.244.87/apache/ant/antlibs/antunit/RELEASE-NOTES-1.3.html");		// 小文件
		d->AddFile("http://mirrors.cnnic.cn/apache/ant/binaries/apache-ant-1.9.1-bin.zip");			// 不存在的文件，0Size
		d->AddFile("http://mirrors.cnnic.cn/apache/ant/binaries/apache-ant-1.9.4-bin.zip");			// 一个大文件
		d->Start();

		// 开启帧更新
		scheduleUpdate();
	}
	void onExit()
	{
		unscheduleUpdate();
		CCLayer::onExit();
	}
public:
	void update( float delta )
	{
		FKCW_Network_FileDownLoader* d = FKCW_Network_FileDownLoader::GetInstance();
		if(d->isDownloading()) 
		{
			m_fileLabel->setString(d->GetCurrentDownloadingFileName().c_str());
			char buf[64];
			sprintf(buf, "%ld/%ld", d->GetCurrentDownloadedSize(), d->GetCurrentDownloadingFileSize());
			m_label->setString(buf);
		} 
		else
		{
			m_fileLabel->setString( A2U("下载完成,请查看程序目录").c_str());
			m_label->setString("");
		}
	}
public:
	CREATE_FUNC( CTest_Network_FileDownLoaderLayer );
protected:
	CCLabelTTF* m_fileLabel;
	CCLabelTTF* m_label;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(Network, FileDownLoader, 文件下载器)
//-------------------------------------------------------------------------