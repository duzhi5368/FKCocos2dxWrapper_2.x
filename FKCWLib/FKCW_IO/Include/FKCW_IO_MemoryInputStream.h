//*************************************************************************
//	创建日期:	2014-10-24
//	文件名称:	FKCW_IO_MemoryInputStream.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_IO_AssetInputStream.h"
//-------------------------------------------------------------------------
class FKCW_IO_MemoryInputStream : public FKCW_IO_AssetInputStream
{
protected:
	char*			m_szBuffer;			// 缓冲区
	size_t			m_unLength;			// 缓冲区长度
	size_t			m_unPosition;		// 当前读指针位置
	bool			m_bIsSelfRelease;	// 缓冲区是否自己销毁
protected:
	FKCW_IO_MemoryInputStream( char* p_szBuffer, size_t p_unLength, bool p_bIsSelfRelease );
public:
	static FKCW_IO_MemoryInputStream* Create( char* p_szBuffer, size_t p_unLength, bool p_bIsSelfRelease = false );
	virtual ~FKCW_IO_MemoryInputStream();
public:
	// 获取数据缓冲区指针
	virtual char*		GetBuffer();
	// 获取当前距离文件头的偏移量
	virtual size_t		GetPosition();
	// 获取全数据长度
	virtual size_t		GetLength();
	// 获取有效数据长度
	virtual size_t		GetAvailableLength();
	// 关闭一个资源
	virtual void		Close();
	// 读取一段数据到缓冲区
	// 返回值：-1 错误；0 无数据；正值 实际读入的数据量
	virtual ssize_t		Read( char* p_szBuffer, size_t p_unLength );
	// 更新读取指针位置
	// 参数：p_nOffset 偏移量
	// 参数：p_nMode SEEK_CUR SEEK_END SEEK_SET 之一
	// 返回值：偏移之后，和文件头的相对偏移量
	virtual size_t		Seek( int p_nOffset, int p_nMode );
};