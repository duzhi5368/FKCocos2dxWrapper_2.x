//*************************************************************************
//	创建日期:	2014-12-31
//	文件名称:	Test_Base_FileZip.h
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
class CTest_Base_FileZipLayer : public CCLayer
{
public:
	bool init()
	{
		//创建基本容器
		m_pWindow = FKCW_UIWidget_WidgetWindow::create( );
		m_pWindow->setMultiTouchEnabled(true);
		this->addChild(m_pWindow);

		CCScale9Sprite* pEditBoxBG1 = CCScale9Sprite::create(s_szSprite9Btn1Path);
		m_pSrcEdit = CCEditBox::create(CCSizeMake(800, 50), pEditBoxBG1 );
		m_pSrcEdit->setPosition( SCREEN_CENTER_POS + ccp(0, 100));
		m_pSrcEdit->setText(FKCW_Base_StringConvert::a2u("这里填写处理源文件").c_str());
		m_pWindow->addChild( m_pSrcEdit );

		CCScale9Sprite* pEditBoxBG2 = CCScale9Sprite::create(s_szSprite9Btn1Path);
		m_pDestEdit = CCEditBox::create(CCSizeMake(800, 50), pEditBoxBG2 );
		m_pDestEdit->setPosition( SCREEN_CENTER_POS + ccp(0, -100) );
		m_pDestEdit->setFontColor( ccRED );
		m_pDestEdit->setText(FKCW_Base_StringConvert::a2u("这里填写处理后文件").c_str());
		m_pWindow->addChild( m_pDestEdit );

		FKCW_UIWidget_Button* pButtonCompress = FKCW_UIWidget_Button::createWith9Sprite(CCSize(150, 50),
			s_szSprite9Btn1Path, s_szSprite9Btn2Path );
		pButtonCompress->setPosition(SCREEN_CENTER_POS + ccp(100, 0));
		pButtonCompress->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_Base_FileZipLayer::onClickCompress));
		pButtonCompress->initText(FKCW_Base_StringConvert::a2u("文件压缩").c_str(), "", 30);
		m_pWindow->addChild(pButtonCompress);

		FKCW_UIWidget_Button* pButtonDecompress = FKCW_UIWidget_Button::createWith9Sprite(CCSize(150, 50),
			s_szSprite9Btn1Path, s_szSprite9Btn2Path );
		pButtonDecompress->setPosition(SCREEN_CENTER_POS + ccp(-100, 0));
		pButtonDecompress->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_Base_FileZipLayer::onClickDecompress));
		pButtonDecompress->initText(FKCW_Base_StringConvert::a2u("文件解压缩").c_str(), "", 30);
		m_pWindow->addChild(pButtonDecompress);

		m_pInfoLable = CCLabelTTF::create(FKCW_Base_StringConvert::a2u("").c_str(), "Arial", 26 );
		m_pInfoLable->setPosition( SCREEN_CENTER_POS + ccp(0, -250) );
		m_pWindow->addChild( m_pInfoLable );
		
		return true;
	}
public:
	void onClickCompress(CCObject* pSender)
	{
		string strSrc = m_pSrcEdit->getText();
		string strDst = m_pDestEdit->getText();

		FILE* pSrc = fopen(strSrc.c_str(), "r+");
		if( pSrc == NULL )
		{
			m_pInfoLable->setString( A2U("源文件不存在").c_str() );
			return;
		}
		FILE* pDst = fopen(strDst.c_str(), "wb");
		if( pDst == NULL )
		{
			m_pInfoLable->setString( A2U("目标文件不存在").c_str() );
			fclose( pSrc );
			return;
		}
		int nRet = FKCW_Base_FileZip::Compress( pSrc, pDst, -1);
		if( nRet == 0 )
		{
			m_pInfoLable->setString( A2U("压缩完成").c_str() );
		}
		else
		{
			FKCW_Base_FileZip::OutputError( nRet );
			m_pInfoLable->setString( A2U("压缩失败，请查看控制台").c_str() );
		}
		fclose( pSrc );
		fclose( pDst );
	}
	void onClickDecompress(CCObject* pSender)
	{
		string strSrc = m_pSrcEdit->getText();
		string strDst = m_pDestEdit->getText();

		FILE* pSrc = fopen(strSrc.c_str(), "rb+");
		if( pSrc == NULL )
		{
			m_pInfoLable->setString( A2U("源文件不存在").c_str() );
			return;
		}
		FILE* pDst = fopen(strDst.c_str(), "w+");
		if( pDst == NULL )
		{
			m_pInfoLable->setString( A2U("目标文件不存在").c_str() );
			fclose( pSrc );
			return;
		}
		int nRet = FKCW_Base_FileZip::Decompress( pSrc, pDst );
		if( nRet == 0 )
		{
			m_pInfoLable->setString( A2U("解压缩完成").c_str() );
		}
		else
		{
			FKCW_Base_FileZip::OutputError( nRet );
			m_pInfoLable->setString( A2U("解压缩失败，请查看控制台").c_str() );
		}
		fclose( pSrc );
		fclose( pDst );
	}
public:
	CREATE_FUNC( CTest_Base_FileZipLayer );
protected:
	FKCW_UIWidget_WidgetWindow*		m_pWindow;
	CCEditBox*						m_pDestEdit;
	CCEditBox*						m_pSrcEdit;
	CCLabelTTF*						m_pInfoLable;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(Base, FileZip, 文件压缩类)
//-------------------------------------------------------------------------