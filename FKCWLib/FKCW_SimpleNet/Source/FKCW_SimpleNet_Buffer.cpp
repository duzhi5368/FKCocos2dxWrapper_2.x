//-------------------------------------------------------------------------
#include "../Include/FKCW_SimpleNet_Buffer.h"
//-------------------------------------------------------------------------
FKCW_SimpleNet_Buffer::FKCW_SimpleNet_Buffer()
	: _p_buffer(NULL)
	, _u_write_pos(0)
	, _u_read_pos(0)
	, _u_content_size(0)
	, _u_buffer_size(CCBUFFER_ORGINAL_SIZE)
	, _u_mark_pos(0)
{
	_p_buffer = (char*) malloc(_u_buffer_size);
}
//-------------------------------------------------------------------------
FKCW_SimpleNet_Buffer::FKCW_SimpleNet_Buffer(unsigned int n_capacity)
	: _p_buffer(NULL)
	, _u_write_pos(0)
	, _u_read_pos(0)
	, _u_content_size(0)
	, _u_mark_pos(0)
{
	_u_buffer_size = n_capacity;
	_p_buffer = (char*) malloc(_u_buffer_size);
}
//-------------------------------------------------------------------------
FKCW_SimpleNet_Buffer::FKCW_SimpleNet_Buffer(const char* p_data, unsigned int u_len)
	: _p_buffer(NULL)
	, _u_read_pos(0)
	, _u_mark_pos(0)
{
	CCAssert(p_data && (u_len > 0), "p_data && u_len > 0");

	_u_buffer_size = u_len;
	_u_write_pos = u_len;
	_u_content_size = u_len;
	_p_buffer = (char*) malloc(u_len);

	memcpy(_p_buffer, p_data, u_len);
}
//-------------------------------------------------------------------------
FKCW_SimpleNet_Buffer::~FKCW_SimpleNet_Buffer()
{
	CC_SAFE_FREE(_p_buffer);
}
//-------------------------------------------------------------------------
FKCW_SimpleNet_Buffer* FKCW_SimpleNet_Buffer::create()
{
	FKCW_SimpleNet_Buffer* pRet = new FKCW_SimpleNet_Buffer();
	pRet->autorelease();
	return (pRet);
}
//-------------------------------------------------------------------------
FKCW_SimpleNet_Buffer* FKCW_SimpleNet_Buffer::create(unsigned int n_capacity)
{
	FKCW_SimpleNet_Buffer* pRet = new FKCW_SimpleNet_Buffer(n_capacity);
	pRet->autorelease();
	return (pRet);
}
//-------------------------------------------------------------------------
FKCW_SimpleNet_Buffer* FKCW_SimpleNet_Buffer::create(const char* p_data, unsigned int u_len)
{
	FKCW_SimpleNet_Buffer* pRet = new FKCW_SimpleNet_Buffer(p_data, u_len);
	pRet->autorelease();
	return (pRet);
}
//-------------------------------------------------------------------------
CCObject* FKCW_SimpleNet_Buffer::copy()
{
	if(_u_content_size > 0)
	{
		FKCW_SimpleNet_Buffer* p_buf = new FKCW_SimpleNet_Buffer(_p_buffer, _u_content_size);
		return (p_buf);
	}
	else
	{
		return NULL;;
	}
}
//-------------------------------------------------------------------------
unsigned int FKCW_SimpleNet_Buffer::getWriterIndex() const
{
	return (_u_write_pos);
}
//-------------------------------------------------------------------------
unsigned int FKCW_SimpleNet_Buffer::getContentSize() const
{
	return (_u_content_size);
}
//-------------------------------------------------------------------------
unsigned int FKCW_SimpleNet_Buffer::getReadableSize() const
{
	return (_u_content_size -  _u_read_pos);
}
//-------------------------------------------------------------------------
unsigned int FKCW_SimpleNet_Buffer::getReaderIndex() const
{
	return (_u_read_pos);
}
//-------------------------------------------------------------------------
unsigned int FKCW_SimpleNet_Buffer::getBufferSize() const
{
	return (_u_buffer_size);
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_Buffer::discardReadContent()
{
	if(_u_read_pos > 0 )
	{
		moveLeft(_u_read_pos);
	}
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_Buffer::skipData(unsigned int u_len)
{
	if((_u_read_pos + u_len) <= _u_content_size )
	{
		(_u_read_pos +=  u_len);
	}
	else
	{
		(_u_read_pos += (_u_content_size - _u_read_pos ));
	}
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_Buffer::moveWriterIndexToFront()
{
	_u_write_pos = 0;
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_Buffer::moveWriterIndexToBack()
{
	_u_write_pos = _u_content_size;
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_Buffer::moveReaderIndexToFront()
{
	_u_read_pos = 0;
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_Buffer::moveReaderIndexToBack()
{
	_u_read_pos = _u_content_size;
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_Buffer::reset()
{
	moveWriterIndexToFront();
	moveReaderIndexToFront();
	_u_mark_pos = 0;
	_u_content_size = 0;
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_Buffer::markReaderIndex()
{
	_u_mark_pos = _u_read_pos;
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_Buffer::resetReaderIndex()
{
	_u_read_pos = _u_mark_pos;
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_Buffer::setReaderIndex(unsigned int u_pos)
{
	CCAssert(u_pos <= _u_content_size, "u_pos <= _u_content_size");
	_u_read_pos = u_pos;
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_Buffer::setWriterIndex(unsigned int u_pos)
{
	CCAssert(u_pos <= _u_content_size, "u_pos <= _u_content_size");
	_u_write_pos = u_pos;
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_Buffer::moveLeft(unsigned int u_len)
{
	if( _u_content_size == 0 )
		return;
	CCAssert( u_len !=  0, "u_len !=  0" );

	if( u_len >= _u_content_size)
		clear();
	else
	{
		for(unsigned int i = u_len; i < _u_content_size; ++i)
		{
			*(_p_buffer + (i - u_len)) = *(_p_buffer + i);
			*(_p_buffer + i) = 0;
		}
		_u_write_pos = (unsigned int)(MAX(0, ((int)(_u_write_pos) - (int)(u_len))));
		_u_read_pos = (unsigned int)(MAX(0, ((int)(_u_read_pos) - (int)(u_len))));
		_u_mark_pos = (unsigned int)(MAX(0, ((int)(_u_mark_pos) - (int)(u_len))));
		_u_content_size = _u_content_size - u_len;
	}
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_Buffer::moveRight(unsigned int u_len)
{
	if( _u_content_size == 0 )
		return;

	_reallocBufferSizeInChanged(u_len);

	for( int i = _u_content_size - 1; i >= 0; --i )
	{
		*(_p_buffer + i + (int)(u_len) ) = *( _p_buffer + i );
		*( _p_buffer + i ) = 0;
	}
	
	_u_write_pos += u_len;
	_u_read_pos += u_len;
	_u_mark_pos += u_len;
	_u_content_size += u_len;
}
//-------------------------------------------------------------------------
bool FKCW_SimpleNet_Buffer::isReadable() const
{
	if(_u_content_size == 0)
		return false;
	return _u_read_pos < _u_content_size;
}
//-------------------------------------------------------------------------
bool FKCW_SimpleNet_Buffer::isReadable(unsigned int u_len) const
{
	if((_u_content_size == 0) || (u_len == 0))
		return false;

	return (_u_read_pos + u_len) <= _u_content_size;
}
//-------------------------------------------------------------------------
unsigned int FKCW_SimpleNet_Buffer::length() const
{
	return (getContentSize());
}
//-------------------------------------------------------------------------
unsigned int FKCW_SimpleNet_Buffer::capacity() const
{
	return (getBufferSize());
}
//-------------------------------------------------------------------------
bool FKCW_SimpleNet_Buffer::empty() const
{
	return length() == 0;
}
//-------------------------------------------------------------------------
char* FKCW_SimpleNet_Buffer::data() const
{
	return (_p_buffer);
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_Buffer::clear()
{
	_u_content_size = 0;
	_u_write_pos = 0;
	_u_read_pos = 0;
	_u_mark_pos = 0;
	memset(_p_buffer, 0, _u_buffer_size);
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_Buffer::_reallocBufferSize()
{
	_u_buffer_size = _u_buffer_size * 2;
	_p_buffer = (char*) realloc(_p_buffer, _u_buffer_size);
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_Buffer::_reallocBufferSizeInChanged(unsigned int u_len)
{
	while( true )
	{
		if( _u_write_pos + u_len <= _u_buffer_size )
		{
			break;
		}
		else
		{
			_reallocBufferSize();
		}
	}
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_Buffer::writeData(const char* p_data, unsigned int u_len)
{
	CCAssert(p_data && u_len > 0, "p_data && u_len > 0");
	_reallocBufferSizeInChanged(u_len);
	memcpy(_p_buffer + _u_write_pos, p_data, u_len);
	_u_write_pos += u_len;
	_u_content_size = (_u_content_size < _u_write_pos ? _u_write_pos : _u_content_size);
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_Buffer::writeLengthAndData(const char* p_data, unsigned int u_len)
{
	writeUInt(u_len);
	writeData(p_data, u_len);
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_Buffer::writeChar(char data)
{
	_reallocBufferSizeInChanged(sizeof(char));
	*(_p_buffer + _u_write_pos) = data;
	++_u_write_pos;
	_u_content_size = (_u_content_size < _u_write_pos ? _u_write_pos : _u_content_size);
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_Buffer::writeUChar(unsigned char data)
{
	writeChar((char)data);
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_Buffer::writeShort(short data)
{
#if MEMORYTYPE_REVERSE
	char p_data[sizeof(short)] = {0};
	memcpy(p_data, (short*)(&(data)), sizeof(short));
	std::reverse(&(p_data[0]), &(p_data[sizeof(short)]));
	writeData(p_data, sizeof(short));
#else
	char* p_data = (char*)(&(data));
	writeData(p_data, sizeof(short));
#endif
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_Buffer::writeUShort(unsigned short data)
{
	writeShort((short)(data));
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_Buffer::writeInt(int data)
{
#if MEMORYTYPE_REVERSE
	char p_data[sizeof(int)] = {0};
	memcpy(p_data, (char*)(&(data)), sizeof(int));
	std::reverse(&(p_data[0]), &(p_data[sizeof(int)]));
	writeData(p_data, sizeof(int));
#else
	char* p_data = (char*)(&(data));
	writeData(p_data, sizeof(int));
#endif
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_Buffer::writeUInt(unsigned int data)
{
	writeInt((int)(data));
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_Buffer::writeLongLong(long long data)
{
#if MEMORYTYPE_REVERSE
	char p_data[sizeof(long long)] = {0};
	memcpy(p_data, (char*)(&(data)), sizeof(long long));
	std::reverse(&(p_data[0]), &(p_data[sizeof(long long)]));
	writeData(p_data, sizeof(long long));
#else
	char* p_data = (char*)(&(data));
	writeData(p_data, sizeof(long long));
#endif
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_Buffer::writeULongLong(unsigned long long data)
{
	writeLongLong((long long)(data));
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_Buffer::writeFloat(float data)
{
#if MEMORYTYPE_REVERSE
	char p_data[sizeof(float)] = {0};
	memcpy(p_data, (char*)(&(data)), sizeof(float));
	std::reverse(&(p_data[0]), &(p_data[sizeof(float)]));
	writeData(p_data, sizeof(float));
#else
	char* p_data = (char*)(&(data));
	writeData(p_data, sizeof(float));
#endif
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_Buffer::writeDouble(double data)
{
#if MEMORYTYPE_REVERSE
	char p_data[sizeof(double)] = {0};
	memcpy(p_data, (char*)(&(data)), sizeof(double));
	std::reverse(&(p_data[0]), &(p_data[sizeof(double)]));
	writeData(p_data, sizeof(double));
#else
	char* p_data = (char*)(&(data));
	writeData(p_data, sizeof(double));
#endif
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_Buffer::writeString(const char* p_data)
{
	CCAssert(p_data && (strlen(p_data) > 0), "p_data, && (strlen(p_data) > 0)");
	writeData(p_data, strlen(p_data));
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_Buffer::writeLengthAndString(const char* p_data)
{
	CCAssert(p_data && (strlen(p_data) > 0), "p_data, && (strlen(p_data) > 0)");
	writeInt(strlen(p_data));
	writeString(p_data);
}
//-------------------------------------------------------------------------
char* FKCW_SimpleNet_Buffer::readWholeData()
{
	if(_u_content_size != 0)
	{
		char* p_buffer = new char[_u_content_size];
		memcpy(p_buffer, _p_buffer, _u_content_size);
		_u_read_pos = _u_content_size;
		return (p_buffer);
	}
	else
	{
		return NULL;
	}
}
//-------------------------------------------------------------------------
FKCW_SimpleNet_Buffer* FKCW_SimpleNet_Buffer::readData(unsigned int u_len)
{
	if(isReadable(u_len))
	{
		char* p_data = (char*) malloc(u_len);
		readData(p_data, u_len);
		FKCW_SimpleNet_Buffer* p_ret = new FKCW_SimpleNet_Buffer(p_data, u_len);
		p_ret->autorelease();
		CC_SAFE_FREE(p_data);
		return (p_ret);
	}
	else
	{
		return NULL;
	}
}
//-------------------------------------------------------------------------
void FKCW_SimpleNet_Buffer::readData(char* p_out_data, unsigned int u_len)
{
	if(isReadable(u_len))
	{
		memcpy(p_out_data, _p_buffer + _u_read_pos, u_len);
		_u_read_pos += u_len;
	}
	else if(_u_content_size - _u_read_pos >= 0 && u_len != 0)
	{
		u_len = _u_content_size - _u_read_pos;
		memcpy(p_out_data, _p_buffer + _u_read_pos, u_len);
		_u_read_pos += u_len;
	}
}
//-------------------------------------------------------------------------
char FKCW_SimpleNet_Buffer::readChar()
{
	if(isReadable(sizeof(char)))
	{
		char ret = *(_p_buffer + _u_read_pos);
		_u_read_pos += sizeof(char);
		return (ret);
	}
	else
	{
		return 0;
	}
}
//-------------------------------------------------------------------------
unsigned char FKCW_SimpleNet_Buffer::readUChar()
{
	return ((unsigned char)readChar());
}
//-------------------------------------------------------------------------
short FKCW_SimpleNet_Buffer::readShort()
{
	if(isReadable(sizeof(short)))
	{
#if MEMORYTYPE_REVERSE
		char p_temp[sizeof(short)] = {0};
		memcpy(p_temp, _p_buffer + _u_read_pos, sizeof(short));
		std::reverse(&(p_temp[0]), &(p_temp[sizeof(short)]));
		short ret = *((short*)p_temp);
#else
		short ret = *((short*)(_p_buffer + _u_read_pos));
#endif
		_u_read_pos += sizeof(short);
		return (ret);
	}
	else
	{
		return 0;
	}
}
//-------------------------------------------------------------------------
unsigned short FKCW_SimpleNet_Buffer::readUShort()
{
	return ((unsigned short)(readShort()));
}
//-------------------------------------------------------------------------
int FKCW_SimpleNet_Buffer::readInt()
{
	if(isReadable(sizeof(int)))
	{
#if MEMORYTYPE_REVERSE
		char p_temp[sizeof(int)] = {0};
		memcpy(p_temp, _p_buffer + _u_read_pos, sizeof(int));
		std::reverse(&(p_temp[0]), &(p_temp[sizeof(int)]));
		int ret = *((int*)p_temp);
#else
		int ret = *((int*)(_p_buffer + _u_read_pos));
#endif
		_u_read_pos += sizeof(int);
		return (ret);
	}
	else
	{
		return 0;
	}
}
//-------------------------------------------------------------------------
unsigned int FKCW_SimpleNet_Buffer::readUInt()
{
	return (int(readInt()));
}
//-------------------------------------------------------------------------
long long FKCW_SimpleNet_Buffer::readLongLong()
{
	if(isReadable(sizeof(long long)))
	{
#if MEMORYTYPE_REVERSE
		char p_temp[sizeof(long long)] = {0};
		memcpy(p_temp, _p_buffer + _u_read_pos, sizeof(long long));
		std::reverse(&(p_temp[0]), &(p_temp[sizeof(long long)]));
		long long ret = *((long long*)p_temp);
#else
		long long ret = *((long long*)(_p_buffer + _u_read_pos));
#endif
		_u_read_pos += sizeof(long long);
		return (ret);
	}
	else
	{
		return 0;
	}
}
//-------------------------------------------------------------------------
unsigned long long FKCW_SimpleNet_Buffer::readULongLong()
{
	return ((unsigned long long)(readLongLong()));
}
//-------------------------------------------------------------------------
float FKCW_SimpleNet_Buffer::readFloat()
{
	if(isReadable(sizeof(float)))
	{
#if MEMORYTYPE_REVERSE
		char p_temp[sizeof(float)] = {0};
		memcpy(p_temp, _p_buffer + _u_read_pos, sizeof(float));
		std::reverse(&(p_temp[0]), &(p_temp[sizeof(float)]));
		float ret = *((float*)p_temp);
#else
		float ret = *((float*)(_p_buffer + _u_read_pos));
#endif
		_u_read_pos += sizeof(float);
		return (ret);
	}
	else
	{
		return 0;
	}
}
//-------------------------------------------------------------------------
double FKCW_SimpleNet_Buffer::readDouble()
{
	if(isReadable(sizeof(double)))
	{
#if MEMORYTYPE_REVERSE
		char p_temp[sizeof(double)] = {0};
		memcpy(p_temp, _p_buffer + _u_read_pos, sizeof(double));
		std::reverse(&(p_temp[0]), &(p_temp[sizeof(double)]));
		double ret = *((double*)p_temp);
#else
		double ret = *((double*)(_p_buffer + _u_read_pos));
#endif
		_u_read_pos += sizeof(double);
		return (ret);
	}
	else
	{
		return 0;
	}
}
//-------------------------------------------------------------------------
std::string FKCW_SimpleNet_Buffer::readString(unsigned int u_len)
{
	CCAssert(u_len> 0, "u_len> 0");
	std::string str;
	char* p_data = (char*)malloc(u_len + 1);
	readData(p_data, u_len);
	p_data[u_len] = 0;
	str.assign(p_data);
	CC_SAFE_FREE(p_data);
	return str;
}
//-------------------------------------------------------------------------
std::string FKCW_SimpleNet_Buffer::readLengthAndString()
{
	return readString(readUInt());
}
//-------------------------------------------------------------------------