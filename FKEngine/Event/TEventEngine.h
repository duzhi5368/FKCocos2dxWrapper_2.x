//*************************************************************************
//	创建日期:	2015-1-12
//	文件名称:	TEventEngine.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKEventSystemMacros.h"
#include <map>

//#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
////#include <hash_map>
//#endif
//
//#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
////#include <ext/hash_map>
////using namespace std;
////#define stdext __gnu_cxx
//#endif
//-------------------------------------------------------------------------
// 消息类型定义
typedef unsigned short EVENT_TYPE;
//-------------------------------------------------------------------------
// 事件Key源
struct SCoreEventKey
{
	unsigned int		m_unSrcID;		// 发送源唯一标示（可能是角色UID，物品UID，以及某对象指针等，视消息类型逻辑程序员自己规定）
	unsigned short		m_usSrcType;	// 发送者源类型（参见相关枚举）
	EVENT_TYPE			m_usEventID;	// 发送事件ID
};
//-------------------------------------------------------------------------
// 事件Key
//-------------------------------------------------------------------------
struct SEventKey
{
	union
	{
		SCoreEventKey		m_CoreKey;
		long long			m_RecalculateKey;
	};
	SEventKey(){ m_RecalculateKey = 0; }
	bool operator == ( const SEventKey& other ) const
	{
		return m_RecalculateKey == other.m_RecalculateKey;
	}
	bool operator < ( const SEventKey& other ) const
	{
		return m_RecalculateKey < other.m_RecalculateKey;
	}
};
//-------------------------------------------------------------------------
//// 便捷函数
//#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
//	namespace stdext
//	{
//		inline size_t hash_value(const SEventKey &eventkey)
//		{
//			unsigned int k1 = (eventkey.m_CoreKey.m_unSrcID & 0xFFFF) << 16;
//			unsigned int k2 = (eventkey.m_CoreKey.m_usSrcType & 0xFF) << 8;
//			unsigned int k3 = eventkey.m_CoreKey.m_usEventID;
//			return k1 + k2 + k3;
//		}
//	}
//#else
//	namespace stdext
//	{
//		//template <>
//		//struct hash<SEventKey>
//		//{
//			inline size_t operator() (const SEventKey &eventkey) 
//			{
//				unsigned int k1 = (eventkey.m_CoreKey.m_unSrcID & 0xFFFF) << 16;
//				unsigned int k2 = (eventkey.m_CoreKey.m_usSrcType & 0xFF) << 8;
//				unsigned int k3 = eventkey.m_CoreKey.m_usEventID;
//				return k1 + k2 + k3;
//			}
//		//};
//	}
//#endif
//-------------------------------------------------------------------------
// 事件引擎模板
template< class TRecvHander, class TFireEventObj >
class TEventEngine
{
private:
	// 事件订阅者信息
	struct SSubscibeObjInfo
	{
		TRecvHander*		m_pHandler;		// 订阅者类指针
		int					m_nCallCount;	// 订阅者类调用次数
		bool				m_bIsNeedRemove;// 是否需要删除
		char				m_szDesc[32];	// 描述信息，便于DEBUG

		SSubscibeObjInfo( TRecvHander* pHandler, const char* szDesc )
		{
			m_pHandler = pHandler;
			m_nCallCount = 0;
			m_bIsNeedRemove = false;
			memset( m_szDesc, 0, 32 );
			if( szDesc != NULL )
				memcpy( m_szDesc, szDesc, sizeof(szDesc) );
		}

		void AddRef()
		{
			m_nCallCount++;
		}
		void SubRef()
		{
			m_nCallCount--;
			if( m_nCallCount <= 0 )
				m_nCallCount = 0;
		}
	};

