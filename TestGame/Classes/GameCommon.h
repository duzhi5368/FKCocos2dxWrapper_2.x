//*************************************************************************
//	创建日期:	2015-3-19
//	文件名称:	sceneMacro.h
//  创 建 人:   邓观福 Rect 	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../../FKEngine/FKEngineCommonHead.h"
#include "GameResource.h"
#include "EventCommon.h"
//-------------------------------------------------------------------------
// 场景类型
enum ENMU_SubSceneType
{
	eSubSceneType_Min = 0,					
	eSubSceneType_Login,						// 登录主场景
	eSubSceneType_Play,							// 消灭星星主场景
	eSubSceneType_Shop,							// 商城场景
	eSubSceneType_Achie,						// 成就场景
	eSubSceneType_Max,
};

// 游戏场景中的按钮枚举
enum ENMU_TEST_PLAYBUTTON_TYPE
{
	eTestPlayButton_Min = eSubSceneType_Max,
	eTestPlayButton_Music,						// 音乐按钮
	eTestPlayButton_AddMoney,					// 加钱按钮
	eTestPlayButton_All,						// 炸弹，消除指定小范围方块按钮
	eTestPlayButton_SortStar,					// 随机重组全部方块按钮按钮
	eTestPlayButton_HandMagic,					// 单消某一个方块按钮
	eTestPlayButton_Max,
};

// 消息弹出框类型 - 奖励
enum ENMU_MSG_GIFT_TYPE
{
	eAwardType_Min = 0,
	eAwardType_LoginGift,						// 登录奖励
	eAwardType_NewGuyGift,						// 新人奖励
	eAwardType_PetGift,							// 宠物奖励
	eAwardType_Max,
};

// 消息弹出框类型 - 普通
enum ENMU_MSG_NORMAL_TYPE
{
	eNormalType_Min = 0,
	eNormalType_GameSuccess,					// 通关
	eNormalType_GameOver,						// 失败
	eNormalType_Max,
};

// 音效类型
enum ENMU_SOUND_TYPE
{
	eSound_Min = 0,
	eSound_Button,
	eSound_StarOut,
	eSound_StarClick,
	eSound_StarCombo,
	eSound_StarGood,
	eSound_StarAmazing,
	eSound_StarUnbeliveable,
	eSound_Max,
};

// 网络消息主类型
enum ENMU_NET_MSGMAIN_TYPE
{
	eNetMsg_Main_Min = 100,
	eNetMsg_Main_Game,
	eNetMsg_Main_Max,
};

// 网络消息子类型
enum ENMU_NET_MSGSUB_TYPE
{
	eNetMsg_Sub_Min = 10,
	eNetMsg_Sub_PlayStar,
	eNetMsg_Sub_ShopItem,
	eNetMsg_Sub_AchieThings,
	eNetMsg_Sub_Max,
};

// 上层使用,网络消息模拟结构体 server to client 
struct S_GAME_INTVALUE_SUB_S2C
{
	int nValue;
	int nFlag;
	S_GAME_INTVALUE_SUB_S2C()
	{
		memset(this,0,sizeof(S_GAME_INTVALUE_SUB_S2C));
	}
};

// 底层传输使用,网络消息模拟结构体  server to client
struct S_GAME_INTVALUE_S2C
{
	MSG_MAIN_TYPE mainType;
	MSG_SUB_TYPE subType;
	S_GAME_INTVALUE_SUB_S2C  sSubS2C;
	S_GAME_INTVALUE_S2C()
	{
		memset(this,0,sizeof(S_GAME_INTVALUE_S2C));
	}
};