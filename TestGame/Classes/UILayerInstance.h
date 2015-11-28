//*************************************************************************
//	创建日期:	2015-3-25
//	文件名称:	PlayUILayer.h
//  创 建 人:   邓观福 Rect 	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "GameCommon.h"
//-------------------------------------------------------------------------

class CUILayerInstance : public CCLayer
{
	CUILayerInstance(void);
	~CUILayerInstance(void);
public:
	static CUILayerInstance& instance()
	{
		static CUILayerInstance _inst;
		return _inst;
	}

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

public:

	void	AddTouchObject(CCNode* pNode);
	void	RemoveTouchObject(CCNode* pNode);
	void	RemoveTouchObjecetAll();

private:

	std::vector<CCNode*>		m_vecTouchNode;
};

