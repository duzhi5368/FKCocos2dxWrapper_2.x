//-------------------------------------------------------------------------
#include "../Include/FKCW_UIWidget_ExpandableListView.h"
//-------------------------------------------------------------------------
FKCW_UIWidget_ExpandableNode::FKCW_UIWidget_ExpandableNode()
	: m_bExpanded(false)
	, m_pExpandableListViewParent(NULL)
	, m_nIdx(CC_INVALID_INDEX)
{

}
//-------------------------------------------------------------------------
FKCW_UIWidget_ExpandableNode::~FKCW_UIWidget_ExpandableNode()
{
	removeAllItemNodes();
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ExpandableNode* FKCW_UIWidget_ExpandableNode::create()
{
	FKCW_UIWidget_ExpandableNode* pRet = new FKCW_UIWidget_ExpandableNode();
	pRet->init();
	pRet->setContentSize(CCSize(100, 100));
	pRet->setAnchorPoint(CCPoint(0.5f, 0.5f));
	pRet->autorelease();
	return pRet;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ExpandableNode::insertItemNodeAtLast(CCNode* pNode)
{
	CCAssert(pNode, "should not null");
	m_vExpandableNodeItemList.push_back(pNode);
	pNode->retain();
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ExpandableNode::insertItemNodeAtFront(CCNode* pNode)
{
	CCAssert(pNode, "should not null");
	m_vExpandableNodeItemList.insert(m_vExpandableNodeItemList.begin(), pNode);
	pNode->retain();
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ExpandableNode::removeItemNode(CCNode* pNode)
{
	CCAssert(pNode, "should not null");

	if( m_vExpandableNodeItemList.size() == 0 )
		return;

// 	vector<CCNode*>::iterator itr = std::find(
// 		m_vExpandableNodeItemList.begin(),
// 		m_vExpandableNodeItemList.end(), 
// 		pNode);

	vector<CCNode*>::iterator itr = m_vExpandableNodeItemList.end();
	for (vector<CCNode*>::iterator  itrTemp = m_vExpandableNodeItemList.begin();
		itrTemp != m_vExpandableNodeItemList.end();
		++itrTemp)
	{
		if (pNode == *itrTemp)
		{
			itr = itrTemp;
			break;
		}
		
	}
	

	if( itr != m_vExpandableNodeItemList.end() )
	{
		pNode->release();
		m_vExpandableNodeItemList.erase(itr);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ExpandableNode::removeItemNodeAtIndex(unsigned int idx)
{
	if( m_vExpandableNodeItemList.size() == 0 )
		return;

	m_vExpandableNodeItemList[idx]->release();
	m_vExpandableNodeItemList.erase(m_vExpandableNodeItemList.begin() + idx);
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ExpandableNode::removeAllItemNodes()
{
	if( m_vExpandableNodeItemList.size() == 0 )
		return;

	vector<CCNode*>::iterator itr = m_vExpandableNodeItemList.begin();
	vector<CCNode*>::iterator end = m_vExpandableNodeItemList.end();

	for(; itr != end ; ++itr )
	{
		(*itr)->release();
	}

	m_vExpandableNodeItemList.clear();
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ExpandableNode::setExpanded(bool bExpanded)
{
	m_bExpanded = bExpanded;
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_ExpandableNode::isExpanded() const
{
	return m_bExpanded;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ExpandableNode::setExpandableListViewParent(FKCW_UIWidget_ExpandableListView* pListView)
{
	m_pExpandableListViewParent = pListView;
}
//-------------------------------------------------------------------------
std::vector<CCNode*>& FKCW_UIWidget_ExpandableNode::getExpandableNodeItemList()
{
	return m_vExpandableNodeItemList;
}
//-------------------------------------------------------------------------
ENUM_WidgetTouchModel FKCW_UIWidget_ExpandableNode::onTouchBegan(CCTouch* pTouch)
{
	m_pSelectedWidget = NULL;
	m_eSelectedWidgetTouchModel = eWidgetTouchNone;

	CCPoint tNodePoint = convertToNodeSpace(pTouch->getLocation());
	if( m_pChildren && m_pChildren->count() > 0 )
	{
		CCObject* pObject = NULL;
		CCARRAY_FOREACH_REVERSE( m_pChildren, pObject )
		{
			CCNode* pNode = dynamic_cast<CCNode*>(pObject);
			FKCW_UIWidget_Widget* pWidget = dynamic_cast<FKCW_UIWidget_Widget*>(pObject);
			if( pWidget && pNode->isVisible() && pWidget->isEnabled() && pWidget->isTouchEnabled() )
			{
				if( pNode->boundingBox().containsPoint(tNodePoint) )
				{
					m_eSelectedWidgetTouchModel = pWidget->executeTouchBeganHandler(pTouch);
					if( m_eSelectedWidgetTouchModel == eWidgetTouchNone )
					{
						m_pSelectedWidget = NULL;
						m_eSelectedWidgetTouchModel = eWidgetTouchNone;
					}
					else
					{
						m_pSelectedWidget = pWidget;
						return m_eSelectedWidgetTouchModel;
					}
				}
			}
		}
	}
	return eWidgetTouchTransient;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ExpandableNode::onTouchMoved(CCTouch* pTouch, float fDuration)
{
	if( m_pSelectedWidget && !m_pSelectedWidget->isTouchInterrupted() )
	{
		m_pSelectedWidget->executeTouchMovedHandler(pTouch, fDuration);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ExpandableNode::onTouchEnded(CCTouch* pTouch, float fDuration)
{
	if( m_pSelectedWidget && !m_pSelectedWidget->isTouchInterrupted() )
	{
		m_pSelectedWidget->executeTouchEndedHandler(pTouch, fDuration);
	}
	else
	{
		CCAssert(m_pExpandableListViewParent, "parent should not null");

		CCPoint tPoint = m_pParent->convertToNodeSpace(pTouch->getLocation());
		if( boundingBox().containsPoint(tPoint) )
		{
			if( m_bExpanded )
			{
				m_bExpanded = false;
				m_pExpandableListViewParent->reloadData();
			}
			else
			{
				m_bExpanded = true;
				m_pExpandableListViewParent->reloadData();
			}
		}
	}
	m_pSelectedWidget = NULL;
	m_eSelectedWidgetTouchModel = eWidgetTouchNone;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ExpandableNode::onTouchCancelled(CCTouch* pTouch, float fDuration)
{
	if( m_pSelectedWidget && !m_pSelectedWidget->isTouchInterrupted() )
	{
		m_pSelectedWidget->executeTouchCancelledHandler(pTouch, fDuration);
	}
	m_pSelectedWidget = NULL;
	m_eSelectedWidgetTouchModel = eWidgetTouchNone;
}
//-------------------------------------------------------------------------
// ExpandableListView
//-------------------------------------------------------------------------
FKCW_UIWidget_ExpandableListView::FKCW_UIWidget_ExpandableListView()
{
	m_eDirection = eScrollViewDirectionVertical;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ExpandableListView::~FKCW_UIWidget_ExpandableListView()
{
	removeAllExpandableNodes();
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ExpandableListView* FKCW_UIWidget_ExpandableListView::create(const CCSize& contentSize)
{
	FKCW_UIWidget_ExpandableListView* pRet = new FKCW_UIWidget_ExpandableListView();
	pRet->initWithSize(contentSize);
	pRet->autorelease();
	return pRet;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ExpandableListView::expand(unsigned int idx)
{
	m_vExpandableNodeList[idx]->setExpanded(true);
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ExpandableListView::collapse(unsigned int idx)
{
	m_vExpandableNodeList[idx]->setExpanded(false);
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ExpandableListView::insertExpandableNodeAtLast(FKCW_UIWidget_ExpandableNode* pNode)
{
	CCAssert(pNode, "should not null");
	m_vExpandableNodeList.push_back(pNode);
	pNode->retain();
	pNode->setExpandableListViewParent(this);
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ExpandableListView::insertExpandableNodeAtFront(FKCW_UIWidget_ExpandableNode* pNode)
{
	CCAssert(pNode, "should not null");
	m_vExpandableNodeList.insert(m_vExpandableNodeList.begin(), pNode);
	pNode->retain();
	pNode->setExpandableListViewParent(this);
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ExpandableListView::removeExpandableNode(FKCW_UIWidget_ExpandableNode* pNode)
{
	if( m_vExpandableNodeList.size() == 0 )
		return;

// 	vector<FKCW_UIWidget_ExpandableNode*>::iterator itr = std::find(
// 		m_vExpandableNodeList.begin(),
// 		m_vExpandableNodeList.end(),
// 		pNode);

	vector<FKCW_UIWidget_ExpandableNode*>::iterator itr = m_vExpandableNodeList.end();
	for (vector<FKCW_UIWidget_ExpandableNode*>::iterator  itrTemp = m_vExpandableNodeList.begin();
		itrTemp != m_vExpandableNodeList.end();
		++itrTemp)
	{
		if (pNode == *itrTemp)
		{
			itr = itrTemp;
			break;
		}

	}

	if( itr != m_vExpandableNodeList.end() )
	{
		pNode->release();
		m_vExpandableNodeList.erase(itr);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ExpandableListView::removeExpandableNodeAtIndex(unsigned int idx)
{
	if( m_vExpandableNodeList.size() == 0 )
		return;

	m_vExpandableNodeList[idx]->release();
	m_vExpandableNodeList.erase(m_vExpandableNodeList.begin() + idx);
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ExpandableListView::removeLastExpandableNode()
{
	if( m_vExpandableNodeList.size() == 0 )
		return;

	m_vExpandableNodeList[m_vExpandableNodeList.size() - 1]->release();
	m_vExpandableNodeList.pop_back();
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ExpandableListView::removeFrontExpandableNode()
{
	if( m_vExpandableNodeList.size() == 0 )
		return;

	m_vExpandableNodeList[0]->release();
	m_vExpandableNodeList.erase(m_vExpandableNodeList.begin());
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ExpandableListView::removeAllExpandableNodes()
{
	if( m_vExpandableNodeList.size() == 0 )
		return;

	unsigned int i = 0;
	unsigned int end = m_vExpandableNodeList.size();

	for(; i < end; ++i )
	{
		m_vExpandableNodeList[i]->release();
	}

	m_vExpandableNodeList.clear();
}
//-------------------------------------------------------------------------
CCArray* FKCW_UIWidget_ExpandableListView::getExpandableNodes()
{
	CCArray* pArray = new CCArray();
	pArray->initWithCapacity(10);

	if( !m_vExpandableNodeList.empty() )
	{
		vector<FKCW_UIWidget_ExpandableNode*>::iterator iter = m_vExpandableNodeList.begin();
		vector<FKCW_UIWidget_ExpandableNode*>::iterator iend = m_vExpandableNodeList.end();

		for(; iter != iend; ++iter )
		{
			pArray->addObject(*iter);
		}
	}

	pArray->autorelease();
	return pArray;
}
//-------------------------------------------------------------------------
unsigned int FKCW_UIWidget_ExpandableListView::getExpandableNodeCount()
{
	return m_vExpandableNodeList.size();
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ExpandableNode* FKCW_UIWidget_ExpandableListView::getExpandableNodeAtIndex(unsigned int idx)
{
	return m_vExpandableNodeList[idx];
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ExpandableListView::updateNodesPosition()
{
	m_pContainer->removeAllChildrenWithCleanup(true);

	if( m_vExpandableNodeList.size() == 0 )
		return;

	float fAllNodesHeight = 0.0f;

	unsigned int uBegin = 0;
	unsigned int uEnd = m_vExpandableNodeList.size();

	for(; uBegin < uEnd; ++uBegin )
	{
		FKCW_UIWidget_ExpandableNode* pExpandableNode = m_vExpandableNodeList[uBegin];
		fAllNodesHeight += pExpandableNode->getContentSize().height;

		if( pExpandableNode->isExpanded() )
		{
			vector<CCNode*>&  vNodeItemList = pExpandableNode->getExpandableNodeItemList();
			if( vNodeItemList.size() != 0 )
			{
				for( unsigned int i = 0; i < vNodeItemList.size(); ++i )
				{
					fAllNodesHeight += (vNodeItemList[i])->getContentSize().height;
				}
			}
		}
	}

	fAllNodesHeight = MAX(m_obContentSize.height, fAllNodesHeight);
	setContainerSize(CCSize(m_obContentSize.width, fAllNodesHeight));

	uBegin = 0;
	uEnd = m_vExpandableNodeList.size();

	for(; uBegin < uEnd; ++uBegin )
	{
		FKCW_UIWidget_ExpandableNode* pExpandableNode = m_vExpandableNodeList[uBegin];
		fAllNodesHeight = fAllNodesHeight - pExpandableNode->getContentSize().height;

		pExpandableNode->setAnchorPoint(CCPointZero);
		pExpandableNode->setPosition(CCPoint(0, fAllNodesHeight));
		m_pContainer->addChild(pExpandableNode);

		if( pExpandableNode->isExpanded() )
		{
			vector<CCNode*>& vNodeItemList = pExpandableNode->getExpandableNodeItemList();
			if( vNodeItemList.size() != 0 )
			{
				for( unsigned int i = 0; i < vNodeItemList.size(); ++i )
				{
					CCNode* pNode = vNodeItemList[i];
					fAllNodesHeight = fAllNodesHeight - pNode->getContentSize().height;

					pNode->setAnchorPoint(CCPointZero);
					pNode->setPosition(CCPoint(0, fAllNodesHeight));
					m_pContainer->addChild(pNode);
				}
			}
		}
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ExpandableListView::reloadData()
{
	CCAssert(m_eDirection == eScrollViewDirectionVertical, "should be Vertical");

	float fOldHeight = getContainerSize().height;

	updateNodesPosition();

	float tNewHeight = getContainerSize().height - fOldHeight;

	setContentOffset(getContentOffset() - CCPoint(0, tNewHeight));

	relocateContainer();
}
//-------------------------------------------------------------------------