//*************************************************************************
//	创建日期:	2015-3-19
//	文件名称:	sceneMacro.h
//  创 建 人:   邓观福 Rect 	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../FKEngine/FKEngineCommonHead.h"
#include "GameResource.h"
#include "EventCommon.h"
//-------------------------------------------------------------------------
enum ENMU_TEST_KFENGINE_TYPE
{
	eTestFKEngine_Default = 0,
	eTestFKEngine_Login,
	eTestFKEngine_Play,
	eTestFKEngine_Shop,
	eTestFKEngine_Achie,
};

enum ENMU_AWARD_TYPE
{
	eAwardType_Min = 0,
	eAwardType_LoginGift,  // 登录奖励
	eAwardType_NewGuyGift, // 新人奖励
	eAwardType_PetGift,    // 宠物奖励
	eAwardType_Max,
};

enum ENMU_EFFECT_TYPE
{
	eEffect_Default = 0,
	eEffect_Button,
};
