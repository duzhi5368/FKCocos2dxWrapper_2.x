//-------------------------------------------------------------------------
#include "../Include/FKCW_Base_Bundle.h"
using std::make_pair;
//-------------------------------------------------------------------------
#define CCBUNDLE_SHORT			1
#define CCBUNDLE_UNSIGNEDSHORT	2
#define CCBUNDLE_INT			3
#define CCBUNDLE_UNSIGNEDINT	4
#define CCBUNDLE_FLOAT			5
#define CCBUNDLE_DOUBLE			6
#define CCBUNDLE_STRING			7
#define CCBUNDLE_BUNDLE			8
#define CCBUNDLE_OBJECT			9
#define CCBUNDLE_PARAM			10
//-------------------------------------------------------------------------
typedef std::map<std::string, FKCW_Base_Bundle::SBoundValue> MAP_DATAS;
//-------------------------------------------------------------------------
FKCW_Base_Bundle::FKCW_Base_Bundle()
{

}
//-------------------------------------------------------------------------
FKCW_Base_Bundle::~FKCW_Base_Bundle()
{
	MAP_DATAS::iterator iter = m_mDatas.begin();
	for(; iter != m_mDatas.end(); ++iter)
	{
		removeValue(iter->second);
	}
	m_mDatas.clear();
}
//-------------------------------------------------------------------------
FKCW_Base_Bundle* FKCW_Base_Bundle::create()
{
	FKCW_Base_Bundle* pRet = new FKCW_Base_Bundle();
	if( pRet )
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
CCObject* FKCW_Base_Bundle::copyWithZone(CCZone* pZone)
{
	CCZone *pNewZone = NULL;
	FKCW_Base_Bundle *pRet = NULL;
	if( pZone && pZone->m_pCopyObject )
	{
		pRet = (FKCW_Base_Bundle*)(pZone->m_pCopyObject);
	}
	else
	{
		pRet = new FKCW_Base_Bundle();
		pNewZone = new CCZone(pRet);
	}

	MAP_DATAS::iterator iter = m_mDatas.begin();
	for(; iter != m_mDatas.end(); ++iter)
	{
		switch(iter->second.nType)
		{
		case CCBUNDLE_SHORT:
			pRet->putShort(iter->first.c_str(), *((short*)(iter->second.pValue)));
			break;
		case CCBUNDLE_UNSIGNEDSHORT:
			pRet->putUShort(iter->first.c_str(), *((unsigned short*)(iter->second.pValue)));
			break;
		case CCBUNDLE_INT:
			pRet->putInt(iter->first.c_str(), *((int*)(iter->second.pValue)));
			break;
		case CCBUNDLE_UNSIGNEDINT:
			pRet->putUInt(iter->first.c_str(), *((unsigned int*)(iter->second.pValue)));
			break;
		case CCBUNDLE_FLOAT:
			pRet->putFloat(iter->first.c_str(), *((float*)(iter->second.pValue)));
			break;
		case CCBUNDLE_DOUBLE:
			pRet->putDouble(iter->first.c_str(), *((double*)(iter->second.pValue)));
			break;
		case CCBUNDLE_STRING:
			pRet->putString(iter->first.c_str(), (char*)(iter->second.pValue));
			break;
		case CCBUNDLE_BUNDLE:
			pRet->putBundle(iter->first.c_str(), (FKCW_Base_Bundle*)((FKCW_Base_Bundle*)(iter->second.pValue))->copy());
			break;
		case CCBUNDLE_OBJECT:
			pRet->putObject(iter->first.c_str(), ((CCObject*)(iter->second.pValue))->copy());
			break;
		case CCBUNDLE_PARAM:
			pRet->putParam(iter->first.c_str(), iter->second.pValue);
			break;
		default:
			break;
		}
	}

	CC_SAFE_DELETE(pNewZone);
	return pRet;
}
//-------------------------------------------------------------------------
void FKCW_Base_Bundle::removeValue(const FKCW_Base_Bundle::SBoundValue& tValue)
{
	switch(tValue.nType)
	{
	case CCBUNDLE_SHORT:
		delete ((short*)(tValue.pValue));
		break;
	case CCBUNDLE_UNSIGNEDSHORT:
		delete ((unsigned short*)(tValue.pValue));
		break;
	case CCBUNDLE_INT:
		delete ((int*)(tValue.pValue));
		break;
	case CCBUNDLE_UNSIGNEDINT:
		delete ((unsigned int*)(tValue.pValue));
		break;
	case CCBUNDLE_FLOAT:
		delete ((float*)(tValue.pValue));
		break;
	case CCBUNDLE_DOUBLE:
		delete ((double*)(tValue.pValue));
		break;
	case CCBUNDLE_STRING:
		delete[] ((char*)(tValue.pValue));
		break;
	case CCBUNDLE_BUNDLE:
		((FKCW_Base_Bundle*)(tValue.pValue))->release();
		break;
	case CCBUNDLE_OBJECT:
		((CCObject*)(tValue.pValue))->release();
		break;
	case CCBUNDLE_PARAM:
		break;
	default:
		break;
	}
}
//-------------------------------------------------------------------------
void FKCW_Base_Bundle::removeValueByKey(const char* key)
{
	MAP_DATAS::iterator itr = m_mDatas.find(key);
	if( itr != m_mDatas.end() )
	{
		removeValue(itr->second);
		m_mDatas.erase(itr->first);
	}
}
//-------------------------------------------------------------------------
void FKCW_Base_Bundle::putShort(const char* key, short value)
{
	removeValueByKey(key);
	short* ret = new short(value);
	FKCW_Base_Bundle::SBoundValue tag = { CCBUNDLE_SHORT, ret };
	m_mDatas.insert(make_pair(key, tag));
}
//-------------------------------------------------------------------------
void FKCW_Base_Bundle::putUShort(const char* key, unsigned short value)
{
	removeValueByKey(key);
	unsigned short* ret = new unsigned short(value);
	FKCW_Base_Bundle::SBoundValue tag = { CCBUNDLE_UNSIGNEDSHORT, ret };
	m_mDatas.insert(make_pair(key, tag));
}
//-------------------------------------------------------------------------
void FKCW_Base_Bundle::putInt(const char* key, int value)
{
	removeValueByKey(key);
	int* ret = new int(value);
	FKCW_Base_Bundle::SBoundValue tag = { CCBUNDLE_INT, ret };
	m_mDatas.insert(make_pair(key, tag));
}
//-------------------------------------------------------------------------
void FKCW_Base_Bundle::putUInt(const char* key, unsigned int value)
{
	removeValueByKey(key);
	unsigned int* ret = new unsigned int(value);
	FKCW_Base_Bundle::SBoundValue tag = { CCBUNDLE_UNSIGNEDINT, ret };
	m_mDatas.insert(make_pair(key, tag));
}
//-------------------------------------------------------------------------
void FKCW_Base_Bundle::putFloat(const char* key, float value)
{
	removeValueByKey(key);
	float* ret = new float(value);
	FKCW_Base_Bundle::SBoundValue tag = { CCBUNDLE_FLOAT, ret };
	m_mDatas.insert(make_pair(key, tag));
}
//-------------------------------------------------------------------------
void FKCW_Base_Bundle::putDouble(const char* key, double value)
{
	removeValueByKey(key);
	double* ret = new double(value);
	FKCW_Base_Bundle::SBoundValue tag = { CCBUNDLE_DOUBLE, ret };
	m_mDatas.insert(make_pair(key, tag));
}
//-------------------------------------------------------------------------
void FKCW_Base_Bundle::putString(const char* key, const char* value)
{
	removeValueByKey(key);
	size_t ulen = strlen(value);
	char* ret = new char[ulen + 1];
	memcpy(ret, value, ulen);
	ret[ulen] = '\0';

	FKCW_Base_Bundle::SBoundValue tag = { CCBUNDLE_STRING, ret };
	m_mDatas.insert(make_pair(key, tag));
}
//-------------------------------------------------------------------------
void FKCW_Base_Bundle::putBundle(const char* key, FKCW_Base_Bundle* value)
{
	removeValueByKey(key);
	value->retain();
	FKCW_Base_Bundle::SBoundValue tag = { CCBUNDLE_BUNDLE, value };
	m_mDatas.insert(make_pair(key, tag));
}
//-------------------------------------------------------------------------
void FKCW_Base_Bundle::putObject(const char* key, CCObject* value)
{
	removeValueByKey(key);
	value->retain();
	FKCW_Base_Bundle::SBoundValue tag = { CCBUNDLE_OBJECT, value };
	m_mDatas.insert(make_pair(key, tag));
}
//-------------------------------------------------------------------------
void FKCW_Base_Bundle::putParam(const char* key, void* value)
{
	removeValueByKey(key);
	FKCW_Base_Bundle::SBoundValue tag = { CCBUNDLE_PARAM, value };
	m_mDatas.insert(make_pair(key, tag));
}
//-------------------------------------------------------------------------
short FKCW_Base_Bundle::getShort(const char* key)
{
	MAP_DATAS::iterator iter = m_mDatas.find(key);
	if( iter != m_mDatas.end() )
	{
		if( iter->second.nType == CCBUNDLE_SHORT )
		{
			return *((short*)(iter->second.pValue));
		}
	}
	return 0;
}
//-------------------------------------------------------------------------
unsigned short FKCW_Base_Bundle::getUShort(const char* key)
{
	MAP_DATAS::iterator iter = m_mDatas.find(key);
	if( iter != m_mDatas.end() )
	{
		if( iter->second.nType == CCBUNDLE_UNSIGNEDSHORT )
		{
			return *((unsigned short*)(iter->second.pValue));
		}
	}
	return 0;
}
//-------------------------------------------------------------------------
int FKCW_Base_Bundle::getInt(const char* key)
{
	MAP_DATAS::iterator iter = m_mDatas.find(key);
	if( iter != m_mDatas.end() )
	{
		if( iter->second.nType == CCBUNDLE_INT )
		{
			return *((int*)(iter->second.pValue));
		}
	}
	return 0;
}
//-------------------------------------------------------------------------
unsigned int FKCW_Base_Bundle::getUInt(const char* key)
{
	MAP_DATAS::iterator iter = m_mDatas.find(key);
	if( iter != m_mDatas.end() )
	{
		if( iter->second.nType == CCBUNDLE_UNSIGNEDINT )
		{
			return *((unsigned int*)(iter->second.pValue));
		}
	}
	return 0;
}
//-------------------------------------------------------------------------
float FKCW_Base_Bundle::getFloat(const char* key)
{
	MAP_DATAS::iterator iter = m_mDatas.find(key);
	if( iter != m_mDatas.end() )
	{
		if( iter->second.nType == CCBUNDLE_FLOAT )
		{
			return *((float*)(iter->second.pValue));
		}
	}
	return 0;
}
//-------------------------------------------------------------------------
double FKCW_Base_Bundle::getDouble(const char* key)
{
	MAP_DATAS::iterator iter = m_mDatas.find(key);
	if( iter != m_mDatas.end() )
	{
		if( iter->second.nType == CCBUNDLE_DOUBLE )
		{
			return *((double*)(iter->second.pValue));
		}
	}
	return 0;
}
//-------------------------------------------------------------------------
const char* FKCW_Base_Bundle::getString(const char* key)
{
	MAP_DATAS::iterator iter = m_mDatas.find(key);
	if( iter != m_mDatas.end() )
	{
		if( iter->second.nType == CCBUNDLE_STRING )
		{
			return (char*)(iter->second.pValue);
		}
	}
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_Base_Bundle* FKCW_Base_Bundle::getBundle(const char* key)
{
	MAP_DATAS::iterator iter = m_mDatas.find(key);
	if( iter != m_mDatas.end() )
	{
		if( iter->second.nType == CCBUNDLE_BUNDLE )
		{
			return (FKCW_Base_Bundle*)(iter->second.pValue);
		}
	}
	return NULL;
}
//-------------------------------------------------------------------------
CCObject* FKCW_Base_Bundle::getObject(const char* key)
{
	MAP_DATAS::iterator iter = m_mDatas.find(key);
	if( iter != m_mDatas.end() )
	{
		if( iter->second.nType == CCBUNDLE_OBJECT )
		{
			return (CCObject*)(iter->second.pValue);
		}
	}
	return NULL;
}
//-------------------------------------------------------------------------
void* FKCW_Base_Bundle::getParam(const char* key)
{
	MAP_DATAS::iterator iter = m_mDatas.find(key);
	if( iter != m_mDatas.end() )
	{
		if( iter->second.nType == CCBUNDLE_PARAM )
		{
			return iter->second.pValue;
		}
	}
	return NULL;
}
//-------------------------------------------------------------------------