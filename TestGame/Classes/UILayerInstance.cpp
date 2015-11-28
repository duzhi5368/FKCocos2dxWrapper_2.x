//-------------------------------------------------------------------------
#include "UILayerInstance.h"
//-------------------------------------------------------------------------


CUILayerInstance::CUILayerInstance(void)
{
	m_vecTouchNode.clear();
}


CUILayerInstance::~CUILayerInstance(void)
{
}
//-------------------------------------------------------------------------
bool CUILayerInstance::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return CCLayer::ccTouchBegan(pTouch,pEvent);
}
//-------------------------------------------------------------------------
void CUILayerInstance::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	CCLayer::ccTouchMoved(pTouch,pEvent);
}
//-------------------------------------------------------------------------
void CUILayerInstance::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{

	CCLayer::ccTouchEnded(pTouch,pEvent);
}
//-------------------------------------------------------------------------
void CUILayerInstance::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
	CCLayer::ccTouchCancelled(pTouch,pEvent);
}
//-------------------------------------------------------------------------
void CUILayerInstance::AddTouchObject(CCNode* pNode)
{
	if (pNode == NULL)
	{
		return;
	}

	std::vector<CCNode*>::iterator itor = find(m_vecTouchNode.begin(),m_vecTouchNode.end(),pNode);
	if (itor == m_vecTouchNode.end())
	{
		m_vecTouchNode.push_back(pNode);
	}
}
//-------------------------------------------------------------------------
void CUILayerInstance::RemoveTouchObject(CCNode* pNode)
{
	if (pNode == NULL)
	{
		return;
	}

	std::vector<CCNode*>::iterator itor = find(m_vecTouchNode.begin(),m_vecTouchNode.end(),pNode);
	if (itor != m_vecTouchNode.end())
	{
		m_vecTouchNode.erase(itor);
	}
}
//-------------------------------------------------------------------------
void CUILayerInstance::RemoveTouchObjecetAll()
{
	m_vecTouchNode.clear();
}
//-------------------------------------------------------------------------
