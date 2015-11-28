//*************************************************************************
//	创建日期:	2014-12-23
//	文件名称:	Test_Base_SecureUserDefault.h
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
const char* decrypt(const char* p_szEnc, int p_nEncLen, int* p_pPlainLen )
{
	char* dec = (char*)malloc(p_nEncLen+1 * sizeof(char));
	for(int i = 0; i < p_nEncLen; i++) {
		dec[i] = ~p_szEnc[i];
	}
	// 避免尾部无\0结束符
	dec[p_nEncLen] = '\0';
	if(p_pPlainLen)
		*p_pPlainLen = p_nEncLen+1;

	return dec;
}
const char* encrypt(const char* p_pPlain, int p_nPlainLen, int* p_pEncLen )
{
	char* enc = (char*)malloc(p_nPlainLen * sizeof(char));
	for(int i = 0; i < p_nPlainLen; i++) {
		enc[i] = ~p_pPlain[i];
	}
	if(p_pEncLen)
		*p_pEncLen = p_nPlainLen;
	return enc;
}
//-------------------------------------------------------------------------
class CTest_Base_SecureUserDefaultLayer : public CCLayer
{
public:
	virtual bool init()
	{
		FKCW_Base_SecureUserDefault::Init(encrypt, decrypt);
		FKCW_Base_SecureUserDefault::GetInstance()->setStringForKey("testkey", "testvalue你好世界");

		string v = CCUserDefault::sharedUserDefault()->getStringForKey("testkey");
		char buf[1024];
		sprintf(buf, "CCUserDefault加载得到:  %s", v.c_str());
		CCLabelTTF* label = CCLabelTTF::create( A2U(buf).c_str(), "Helvetica", 30);
		label->setPosition(SCREEN_CENTER_POS+ccp(0, 100));
		addChild(label);

		v = FKCW_Base_SecureUserDefault::GetInstance()->getStringForKey("testkey");
		memset( buf, 0, 1024 );
		sprintf(buf, "CCSecureUserDefault加载得到:  %s", v.c_str());
		label = CCLabelTTF::create( A2U(buf).c_str(), "Helvetica", 30);
		label->setPosition(SCREEN_CENTER_POS+ccp(0,-100));
		addChild(label);

		return true;
	}
public:
	CREATE_FUNC( CTest_Base_SecureUserDefaultLayer );
};
//-------------------------------------------------------------------------
CREATE_TEST_SCENE_CLASS(Base, SecureUserDefault, 带加密的基本配置文件)
//-------------------------------------------------------------------------