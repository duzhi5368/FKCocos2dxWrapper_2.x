//-------------------------------------------------------------------------
#include "../Include/FKCW_Base_SHA1.h"
//-------------------------------------------------------------------------
#define SHA1_MAX_FILE_BUFFER 8000
//-------------------------------------------------------------------------
#define ROL32(_val32, _nBits) (((_val32)<<(_nBits))|((_val32)>>(32-(_nBits))))
//-------------------------------------------------------------------------
#ifdef SHA1_LITTLE_ENDIAN
#define SHABLK0(i) (m_pBlock->l[i] = \
	(ROL32(m_pBlock->l[i],24) & 0xFF00FF00) | (ROL32(m_pBlock->l[i],8) & 0x00FF00FF))
#else
#define SHABLK0(i) (m_pBlock->l[i])
#endif
//-------------------------------------------------------------------------
#define SHABLK(i) (m_pBlock->l[i&15] = ROL32(m_pBlock->l[(i+13)&15] ^ m_pBlock->l[(i+8)&15] \
	^ m_pBlock->l[(i+2)&15] ^ m_pBlock->l[i&15],1))
//-------------------------------------------------------------------------
// SHA-1 rounds
#define _R0(v,w,x,y,z,i) { z+=((w&(x^y))^y)+SHABLK0(i)+0x5A827999+ROL32(v,5); w=ROL32(w,30); }
#define _R1(v,w,x,y,z,i) { z+=((w&(x^y))^y)+SHABLK(i)+0x5A827999+ROL32(v,5); w=ROL32(w,30); }
#define _R2(v,w,x,y,z,i) { z+=(w^x^y)+SHABLK(i)+0x6ED9EBA1+ROL32(v,5); w=ROL32(w,30); }
#define _R3(v,w,x,y,z,i) { z+=(((w|x)&y)|(w&x))+SHABLK(i)+0x8F1BBCDC+ROL32(v,5); w=ROL32(w,30); }
#define _R4(v,w,x,y,z,i) { z+=(w^x^y)+SHABLK(i)+0xCA62C1D6+ROL32(v,5); w=ROL32(w,30); }
//-------------------------------------------------------------------------
void FKCW_Base_HMAC_SHA1::HMAC_SHA1(unsigned char *text, int text_len, 
									unsigned char*key, int key_len, unsigned char *digest)
{
	memset(SHA1_Key, 0, SHA1_BLOCK_SIZE);

	memset(m_ipad, 0x36, sizeof(m_ipad));
	memset(m_opad, 0x5c, sizeof(m_opad));

	/* 步骤一 */
	if (key_len > SHA1_BLOCK_SIZE)
	{
		FKCW_Base_SHA1::Reset();
		FKCW_Base_SHA1::SHA1Hash((UINT_8 *)key, key_len);
		FKCW_Base_SHA1::Final();

		FKCW_Base_SHA1::GetHashValue((UINT_8 *)SHA1_Key);
	}
	else
		memcpy(SHA1_Key, key, key_len);

	/* 步骤二 */
	for (size_t i=0; i<sizeof(m_ipad); i++)
	{
		m_ipad[i] ^= SHA1_Key[i];		
	}

	/* 步骤三 */
	memcpy(AppendBuf1, m_ipad, sizeof(m_ipad));
	memcpy(AppendBuf1 + sizeof(m_ipad), text, text_len);

	/* 步骤四 */
	FKCW_Base_SHA1::Reset();
	FKCW_Base_SHA1::SHA1Hash((UINT_8 *)AppendBuf1, sizeof(m_ipad) + text_len);
	FKCW_Base_SHA1::Final();

	FKCW_Base_SHA1::GetHashValue((UINT_8 *)szReport);

	/* 步骤五 */
	for (size_t j=0; j<sizeof(m_opad); j++)
	{
		m_opad[j] ^= SHA1_Key[j];
	}

	/* 步骤六 */
	memcpy(AppendBuf2, m_opad, sizeof(m_opad));
	memcpy(AppendBuf2 + sizeof(m_opad), szReport, SHA1_DIGEST_LENGTH);

	/* 步骤七 */
	FKCW_Base_SHA1::Reset();
	FKCW_Base_SHA1::SHA1Hash((UINT_8 *)AppendBuf2, sizeof(m_opad) + SHA1_DIGEST_LENGTH);
	FKCW_Base_SHA1::Final();

	FKCW_Base_SHA1::GetHashValue((UINT_8 *)digest);
}
//-------------------------------------------------------------------------
// FKCW_Base_SHA1
//-------------------------------------------------------------------------
FKCW_Base_SHA1::FKCW_Base_SHA1()
{
	m_pBlock = (USHA1WorkspaceBlock *)m_pWorkspace;
	Reset();
}
//-------------------------------------------------------------------------
FKCW_Base_SHA1::~FKCW_Base_SHA1()
{
	Reset();
}
//-------------------------------------------------------------------------
void FKCW_Base_SHA1::Reset()
{
	// SHA1初始常量
	m_state[0] = 0x67452301;
	m_state[1] = 0xEFCDAB89;
	m_state[2] = 0x98BADCFE;
	m_state[3] = 0x10325476;
	m_state[4] = 0xC3D2E1F0;

	m_count[0] = 0;
	m_count[1] = 0;
}
//-------------------------------------------------------------------------
void FKCW_Base_SHA1::__Transform(UINT_32 *state, UINT_8 *buffer)
{
	// 拷贝到工作区域变量
	UINT_32 a = state[0], b = state[1], c = state[2], d = state[3], e = state[4];

	memcpy(m_pBlock, buffer, 64);

	_R0(a,b,c,d,e, 0); _R0(e,a,b,c,d, 1); _R0(d,e,a,b,c, 2); _R0(c,d,e,a,b, 3);
	_R0(b,c,d,e,a, 4); _R0(a,b,c,d,e, 5); _R0(e,a,b,c,d, 6); _R0(d,e,a,b,c, 7);
	_R0(c,d,e,a,b, 8); _R0(b,c,d,e,a, 9); _R0(a,b,c,d,e,10); _R0(e,a,b,c,d,11);
	_R0(d,e,a,b,c,12); _R0(c,d,e,a,b,13); _R0(b,c,d,e,a,14); _R0(a,b,c,d,e,15);
	_R1(e,a,b,c,d,16); _R1(d,e,a,b,c,17); _R1(c,d,e,a,b,18); _R1(b,c,d,e,a,19);
	_R2(a,b,c,d,e,20); _R2(e,a,b,c,d,21); _R2(d,e,a,b,c,22); _R2(c,d,e,a,b,23);
	_R2(b,c,d,e,a,24); _R2(a,b,c,d,e,25); _R2(e,a,b,c,d,26); _R2(d,e,a,b,c,27);
	_R2(c,d,e,a,b,28); _R2(b,c,d,e,a,29); _R2(a,b,c,d,e,30); _R2(e,a,b,c,d,31);
	_R2(d,e,a,b,c,32); _R2(c,d,e,a,b,33); _R2(b,c,d,e,a,34); _R2(a,b,c,d,e,35);
	_R2(e,a,b,c,d,36); _R2(d,e,a,b,c,37); _R2(c,d,e,a,b,38); _R2(b,c,d,e,a,39);
	_R3(a,b,c,d,e,40); _R3(e,a,b,c,d,41); _R3(d,e,a,b,c,42); _R3(c,d,e,a,b,43);
	_R3(b,c,d,e,a,44); _R3(a,b,c,d,e,45); _R3(e,a,b,c,d,46); _R3(d,e,a,b,c,47);
	_R3(c,d,e,a,b,48); _R3(b,c,d,e,a,49); _R3(a,b,c,d,e,50); _R3(e,a,b,c,d,51);
	_R3(d,e,a,b,c,52); _R3(c,d,e,a,b,53); _R3(b,c,d,e,a,54); _R3(a,b,c,d,e,55);
	_R3(e,a,b,c,d,56); _R3(d,e,a,b,c,57); _R3(c,d,e,a,b,58); _R3(b,c,d,e,a,59);
	_R4(a,b,c,d,e,60); _R4(e,a,b,c,d,61); _R4(d,e,a,b,c,62); _R4(c,d,e,a,b,63);
	_R4(b,c,d,e,a,64); _R4(a,b,c,d,e,65); _R4(e,a,b,c,d,66); _R4(d,e,a,b,c,67);
	_R4(c,d,e,a,b,68); _R4(b,c,d,e,a,69); _R4(a,b,c,d,e,70); _R4(e,a,b,c,d,71);
	_R4(d,e,a,b,c,72); _R4(c,d,e,a,b,73); _R4(b,c,d,e,a,74); _R4(a,b,c,d,e,75);
	_R4(e,a,b,c,d,76); _R4(d,e,a,b,c,77); _R4(c,d,e,a,b,78); _R4(b,c,d,e,a,79);

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
	state[4] += e;

	a = b = c = d = e = 0;
}
//-------------------------------------------------------------------------
void FKCW_Base_SHA1::SHA1Hash(UINT_8 *data, UINT_32 len)
{
	UINT_32 i, j;

	j = (m_count[0] >> 3) & 63;

	if((m_count[0] += len << 3) < (len << 3)) m_count[1]++;

	m_count[1] += (len >> 29);

	if((j + len) > 63)
	{
		i = 64 - j;
		memcpy(&m_buffer[j], data, i);
		__Transform(m_state, m_buffer);

		for(; i + 63 < len; i += 64) __Transform(m_state, &data[i]);

		j = 0;
	}
	else i = 0;

	memcpy(&m_buffer[j], &data[i], len - i);
}
//-------------------------------------------------------------------------
bool FKCW_Base_SHA1::SHA1Hash(char *szFileName)
{
	unsigned long ulFileSize, ulRest, ulBlocks;
	unsigned long i;
	UINT_8 uData[SHA1_MAX_FILE_BUFFER];
	FILE *fIn;

	if(szFileName == NULL) return false;

	fIn = fopen(szFileName, "rb");
	if(fIn == NULL) return false;

	fseek(fIn, 0, SEEK_END);
	ulFileSize = (unsigned long)ftell(fIn);
	fseek(fIn, 0, SEEK_SET);

	if(ulFileSize != 0)
	{
		ulBlocks = ulFileSize / SHA1_MAX_FILE_BUFFER;
		ulRest = ulFileSize % SHA1_MAX_FILE_BUFFER;
	}
	else
	{
		ulBlocks = 0;
		ulRest = 0;
	}

	for(i = 0; i < ulBlocks; i++)
	{
		fread(uData, 1, SHA1_MAX_FILE_BUFFER, fIn);
		SHA1Hash((UINT_8 *)uData, SHA1_MAX_FILE_BUFFER);
	}

	if(ulRest != 0)
	{
		fread(uData, 1, ulRest, fIn);
		SHA1Hash((UINT_8 *)uData, ulRest);
	}

	fclose(fIn); fIn = NULL;
	return true;
}
//-------------------------------------------------------------------------
void FKCW_Base_SHA1::Final()
{
	UINT_32 i;
	UINT_8 finalcount[8];

	for(i = 0; i < 8; i++)
		finalcount[i] = (UINT_8)((m_count[((i >= 4) ? 0 : 1)]
	>> ((3 - (i & 3)) * 8) ) & 255);

	SHA1Hash((UINT_8 *)"\200", 1);

	while ((m_count[0] & 504) != 448)
		SHA1Hash((UINT_8 *)"\0", 1);

	SHA1Hash(finalcount, 8);

	for(i = 0; i < 20; i++)
	{
		m_digest[i] = (UINT_8)((m_state[i >> 2] >> ((3 - (i & 3)) * 8) ) & 255);
	}

	i = 0;
	memset(m_buffer, 0, 64);
	memset(m_state, 0, 20);
	memset(m_count, 0, 8);
	memset(finalcount, 0, 8);
	__Transform(m_state, m_buffer);
}
//-------------------------------------------------------------------------
// 获取最终Hash值，并将其转为一段字符串
void FKCW_Base_SHA1::GetHashValue(char *szReport, unsigned char uReportType)
{
	unsigned char i;
	char szTemp[16];

	if(szReport == NULL) return;

	if(uReportType == eReportHashFormat_Hex)
	{
		sprintf(szTemp, "%02X", m_digest[0]);
		strcat(szReport, szTemp);

		for(i = 1; i < 20; i++)
		{
			sprintf(szTemp, " %02X", m_digest[i]);
			strcat(szReport, szTemp);
		}
	}
	else if(uReportType == eReportHashFormat_Digit)
	{
		sprintf(szTemp, "%u", m_digest[0]);
		strcat(szReport, szTemp);

		for(i = 1; i < 20; i++)
		{
			sprintf(szTemp, " %u", m_digest[i]);
			strcat(szReport, szTemp);
		}
	}
	else strcpy(szReport, "Error: Unknown report type!");
}
//-------------------------------------------------------------------------
// 获取最终数据
void FKCW_Base_SHA1::GetHashValue(UINT_8 *puDest)
{
	memcpy(puDest, m_digest, 20);
}
//-------------------------------------------------------------------------