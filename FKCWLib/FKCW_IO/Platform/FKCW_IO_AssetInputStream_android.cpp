
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#include "FKCW_IO_AssetInputStream_android.h"
#include <errno.h>


FKCW_IO_AssetInputStream* FKCW_IO_AssetInputStream::Create(const string& path) 
{
		FKCW_IO_AssetInputStream* ais = new FKCW_IO_AssetInputStream_android(path);
		return (FKCW_IO_AssetInputStream*)ais->autorelease();
}

FKCW_IO_AssetInputStream_android::FKCW_IO_AssetInputStream_android(const string& path) :
	FKCW_IO_AssetInputStream(path),
	m_buffer(NULL),
	m_position(0),
	m_length(0) 
{
		unsigned long len;
		m_buffer = (char*)CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "rb", &len);
		m_length = (size_t)len;
}

FKCW_IO_AssetInputStream_android::~FKCW_IO_AssetInputStream_android() 
{
}


char* FKCW_IO_AssetInputStream_android::GetBuffer()
{
	return m_buffer;
}

size_t FKCW_IO_AssetInputStream_android::GetPosition() 
{
	return m_position;
}

size_t FKCW_IO_AssetInputStream_android::GetLength() 
{
	return m_length;
}

size_t FKCW_IO_AssetInputStream_android::GetAvailableLength() 
{
	return m_length - m_position;
}

void FKCW_IO_AssetInputStream_android::Close() 
{
	if(m_buffer) {
		free(m_buffer);
		m_buffer = NULL;
		m_length = 0;
		m_position = 0;
	}
}

ssize_t FKCW_IO_AssetInputStream_android::Read(char* buffer, size_t length)
{
	int canRead = MIN(length, GetAvailableLength());
	memcpy(buffer, m_buffer + m_position, canRead);
	m_position += canRead;
	return canRead;
}

size_t FKCW_IO_AssetInputStream_android::Seek(int offset, int mode)
{
	switch(mode) {
	case SEEK_CUR:
		m_position = clampf(m_position + offset, 0, m_length);
		break;
	case SEEK_END:
		m_position = clampf(m_length + offset, 0, m_length);
		break;
	case SEEK_SET:
		m_position = clampf(offset, 0, m_length);
		break;
	}

	return m_position;
}

#endif
