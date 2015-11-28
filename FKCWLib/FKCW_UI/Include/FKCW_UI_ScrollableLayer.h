//*************************************************************************
//	创建日期:	2014-11-14
//	文件名称:	FKCW_UI_ScrollableLayer.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_UI_Macros.h"
//-------------------------------------------------------------------------
enum ENUM_ScrollableLayerBindingType
{
	eScrollableLayerBindingType_BottomLeft,
	eScrollableLayerBindingType_BottomRight,
	eScrollableLayerBindingType_UpperLeft,
	eScrollableLayerBindingType_UpperRight
};
//-------------------------------------------------------------------------
class FKCW_UI_ScrollableLayer : public CCLayer
{
public:
	FKCW_UI_ScrollableLayer();
	static FKCW_UI_ScrollableLayer* create(const CCSize& tWindowSize, const CCSize& tLayerSize, ENUM_ScrollableLayerBindingType eBinding = eScrollableLayerBindingType_BottomLeft);
	bool init(const CCSize& tWindowSize, const CCSize& tLayerSize, ENUM_ScrollableLayerBindingType eBinding = eScrollableLayerBindingType_BottomLeft);

	bool isNodeVisibleRecursive(CCNode* pNode);
	void setShift(const CCSize& tShift);
	void setLayerSize(const CCSize& tSize);

	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchCancelled(CCTouch* pTouch, CCEvent* pEvent);

private:
	ENUM_ScrollableLayerBindingType	m_eBinding;
	CCSize							m_tMinShift;
	CCSize							m_tMaxShift;
	bool							m_bScrolling;
	CCPoint							m_tBeginTouchPoint;
};
//-------------------------------------------------------------------------