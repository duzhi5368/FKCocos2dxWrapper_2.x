//-------------------------------------------------------------------------
#include "EventBridge.h"
#include "GameCommon.h"
//-------------------------------------------------------------------------


EventBridge::EventBridge(void)
{
}


EventBridge::~EventBridge(void)
{
}
//-------------------------------------------------------------------------
// 收到执行消息处理回调
// 参数：usID 事件ID
// 参数：usSrcType 发送者源类型（参见相关枚举）
// 参数：unSrcID 发送者唯一标示（可能是角色UID，物品UID，
//		以及某对象指针等，视消息类型逻辑程序员自己规定）
// 参数：szEvent 消息具体数据指针
// 参数：nEventLen 消息数据的长度
void EventBridge::OnExec( EVENT_TYPE usID, unsigned short usSrcType,
					 unsigned int unSrcID, const char* szEvent, int nEventLen )
{
	if (usID <= eEvent_Min || eEventE_Max <= usID)
	{
		return;
	}
	if (usSrcType <= eEvent_Src_Min || eEvent_Src_Max <= usSrcType)
	{
		return;
	}

	int nTag = unSrcID;
	switch (usSrcType)
	{
	case eEvent_Scene_Switch:
		__SwitchScene(nTag);
		break;

	case eEvent_Play_ButtonClick:
		__ClickPlayButton(nTag);
		break;

	default:
		break;
	}


}
//-------------------------------------------------------------------------
// 切换Scene
void EventBridge::__SwitchScene(int nTag)
{
	FKCW_SceneExMgr_SceneExMgr* pSceneSystem = GetSceneSystem();
	if (pSceneSystem == NULL)
	{
		return;
	}

	switch (nTag)
	{
	case eSubSceneType_Login:
		pSceneSystem->pushScene(LoadScene("CLoginScene") );
		break;
	case eSubSceneType_Play:
		pSceneSystem->pushScene(LoadScene("CPlayTestScene") );
		break;
	case eSubSceneType_Shop:
		pSceneSystem->pushScene(LoadScene("CShopTestScene") );
		break;
	case eSubSceneType_Achie:
		pSceneSystem->pushScene(LoadScene("CAchieTestScene") );
		break;
	default:
		break;
	}
}
//-------------------------------------------------------------------------
// 按钮回调信息处理
void EventBridge::__ClickPlayButton(int nTag)
{
	switch (nTag)
	{
	case eTestPlayButton_AddMoney:
		break;
	case eTestPlayButton_All:
		break;
	case eTestPlayButton_SortStar:
		break;
	case eTestPlayButton_HandMagic:
		break;
	default:
		break;
	}

	// 模拟服务器向 客户端 发送消息
	S_GAME_INTVALUE_S2C sGameIntValue;
	sGameIntValue.mainType = eNetMsg_Main_Game;
	sGameIntValue.subType = eNetMsg_Sub_PlayStar;
	S_GAME_INTVALUE_SUB_S2C sGameIntVlaueSub;
	sGameIntVlaueSub.nValue = nTag;
	sGameIntVlaueSub.nFlag = 1;
	sGameIntValue.sSubS2C = sGameIntVlaueSub;

	IMsgDispatchSystem* pMsgDispath = GetMsgDispatchSystem();
	if (pMsgDispath)
	{
		pMsgDispath->DispatchMsg((const char*)&sGameIntValue,sizeof(S_GAME_INTVALUE_S2C));
	}
}
//-------------------------------------------------------------------------
// 注册全局事件
void EventBridge::RegisteFKEvent()
{
	IEventSystem* pEvenSystem = GetEventSystem();
	if (pEvenSystem == NULL)
	{
		return;
	}
	pEvenSystem->SubscibeExec(this,eEvent_UIEvent,eEvent_Scene_Switch);
	pEvenSystem->SubscibeExec(this,eEvent_UIEvent,eEvent_Play_ButtonClick);
}
//-------------------------------------------------------------------------
// 移除全局事件
void EventBridge::UnRegisteFKEvent()
{
	IEventSystem* pEvenSystem = GetEventSystem();
	if (pEvenSystem == NULL)
	{
		return;
	}
	pEvenSystem->UnSubscibeExec(this,eEvent_UIEvent,eEvent_Scene_Switch);
	pEvenSystem->UnSubscibeExec(this,eEvent_UIEvent,eEvent_Play_ButtonClick);
}
//-------------------------------------------------------------------------
