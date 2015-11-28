//-------------------------------------------------------------------------
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
//-------------------------------------------------------------------------
#include "FKCW_IO_AssetOutputStream_Win.h"
//-------------------------------------------------------------------------
FKCW_IO_AssetOutputStream* FKCW_IO_AssetOutputStream::Create( const string& p_strPath, bool p_bIsAppend )
{
	FKCW_IO_AssetOutputStream* aos = new FKCW_IO_AssetOutputStream_Win(p_strPath, p_bIsAppend);
	return (FKCW_IO_AssetOutputStream*)aos->autorelease();
}
//-------------------------------------------------------------------------
FKCW_IO_AssetOutputStream_Win::FKCW_IO_AssetOutputStream_Win(const string& path, bool append) 
	: FKCW_IO_AssetOutputStream(path, append)
	, m_fp(NULL) 
{
		if (m_bIsAppend) 
		{
			if((m_fp = fopen(path.c_str(), "ab")) == NULL) {
				CCLOGWARN("open file %s failed: %s", path.c_str(), strerror(errno));
				m_fp = NULL;
			}
		} else {
			if((m_fp = fopen(path.c_str(), "wb")) == NULL) {
				CCLOGWARN("open file %s failed: %s", path.c_str(), strerror(errno));
				m_fp = NULL;
			}
		}
}
//-------------------------------------------------------------------------
FKCW_IO_AssetOutputStream_Win::~FKCW_IO_AssetOutputStream_Win() 
{
	if(m_fp != NULL)
	{
		fclose(m_fp);
		m_fp = NULL;
	}
}
//-------------------------------------------------------------------------
void FKCW_IO_AssetOutputStream_Win::Close() 
{
	if(m_fp != NULL) 
	{
		fclose(m_fp);
		m_fp = NULL;
	}
}
//-------------------------------------------------------------------------
ssize_t FKCW_IO_AssetOutputStream_Win::Write(const char* data, size_t len) 
{
	if (data == NULL)
		return -1;

	if(m_fp != NULL)
		return fwrite((void*)data, sizeof(char) * len, 1, m_fp);
	else
		return -1;
}
//-------------------------------------------------------------------------
ssize_t FKCW_IO_AssetOutputStream_Win::Write(const int* data, size_t len) 
{
	if (data == NULL)
		return -1;

	if(m_fp != NULL)
		return fwrite((void*)data, sizeof(int) * len, 1, m_fp);
	else
		return -1;
}
//-------------------------------------------------------------------------
size_t FKCW_IO_AssetOutputStream_Win::GetPosition() 
{
	if(m_fp != NULL)
		return ftell(m_fp);
	else
		return 0;
}
//-------------------------------------------------------------------------
size_t FKCW_IO_AssetOutputStream_Win::Seek(int offset, int mode) 
{
	if(m_fp != NULL)
		return fseek(m_fp, offset, mode);
	else
		return 0;
}
//-------------------------------------------------------------------------
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
//-------------------------------------------------------------------------