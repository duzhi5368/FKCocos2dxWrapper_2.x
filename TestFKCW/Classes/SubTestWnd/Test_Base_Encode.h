//*************************************************************************
//	创建日期:	2014-12-22
//	文件名称:	Test_Base_Encode.h
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
class CTest_Base_EncodeLayer : public CCLayer
{
public:
	virtual bool init()
	{
		m_pWindow = FKCW_UIWidget_WidgetWindow::create();
		m_pWindow->setMultiTouchEnabled(true);
		addChild(m_pWindow);

		FKCW_UIWidget_Button* pButtonMD5 = FKCW_UIWidget_Button::createWith9Sprite(CCSize(150, 50),
			s_szSprite9Btn1Path, s_szSprite9Btn2Path );
		pButtonMD5->setPosition(SCREEN_CENTER_POS + ccp(-380, 0));
		pButtonMD5->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_Base_EncodeLayer::onClickMD5));
		pButtonMD5->initText(FKCW_Base_StringConvert::a2u("计算MD5").c_str(), "", 30);
		m_pWindow->addChild(pButtonMD5);

		FKCW_UIWidget_Button* pButtonSHA1 = FKCW_UIWidget_Button::createWith9Sprite(CCSize(150, 50),
			s_szSprite9Btn1Path, s_szSprite9Btn2Path );
		pButtonSHA1->setPosition(SCREEN_CENTER_POS + ccp(-190, 0));
		pButtonSHA1->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_Base_EncodeLayer::onClickSHA1));
		pButtonSHA1->initText(FKCW_Base_StringConvert::a2u("计算字符SHA1").c_str(), "", 30);
		m_pWindow->addChild(pButtonSHA1);

		FKCW_UIWidget_Button* pButtonHMAC_SHA1 = FKCW_UIWidget_Button::createWith9Sprite(CCSize(150, 50),
			s_szSprite9Btn1Path, s_szSprite9Btn2Path );
		pButtonHMAC_SHA1->setPosition(SCREEN_CENTER_POS + ccp(0, 0));
		pButtonHMAC_SHA1->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_Base_EncodeLayer::onClickHMAC_SHA1));
		pButtonHMAC_SHA1->initText(FKCW_Base_StringConvert::a2u("计算HAMCSHA1").c_str(), "", 30);
		m_pWindow->addChild(pButtonHMAC_SHA1);

		FKCW_UIWidget_Button* pButtonFileSHA1 = FKCW_UIWidget_Button::createWith9Sprite(CCSize(150, 50),
			s_szSprite9Btn1Path, s_szSprite9Btn2Path );
		pButtonFileSHA1->setPosition(SCREEN_CENTER_POS + ccp(190, 0));
		pButtonFileSHA1->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_Base_EncodeLayer::onClickFileSHA1));
		pButtonFileSHA1->initText(FKCW_Base_StringConvert::a2u("计算文件SHA1").c_str(), "", 30);
		m_pWindow->addChild(pButtonFileSHA1);

		FKCW_UIWidget_Button* pButtonFileBase64 = FKCW_UIWidget_Button::createWith9Sprite(CCSize(150, 50),
			s_szSprite9Btn1Path, s_szSprite9Btn2Path );
		pButtonFileBase64->setPosition(SCREEN_CENTER_POS + ccp(380, 0));
		pButtonFileBase64->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_Base_EncodeLayer::onClickFileBase64Encode));
		pButtonFileBase64->initText(FKCW_Base_StringConvert::a2u("Base64加密").c_str(), "", 30);
		m_pWindow->addChild(pButtonFileBase64);

		FKCW_UIWidget_Button* pButtonFileBase64De = FKCW_UIWidget_Button::createWith9Sprite(CCSize(150, 50),
			s_szSprite9Btn1Path, s_szSprite9Btn2Path );
		pButtonFileBase64De->setPosition(SCREEN_CENTER_POS + ccp(-380, -100));
		pButtonFileBase64De->setOnClickListener(this, fkcw_uiwidget_click_selector(CTest_Base_EncodeLayer::onClickFileBase64Denode));
		pButtonFileBase64De->initText(FKCW_Base_StringConvert::a2u("解密Base64").c_str(), "", 30);
		m_pWindow->addChild(pButtonFileBase64De);

		CCScale9Sprite* pEditBoxBG1 = CCScale9Sprite::create(s_szSprite9Btn1Path);
		m_pSrcEdit = CCEditBox::create(CCSizeMake(800, 50), pEditBoxBG1 );
		m_pSrcEdit->setPosition( SCREEN_CENTER_POS + ccp(0, 100));
		m_pSrcEdit->setFontSize( 48 );
		m_pSrcEdit->setText(FKCW_Base_StringConvert::a2u("这里输入源数据").c_str());
		m_pWindow->addChild( m_pSrcEdit );

		CCScale9Sprite* pEditBoxBG2 = CCScale9Sprite::create(s_szSprite9Btn1Path);
		m_pDestEdit = CCEditBox::create(CCSizeMake(800, 50), pEditBoxBG2 );
		m_pDestEdit->setPosition( SCREEN_CENTER_POS + ccp(0, -200) );
		m_pDestEdit->setFontColor( ccRED );
		m_pDestEdit->setFontSize( 48 );
		m_pDestEdit->setText(FKCW_Base_StringConvert::a2u("这里输出处理后的数据").c_str());
		m_pWindow->addChild( m_pDestEdit );

		return true;
	}
