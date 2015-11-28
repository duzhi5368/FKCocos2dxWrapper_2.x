/********************************************************************
	created:	2014/11/03
	file base:	FKCW_Action_TreeFadeIn
	author:		FreeKnight
	
	purpose:	
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_Action_Macros.h"
//--------------------------------------------------------------------
// 渐隐一个对象，同时，渐隐其全部子节点
// 注：CCFadeIn 不会渐隐其子节点
class FKCW_Action_TreeFadeIn : public CCFadeIn
{
private:
	CCArray			m_vecExcludeList;			// 排除的节点列表
	bool			m_bIsRecursivelyExclude;	// 此值为true则表示被排除的节点连其子节点也将被排除在外
protected:
	FKCW_Action_TreeFadeIn();
	// 递归渐隐
	void FadeInRecursively( CCNode* p_pRoot, float p_fTime );
public:
	static FKCW_Action_TreeFadeIn* Create( float p_fDuration );
	// 添加需要被排除的节点列表
	void ExcludeNode( CCNode* p_pRoot, bool p_bIsRecursively = true );

	virtual void update(float time);
	virtual CCActionInterval* reverse(void);
};