//--------------------------------------------------------------------
#include "../Include/FKCW_Base_Base64.h"
//--------------------------------------------------------------------
static char s_InAlphabets[256];
static char s_Base64DecodeTable[256];
static const char s_Base64Char[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
//--------------------------------------------------------------------
static void InitBase64DecodeTable() 
{
	for(int i = sizeof(s_Base64Char) - 1; i >= 0 ; i--) {
		s_InAlphabets[s_Base64Char[i]] = 1;
		s_Base64DecodeTable[s_Base64Char[i]] = i;
	}
}
//--------------------------------------------------------------------
static char* StrDup(char const* str) 
{
	if (str == NULL) return NULL;
	size_t len = strlen(str) + 1;
	char* copy = new char[len];
	if (copy != NULL) {
		memcpy(copy, str, len);
	}
	return copy;
}
//--------------------------------------------------------------------
static char* StrDupSize(char const* str) 
{
	if (str == NULL) return NULL;
	size_t len = strlen(str) + 1;
	char* copy = new char[len];
	return copy;
}
//--------------------------------------------------------------------
// 压缩一段数据
string FKCW_Base_Base64::Encode( const void* p_pData, int p_nLen )
{
	unsigned char const* orig = (unsigned char const*)p_pData; // in case any input bytes have the MSB set
	if (orig == NULL || p_nLen <= 0)
		return "";

	int numOrig24BitValues = p_nLen / 3;
	bool havePadding = p_nLen > numOrig24BitValues * 3;
	bool havePadding2 = p_nLen == numOrig24BitValues * 3 + 2;
	int numResultBytes = 4 * (numOrig24BitValues + havePadding);
	char* result = new char[numResultBytes + 1]; // allow for trailing '/0'

	// Map each full group of 3 input bytes into 4 output base-64 characters:
	int i;
	for (i = 0; i < numOrig24BitValues; ++i) {
		result[4 * i] = s_Base64Char[(orig[3 * i] >> 2) & 0x3F];
		result[4 * i + 1] = s_Base64Char[(((orig[3 * i] & 0x3) << 4) | (orig[3 * i + 1] >> 4)) & 0x3F];
		result[4 * i + 2] = s_Base64Char[((orig[3 * i + 1] << 2) | (orig[3 * i + 2] >> 6)) & 0x3F];
		result[4 * i + 3] = s_Base64Char[orig[3 * i + 2] & 0x3F];
	}

	// Now, take padding into account.  (Note: i == numOrig24BitValues)
	if (havePadding) {
		result[4 * i + 0] = s_Base64Char[(orig[3 * i] >> 2) & 0x3F];
		if (havePadding2) {
			result[4 * i + 1] = s_Base64Char[(((orig[3 * i] & 0x3) << 4) | (orig[3 * i + 1] >> 4)) & 0x3F];
			result[4 * i + 2] = s_Base64Char[(orig[3 * i + 1] << 2) & 0x3F];
		} else {
			result[4 * i + 1] = s_Base64Char[((orig[3 * i] & 0x3) << 4) & 0x3F];
			result[4 * i + 2] = '=';
		}
		result[4 * i + 3] = '=';
	}

	result[numResultBytes] = '\0';
	string ret = result;
	delete[] result;

	return ret;
}
//--------------------------------------------------------------------
// 解压缩一段数据
const char* FKCW_Base_Base64::Decode( const string& p_strData, int* p_pOutLen )
{
	// 初始化表
	static bool haveInitedBase64DecodeTable = false;
	if (!haveInitedBase64DecodeTable) {
		InitBase64DecodeTable();
		haveInitedBase64DecodeTable = true;
	}

	// 预处理
	int k = 0;
	char* result = NULL;
	const char* in = p_strData.c_str();
	size_t inLen = strlen(in);
	unsigned char* out = (unsigned char*)StrDupSize(in); // ensures we have enough space

	// 解密
	int char_count = 0;
	int bits = 0;
	unsigned char c;
	for(unsigned int i = 0; i < inLen; i++) {
		c = in[i];
		if (c == '=')
			break;
		if (c > 255 || !s_InAlphabets[c])
			continue;
		bits += s_Base64DecodeTable[c];
		char_count++;
		if (char_count == 4) {
			out[k++] = (bits >> 16);
			out[k++] = ((bits >> 8) & 0xff);
			out[k++] = (bits & 0xff);
			bits = 0;
			char_count = 0;
		} else {
			bits <<= 6;
		}
	}

	// 最后的片段
	if(c == '=') {
		switch (char_count) {
		case 2:
			out[k++] = bits >> 10;
			break;
		case 3:
			out[k++] = bits >> 16;
			out[k++] = (bits >> 8) & 0xff;
			break;
		}
	}

	// 创建返回数组
	if(k > 0) {
		result = (char*)malloc(sizeof(char) * k);
		memmove(result, out, k);
	}
	delete[] out;

	if(p_pOutLen)
		*p_pOutLen = k;

	return result;
}
//--------------------------------------------------------------------
// 解压缩一段数据（后端会补'/0'结束符）
const char* FKCW_Base_Base64::DecodeAsCString( const string& p_strData, int* p_pOutLen )
{
	int len;
	const char* v = Decode(p_strData, &len);

	if(p_pOutLen)
		*p_pOutLen = len;

	// 添加尾部结束'\0'
	char* r = (char*)malloc((len + 1) * sizeof(char));
	memcpy(r, v, len);
	r[len] = 0;

	free((void*)v);

	return r;
}
//--------------------------------------------------------------------