	// 单个事件订阅者Key列表
	typedef list<SSubscibeObjInfo>		LIST_SUBSCIBEOBJ_INFOS;
	// 全部事件订阅者Key列表
	typedef std::map<SEventKey, LIST_SUBSCIBEOBJ_INFOS>	MAP_SUBSCIBEOBJ_INFOS;
private:
	MAP_SUBSCIBEOBJ_INFOS			m_mapSubsibeObjInfos;		// 全部事件订阅者列表
	TFireEventObj					m_FireEventObj;				// 事件发送者
	int								m_nFiredLayerNum;			// 事件已经发送的层级数
public:
	TEventEngine()
		:m_nFiredLayerNum( 0 )
	{

	}
	virtual ~TEventEngine()
	{
		typename MAP_SUBSCIBEOBJ_INFOS::iterator Ite = m_mapSubsibeObjInfos.begin();
		for( ; Ite != m_mapSubsibeObjInfos.end(); ++Ite )
		{
			LIST_SUBSCIBEOBJ_INFOS* pList = &((*Ite).second);
			pList->clear();
		}
		m_mapSubsibeObjInfos.clear();
	}
	// 订阅事件
	bool Subscibe( TRecvHander* pHandler, EVENT_TYPE usEventID, unsigned short usSrcType,
		unsigned int unSrcID, const char* szDesc )
	{
		if( pHandler == NULL )
			return false;
		// 事件Key
		SEventKey tagEventKey;
		tagEventKey.m_CoreKey.m_usEventID = usEventID;
		tagEventKey.m_CoreKey.m_usSrcType = usSrcType;
		tagEventKey.m_CoreKey.m_unSrcID   = unSrcID;
		// 订阅者信息
		SSubscibeObjInfo tagSubscibeObjInfo( pHandler, szDesc );

		// 添加到订阅者列表中
		typename MAP_SUBSCIBEOBJ_INFOS::iterator Ite = m_mapSubsibeObjInfos.find( tagEventKey );
		if( Ite == m_mapSubsibeObjInfos.end() )
		{
			LIST_SUBSCIBEOBJ_INFOS tagList;
			tagList.push_front( tagSubscibeObjInfo );
			m_mapSubsibeObjInfos.insert( std::make_pair(tagEventKey, tagList) );
		}
		else
		{
			LIST_SUBSCIBEOBJ_INFOS* pList = &((*Ite).second);
			pList->push_front( tagSubscibeObjInfo );
		}
		return true;
	}
	// 取消事件订阅
	bool UnSubscibe( TRecvHander* pHandler, EVENT_TYPE usEventID, 
		unsigned short usSrcType, unsigned int unSrcID )
	{
		if( pHandler == NULL )
			return false;
		// 事件Key
		SEventKey tagEventKey;
		tagEventKey.m_CoreKey.m_usEventID = usEventID;
		tagEventKey.m_CoreKey.m_usSrcType = usSrcType;
		tagEventKey.m_CoreKey.m_unSrcID   = unSrcID;

		typename MAP_SUBSCIBEOBJ_INFOS::iterator Ite = m_mapSubsibeObjInfos.find( tagEventKey );
		if( Ite != m_mapSubsibeObjInfos.end() )
		{
			LIST_SUBSCIBEOBJ_INFOS* pList = &((*Ite).second);
			typename LIST_SUBSCIBEOBJ_INFOS::iterator IteList = pList->begin();
			for( ; IteList != pList->end(); ++IteList )
			{
				SSubscibeObjInfo* pInfo = &(*IteList);
				if( pInfo->m_pHandler == pHandler )
				{
					if( pInfo->m_nCallCount == 0 )
					{
						pList->erase( IteList );
					}
					else
					{
						pInfo->m_bIsNeedRemove = true;
					}
					break;
				}
			}
		}
		return true;
	}
	// 发送订阅事件
	bool Fire( EVENT_TYPE usID, unsigned short usSrcType,
		unsigned int unSrcID, const char* szEventDesc, int nEventLen )
	{
		// 事件Key
		SEventKey tagEventKey;
		tagEventKey.m_CoreKey.m_usEventID = usID;
		tagEventKey.m_CoreKey.m_usSrcType = usSrcType;
		tagEventKey.m_CoreKey.m_unSrcID   = unSrcID;

		// 如果有发送者源UID则增加一次单独发送
		if( tagEventKey.m_CoreKey.m_unSrcID != 0 )
		{
			bool bRet = __Fire( tagEventKey, usID, usSrcType, unSrcID, szEventDesc, nEventLen );
			if( !bRet )
			{
				return false;
			}
		}

		// 无论是否有发送者源UID的都要进行一次通告发送
		tagEventKey.m_CoreKey.m_unSrcID = 0;
		bool bRet = __Fire( tagEventKey, usID, usSrcType, unSrcID, szEventDesc, nEventLen );
		if( !bRet )
		{
			return false;
		}

		return true;
	}
private:
	bool __Fire( SEventKey& tagEventKey, EVENT_TYPE usID, unsigned short usSrcType,
		unsigned int unSrcID, const char* szEventDesc, int nEventLen )
	{
		m_nFiredLayerNum++;		// 增加传递层级次数

		if( m_nFiredLayerNum >= gs_MAX_EVENT_FIRE_LAYER_NUM )
		{
			FKLOG("事件发送死循环调用 EventID = %d, SrcType = %d", usID, usSrcType );
			m_nFiredLayerNum--;
			return false;
		}

		typename MAP_SUBSCIBEOBJ_INFOS::iterator Ite = m_mapSubsibeObjInfos.find( tagEventKey );
		if( Ite != m_mapSubsibeObjInfos.end() )
		{
			LIST_SUBSCIBEOBJ_INFOS* pList = &((*Ite).second);
			typename LIST_SUBSCIBEOBJ_INFOS::iterator IteList = pList->begin();
			for( ; IteList != pList->end(); )
			{
				SSubscibeObjInfo* pInfo = &(*IteList);
				if( pInfo->m_nCallCount >= gs_MAX_EVENT_REF_LAYER_NUM )
				{
					FKLOG("Error:同一事件循环调用 EventID = %d, SrcType = %d", usID, usSrcType );
					m_nFiredLayerNum--;
					return false;
				}

				if( !pInfo->m_bIsNeedRemove )
				{
					// 不需要移除，则实际处理
					bool bRet = false;

					try
					{
						// 这里要容错，错误概率较高= =
						pInfo->AddRef();
						bRet = m_FireEventObj(pInfo->m_pHandler, usID, usSrcType, unSrcID, szEventDesc, nEventLen );
						pInfo->SubRef();
					}
					catch ( ... )
					{
						pInfo->SubRef();
						FKLOG("非法事件 EventID = %d, SrcType = %d", usID, usSrcType);
						return false;
					}

					{
						// 因为被处理后，有一定概率再次被修改RemoveFlag，所以这里增加一道检查
						if( pInfo->m_bIsNeedRemove && pInfo->m_nCallCount == 0 )
						{
							IteList = pList->erase( IteList );
						}
						else
						{
							++IteList;
						}
					}

					if( !bRet )
					{
						m_nFiredLayerNum--;
						return false;
					}
				}
				else
				{
					// 该移除了
					if( pInfo->m_nCallCount == 0 )
					{
						IteList = pList->erase( IteList );
					}
					else
					{
						++IteList;
					}
				}
			}
		}

		m_nFiredLayerNum--;

		return true;
	}
};