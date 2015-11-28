/********************************************************************
	created:	2014/10/31
	file base:	FKCW_Base_ByteBuffer
	author:		FreeKnight
	
	purpose:	
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_Base_Macro.h"
#include "FKCW_Base_Memory.h"
//--------------------------------------------------------------------
class FKCW_Base_ByteBuffer : public CCObject
{
private:
	static const int		DEFAULT_INCREASE_SIZE = 200;		// 默认增量值
protected:
	uint8*			m_pBuffer;			// 缓冲区指针
	size_t			m_unReadPos;		// 读位置
	size_t			m_unWritePos;		// 写位置
	size_t			m_unBufferSize;		// 缓冲区大小
	bool			m_bExternal;		// 是否使用外部模式
protected:
	// 分配一个制定大小的内存
	void			_Reserve( size_t p_unRes );
	// 确保缓冲区足够大，能够写指定字节大小的数据
	void			_EnsureCanWrite( size_t p_unSize );
public:
	FKCW_Base_ByteBuffer();
	FKCW_Base_ByteBuffer( size_t p_unRes );
	FKCW_Base_ByteBuffer( const FKCW_Base_ByteBuffer& p_Other );
	// 它包含了一个外部数据，而不是自己内分内存
	// 参数：p_szBuffer 缓冲区 p_unBufSize 缓冲区大小 p_unDataLen 在缓冲区中的可用数据
	FKCW_Base_ByteBuffer( const char* p_szBuffer, size_t p_unBufSize, size_t p_unDataLen );
	virtual ~FKCW_Base_ByteBuffer();

	// 创建一个默认大小的 FKCW_Base_ByteBuffer
	static FKCW_Base_ByteBuffer*		Create();
	// 创建一个指定大小的 FKCW_Base_ByteBuffer、
	static FKCW_Base_ByteBuffer*		Create( size_t p_unRes );
	// 重置读写指针位置
	void			Clear();
	// 返回缓冲区指针
	const uint8*	GetBuffer();
	// 获取可读缓冲大小
	size_t			GetAvailable();
	// 向前移动读指针
	void			Skip( size_t p_unLen );
	// 向后移动读指针
	void			Revoke( size_t p_unLen );
	// 移动当前数据到缓冲区首部
	void			Compact();

	// 从缓冲区读取指定大小的数据，若读取Len超过了缓冲区大小，则仅读缓冲区可用字节
	// 返回值：返回实际读取的字节数
	size_t			Read( uint8* p_pBuffer, size_t p_unLen );
	// 读取一个string
	void			Read( string& p_strDest );
	// 读取一个C string
	void			ReadCString( string& p_strDest );
	// 读取一个pascal 字符串，即前两个字节必须是字符串长度
	void			ReadPascalString( string& p_strDest );
	// 读取一个字符串，除非到了新行或者到了结尾才会停止
	void			ReadLine( string& p_strDest );

	// 写入指定的数据
	void			Write( const uint8* p_pData, size_t p_unSize );
	// 写入一个string
	void			Write( const string& p_strData );
	// 写入一个C string
	void			WriteCString( const string& p_strData );
	// 写入一个pascal 字符串，即前两个字节必须是字符串长度，但注意，字符串不会尾部补0
	void			WritePascalString( const string& p_strData );
	// 写入一个string并换行
	void			WriteLine( const string& p_strData );

	// 获取读指针位置
	size_t			GetReadPos();
	// 设置读指针
	void			SetReadPos( size_t p_unPos );
	// 设置写指针
	void			SetWritePos( size_t p_unPos );


	// 从缓冲中读取 sizeof(T) 大小的数据
	template< typename T > T Read()
	{
		if(m_unReadPos + sizeof(T) > m_unWritePos)
			return (T)0;
		T ret = *(T*)&m_pBuffer[m_unReadPos];
		m_unReadPos += sizeof(T);
		return ret;
	}
	// 向缓冲区中写一些数据
	template<typename T> void Write(const T& data) 
	{
		size_t new_size = m_unWritePos + sizeof(T);
		if(new_size > m_unBufferSize) 
		{
			new_size = (new_size / DEFAULT_INCREASE_SIZE + 1) * DEFAULT_INCREASE_SIZE;
			_Reserve(new_size);
		}

		*(T*)&m_pBuffer[m_unWritePos] = data;
		m_unWritePos += sizeof(T);
	}
	// 向缓冲区写入一个vector
	template<typename T> size_t WriteVector(const vector<T>& v) 
	{
		for(typename vector<T>::const_iterator i = v.begin(); i != v.end(); i++) 
		{
			Write<T>(*i);
		}
		return v.size();
	}
	// 从缓冲区读取一个vector
	template<typename T> size_t ReadVector(size_t vsize, vector<T>& v) 
	{
		v.clear();
		while(vsize--) 
		{
			T t = Read<T>();
			v.push_back(t);
		}
		return v.size();
	}
	// 向缓冲区写入一个list
	template<typename T> size_t WriteList(const list<T>& v) 
	{
		for(typename list<T>::const_iterator i = v.begin(); i != v.end(); i++) 
		{
			Write<T>(*i);
		}
		return v.size();
	}
	// 从缓冲区读取一个vector
	template<typename T> size_t ReadList(size_t vsize, list<T>& v) 
	{
		v.clear();
		while(vsize--) 
		{
			T t = Read<T>();
			v.push_back(t);
		}
		return v.size();
	}
	// 向缓冲区写入一个map
	template <typename K, typename V> size_t WriteMap(const map<K, V>& m) 
	{
		for(typename map<K, V>::const_iterator i = m.begin(); i != m.end(); i++) 
		{
			Write<K>(i->first);
			Write<V>(i->second);
		}
		return m.size();
	}
	// 从缓冲区读取一个map
	template <typename K, typename V> size_t readMap(size_t msize, map<K, V>& m) 
	{
		m.clear();
		while(msize--) {
			K k = Read<K>();
			V v = Read<V>();
			m.insert(make_pair(k, v));
		}
		return m.size();
	}
};