//*************************************************************************
//	创建日期:	2014-12-23
//	文件名称:	FKCW_IO_AssetOutputStream_Win.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
//-------------------------------------------------------------------------
#include "../Include/FKCW_IO_AssetOutputStream.h"
//-------------------------------------------------------------------------
class FKCW_IO_AssetOutputStream_Win : public FKCW_IO_AssetOutputStream
{
	friend class FKCW_IO_AssetOutputStream;
private:
	FILE*		m_fp;
public:
	FKCW_IO_AssetOutputStream_Win(const string& path, bool append = false);

public:
	virtual ~FKCW_IO_AssetOutputStream_Win();

	virtual void	Close();
	virtual ssize_t Write(const char* data, size_t len);
	virtual ssize_t Write(const int* data, size_t len);
	virtual size_t	GetPosition();
	virtual size_t	Seek(int offset, int mode);
};
//-------------------------------------------------------------------------
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
//-------------------------------------------------------------------------