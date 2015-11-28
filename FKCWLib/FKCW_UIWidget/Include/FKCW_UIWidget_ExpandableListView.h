//*************************************************************************
//	创建日期:	2014-12-5
//	文件名称:	FKCW_UIWidget_ExpandableListView.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "cocos2d.h"
#include "FKCW_UIWidget_Widget.h"
#include "FKCW_UIWidget_Protocol.h"
#include "FKCW_UIWidget_ScrollView.h"
#include <vector>
using std::vector;
//-------------------------------------------------------------------------
USING_NS_CC;
//-------------------------------------------------------------------------
class FKCW_UIWidget_ExpandableNode;
class FKCW_UIWidget_ExpandableListView;
//-------------------------------------------------------------------------
class FKCW_UIWidget_ExpandableNode: public FKCW_UIWidget_Layout
{
public:
	FKCW_UIWidget_ExpandableNode();
	virtual ~FKCW_UIWidget_ExpandableNode();
	static FKCW_UIWidget_ExpandableNode* create();

	void insertItemNodeAtLast(CCNode* pNode);
	void insertItemNodeAtFront(CCNode* pNode);
	void removeItemNode(CCNode* pNode);
	void removeItemNodeAtIndex(unsigned int idx);
	void removeAllItemNodes();

public:
	virtual ENUM_WidgetTouchModel onTouchBegan(CCTouch* pTouch);
	virtual void onTouchMoved(CCTouch* pTouch, float fDuration);
	virtual void onTouchEnded(CCTouch* pTouch, float fDuration);
	virtual void onTouchCancelled(CCTouch* pTouch, float fDuration);

protected:
	friend class FKCW_UIWidget_ExpandableListView;
	void setExpanded(bool bExpanded);
	bool isExpanded() const;
	std::vector<CCNode*>& getExpandableNodeItemList();
	void setExpandableListViewParent(FKCW_UIWidget_ExpandableListView* pListView);

protected:
	bool m_bExpanded;
	unsigned int m_nIdx;
	std::vector<CCNode*> m_vExpandableNodeItemList;
	FKCW_UIWidget_ExpandableListView* m_pExpandableListViewParent;
};
//-------------------------------------------------------------------------
class FKCW_UIWidget_ExpandableListView : public FKCW_UIWidget_ScrollView
{
public:
	FKCW_UIWidget_ExpandableListView();
	virtual ~FKCW_UIWidget_ExpandableListView();
	static FKCW_UIWidget_ExpandableListView* create(const CCSize& contentSize);

	void expand(unsigned int idx);
	void collapse(unsigned int idx);

	void insertExpandableNodeAtLast(FKCW_UIWidget_ExpandableNode* pNode);
	void insertExpandableNodeAtFront(FKCW_UIWidget_ExpandableNode* pNode);
	void removeExpandableNode(FKCW_UIWidget_ExpandableNode* pNode);
	void removeExpandableNodeAtIndex(unsigned int idx);
	void removeLastExpandableNode();
	void removeFrontExpandableNode();
	void removeAllExpandableNodes();
	CCArray* getExpandableNodes();
	unsigned int getExpandableNodeCount();
	FKCW_UIWidget_ExpandableNode* getExpandableNodeAtIndex(unsigned int idx);

	void reloadData();

protected:
	friend class FKCW_UIWidget_ExpandableNode;
	void updateNodesPosition();

protected:
	std::vector<FKCW_UIWidget_ExpandableNode*> m_vExpandableNodeList;
};
