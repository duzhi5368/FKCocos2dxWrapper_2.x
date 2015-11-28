/********************************************************************
	created:	2014/10/27
	file base:	FKCW_Base_SecureUserDefault
	author:		FreeKnight
	
	purpose:	
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_Base_Macro.h"
#include "FKCW_Base_Types.h"
//--------------------------------------------------------------------
// 本身类似 CCUserDefault，如果不进行加解密函数处理的话，则和 CCUserDefault 完全一致
class FKCW_Base_SecureUserDefault
{
private:
	DECRYPT_FUNC m_pDecryptFunc;		// 解密函数
	ENCRYPT_FUNC m_pEncryptFunc;		// 加密函数
protected:
	FKCW_Base_SecureUserDefault();
	const char* _GetSecureValue(const char* p_szKey, int* p_pOutLen);
public:
	virtual ~FKCW_Base_SecureUserDefault();
	static	FKCW_Base_SecureUserDefault* GetInstance();
public:
	// 初始化
	static void Init( ENCRYPT_FUNC p_pEnFunc, DECRYPT_FUNC p_pDeFunc );
	// 清空数据
	static void Purge();
	// 刷新
	void Flush();
public:
	bool	getBoolForKey(const char* pKey);
	bool	getBoolForKey(const char* pKey, bool defaultValue);
	int		getIntegerForKey(const char* pKey);
	int		getIntegerForKey(const char* pKey, int defaultValue);
	float	getFloatForKey(const char* pKey);
	float	getFloatForKey(const char* pKey, float defaultValue);
	double	getDoubleForKey(const char* pKey);
	double	getDoubleForKey(const char* pKey, double defaultValue);
	string	getStringForKey(const char* pKey);
	string	getStringForKey(const char* pKey, const string& defaultValue);
public:
	void	setBoolForKey(const char* pKey, bool value);
	void	setIntegerForKey(const char* pKey, int value);
	void	setFloatForKey(const char* pKey, float value);
	void	setDoubleForKey(const char* pKey, double value);
	void	setStringForKey(const char* pKey, const string& value);
};