//*************************************************************************
//	创建日期:	2014-12-4
//	文件名称:	FKCW_UIWidget_ScrollView.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "cocos2d.h"
#include "FKCW_UIWidget_Widget.h"
#include "FKCW_UIWidget_Protocol.h"
#include "FKCW_UIWidget_Layout.h"
//-------------------------------------------------------------------------
class FKCW_UIWidget_ScrollView;
class FKCW_UIWidget_ScrollViewContainer;
//-------------------------------------------------------------------------
// 滚动轴方向
enum ENUM_ScrollViewDirection
{
	eScrollViewDirectionHorizontal,
	eScrollViewDirectionVertical,
	eScrollViewDirectionBoth
};
//-------------------------------------------------------------------------
// 滚动条底容器
class FKCW_UIWidget_ScrollViewContainer : public FKCW_UIWidget_Layout
{
public:
	virtual void reset();
	friend class FKCW_UIWidget_ScrollView;
};
//-------------------------------------------------------------------------
// 滚动条
class FKCW_UIWidget_ScrollView : public FKCW_UIWidget_Layout, public FKCW_UIWidget_ScrollableProtocol
{
public:
	FKCW_UIWidget_ScrollView();
	virtual ~FKCW_UIWidget_ScrollView();

	virtual bool init();
	static FKCW_UIWidget_ScrollView* create(const CCSize& contentSize);

	virtual bool initWithSize(const CCSize& tSize);
	void setContainerSize(CCSize tSize);
	const CCSize& getContainerSize() const;
	ENUM_ScrollViewDirection getDirection();
	void setDirection(ENUM_ScrollViewDirection eDirection);
	FKCW_UIWidget_ScrollViewContainer* getContainer();
	virtual void setContentSize(const CCSize& contentSize);
	
	const CCPoint& getContentOffset() const;
	void setBounceable(bool bBounceable);
	bool isBounceable();
	void setDeaccelerateable(bool bDeaccelerateable);
	bool isDeaccelerateable();
	void setDragable(bool bDragable);
	bool isDragable();
    bool isTouchMoved();
    
	void stopContainerAnimation();
	const CCPoint& getMaxOffset() const;
	const CCPoint& getMinOffset() const;
	void setContentOffsetToTop();
	void setContentOffsetToTopInDuration(float fDuration);
	void setContentOffsetToTopEaseIn(float fDuration, float fRate);
	void setContentOffsetToBottom();
	void setContentOffsetToRight();
	void setContentOffsetToLeft();
	void relocateContainer();
	void setContentOffset(CCPoint tOffset);
	void setContentOffsetInDuration(CCPoint tOffset, float fDuration);
	void setContentOffsetEaseIn(CCPoint tOffset, float fDuration, float fRate);

public:
	virtual void visit();
    virtual void onExit();
	virtual ENUM_WidgetTouchModel onTouchBegan(CCTouch *pTouch);
    virtual void onTouchMoved(CCTouch *pTouch, float fDuration);
    virtual void onTouchEnded(CCTouch *pTouch, float fDuration);
    virtual void onTouchCancelled(CCTouch *pTouch, float fDuration);

protected:
	void relocateContainerWithoutCheck(const CCPoint& tOffset);
	void setContentOffsetWithoutCheck(const CCPoint& tOffset);
	void setContentOffsetInDurationWithoutCheck(const CCPoint& tOffset, float fDuration);
	void setContentOffsetEaseInWithoutCheck(const CCPoint& tOffset, float fDuration, float fRate);

	bool m_bDeaccelerateScrolling;
	void perpareDeaccelerateScroll();
	void performedDeaccelerateScrolling(float dt);
	void stoppedDeaccelerateScroll();

	bool m_bAnimatedScrolling;
	void perpareAnimatedScroll();
    void performedAnimatedScrolling(float dt);
    void stoppedAnimatedScroll();

    virtual void onScrolling(){};
	virtual void onDeaccelerateScrollEnded(){};
	virtual void onDraggingScrollEnded(){};
	void updateLimitOffset();
	bool validateOffset(CCPoint& tPoint);

protected:
	FKCW_UIWidget_ScrollViewContainer *m_pContainer;
	ENUM_ScrollViewDirection m_eDirection;
	CCPoint m_tLastMovePoint;
	bool m_bDeaccelerateable;
	bool m_bBounceable;
	bool m_bDragging;
	bool m_bDragable;
	float m_fDragSpeed;
	CCPoint m_tScrollDistance;
	CCPoint m_tTouchBeganPoint;
    bool m_bTouchMoved;
	CCPoint m_tMinOffset;
	CCPoint m_tMaxOffset;
};