//-------------------------------------------------------------------------
#include "../Include/FKCW_IO_MemoryInputStream.h"
//-------------------------------------------------------------------------
FKCW_IO_MemoryInputStream::FKCW_IO_MemoryInputStream(char* buffer, size_t length, bool release) 
	: m_szBuffer(buffer)
	, m_unLength(length)
	, m_bIsSelfRelease(release)
	, m_unPosition(0) 
{
}
//-------------------------------------------------------------------------
FKCW_IO_MemoryInputStream::~FKCW_IO_MemoryInputStream()
{
	if( m_bIsSelfRelease )
	{
		if( m_szBuffer )
		{
			free( m_szBuffer );
			m_szBuffer = NULL;
		}
	}
}
//-------------------------------------------------------------------------
FKCW_IO_MemoryInputStream* FKCW_IO_MemoryInputStream::Create( char* p_szBuffer, size_t p_unLength, bool p_bIsSelfRelease )
{
	FKCW_IO_MemoryInputStream* s = new FKCW_IO_MemoryInputStream(p_szBuffer, p_unLength, p_bIsSelfRelease);
	return (FKCW_IO_MemoryInputStream*)s->autorelease();
}
//-------------------------------------------------------------------------
// 获取数据缓冲区指针
char* FKCW_IO_MemoryInputStream::GetBuffer()
{
	return m_szBuffer;
}
//-------------------------------------------------------------------------
// 获取当前距离文件头的偏移量
size_t FKCW_IO_MemoryInputStream::GetPosition()
{
	return m_unPosition;
}
//-------------------------------------------------------------------------
// 获取全数据长度
size_t FKCW_IO_MemoryInputStream::GetLength()
{
	return m_unLength;
}
//-------------------------------------------------------------------------
// 获取有效数据长度
size_t FKCW_IO_MemoryInputStream::GetAvailableLength()
{
	return m_unLength - m_unPosition;
}
//-------------------------------------------------------------------------
// 关闭一个资源
void FKCW_IO_MemoryInputStream::Close()
{

}
//-------------------------------------------------------------------------
// 读取一段数据到缓冲区
// 返回值：-1 错误；0 无数据；正值 实际读入的数据量
ssize_t FKCW_IO_MemoryInputStream::Read( char* p_szBuffer, size_t p_unLength )
{
	int canRead = MIN(p_unLength, GetAvailableLength());
	memcpy(p_szBuffer, m_szBuffer + m_unPosition, canRead);
	m_unPosition += canRead;
	return canRead;
}
//-------------------------------------------------------------------------
// 更新读取指针位置
// 参数：p_nOffset 偏移量
// 返回值：偏移之后，和文件头的相对偏移量
size_t FKCW_IO_MemoryInputStream::Seek( int p_nOffset, int p_nMode )
{
	switch( p_nMode ) 
	{
	case SEEK_CUR:
		m_unPosition = (size_t)clampf((float)m_unPosition + (float)p_nOffset, 0, (float)m_unLength);
		break;
	case SEEK_END:
		m_unPosition = (size_t)clampf((float)m_unLength + (float)p_nOffset, 0, (float)m_unLength);
		break;
	case SEEK_SET:
		m_unPosition = (size_t)clampf((float)p_nOffset, 0, (float)m_unLength);
		break;
	}

	return m_unPosition;
}
//-------------------------------------------------------------------------