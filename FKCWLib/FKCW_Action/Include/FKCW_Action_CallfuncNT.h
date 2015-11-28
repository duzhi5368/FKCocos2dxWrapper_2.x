/********************************************************************
	created:	2014/11/03
	file base:	FKCW_Action_CallfuncNT
	author:		FreeKnight
	
	purpose:	
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_Action_Macros.h"
//--------------------------------------------------------------------
template< typename T >
class FKCW_Action_CallfuncNT : public CCCallFunc
{
public:
	typedef void (CCObject::*SEL_CallFuncNT)(CCNode*, T);
private:
	T					m_arg;
	SEL_CallFuncNT		m_selector;
protected:
	FKCW_Action_CallfuncNT() {}

public:
	virtual ~FKCW_Action_CallfuncNT() {}

	static FKCW_Action_CallfuncNT* create(CCObject* pSelectorTarget, SEL_CallFuncNT selector, T arg);

	virtual bool initWithTarget(CCObject* pSelectorTarget, SEL_CallFuncNT selector, T arg);
	virtual CCObject* copyWithZone(CCZone* pZone);
	virtual void execute();
};
//--------------------------------------------------------------------
template <typename T>
FKCW_Action_CallfuncNT<T>* FKCW_Action_CallfuncNT<T>::create(CCObject* pSelectorTarget, SEL_CallFuncNT selector, T arg) 
{
	FKCW_Action_CallfuncNT* c = new FKCW_Action_CallfuncNT();
	if(c->initWithTarget(pSelectorTarget, selector, arg)) 
	{
		return (FKCW_Action_CallfuncNT*)c->autorelease();
	}

	c->release();
	return NULL;
}
//--------------------------------------------------------------------
template <typename T>
bool FKCW_Action_CallfuncNT<T>::initWithTarget(CCObject* pSelectorTarget, SEL_CallFuncNT selector, T arg)
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
CCObject* FKCW_Action_CallfuncNT<T>::copyWithZone(CCZone* pZone) 
{
	CCZone* pNewZone = NULL;
	FKCW_Action_CallfuncNT* pRet = NULL;

	if (pZone && pZone->m_pCopyObject) 
	{
		pRet = (FKCW_Action_CallfuncNT*)pZone->m_pCopyObject;
	} 
	else 
	{
		pRet = new FKCW_Action_CallfuncNT();
		pZone = pNewZone = new CCZone(pRet);
	}

	CCCallFunc::copyWithZone(pZone);
	pRet->initWithTarget(m_pSelectorTarget, m_selector, m_arg);
	CC_SAFE_DELETE(pNewZone);
	return pRet;
}
//--------------------------------------------------------------------
template <typename T>
void FKCW_Action_CallfuncNT<T>::execute() 
{
	if(m_selector) 
	{
		(m_pSelectorTarget->*m_selector)(getTarget(), m_arg);
	}
}
//--------------------------------------------------------------------