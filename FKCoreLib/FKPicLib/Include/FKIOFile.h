//*************************************************************************
//	创建日期:	2014-9-11   9:56
//	文件名称:	FKIOFile.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKPicCommonDef.h"
#include "FKFile.h"
//-------------------------------------------------------------------------
class CFKIOFile : public IFKFile
{
public:
	CFKIOFile( FILE* p_pFile = NULL );
	~CFKIOFile();
public:
	bool					Open( const char* p_szFileName, const char* p_szMode );
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
	FILE*					m_pFile;
	bool					m_bCloseFile;
};
//-------------------------------------------------------------------------