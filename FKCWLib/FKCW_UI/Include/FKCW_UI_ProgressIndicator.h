//*************************************************************************
//	创建日期:	2014-11-6
//	文件名称:	FKCW_UI_ProgressIndicator.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_UI_Macros.h"
//-------------------------------------------------------------------------
// 一个进度提示器（和进度条不同的是，该进度提示器是无法确认进度长度时间的）
// 注：本进度提示器自带一个消息显示Label
// 进度提示器本身可以更换图片（自带一个简易图片效果），也可以直接被隐藏
class FKCW_UI_ProgressIndicator : public CCLayerColor
{
protected:
	CCSprite*					m_pIndicator;			// 提示器精灵
	CCLabelTTF*					m_pMsgLabel;			// 信息提示Label
	CCSize						m_tagPanelSize;			// 面板大小
	float						m_fTime;				// 更新动画的时间
	CC_SYNTHESIZE_BOOL(m_bIsCancellable, Cancellable);							// 是否允许按键取消显示提示器动画
	CC_SYNTHESIZE_PASS_BY_REF_SETTER(ccColor4B, m_tagDimColor, DimColor);		// 暗色
	CC_SYNTHESIZE_PASS_BY_REF_SETTER(ccColor4B, m_tagPanelColor, PanelColor);	// 显示提示器面板的背景色
	CC_SYNTHESIZE_SETTER(CCSprite*, m_pIcon, Icon);								// 提示器动画精灵，若该值为NULL，则会使用默认的提示器动画
	CC_SYNTHESIZE_PASS_BY_REF_SETTER(string, m_strMessage, Message);			// 显示的消息
	CC_SYNTHESIZE_BOOL_SETTER(m_bIsForceSquare, ForceSquare);					// 如果为true，则表示显示提示器必须为正方形
protected:
	FKCW_UI_ProgressIndicator();
	void								_RelayOut();
	static FKCW_UI_ProgressIndicator*	Create( const string& p_strMessage );
public:
	virtual ~FKCW_UI_ProgressIndicator();

	// 重载父类函数
	virtual bool initWithMessage(const string& message);
	virtual void draw();
	virtual void update(float delta);
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	virtual void keyBackClicked();

	// 创建一个进度提示器，若原本没有，则会创建新的。
	// 注：进度提示器将添加到当前运行的场景中，且会以最大Z值出现
	static FKCW_UI_ProgressIndicator* CreateInstance(const string& message);
	static FKCW_UI_ProgressIndicator* CreateInstance(const string& message, CCNode* parent);
	static FKCW_UI_ProgressIndicator* GetInstance();

	// 若当前存在进度提示器单键，则隐藏，否则将不处理
	static void Hide();
};