//--------------------------------------------------------------------
#include "../Include/FKCW_Base_Data.h"
//--------------------------------------------------------------------
USING_NS_CC;
//--------------------------------------------------------------------
FKCW_Base_Data::FKCW_Base_Data() 
{
	m_unSize = 0;
	m_pBytes = (uint8_t*)malloc(m_unSize * sizeof(uint8_t));
}
//--------------------------------------------------------------------
FKCW_Base_Data::FKCW_Base_Data(uint8_t* bytes, size_t size) {
	m_unSize = size;
	m_pBytes = (uint8_t*)malloc(m_unSize * sizeof(uint8_t));
	memcpy(m_pBytes, bytes, m_unSize);
}
//--------------------------------------------------------------------
FKCW_Base_Data::FKCW_Base_Data(FKCW_Base_Data* data) {
	m_unSize = data->m_unSize;
	m_pBytes = (uint8_t*)malloc(m_unSize * sizeof(uint8_t));
	memcpy(m_pBytes, data->m_pBytes, m_unSize);
}
//--------------------------------------------------------------------
FKCW_Base_Data::~FKCW_Base_Data() {
	CC_SAFE_FREE(m_pBytes);
}
//--------------------------------------------------------------------
FKCW_Base_Data* FKCW_Base_Data::Create() {
	FKCW_Base_Data* d = new FKCW_Base_Data();
	return (FKCW_Base_Data*)d->autorelease();
}
//--------------------------------------------------------------------
FKCW_Base_Data* FKCW_Base_Data::CreateWithBytes(uint8_t* bytes, size_t size) {
	FKCW_Base_Data* d = new FKCW_Base_Data(bytes, size);
	return (FKCW_Base_Data*)d->autorelease();
}
//--------------------------------------------------------------------
FKCW_Base_Data* FKCW_Base_Data::CreateWithData(FKCW_Base_Data* data) {
	FKCW_Base_Data* d = new FKCW_Base_Data(data);
	return (FKCW_Base_Data*)d->autorelease();
}
//--------------------------------------------------------------------
void FKCW_Base_Data::AppendData(FKCW_Base_Data* data) {
	AppendBytes(data->getBytes(), data->getSize());
}
//--------------------------------------------------------------------
void FKCW_Base_Data::AppendBytes(uint8_t* bytes, size_t size) {
	m_unSize += size;
	m_pBytes = (uint8_t*)realloc(m_pBytes, m_unSize);
	memcpy(m_pBytes + m_unSize - size, bytes, size);
}
//--------------------------------------------------------------------