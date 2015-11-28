//*************************************************************************
//	创建日期:	2014-12-5
//	文件名称:	FKCW_UIWidget_ListView.h
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
class FKCW_UIWidget_ListView : public FKCW_UIWidget_ScrollView
{
public:
	FKCW_UIWidget_ListView();
	virtual ~FKCW_UIWidget_ListView();
	static FKCW_UIWidget_ListView* create(const CCSize& contentSize);

	void insertNodeAtLast(CCNode* pNode);
	void insertNodeAtFront(CCNode* pNode);
	void insertNode(CCNode* pNode, CCNode* pTarget);
	void insertNode(CCNode* pNode, unsigned int idx);

	void removeNodeAtIndex(unsigned int idx);
	void removeNode(CCNode* pNode);
	void removeFrontNode();
	void removeLastNode();
	void removeAllNodes();

	CCArray* getNodes();
	CCNode* getNodeAtIndex(unsigned int idx);
	unsigned int getNodeCount() const;
	void reloadData();

protected:
	void updateNodesPosition();

protected:
	vector<CCNode*> m_vNodeList;
	float m_fLayoutIndexSize;
};