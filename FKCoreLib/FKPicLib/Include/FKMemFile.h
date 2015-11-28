//*************************************************************************
//	创建日期:	2014-9-11   13:38
//	文件名称:	FKMemFile.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKPicCommonDef.h"
#include "FKFile.h"
//-------------------------------------------------------------------------
class CFKMemFile : public IFKFile
{
public:
	CFKMemFile( BYTE* p_byBuf = NULL, DWORD p_dwSize = 0 );
	~CFKMemFile();
public:
	bool					Open();
	BYTE*					GetBuffer( bool p_bDetachBuffer = true );
public:
	virtual bool			Close();
	virtual size_t			Read(void* p_szBuffer, size_t p_unSize, size_t p_unCount);
	virtual size_t			Write(const void* p_szBuffer, size_t p_unSize, size_t p_unCount);
	virtual bool			Seek(long p_lOffset, int p_nOrigin);
	virtual long			Tell();
	virtual long			Size();
	virtual bool			Flush();
	virtual bool			Eof();
	virtual long			Error();
	virtual long			GetC();
	virtual char*			GetS(char* p_szString, int p_nN );
	virtual long			Scanf(const char* p_szFormat, void* p_pOutput);
	virtual bool			PutC(unsigned char p_ucChar);
protected:
	bool					_Alloc( DWORD p_dwBytes );
	void					_Free();
protected:
	BYTE*					m_pBuffer;
	DWORD					m_dwSize;
	bool					m_bIsFreeOnClose;
	long					m_lPosition;
	long					m_lBufferSize;
};
//-------------------------------------------------------------------------