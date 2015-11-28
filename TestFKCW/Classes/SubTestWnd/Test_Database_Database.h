//*************************************************************************
//	创建日期:	2014-12-19
//	文件名称:	Test_Database_Database.h
//  创 建 人:   WilhanTIan
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../resource.h"
#include "../TestScene.h"
#include "../../../FKCWLib/FKCocos2dxWrapper.h"

#pragma comment( lib, "sqlite3.lib")
//-------------------------------------------------------------------------
class CTest_Database_DatabaseLayer : public CCLayer
{
public:
	bool init()
	{
		m_pDatebase = NULL;

		CCLabelTTF* label1 = CCLabelTTF::create( A2U("打开DB").c_str(), "Helvetica", 26);
		CCMenuItemLabel* item1 = CCMenuItemLabel::create(label1, this, menu_selector(CTest_Database_DatabaseLayer::onOpenDBClicked));

		CCLabelTTF* label2 = CCLabelTTF::create( A2U("执行 SQL 文件").c_str(), "Helvetica", 26);
		CCMenuItemLabel* item2 = CCMenuItemLabel::create(label2, this, menu_selector(CTest_Database_DatabaseLayer::onExecuteSQLFileClicked));

		CCLabelTTF* label3 = CCLabelTTF::create( A2U("插入一行数据").c_str(), "Helvetica", 26);
		CCMenuItemLabel* item3 = CCMenuItemLabel::create(label3, this, menu_selector(CTest_Database_DatabaseLayer::onInsertClicked));

		CCLabelTTF* label4 = CCLabelTTF::create( A2U("提交").c_str(), "Helvetica", 26);
		CCMenuItemLabel* item4 = CCMenuItemLabel::create(label4, this, menu_selector(CTest_Database_DatabaseLayer::onCommitClicked));

		CCLabelTTF* label5 = CCLabelTTF::create( A2U("回滚").c_str(), "Helvetica", 26);
		CCMenuItemLabel* item5 = CCMenuItemLabel::create(label5, this, menu_selector(CTest_Database_DatabaseLayer::onRollbackClicked));

		CCMenu* menu = CCMenu::create(item1, item2, item3, item4, item5,  NULL);
		menu->alignItemsVertically();
		menu->setPosition( SCREEN_CENTER_POS + ccp(0 , 150) );
		addChild(menu);

		m_pHintLabel = CCLabelTTF::create( A2U("这是信息提示Label").c_str(), "Helvetica", 24);
		m_pHintLabel->setPosition( SCREEN_CENTER_POS);
		m_pHintLabel->setColor(ccc3(255,0,0));
		addChild(m_pHintLabel);

		
		return true;
	}

	void onExit()
	{
		CC_SAFE_RELEASE( m_pDatebase );
		CCLayer::onExit();
	}
public:
	// 打开DB
	void onOpenDBClicked(CCObject* pSender)
	{
		string path = FKCW_Base_Utils::Externalize( s_szTestDBFile );
		// 创建数据库并且打开
		if( CCFileUtils::sharedFileUtils()->isFileExist( path ) )
		{
			m_pDatebase = FKCW_Database_Database::Create(path);
			if( m_pDatebase == NULL )
			{
				m_pHintLabel->setString( A2U("数据库打开失败").c_str() );
				return;
			}
			if( !m_pDatebase->Open() )
			{
				m_pHintLabel->setString( A2U("数据库打开失败").c_str() );
				return;
			}
			m_pDatebase->retain();

			char szInfo[256];
			memset( szInfo, 0, 256 );
			sprintf( szInfo, "打开 %s 成功，\n当前SQL版本号为 %d",path.c_str(), m_pDatebase->GetVersion() );
			m_pHintLabel->setString( A2U( szInfo ).c_str() );
		}
		else
		{
			m_pHintLabel->setString( A2U("数据库不存在，请创建空db文件").c_str() );
		}
	}
	// 执行SQL文件
	void onExecuteSQLFileClicked(CCObject* pSender)
	{
		if( (m_pDatebase == NULL) || (!m_pDatebase->IsDatabaseOpened()))
		{
			m_pHintLabel->setString(A2U( "请先打开数据库" ).c_str());
			return;
		}
		// 执行SQL
		if(m_pDatebase->ExecuteSQL( s_szTestSQLFile ))
		{
			string path = FKCW_Base_Utils::Externalize( s_szTestSQLFile );
			char szInfo[200];
			int rowCount = m_pDatebase->IntFromQuery("SELECT count() FROM CDs");
			memset( szInfo, 0, 64 );
			sprintf( szInfo, "执行 %s 成功\n当前 CDs 表中行数为 %d",path.c_str(), rowCount );
			m_pHintLabel->setString(A2U( szInfo ).c_str());
		}
		else
		{
			m_pHintLabel->setString( A2U("SQL文件语句执行失败").c_str());
		}
	}

	void onInsertClicked(CCObject* pSender)
	{
		if( (m_pDatebase == NULL) || (!m_pDatebase->IsDatabaseOpened()))
		{
			m_pHintLabel->setString(A2U( "请先打开数据库" ).c_str());
			return;
		}

		if(!m_pDatebase->isInTransaction())
			m_pDatebase->BeginTransaction();

		// 执行
		char tmp[16];
		memset( tmp, 0, 16 );
		sprintf( tmp, "%d", static_cast<int>(FKCW_Base_Utils::RandomInRange(1,2000) ) );
		m_pDatebase->ExecuteUpdate("insert into CDs (CDID,ArtistID,Title,Date) values (NULL,1,'So',%s);", tmp );

		int rowCount = m_pDatebase->IntFromQuery("SELECT count() FROM CDs");
		char buf[128];
		sprintf(buf, "执行 [SELECT count() FROM CDs] \n 当前 CDs 表中行数为 %d", rowCount);
		m_pHintLabel->setString( A2U( buf ).c_str());
	}
	void onCommitClicked(CCObject* pSender)
	{
		if( (m_pDatebase == NULL) || (!m_pDatebase->IsDatabaseOpened()))
		{
			m_pHintLabel->setString(A2U( "请先打开数据库" ).c_str());
			return;
		}
		
		if(m_pDatebase->Commit()) 
		{
			bool bCommit = m_pDatebase->Commit();
			
			if(bCommit) 
			{
				m_pHintLabel->setString( A2U("提交成功").c_str());
			} else {
				m_pHintLabel->setString( A2U("提交失败").c_str() );
			}
		} 
		else 
		{
			m_pHintLabel->setString( A2U("SQL语句提交失败").c_str() );
		}
	}
	void onRollbackClicked(CCObject* pSender)
	{
		if( (m_pDatebase == NULL) || (!m_pDatebase->IsDatabaseOpened()))
		{
			m_pHintLabel->setString(A2U( "请先打开数据库" ).c_str());
			return;
		}

		if(m_pDatebase->Rollback()) 
		{
			m_pHintLabel->setString( A2U("回滚成功").c_str());
		} else {
			m_pHintLabel->setString( A2U("回滚失败").c_str() );
		}
	}
public:
	CREATE_FUNC( CTest_Database_DatabaseLayer );


protected:
	FKCW_UIWidget_WidgetWindow* m_pWindow;
	CCLabelTTF*					m_pHintLabel;
	FKCW_Database_Database*		m_pDatebase;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(Database, Database, 数据库操作)