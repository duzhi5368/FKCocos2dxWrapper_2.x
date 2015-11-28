//--------------------------------------------------------------------
#include "../Include/FKCW_Base_SecureUserDefault.h"
#include "../Include/FKCW_Base_Base64.h"
//--------------------------------------------------------------------
static FKCW_Base_SecureUserDefault* s_Instance = NULL;
//--------------------------------------------------------------------
FKCW_Base_SecureUserDefault::FKCW_Base_SecureUserDefault()
{
}
//--------------------------------------------------------------------
FKCW_Base_SecureUserDefault::~FKCW_Base_SecureUserDefault()
{
	Purge();
}
//--------------------------------------------------------------------
FKCW_Base_SecureUserDefault* FKCW_Base_SecureUserDefault::GetInstance()
{
	if(!s_Instance) 
	{
		s_Instance = new FKCW_Base_SecureUserDefault();
	}
	return s_Instance;
}
//--------------------------------------------------------------------
// 初始化
void FKCW_Base_SecureUserDefault::Init( ENCRYPT_FUNC p_pEnFunc, DECRYPT_FUNC p_pDeFunc )
{
	FKCW_Base_SecureUserDefault* d = FKCW_Base_SecureUserDefault::GetInstance();
	d->m_pDecryptFunc = p_pDeFunc;
	d->m_pEncryptFunc = p_pEnFunc;
}
//--------------------------------------------------------------------
// 清空数据
void FKCW_Base_SecureUserDefault::Purge()
{
	if(s_Instance) 
	{
		delete s_Instance;
		s_Instance = NULL;
	}
}
//--------------------------------------------------------------------
// 刷新
void FKCW_Base_SecureUserDefault::Flush()
{
	CCUserDefault::sharedUserDefault()->flush();
}
//--------------------------------------------------------------------
const char* FKCW_Base_SecureUserDefault::_GetSecureValue(const char* pKey, int* outLen)
{
	string v = CCUserDefault::sharedUserDefault()->getStringForKey(pKey);
	int len;
	const char* dec = FKCW_Base_Base64::DecodeAsCString(v, &len);
	const char* plain = dec ? (*m_pDecryptFunc)(dec, len, outLen) : NULL;

	if(dec && plain != dec)
		free((void*)dec);

	return plain;
}
//--------------------------------------------------------------------
bool FKCW_Base_SecureUserDefault::getBoolForKey(const char* pKey) 
{
	return getBoolForKey(pKey, false);
}
//--------------------------------------------------------------------
bool FKCW_Base_SecureUserDefault::getBoolForKey(const char* pKey, bool defaultValue) 
{
	if(m_pDecryptFunc) 
	{
		int len;
		const char* plain = _GetSecureValue(pKey, &len);
		bool result = defaultValue;
		if(plain) {
			result = !strcmp(plain, "true");
		}

		if(plain)
			free((void*)plain);
		return result;
	} 
	else
	{
		return CCUserDefault::sharedUserDefault()->getBoolForKey(pKey, defaultValue);
	}
}
//--------------------------------------------------------------------
int FKCW_Base_SecureUserDefault::getIntegerForKey(const char* pKey) 
{
	return getIntegerForKey(pKey, 0);
}
//--------------------------------------------------------------------
int FKCW_Base_SecureUserDefault::getIntegerForKey(const char* pKey, int defaultValue) 
{
	if(m_pDecryptFunc) 
	{
		int len;
		const char* plain = _GetSecureValue(pKey, &len);
		int result = defaultValue;
		if(plain) 
		{
			sscanf(plain, "%d", &result);
		}

		if(plain)
			free((void*)plain);

		return result;
	}
	else 
	{
		return CCUserDefault::sharedUserDefault()->getIntegerForKey(pKey, defaultValue);
	}
}
//--------------------------------------------------------------------
float FKCW_Base_SecureUserDefault::getFloatForKey(const char* pKey) 
{
	return getFloatForKey(pKey, 0);
}
//--------------------------------------------------------------------
float FKCW_Base_SecureUserDefault::getFloatForKey(const char* pKey, float defaultValue) 
{
	if(m_pDecryptFunc) 
	{
		int len;
		const char* plain = _GetSecureValue(pKey, &len);
		float result = defaultValue;
		if(plain) 
		{
			sscanf(plain, "%f", &result);
		}

		if(plain)
			free((void*)plain);

		return result;
	}
	else 
	{
		return CCUserDefault::sharedUserDefault()->getFloatForKey(pKey, defaultValue);
	}
}
//--------------------------------------------------------------------
double FKCW_Base_SecureUserDefault::getDoubleForKey(const char* pKey) 
{
	return getDoubleForKey(pKey, 0);
}
//--------------------------------------------------------------------
double FKCW_Base_SecureUserDefault::getDoubleForKey(const char* pKey, double defaultValue) 
{
	if(m_pDecryptFunc) 
	{
		int len;
		const char* plain = _GetSecureValue(pKey, &len);
		double result = defaultValue;
		if(plain) 
		{
			sscanf(plain, "%lf", &result);
		}

		if(plain)
			free((void*)plain);

		return result;
	} 
	else 
	{
		return CCUserDefault::sharedUserDefault()->getDoubleForKey(pKey, defaultValue);
	}
}
//--------------------------------------------------------------------
string FKCW_Base_SecureUserDefault::getStringForKey(const char* pKey) 
{
	return getStringForKey(pKey, "");
}
//--------------------------------------------------------------------
string FKCW_Base_SecureUserDefault::getStringForKey(const char* pKey, const string& defaultValue) 
{
	if(m_pDecryptFunc) 
	{
		int len;
		const char* plain = _GetSecureValue(pKey, &len);
		if(plain)
		{
			string ret = plain;
			free((void*)plain);
			return ret;
		} 
		else
		{
			return defaultValue;
		}
	} 
	else 
	{
		return CCUserDefault::sharedUserDefault()->getStringForKey(pKey, defaultValue);
	}
}
//--------------------------------------------------------------------
void FKCW_Base_SecureUserDefault::setBoolForKey(const char* pKey, bool value) 
{
	if(m_pEncryptFunc) 
	{
		int encLen;
		const char* enc = (*m_pEncryptFunc)(value ? "true" : "false", value ? 4 : 5, &encLen);
		string b64 = FKCW_Base_Base64::Encode(enc, encLen);
		CCUserDefault::sharedUserDefault()->setStringForKey(pKey, b64);

		if(enc)
			free((void*)enc);
	} 
	else 
	{
		CCUserDefault::sharedUserDefault()->setBoolForKey(pKey, value);
	}
}
//--------------------------------------------------------------------
void FKCW_Base_SecureUserDefault::setIntegerForKey(const char* pKey, int value)
{
	if(m_pEncryptFunc) 
	{
		char buf[32];
		sprintf(buf, "%d", value);
		int encLen;
		const char* enc = (*m_pEncryptFunc)(buf, (int)strlen(buf), &encLen);
		string b64 = FKCW_Base_Base64::Encode(enc, encLen);
		CCUserDefault::sharedUserDefault()->setStringForKey(pKey, b64);

		if(enc && buf != enc)
			free((void*)enc);
	} 
	else 
	{
		CCUserDefault::sharedUserDefault()->setIntegerForKey(pKey, value);
	}
}
//--------------------------------------------------------------------
void FKCW_Base_SecureUserDefault::setFloatForKey(const char* pKey, float value) 
{
	if(m_pEncryptFunc) 
	{
		char buf[64];
		sprintf(buf, "%f", value);
		int encLen;
		const char* enc = (*m_pEncryptFunc)(buf, (int)strlen(buf), &encLen);
		string b64 = FKCW_Base_Base64::Encode(enc, encLen);
		CCUserDefault::sharedUserDefault()->setStringForKey(pKey, b64);

		if(enc && buf != enc)
			free((void*)enc);
	} 
	else 
	{
		CCUserDefault::sharedUserDefault()->setFloatForKey(pKey, value);
	}
}
//--------------------------------------------------------------------
void FKCW_Base_SecureUserDefault::setDoubleForKey(const char* pKey, double value) 
{
	if(m_pEncryptFunc)
	{
		char buf[128];
		sprintf(buf, "%lf", value);
		int encLen;
		const char* enc = (*m_pEncryptFunc)(buf, (int)strlen(buf), &encLen);
		string b64 = FKCW_Base_Base64::Encode(enc, encLen);
		CCUserDefault::sharedUserDefault()->setStringForKey(pKey, b64);

		if(enc && buf != enc)
			free((void*)enc);
	} 
	else 
	{
		CCUserDefault::sharedUserDefault()->setDoubleForKey(pKey, value);
	}
}
//--------------------------------------------------------------------
void FKCW_Base_SecureUserDefault::setStringForKey(const char* pKey, const string& value) 
{
	if(m_pEncryptFunc)
	{
		int encLen;
		const char* enc = (*m_pEncryptFunc)(value.c_str(), (int)value.length(), &encLen);
		string b64 = FKCW_Base_Base64::Encode(enc, encLen);
		CCUserDefault::sharedUserDefault()->setStringForKey(pKey, b64);

		if(enc && value.c_str() != enc)
			free((void*)enc);   
	} 
	else 
	{
		CCUserDefault::sharedUserDefault()->setStringForKey(pKey, value);
	}
}
//--------------------------------------------------------------------