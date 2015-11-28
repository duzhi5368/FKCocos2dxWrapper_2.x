//*************************************************************************
//	创建日期:	2015-1-3
//	文件名称:	FKCW_Story_ClickLayer.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_Story_Macros.h"
//-------------------------------------------------------------------------
class FKCW_Story_Player;
class FKCW_Story_ClickLayer : public CCLayer
{
private:
	FKCW_Story_Player* m_player;

protected:
	FKCW_Story_ClickLayer();

public:
	virtual ~FKCW_Story_ClickLayer();
	static FKCW_Story_ClickLayer* create(FKCW_Story_Player* player);

	virtual bool initWithPlayer(FKCW_Story_Player* player);
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	virtual void keyBackClicked();
};