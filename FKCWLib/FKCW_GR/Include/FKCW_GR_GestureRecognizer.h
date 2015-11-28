//*************************************************************************
//	创建日期:	2014-11-5
//	文件名称:	FKCW_GR_GestureRecognizer.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_GR_Macros.h"
//-------------------------------------------------------------------------
// 手势识别层
class FKCW_GR_GestureRecognizer : public CCLayer
{
protected:
	CCRect				m_tagFrame;				// 一个有效的范围盒
	bool				m_bIsRecognizing;		// 是否正在进行手势识别
	SEL_CallFuncO		m_pSelector;			// 消息接收处理函数
	CCObject*			m_pTarget;				// 消息接收对象
	CCTouchDispatcher*	m_pDispatcher;
	CC_SYNTHESIZE(bool, m_bIsCancelsTouchesInView, CancelsTouchesInView);	// 如果姿势正确被识别，是否取消触摸过的其他试图（缺省值false）
public:
	FKCW_GR_GestureRecognizer();
	virtual			~FKCW_GR_GestureRecognizer();
	// 设置手势识别处理对象和处理函数
	void			SetTarget( CCObject* tar, SEL_CallFuncO sel );

	virtual void	setParent(CCNode* parent);
	virtual void	registerWithTouchDispatcher(void);
protected:
	void			_GestureRecognized(CCObject * gesture);
	void			_StopTouchesPropagation(CCSet * pTouches, CCEvent * pEvent);
	bool			_IsPositionBetweenBounds(CCPoint pos);
	float			_DistanceBetweenPoints(CCPoint p1, CCPoint p2);
	CCSet*			_CreateSetWithTouch(CCTouch * pTouch);

	virtual bool	ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent)=0;
	virtual void	ccTouchMoved(CCTouch * pTouch, CCEvent * pEvent)=0;
	virtual void	ccTouchEnded(CCTouch * pTouch, CCEvent * pEvent)=0;
};