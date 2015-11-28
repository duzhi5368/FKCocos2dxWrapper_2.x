/********************************************************************
	created:	2014/11/03
	file base:	FKCW_Action_CallfuncT
	author:		FreeKnight
	
	purpose:	
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_Action_Macros.h"
//--------------------------------------------------------------------
template< typename T >
class FKCW_Action_CallfuncT : public CCCallFunc
{
public:
	// 对象的函数指针
	typedef void (CCObject::*SEL_CallFuncT)(T);
private:
	T				m_arg;
	SEL_CallFuncT	m_selector;
protected:
	FKCW_Action_CallfuncT() {}

public:
	virtual ~FKCW_Action_CallfuncT() {}

	static FKCW_Action_CallfuncT* Create(CCObject* pSelectorTarget, SEL_CallFuncT selector, T arg);

	virtual bool initWithTarget(CCObject* pSelectorTarget, SEL_CallFuncT selector, T arg);
	virtual CCObject* copyWithZone(CCZone* pZone);
	virtual void execute();
};
//--------------------------------------------------------------------
template< typename T >
FKCW_Action_CallfuncT<T>* FKCW_Action_CallfuncT<T>::Create(CCObject* pSelectorTarget, SEL_CallFuncT selector, T arg)
{
	FKCW_Action_CallfuncT* c = new FKCW_Action_CallfuncT();
	if(c->initWithTarget(pSelectorTarget, selector, arg)) 
	{
		return (FKCW_Action_CallfuncT*)c->autorelease();
	}

	c->release();
	return NULL;
}
//--------------------------------------------------------------------
template <typename T>
bool FKCW_Action_CallfuncT<T>::initWithTarget(CCObject* pSelectorTarget, SEL_CallFuncT selector, T arg) 
{
	if (CCCallFunc::initWithTarget(pSelectorTarget)) 
	{
		m_arg = arg;
		m_selector = selector;
		return true;
	}

	return false;
}
//--------------------------------------------------------------------
template <typename T>
CCObject* FKCW_Action_CallfuncT<T>::copyWithZone(CCZone* pZone) 
{
	CCZone* pNewZone = NULL;
	FKCW_Action_CallfuncT* pRet = NULL;

	if (pZone && pZone->m_pCopyObject) 
	{
		pRet = (FKCW_Action_CallfuncT*)pZone->m_pCopyObject;
	} 
	else 
	{
		pRet = new FKCW_Action_CallfuncT();
		pZone = pNewZone = new CCZone(pRet);
	}

	CCCallFunc::copyWithZone(pZone);
	pRet->initWithTarget(m_pSelectorTarget, m_selector, m_arg);
	CC_SAFE_DELETE(pNewZone);
	return pRet;
}
//--------------------------------------------------------------------
template <typename T>
void FKCW_Action_CallfuncT<T>::execute() 
{
	if(m_selector) 
	{
		(m_pSelectorTarget->*m_selector)(m_arg);
	}
}
//--------------------------------------------------------------------