public:
	void onClickMD5(CCObject* pSender)
	{
		string strDst = FKCW_Base_MD5::md5( m_pSrcEdit->getText() );
		m_pDestEdit->setText( strDst.c_str() );
	}

	void onClickSHA1(CCObject* pSender)
	{
		FKCW_Base_SHA1* p =  new FKCW_Base_SHA1();

		string strSrc = m_pSrcEdit->getText();
		unsigned char szSrc[128]="";
		strcpy((char*)szSrc, strSrc.c_str());

		unsigned char szDst[FKCW_Base_HMAC_SHA1::SHA1_DIGEST_LENGTH+1];
		memset( szDst, 0, FKCW_Base_HMAC_SHA1::SHA1_DIGEST_LENGTH+1 );

		p->Reset();
		p->SHA1Hash( szSrc, strSrc.length() );
		p->Final();
		p->GetHashValue( szDst );
		string strHex = FKCW_Base_Utils::Bin2Hex( szDst, FKCW_Base_HMAC_SHA1::SHA1_DIGEST_LENGTH );
		m_pDestEdit->setText( strHex.c_str() );

		if( p != NULL )
		{
			delete p;
			p = NULL;
		}
	}

	void onClickHMAC_SHA1(CCObject* pSender)
	{
		string strSrc = m_pSrcEdit->getText();
		unsigned char szSrc[128]="";
		strcpy((char*)szSrc, strSrc.c_str());

		unsigned char szDst[FKCW_Base_HMAC_SHA1::SHA1_DIGEST_LENGTH+1];
		memset( szDst, 0, FKCW_Base_HMAC_SHA1::SHA1_DIGEST_LENGTH+1 );

		char szTmpKey[FKCW_Base_HMAC_SHA1::SHA1_BLOCK_SIZE];
		//strcpy_s( szTmpKey, "TestKey" );
		sprintf(szTmpKey,"%s","TestKey" );
		string strKey = szTmpKey;

		FKCW_Base_HMAC_SHA1* p = new FKCW_Base_HMAC_SHA1();
		p->HMAC_SHA1( szSrc, strSrc.length(),
			(unsigned char*)szTmpKey, strKey.length(), szDst);
		string strHex = FKCW_Base_Utils::Bin2Hex( szDst, FKCW_Base_HMAC_SHA1::SHA1_DIGEST_LENGTH );
		m_pDestEdit->setText( strHex.c_str() );
		if( p != NULL )
		{
			delete p;
			p = NULL;
		}
	}

	void onClickFileSHA1(CCObject* pSender)
	{
		FKCW_Base_SHA1* p =  new FKCW_Base_SHA1();

		string strSrc = m_pSrcEdit->getText();
		char szSrc[128]="";
		strcpy(szSrc, strSrc.c_str());

		unsigned char szDst[FKCW_Base_HMAC_SHA1::SHA1_DIGEST_LENGTH+1];
		memset( szDst, 0, FKCW_Base_HMAC_SHA1::SHA1_DIGEST_LENGTH+1 );

		p->Reset();
		if( p->SHA1Hash( szSrc ) )
		{
			p->Final();
			p->GetHashValue( szDst );
			string strHex = FKCW_Base_Utils::Bin2Hex( szDst, FKCW_Base_HMAC_SHA1::SHA1_DIGEST_LENGTH );
			m_pDestEdit->setText( strHex.c_str() );
		}
		else
		{
			m_pDestEdit->setText( FKCW_Base_StringConvert::a2u("不是有效文件").c_str() );
		}

		if( p != NULL )
		{
			delete p;
			p = NULL;
		}
	}

	void onClickFileBase64Encode(CCObject* pSender)
	{
		string strSrc = m_pSrcEdit->getText();
		string strDst = FKCW_Base_Base64::Encode( (void*)strSrc.c_str(), strSrc.length() );
		m_pDestEdit->setText( strDst.c_str() );
	}

	void onClickFileBase64Denode(CCObject* pSender)
	{
		string strSrc = m_pSrcEdit->getText();
		int nOutLen = 0;
		string strDst = FKCW_Base_Base64::DecodeAsCString( strSrc, &nOutLen );
		m_pDestEdit->setText( strDst.c_str() );
	}
public:
	CREATE_FUNC( CTest_Base_EncodeLayer );
private:
	FKCW_UIWidget_WidgetWindow* m_pWindow;
	CCEditBox* m_pDestEdit;
	CCEditBox* m_pSrcEdit;
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(Base, Encode, 加密解密测试)
//-------------------------------------------------------------------------