//*************************************************************************
//	创建日期:	2015-1-12
//	文件名称:	IEventSystem.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
/*
	解释：
		Q1：什么叫vote投票事件，干什么的。
		A1：想象有一堆人投票，要做一个事情。所有关心这个事情的人都可以拥有投票权。
			一旦有人不支持这个事情，则该事情就不可执行。该投票事件就是模拟该类型情况的。
			假设游戏中玩家要购买一个道具BuySth( ... )，该函数内可进行投票事件发起：
					GetEventSystem()->FireVote( BUY_EVENT_ID, .. )
			关心道具购买的类就可以在构造函数中进行该事件的订阅：
					GetEventSystem()->SubscibeVote( BUY_EVENT_ID, ... )
			一旦订阅，则任何对象要求购买道具时，必须向订阅类通知，订阅类将得到
					IVoteEventHandler::OnVote( BUY_EVENT_ID, ... )
			该消息，得到该消息后可进行检查，并最后通过返回值告知发起人自己是否允许该事件发生。
			例如，若金钱管理器认为玩家购买道具时金钱少于100则禁止它购买道具，则伪代码为
					IVoteEventHandler::OnVote( BUY_EVENT_ID, ... ){ if(Gold<100)return false; else return true;}
			一个购买事件可能不仅仅是金钱管理器要投票，背包栏对象可能也会订阅并投票。
			任何一个投票者投反对票(return false;)则发起人得到的( fireVote函数 )结果返回值将为false.
		Q2：什么叫exec执行事件，干什么的。
		A2：这是一个消息通知机制。只要某类订阅了指定事件，则当事件发生时，该类将得到一个通知（onExec() ）,
			注意的人，任何人任何地点（任何代码处）都可以发起事件的通知。
			收到通知后(OnExec函数内)，依然可以创建新的通知并通告( GetEventSystem()->SubscibeExec( BUY_EVENT_ID, ... ))他人,
			但因需要防范A通知B，B又通知A这样的死循环回调出现，事件收到引用层级的保护。
		Q3：这两者相同点和区别是？
		A3：两者都是即时通知的，参数也基本一致，而且在订阅者在收到事件时，都可以做自己的逻辑处理( onVote同样也可以，最终给与一个回馈票就好了，但不推荐 )。
			但区别是两者的功能，因为Vote是一票否决机制，所以当一个订阅者投出否决票后，
			该事件已经确定被否决，故Vote不再通知其他订阅者，所以，如果某订阅者OnVote中做逻辑处理，
			将可能不能被通知到，这取决于他之前的订阅者投票结果。（而投票者顺序是极为混乱的，暂时没有优先级和编号概念）
			而Exec则当然可以保证每个订阅者都可以收到事件通知。
		Q4：它是即时(立即得到投票结果，立即通知并执行)的，还是延时(异步)的？
		A4：它是即时的，单线程的，立刻得到结果的，而不需要等待下一逻辑帧或一段时间。
	注意：IExecEventHandler 不要做网络消息分发。若需要网络消息分发，请使用IMsgDispatchSystem
*/
//-------------------------------------------------------------------------
// 消息类型定义
typedef unsigned short EVENT_TYPE;
//-------------------------------------------------------------------------
// 投票事件消息处理
class IVoteEventHandler
{
public:
	// 收到投票消息处理回调
	// 参数：usID 事件ID
	// 参数：usSrcType 发送者源类型（参见相关枚举）
	// 参数：unSrcID 发送者唯一标示（可能是角色UID，物品UID，
	//		以及某对象指针等，视消息类型逻辑程序员自己规定）
	// 参数：szEvent 消息具体数据指针
	// 参数：nEventLen 消息数据的长度
	// 返回值：是否允许该事件发生( 一票否决制 )
	virtual bool	OnVote( EVENT_TYPE usID, unsigned short usSrcType,
		unsigned int unSrcID, const char* szEvent, int nEventLen ) = 0;
};
//-------------------------------------------------------------------------
// 执行事件消息处理
class IExecEventHandler
{
public:
	// 收到执行消息处理回调
	// 参数：usID 事件ID
	// 参数：usSrcType 发送者源类型（参见相关枚举）
	// 参数：unSrcID 发送者唯一标示（可能是角色UID，物品UID，
	//		以及某对象指针等，视消息类型逻辑程序员自己规定）
	// 参数：szEvent 消息具体数据指针
	// 参数：nEventLen 消息数据的长度
	virtual void	OnExec( EVENT_TYPE usID, unsigned short usSrcType,
		unsigned int unSrcID, const char* szEvent, int nEventLen ) = 0;
};
//-------------------------------------------------------------------------
// 事件系统
class IEventSystem
{
public:
	// 释放
	virtual void		Release() = 0;
	// 发起投票事件
	// 参数说明参见上文 OnVote
	// 返回值：是否全员投票通过
	virtual bool		FireVote( EVENT_TYPE usID, unsigned short usSrcType,
		unsigned int unSrcID, const char* szEvent, int nEventLen ) = 0;
	// 发起执行事件
	// 参数说明参见上文 OnExec
	virtual void		FireExec( EVENT_TYPE usID, unsigned short usSrcType,
		unsigned int unSrcID, const char* szEvent, int nEventLen ) = 0;
	// 订阅投票事件
	// 参数：pHandler 订阅者类
	// 参数：usSrcType 发送者源类型（参见相关枚举）
	// 参数：unSrcID 发送者唯一标示（可能是角色UID，物品UID，
	//		以及某对象指针等，视消息类型逻辑程序员自己规定）
	//		该值允许为0，则表示订阅全部，不再限定指定ID
	// 参数：szDesc 订阅者类的订阅描述，用来调试输出log
	// 返回值：是否成功订阅
	virtual bool		SubscibeVote( IVoteEventHandler* pHandler, EVENT_TYPE usID, 
		unsigned short usSrcType, unsigned int unSrcID = 0, const char* szDesc = NULL ) = 0;
	// 订阅执行事件
	// 参数返回值：同上函数
	virtual bool		SubscibeExec( IExecEventHandler* pHandler, EVENT_TYPE usID, 
		unsigned short usSrcType, unsigned int unSrcID = 0, const char* szDesc = NULL ) = 0;
	// 取消一份投票订阅
	virtual bool		UnSubscibeVote( IVoteEventHandler* pHandler, EVENT_TYPE usID, 
		unsigned short usSrcType, unsigned int unSrcID = 0 ) = 0;
	// 取消一份执行订阅
	virtual bool		UnSubscibeExec( IExecEventHandler* pHandler, EVENT_TYPE usID, 
		unsigned short usSrcType, unsigned int unSrcID = 0 ) = 0;
};
//-------------------------------------------------------------------------
// 获取事件分发器接口
extern IEventSystem* GetEventSystem();
//-------------------------------------------------------------------------