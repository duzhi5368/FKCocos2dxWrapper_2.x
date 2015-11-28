//*************************************************************************
//	创建日期:	2014-10-24
//	文件名称:	FKCW_IO_AssetInputStream.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_IO_Macros.h"
//-------------------------------------------------------------------------
class FKCW_IO_AssetInputStream : public CCObject
{
protected:
	FKCW_IO_AssetInputStream();
	FKCW_IO_AssetInputStream( const string& p_strPath );
public:
	static FKCW_IO_AssetInputStream* Create( const string& p_strPath );
	virtual ~FKCW_IO_AssetInputStream();
public:
	// 获取数据缓冲区指针
	virtual char*		GetBuffer() = 0;
	// 获取当前距离文件头的偏移量
	virtual size_t		GetPosition() = 0;
	// 获取全数据长度
	virtual size_t		GetLength() = 0;
	// 获取有效数据长度
	virtual size_t		GetAvailableLength() = 0;
	// 开启一个资源
	virtual bool		Open();
	// 关闭一个资源
	virtual void		Close() = 0;
	// 读取一段数据到缓冲区
	// 返回值：-1 错误；0 无数据；正值 实际读入的数据量
	virtual ssize_t		Read( char* p_szBuffer, size_t p_unLength ) = 0;
	// 读取一个字节
	virtual ssize_t		ReadByte( char* p_cRet );
	// 读取一个短整形
	virtual ssize_t		ReadShort( short* p_pRet );
	// 读取一个整形
	virtual ssize_t		ReadInt( int* p_pRet );
	// 读取一个int64
	virtual ssize_t		ReadInt64( int64_t* p_pRet );
	// 更新读取指针位置
	// 参数：p_nOffset 偏移量
	// 参数：p_nMode SEEK_CUR SEEK_END SEEK_SET 之一
	// 返回值：偏移之后，和文件头的相对偏移量
	virtual size_t		Seek( int p_nOffset, int p_nMode ) = 0;
	// 设置是否使用大尾模式。
	// 注：默认为小尾模式
	virtual void		SetBigEndian( bool p_bIsUseBigEndian );
	// 判断是否大尾模式
	virtual bool		IsBigEndian();
protected:
	int					m_nEndianState;		// 大尾或小尾
};
//-------------------------------------------------------------------------