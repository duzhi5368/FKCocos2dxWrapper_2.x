//*************************************************************************
//	创建日期:	2014-11-26
//	文件名称:	ListControllerLayer.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "cocos2d.h"
//-------------------------------------------------------------------------
USING_NS_CC;
//-------------------------------------------------------------------------
class CListControllerLayer : public CCLayer
{
public:
	CListControllerLayer();
	~CListControllerLayer();
public:
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);

	virtual void onEnter();

	void menuCallback(CCObject * pSender);
	void closeCallback(CCObject * pSender);
protected:
	ccColor3B	_GetColorByName(std::string p_szName);
private:
	CCPoint m_tBeginPos;
	CCMenu* m_pItemMenu;
};
//-------------------------------------------------------------------------