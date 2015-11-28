//*************************************************************************
//	创建日期:	2014-10-24
//	文件名称:	FKCW_IO_AssetOutputStream.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_IO_Macros.h"
//-------------------------------------------------------------------------
class FKCW_IO_AssetOutputStream : public CCObject
{
protected:
	FKCW_IO_AssetOutputStream();
	FKCW_IO_AssetOutputStream( const string& p_strPath, bool p_bIsAppend = false );
public:
	static FKCW_IO_AssetOutputStream* Create( const string& p_strPath, bool p_bIsAppend = false );
	virtual ~FKCW_IO_AssetOutputStream();
public:
	// 打开一个资源
	virtual bool		Open();
	// 关闭一个资源
	virtual void		Close() = 0;
	// 写一段数据
	// 返回值：-1 错误；其他，实际写入的大小
	virtual ssize_t		Write( const char* p_pData, size_t p_nLen ) = 0;
	virtual ssize_t		Write( const int* p_pData, size_t p_nLen ) = 0;
	// 获取当前距离文件头的偏移量
	virtual size_t		GetPosition() = 0;
	// 更新读取指针位置
	// 参数：p_nOffset 偏移量
	// 参数：p_nMode SEEK_CUR SEEK_END SEEK_SET 之一
	// 返回值：偏移之后，和文件头的相对偏移量
	virtual size_t		Seek( int p_nOffset, int p_nMode ) = 0;
	// 获取输出路径
	const string&		GetPath(){ return m_strPath; }
protected:
	bool				m_bIsAppend;	// 是否附加文件
	string				m_strPath;		// 文件路径
};