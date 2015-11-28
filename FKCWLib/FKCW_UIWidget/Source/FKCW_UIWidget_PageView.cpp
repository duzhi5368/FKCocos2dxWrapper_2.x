//-------------------------------------------------------------------------
#include "../Include/FKCW_UIWidget_PageView.h"
//-------------------------------------------------------------------------
FKCW_UIWidget_PageView::FKCW_UIWidget_PageView()
{
	setAutoRelocate(true);
	setAutoRelocateSpeed(900.0f);
}
//-------------------------------------------------------------------------
FKCW_UIWidget_PageView* FKCW_UIWidget_PageView::create(const CCSize& tPageSize)
{
	FKCW_UIWidget_PageView * pRet = new FKCW_UIWidget_PageView();
	if( pRet && pRet->initWithSize(tPageSize) )
	{
		pRet->setSizeOfCell(tPageSize);
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_PageView* FKCW_UIWidget_PageView::create(const CCSize& tPageSize, unsigned int uPageCount, CCObject* pListener, SEL_DataSoucreAdapterHandler pHandler)
{
	FKCW_UIWidget_PageView* pRet = new FKCW_UIWidget_PageView();
	if( pRet && pRet->initWithSize(tPageSize) )
	{
		pRet->setSizeOfCell(tPageSize);
		pRet->setCountOfCell(uPageCount);
		pRet->setDataSourceAdapter(pListener, pHandler);
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_PageView::onScrolling()
{
	if( m_uCellsCount == 0 )
		return;

	FKCW_UIWidget_TableView::onScrolling();

	CCPoint tPageIdxOffset;
	switch( m_eDirection )
	{
	case eScrollViewDirectionHorizontal:
		tPageIdxOffset = getContentOffset() - CCPoint(m_obContentSize.width / 2, 0);
		break;
	case eScrollViewDirectionVertical:
		tPageIdxOffset = getContentOffset() + CCPoint(0, m_obContentSize.height / 2);
		break;
	default:
		break;
	}

	unsigned int nPage = cellBeginIndexFromOffset(tPageIdxOffset);
	if( nPage != m_nPageIndex )
	{
		m_nPageIndex = nPage;
		executePageChangedHandler(this, m_nPageIndex);
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_PageView::updateCellAtIndex(unsigned int idx)
{
	FKCW_UIWidget_PageViewCell* pCell = (FKCW_UIWidget_PageViewCell*)(executeDataSourceAdapterHandler(dequeueCell(), idx));
	CCAssert(pCell != NULL, "cell can not be nil");

	pCell->setIdx(idx);

	switch(m_eDirection)
	{
	case eScrollViewDirectionHorizontal:
		pCell->setAnchorPoint(CCPointZero);
		break;
	default:
		pCell->setAnchorPoint(CCPoint(0, 1));
		break;
	}

	pCell->setContentSize(m_tCellsSize);
	pCell->setPosition(cellPositionFromIndex(idx));

	m_pContainer->addChild(pCell);
	insertSortableCell(pCell, idx);
	pCell->retain();

	m_sIndices.insert(idx);
}
//-------------------------------------------------------------------------