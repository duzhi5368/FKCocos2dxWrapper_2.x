//-------------------------------------------------------------------------
#include "FKEventSystem.h"
#include "TEventEngine.h"
//-------------------------------------------------------------------------
IEventSystem* gs_pEventSystem;
FKEventSystem gs_FKEventSystem;
//-------------------------------------------------------------------------
// 获取事件分发器接口
IEventSystem* GetEventSystem()
{
	return gs_pEventSystem;
}
//-------------------------------------------------------------------------
FKEventSystem::FKEventSystem()
{
	gs_pEventSystem = this;
}
//-------------------------------------------------------------------------
FKEventSystem::~FKEventSystem()
{
	gs_pEventSystem = NULL;
}
//-------------------------------------------------------------------------
// 释放
void FKEventSystem::Release()
{

}
//-------------------------------------------------------------------------
// 发起投票事件
// 参数说明参见上文 OnVote
// 返回值：是否全员投票通过
bool FKEventSystem::FireVote( EVENT_TYPE usID, unsigned short usSrcType,
							 unsigned int unSrcID, const char* szEvent, int nEventLen )
{
	return m_VoteCenter.Fire( usID, usSrcType, unSrcID, szEvent, nEventLen );
}
//-------------------------------------------------------------------------
// 发起执行事件
// 参数说明参见上文 OnExec
void FKEventSystem::FireExec( EVENT_TYPE usID, unsigned short usSrcType,
							 unsigned int unSrcID, const char* szEvent, int nEventLen )
{
	m_ExecCenter.Fire( usID, usSrcType, unSrcID, szEvent, nEventLen );
}
//-------------------------------------------------------------------------
// 订阅投票事件
// 参数：pHandler 订阅者类
// 参数：usSrcType 发送者源类型（参见相关枚举）
// 参数：unSrcID 发送者唯一标示（可能是角色UID，物品UID，
//		以及某对象指针等，视消息类型逻辑程序员自己规定）
//		该值允许为0，则表示订阅全部，不再限定指定ID
// 参数：szDesc 订阅者类的订阅描述，用来调试输出log
// 返回值：是否成功订阅
bool FKEventSystem::SubscibeVote( IVoteEventHandler* pHandler, EVENT_TYPE usID, 
								 unsigned short usSrcType, unsigned int unSrcID, const char* szDesc )
{
	return m_VoteCenter.Subscibe( pHandler, usID, usSrcType, unSrcID, szDesc );
}
//-------------------------------------------------------------------------
// 订阅执行事件
// 参数返回值：同上函数
bool FKEventSystem::SubscibeExec( IExecEventHandler* pHandler, EVENT_TYPE usID, 
								 unsigned short usSrcType, unsigned int unSrcID, const char* szDesc )
{
	return m_ExecCenter.Subscibe( pHandler, usID, usSrcType, unSrcID, szDesc );
}
//-------------------------------------------------------------------------
// 取消一份投票订阅
bool FKEventSystem::UnSubscibeVote( IVoteEventHandler* pHandler, EVENT_TYPE usID, 
								   unsigned short usSrcType, unsigned int unSrcID )
{
	return m_VoteCenter.UnSubscibe( pHandler, usID, usSrcType, unSrcID );
}
//-------------------------------------------------------------------------
// 取消一份执行订阅
bool FKEventSystem::UnSubscibeExec( IExecEventHandler* pHandler, EVENT_TYPE usID, 
								   unsigned short usSrcType, unsigned int unSrcID )
{
	return m_ExecCenter.UnSubscibe( pHandler, usID, usSrcType, unSrcID );
}
//-------------------------------------------------------------------------