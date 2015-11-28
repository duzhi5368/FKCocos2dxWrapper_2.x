/********************************************************************
	created:	2014/10/28
	file base:	FKCW_IO_MemoryOutputStream
	author:		FreeKnight
	
	purpose:	
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_IO_AssetOutputStream.h"
//--------------------------------------------------------------------
class FKCW_IO_MemoryOutputStream : public FKCW_IO_AssetOutputStream
{
private:
	char*			m_szBuffer;			// 缓冲区
	size_t			m_unLength;			// 缓冲区长度
	size_t			m_unPosition;		// 当前读指针位置
	size_t			m_unCapacity;		// 缓冲区容量
	bool			m_bIsSelfRelease;	// 缓冲区是否自己销毁
protected:
	FKCW_IO_MemoryOutputStream( char* p_szBuffer, size_t p_unCapacity, bool p_bIsSelfRelease );
	// 确保缓冲区容量足够写len个字节
	void		_EnsureCapacity( size_t p_unLen );
public:
	FKCW_IO_MemoryOutputStream();
	virtual ~FKCW_IO_MemoryOutputStream();
	// 创建一个默认内存输出流，该缓存被本内存输出流管理
	static FKCW_IO_MemoryOutputStream* Create();
	// 创建一个指定大小的内存输出流
	// 注：该内存被本输出流创建，但未必由本输出流删除（删除与否参见参数 p_bIsSelfRelease）
	static FKCW_IO_MemoryOutputStream* Create( size_t p_unCapacity, bool p_bIsSelfRelease = false );
public:
	// 关闭当前输出流
	virtual void		Close();
	// 写一段数据
	// 返回值：-1 错误；其他，实际写入的大小
	virtual ssize_t		Write( const char* p_pData, size_t p_nLen );
	virtual ssize_t		Write( const int* p_pData, size_t p_nLen );
	// 获取当前距离文件头的偏移量
	virtual size_t		GetPosition();
	// 更新读取指针位置
	// 参数：p_nOffset 偏移量
	// 参数：p_nMode SEEK_CUR SEEK_END SEEK_SET 之一
	// 返回值：偏移之后，和文件头的相对偏移量
	virtual size_t		Seek( int p_nOffset, int p_nMode );
	// 获取当前输出流内已写入的内存长度
	size_t				GetLength();
	// 获取当前输出流写指针
	const char*			GetBuffer();
	// 重置输出流指针，准备重写新数据
	void				Reset();
};