//-------------------------------------------------------------------------
#include "../Include/FKCW_IO_AssetInputStream.h"
#include "../../FKCW_Base/Include/FKCW_Base_Macro.h"
//-------------------------------------------------------------------------
FKCW_IO_AssetInputStream::FKCW_IO_AssetInputStream()
	: m_nEndianState( LITTLE_ENDIAN )
{

}
//-------------------------------------------------------------------------
FKCW_IO_AssetInputStream::FKCW_IO_AssetInputStream( const string& p_strPath )
	: m_nEndianState( LITTLE_ENDIAN )
{

}
//-------------------------------------------------------------------------
FKCW_IO_AssetInputStream::~FKCW_IO_AssetInputStream()
{

}
//-------------------------------------------------------------------------
// 开启一个资源
bool FKCW_IO_AssetInputStream::Open()
{
	return true;
}
//-------------------------------------------------------------------------
// 读取一个字节
ssize_t FKCW_IO_AssetInputStream::ReadByte( char* p_cRet )
{
	return Read( p_cRet, sizeof(char) );
}
//-------------------------------------------------------------------------
// 读取一个短整形
ssize_t FKCW_IO_AssetInputStream::ReadShort( short* p_pRet )
{
	*p_pRet = 0;
	ssize_t nReadBytes = Read( (char*)p_pRet, sizeof(short) );
	if( nReadBytes > 0 )
	{
		if( IsBigEndian() )
		{
			*p_pRet = betoh16(*p_pRet);
		}
		else
		{
			*p_pRet = letoh16(*p_pRet);
		}
	}
	return nReadBytes;
}
//-------------------------------------------------------------------------
// 读取一个整形
ssize_t	FKCW_IO_AssetInputStream::ReadInt( int* p_pRet )
{
	*p_pRet = 0;
	ssize_t nReadBytes = Read( (char*)p_pRet, sizeof(int) );
	if( nReadBytes > 0 )
	{
		if( IsBigEndian() )
		{
			*p_pRet = betoh32(*p_pRet);
		}
		else
		{
			*p_pRet = letoh32(*p_pRet);
		}
	}
	return nReadBytes;
}
//-------------------------------------------------------------------------
// 读取一个int64
ssize_t FKCW_IO_AssetInputStream::ReadInt64( int64_t* p_pRet )
{
	*p_pRet = 0;
	ssize_t nReadBytes = Read( (char*)p_pRet, sizeof(int64_t) );
	if( nReadBytes > 0 )
	{
		if( IsBigEndian() )
		{
			*p_pRet = betoh64(*p_pRet);
		}
		else
		{
			*p_pRet = letoh64(*p_pRet);
		}
	}
	return nReadBytes;
}
//-------------------------------------------------------------------------
// 设置是否使用大尾模式。
void FKCW_IO_AssetInputStream::SetBigEndian( bool p_bIsUseBigEndian )
{
	m_nEndianState = p_bIsUseBigEndian ? BIG_ENDIAN : LITTLE_ENDIAN;
}
//-------------------------------------------------------------------------
// 判断是否大尾模式
bool FKCW_IO_AssetInputStream::IsBigEndian()
{
	return m_nEndianState == BIG_ENDIAN;
}
//-------------------------------------------------------------------------