//--------------------------------------------------------------------
#include "../Include/FKCW_UI_ToastLayer.h"
#include "../../FKCW_Action/Include/FKCW_Action_TreeFadeIn.h"
#include "../../FKCW_Action/Include/FKCW_Action_TreeFadeOut.h"
#include "../../FKCW_Base/Include/FKCW_Base_Macro.h"
//--------------------------------------------------------------------
FKCW_UI_ToastLayer::FKCW_UI_ToastLayer()
{

}
//--------------------------------------------------------------------
FKCW_UI_ToastLayer::~FKCW_UI_ToastLayer()
{

}
//--------------------------------------------------------------------
// 创建一个Toast对象实例
// 参数：owner		所有者节点（toast将被作为子节点添加到 owner中，并设置为最大Z值）
//		content		内容节点（这个将是显示的真实信息，但用户需要设置好合适的锚点和位置）
//		tag			对象标签（如果该值为-1，则可同时显示多个实例，否则不可同时显示）
//		duration	显示时常（若为默认0，则表示显示3秒）
//		inAction	显示时的动作（若为默认0，则表示使用淡入动作）
//		outAction	消失时的动作（若为默认0，则表示使用淡出动作）
FKCW_UI_ToastLayer* FKCW_UI_ToastLayer::Create(CCNode* owner, CCNode* content, int tag,
			float duration, CCFiniteTimeAction* inAction, CCFiniteTimeAction* outAction )
{
	// 检查标签是否已经存在
	if(tag != -1) 
	{
		if(owner->getChildByTag(tag)) 
		{
			return NULL;
		}
	}

	FKCW_UI_ToastLayer* t = new FKCW_UI_ToastLayer();
	t->init();
	t->setTag(tag);

	// 添加到owner
	owner->addChild(t, MAX_INT);

	// 添加子显示节点
	t->addChild(content);

	// 执行显示隐藏动作
	if(inAction == NULL) 
	{
		CCNodeRGBA* n = dynamic_cast<CCNodeRGBA*>(content);
		if(n)
			n->setOpacity(0);
		inAction = FKCW_Action_TreeFadeIn::create(0.5f);
	}
	if(outAction == NULL) 
	{
		outAction = FKCW_Action_TreeFadeOut::create(0.5f);
	}
	content->runAction(CCSequence::create(inAction,
		CCDelayTime::create(duration > 0 ? duration : 3),
		outAction,
		CCCallFunc::create(t, callfunc_selector(CCNode::removeFromParent)),
		NULL));
	t->autorelease();
	return t;
}
//--------------------------------------------------------------------
// 强制移除一个Toast( 通过tag查找 )
void FKCW_UI_ToastLayer::ForceRemove( CCNode* p_pOwner, int p_nTag )
{
	CCNode* c = p_pOwner->getChildByTag(p_nTag);
	if(c) 
	{
		c->removeFromParent();
	}
}
//--------------------------------------------------------------------