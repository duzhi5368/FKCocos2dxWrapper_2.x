//*************************************************************************
//	创建日期:	2015-4-1
//	文件名称:	FKCW_IO_AssetInputStream_android.h
//  创 建 人:   邓观福 Rect 	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//-------------------------------------------------------------------------
#include "../Include/FKCW_IO_AssetInputStream.h"
//-------------------------------------------------------------------------
class FKCW_IO_AssetInputStream_android : public FKCW_IO_AssetInputStream {
	friend class FKCW_IO_AssetInputStream;

private:
    char*  m_buffer;
	size_t m_length;
    size_t m_position;

public:

	FKCW_IO_AssetInputStream_android(const string& path);

public:
	virtual ~FKCW_IO_AssetInputStream_android();

	virtual char*	GetBuffer();
	virtual size_t	GetPosition();
	virtual size_t	GetLength();
	virtual size_t	GetAvailableLength();
	virtual void	Close();
	virtual ssize_t Read(char* buffer, size_t length);
	virtual size_t	Seek(int offset, int mode);
};
//-------------------------------------------------------------------------
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//-------------------------------------------------------------------------