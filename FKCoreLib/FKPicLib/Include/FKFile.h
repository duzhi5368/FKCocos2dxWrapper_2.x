//*************************************************************************
//	创建日期:	2014-9-11   9:41
//	文件名称:	FKFile.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
class IFKFile
{
public:
	IFKFile(){};
	virtual ~IFKFile(){};
public:
	virtual bool			Close() = 0;
	virtual size_t			Read(void* p_szBuffer, size_t p_unSize, size_t p_unCount) = 0;
	virtual size_t			Write(const void* p_szBuffer, size_t p_unSize, size_t p_unCount) = 0;
	virtual bool			Seek(long p_lOffset, int p_nOrigin) = 0;
	virtual long			Tell() = 0;
	virtual long			Size() = 0;
	virtual bool			Flush() = 0;
	virtual bool			Eof() = 0;
	virtual long			Error() = 0;
	virtual long			GetC() = 0;
	virtual char*			GetS(char* p_szString, int p_nN ) = 0;
	virtual long			Scanf(const char* p_szFormat, void* p_pOutput) = 0;
	virtual bool			PutC(unsigned char p_ucChar) = 0;
};
//-------------------------------------------------------------------------