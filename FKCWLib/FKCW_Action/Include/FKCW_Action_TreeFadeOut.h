/********************************************************************
	created:	2014/11/03
	file base:	FKCW_Action_TreeFadeOut
	author:		FreeKnight
	
	purpose:	
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_Action_Macros.h"
//--------------------------------------------------------------------
// 渐显一个对象，同时，渐显其全部子节点
// 注：CCFadeOut 不会渐显其子节点
class FKCW_Action_TreeFadeOut : public CCFadeOut
{
private:
	CCArray			m_vecExcludeList;			// 排除的节点列表
	bool			m_bIsRecursivelyExclude;	// 此值为true则表示被排除的节点连其子节点也将被排除在外
protected:
	FKCW_Action_TreeFadeOut();
	// 递归渐显
	void FadeOutRecursively( CCNode* p_pRoot, float p_fTime );
public:
	static FKCW_Action_TreeFadeOut* Create( float p_fDuration );
	// 添加需要被排除的节点列表
	void ExcludeNode( CCNode* p_pRoot, bool p_bIsRecursively = true );

	virtual void update(float time);
	virtual CCActionInterval* reverse(void);
};