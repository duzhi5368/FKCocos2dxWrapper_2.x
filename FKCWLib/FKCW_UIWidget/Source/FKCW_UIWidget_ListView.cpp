//-------------------------------------------------------------------------
#include "../Include/FKCW_UIWidget_ListView.h"
//-------------------------------------------------------------------------
static const CCPoint ListViewHorizontalNodeAnchorPoint = CCPoint(0, 0);
static const CCPoint ListViewVerticalNodeAnchorPoint = CCPoint(0, 0);
//-------------------------------------------------------------------------
FKCW_UIWidget_ListView::FKCW_UIWidget_ListView()
	: m_fLayoutIndexSize(0.0f)
{
	m_eDirection = eScrollViewDirectionVertical;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ListView::~FKCW_UIWidget_ListView()
{
	removeAllNodes();
}
//-------------------------------------------------------------------------
FKCW_UIWidget_ListView* FKCW_UIWidget_ListView::create(const CCSize& contentSize)
{
	FKCW_UIWidget_ListView* pRet = new FKCW_UIWidget_ListView();
	if( pRet && pRet->initWithSize(contentSize) )
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
unsigned int FKCW_UIWidget_ListView::getNodeCount() const
{
	return m_vNodeList.size();
}
//-------------------------------------------------------------------------
CCArray* FKCW_UIWidget_ListView::getNodes()
{
	CCArray* pArray = new CCArray();
	pArray->initWithCapacity(10);

	if( !m_vNodeList.empty() )
	{
		vector<CCNode*>::iterator iter = m_vNodeList.begin();
		vector<CCNode*>::iterator iend = m_vNodeList.end();

		for(; iter != iend; ++iter )
		{
			pArray->addObject(*iter);
		}
	}

	pArray->autorelease();
	return pArray;
}
//-------------------------------------------------------------------------
CCNode* FKCW_UIWidget_ListView::getNodeAtIndex(unsigned int idx)
{
	return m_vNodeList[idx];
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ListView::insertNodeAtLast(CCNode* pNode)
{
	CCAssert(pNode, "should not null");
	m_vNodeList.push_back(pNode);
	pNode->retain();
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ListView::insertNodeAtFront(CCNode* pNode)
{
	CCAssert(pNode, "should not null");
	m_vNodeList.insert(m_vNodeList.begin(), pNode);
	pNode->retain();
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ListView::insertNode(CCNode* pNode, CCNode* pTarget)
{
	CCAssert(pTarget && pNode, "should not null");

	unsigned int idx = 0;
	unsigned int end = m_vNodeList.size();

	for(; idx < end; ++idx )
	{
		if( pTarget == m_vNodeList[idx] )
		{
			m_vNodeList.insert(m_vNodeList.begin() + idx, pNode);
			pNode->retain();
		}
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ListView::insertNode(CCNode* pNode, unsigned int idx)
{
	CCAssert(pNode, "should not null");

	if( idx >= m_vNodeList.size() )
	{
		insertNodeAtLast(pNode);
		return;
	}

	m_vNodeList.insert(m_vNodeList.begin() + idx, pNode);
	pNode->retain();
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ListView::removeNodeAtIndex(unsigned int idx)
{
	if( m_vNodeList.size() == 0 )
		return;

	m_vNodeList[idx]->release();
	m_vNodeList.erase(m_vNodeList.begin() + idx);
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ListView::removeNode(CCNode* pNode)
{
	CCAssert(pNode, "should not null");

	if( m_vNodeList.size() == 0 )
		return;

// 	vector<CCNode*>::iterator itr = std::find(
// 		m_vNodeList.begin(),
// 		m_vNodeList.end(),
// 		pNode);

	vector<CCNode*>::iterator itr = m_vNodeList.end();
	for (vector<CCNode*>::iterator  itrTemp = m_vNodeList.begin();
		itrTemp != m_vNodeList.end();
		++itrTemp)
	{
		if (pNode == *itrTemp)
		{
			itr = itrTemp;
			break;
		}

	}

	if( itr != m_vNodeList.end() )
	{
		pNode->release();
		m_vNodeList.erase(itr);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ListView::removeFrontNode()
{
	if( m_vNodeList.size() == 0 )
		return;

	m_vNodeList[0]->release();
	m_vNodeList.erase(m_vNodeList.begin());
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ListView::removeLastNode()
{
	if( m_vNodeList.size() == 0 )
		return;

	m_vNodeList[m_vNodeList.size() - 1]->release();
	m_vNodeList.pop_back();
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ListView::removeAllNodes()
{
	if( m_vNodeList.size() == 0 )
		return;

	unsigned int i = 0;
	unsigned int end = m_vNodeList.size();

	for(; i < end; ++i )
	{
		m_vNodeList[i]->release();
	}

	m_vNodeList.clear();
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ListView::updateNodesPosition()
{
	m_pContainer->removeAllChildrenWithCleanup(true);

	if( m_vNodeList.size() == 0 )
		return;

	m_fLayoutIndexSize = 0.0f;

	switch( m_eDirection )
	{
	case eScrollViewDirectionHorizontal:
		{
			unsigned int i = 0;
			unsigned int end = m_vNodeList.size();
			CCNode* pNode = NULL;

			for(; i < end; ++i )
			{
				pNode = m_vNodeList[i];
				pNode->setAnchorPoint(ListViewHorizontalNodeAnchorPoint);
				pNode->setPosition(CCPoint(m_fLayoutIndexSize, 0));
				m_fLayoutIndexSize += pNode->getContentSize().width;
			}

			setContainerSize(CCSize(m_fLayoutIndexSize, m_obContentSize.height));
		}
		break;
	case eScrollViewDirectionVertical:
		{
			float fAllNodesSize = 0.0f;

			unsigned int i = 0;
			unsigned int end = m_vNodeList.size();
			CCNode* pNode = NULL;

			for(; i < end; ++i )
			{
				pNode = m_vNodeList[i];
				fAllNodesSize += pNode->getContentSize().height;
			}

			m_fLayoutIndexSize = fAllNodesSize;
			fAllNodesSize = MAX(m_obContentSize.height, fAllNodesSize);
			float ELSP_WIDTH = 15.0f;
			setContainerSize(CCSize(m_obContentSize.width, fAllNodesSize + ELSP_WIDTH));

			i = 0;
			end = m_vNodeList.size();

			for(; i < end; ++i )
			{
				pNode = m_vNodeList[i];
				fAllNodesSize = fAllNodesSize - pNode->getContentSize().height;
				pNode->setAnchorPoint(ListViewVerticalNodeAnchorPoint);
				pNode->setPosition(CCPoint(0, fAllNodesSize + ELSP_WIDTH/2));

				m_pContainer->addChild(pNode);
			}
		}
		break;
	default:
		break;
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_ListView::reloadData()
{
	CCAssert( m_eDirection != eScrollViewDirectionBoth, "reloadData");

	if( m_eDirection == eScrollViewDirectionVertical )
	{
		float fOldHeight = getContainerSize().height;

		updateNodesPosition();

		float tNewHeight = getContainerSize().height - fOldHeight;

		setContentOffset(getContentOffset() - CCPoint(0, tNewHeight));
	}
	else
	{
		updateNodesPosition();
	}

	relocateContainer();
}
//-------------------------------------------------------------------------