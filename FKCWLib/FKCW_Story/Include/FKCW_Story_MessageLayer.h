//*************************************************************************
//	创建日期:	2015-1-3
//	文件名称:	FKCW_Story_MessageLayer.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_Story_Macros.h"
//-------------------------------------------------------------------------
class FKCW_Story_Command;
class FKCW_Story_Player;
class FKCW_Story_TimerLabelTTF;
//-------------------------------------------------------------------------
class FKCW_Story_MessageLayer : public CCLayer 
{
private:
	CCLabelTTF* m_msgLabel;					// 信息标签
	CCLabelTTF* m_nameLabel;				// 名字标签
	bool m_looping;							// 是否循环显示完毕
	FKCW_Story_Player* m_player;			// Player
	float m_wait;							// 等待时间

protected:
	FKCW_Story_MessageLayer();

	// 回调函数
	void onDialogEndLooping();
	void onDialogDisplayedSomeWhile(float delta);

	// 用户点击消息
	void handleUserClick();

public:
	virtual ~FKCW_Story_MessageLayer();
	static FKCW_Story_MessageLayer* create(FKCW_Story_Player* player);

	virtual bool initWithPlayer(FKCW_Story_Player* player);
	virtual void onExit();
	virtual void onEnter();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void keyBackClicked();

	// 开始显示一段对话
	void showMessage(FKCW_Story_Command* cmd);
	// 自适应换行文字
	/*	horizontalSpacing： 水平间距 
		verticalSpacing：   垂直间距 
		lineWidth：         一行的最大宽度 
	*/  
	CCLabelTTF* AutoNewLineText(std::string _string, const char *fontName, 
	float fontSize, float horizontalSpacing, float verticalSpacing, float lineWidth, ccColor3B p_fontColor );
	// 字符事件回调
	void TTFItemDelayOver(CCNode* pSender);
};
