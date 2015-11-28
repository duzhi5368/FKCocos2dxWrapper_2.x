//*************************************************************************
//	创建日期:	2014-11-5
//	文件名称:	FKCW_UI_ToastLayer.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	弹出一个Tip信息
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_UI_Macros.h"
//-------------------------------------------------------------------------
// 类似dialog的信息窗口，但不占有焦点，且过一段时间会自动消失
class FKCW_UI_ToastLayer : public CCLayer
{
protected:
	FKCW_UI_ToastLayer();
public:
	virtual ~FKCW_UI_ToastLayer();
	// 创建一个Toast对象实例
	// 参数：owner		所有者节点（toast将被作为子节点添加到 owner中，并设置为最大Z值）
	//		content		内容节点（这个将是显示的真实信息，但用户需要设置好合适的锚点和位置）
	//		tag			对象标签（如果该值为-1，则可同时显示多个实例，否则不可同时显示）
	//		duration	显示时常（若为默认0，则表示显示3秒）
	//		inAction	显示时的动作（若为默认0，则表示使用淡入动作）
	//		outAction	消失时的动作（若为默认0，则表示使用淡出动作）
	static FKCW_UI_ToastLayer* Create(CCNode* owner, CCNode* content, int tag = -1,
		float duration = 0, CCFiniteTimeAction* inAction = NULL, CCFiniteTimeAction* outAction = NULL);

	// 强制移除一个Toast( 通过tag查找 )
	static void					ForceRemove( CCNode* p_pOwner, int p_nTag );
};