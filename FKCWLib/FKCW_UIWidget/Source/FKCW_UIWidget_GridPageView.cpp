//-------------------------------------------------------------------------
#include "../Include/FKCW_UIWidget_GridPageView.h"
//-------------------------------------------------------------------------
#define CGRIDPAGEVIEW_AUTO_RELOCATE_SPPED 900.0f
//-------------------------------------------------------------------------
// GridPageViewPage
//-------------------------------------------------------------------------
FKCW_UIWidget_GridPageViewPage::FKCW_UIWidget_GridPageViewPage()
{
	m_vGirdCells.reserve(10);
}
//-------------------------------------------------------------------------
FKCW_UIWidget_GridPageViewPage::~FKCW_UIWidget_GridPageViewPage()
{
	if( !m_vGirdCells.empty() )
	{
		vector<FKCW_UIWidget_GridPageViewCell*>::iterator itr = m_vGirdCells.begin();
		vector<FKCW_UIWidget_GridPageViewCell*>::iterator end = m_vGirdCells.end();

		for(; itr != end; ++itr )
		{
			(*itr)->release();
		}

		m_vGirdCells.clear();
	}
}
//-------------------------------------------------------------------------
vector<FKCW_UIWidget_GridPageViewCell*>& FKCW_UIWidget_GridPageViewPage::getGirdCells()
{
	return m_vGirdCells;
}
//-------------------------------------------------------------------------
// GridPageViewCell
//-------------------------------------------------------------------------
FKCW_UIWidget_GridPageViewCell::FKCW_UIWidget_GridPageViewCell()
{

}
//-------------------------------------------------------------------------
FKCW_UIWidget_GridPageViewCell::~FKCW_UIWidget_GridPageViewCell()
{

}
//-------------------------------------------------------------------------
FKCW_UIWidget_GridPageView::FKCW_UIWidget_GridPageView()
	: m_uGridCellsCount(0)
	, m_uColumns(0)
	, m_uRows(0)
	, m_tGridCellsSize(CCSizeZero)
	, m_uCellsMaxCountInPage(0)
{
	m_fAutoRelocateSpeed = CGRIDPAGEVIEW_AUTO_RELOCATE_SPPED;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_GridPageView::~FKCW_UIWidget_GridPageView()
{

}
//-------------------------------------------------------------------------
FKCW_UIWidget_GridPageView* FKCW_UIWidget_GridPageView::create(const CCSize& tViewSize)
{
	FKCW_UIWidget_GridPageView * pRet = new FKCW_UIWidget_GridPageView();
	if( pRet && pRet->initWithSize(tViewSize) )
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_GridPageView* FKCW_UIWidget_GridPageView::create(const CCSize& tViewSize, const CCSize& tCellSize, unsigned int uCellCount, 
									 CCObject* pListener, SEL_DataSoucreAdapterHandler pHandler)
{
	FKCW_UIWidget_GridPageView * pRet = new FKCW_UIWidget_GridPageView();
	if( pRet && pRet->initWithSize(tViewSize) )
	{
		pRet->setSizeOfCell(tCellSize);
		pRet->setCountOfCell(uCellCount);
		pRet->setDataSourceAdapter(pListener, pHandler);
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_GridPageView::setCountOfCell(unsigned int uCellsCount)
{
	m_uGridCellsCount = uCellsCount;
}
//-------------------------------------------------------------------------
unsigned int FKCW_UIWidget_GridPageView::getCountOfCell() const
{
	return m_uGridCellsCount;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_GridPageView::setSizeOfCell(const CCSize& tCellsSize)
{
	m_tGridCellsSize = tCellsSize;
	FKCW_UIWidget_TableView::setSizeOfCell(m_obContentSize);
}
//-------------------------------------------------------------------------
const CCSize& FKCW_UIWidget_GridPageView::getSizeOfCell() const
{
	return m_tGridCellsSize;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_GridPageView::setRows(unsigned int uRows)
{
	m_uRows = uRows;
}
//-------------------------------------------------------------------------
unsigned int FKCW_UIWidget_GridPageView::getRows() const
{
	return m_uRows;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_GridPageView::setColumns(unsigned int uColumns)
{
	m_uColumns = uColumns;
}
//-------------------------------------------------------------------------
unsigned int FKCW_UIWidget_GridPageView::getColumns() const
{
	return m_uColumns;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_GridPageView::updateGridCellsPosition()
{
	float x = 0.0f;
	float y = m_obContentSize.height - m_tGridCellsSize.height;

	for(unsigned int i = 0; i < m_uCellsMaxCountInPage; ++i)
	{
		if( i != 0 && i % m_uColumns == 0 )
		{
			x = 0.0f;
			y = y - m_tGridCellsSize.height;
		}
		m_vGridCellsPosition.push_back(CCPoint(x, y));
		x += m_tGridCellsSize.width;
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_GridPageView::updatePageCount()
{
	m_uCellsMaxCountInPage = m_uColumns * m_uRows;

	if( m_uGridCellsCount % m_uCellsMaxCountInPage == 0 )
	{
		m_uCellsCount = m_uGridCellsCount / m_uCellsMaxCountInPage;
	}
	else
	{
		m_uCellsCount = m_uGridCellsCount / m_uCellsMaxCountInPage + 1;
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_GridPageView::reloadData()
{
	CCAssert(m_uColumns != 0, "reloadData");
	CCAssert(m_uRows != 0, "reloadData");
	CCAssert(m_tGridCellsSize.width != 0, "reloadData");
	CCAssert(m_tGridCellsSize.height != 0, "reloadData");

	this->updatePageCount();
	this->updateGridCellsPosition();
	FKCW_UIWidget_TableView::reloadData();
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_GridPageView::onScrolling()
{
	if( m_uGridCellsCount == 0 )
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
void FKCW_UIWidget_GridPageView::updateCellAtIndex(unsigned int page)
{
	FKCW_UIWidget_GridPageViewPage* pPageCell = (FKCW_UIWidget_GridPageViewPage*) dequeueCell();

	if(!pPageCell)
	{
		pPageCell = new FKCW_UIWidget_GridPageViewPage();
		pPageCell->autorelease();

		vector<FKCW_UIWidget_GridPageViewCell*>& vGridCells = pPageCell->getGirdCells();
		unsigned int uBeginIdx = page * m_uCellsMaxCountInPage;
		unsigned int uEndIdx = uBeginIdx + m_uCellsMaxCountInPage;

		for( unsigned int idx = uBeginIdx, i = 0; idx < uEndIdx; ++idx, ++i )
		{
			FKCW_UIWidget_GridPageViewCell* pGridCell = NULL;
			if( idx < m_uGridCellsCount )
			{
				pGridCell = (FKCW_UIWidget_GridPageViewCell*) executeDataSourceAdapterHandler(NULL, idx);
				CCAssert(pGridCell != NULL, "cell can not be nil");
				pGridCell->setAnchorPoint(CCPointZero);
				pGridCell->setContentSize(m_tGridCellsSize);
				pGridCell->setPosition(m_vGridCellsPosition[i]);
				pGridCell->setIdx(idx);
				pPageCell->addChild(pGridCell);
				vGridCells.push_back(pGridCell);
				pGridCell->retain();
			}
			else
			{
				pGridCell = (FKCW_UIWidget_GridPageViewCell*) executeDataSourceAdapterHandler(NULL, CC_INVALID_INDEX);
				CCAssert(pGridCell != NULL, "cell can not be nil");
				pGridCell->setAnchorPoint(CCPointZero);
				pGridCell->setContentSize(m_tGridCellsSize);
				pGridCell->setPosition(m_vGridCellsPosition[i]);
				pGridCell->setIdx(CC_INVALID_INDEX);
				pGridCell->reset();
				pPageCell->addChild(pGridCell);
				vGridCells.push_back(pGridCell);
				pGridCell->retain();
			}
		}
	}
	else
	{
		vector<FKCW_UIWidget_GridPageViewCell*>& vGridCells = pPageCell->getGirdCells();
		unsigned int uBeginIdx = page * m_uCellsMaxCountInPage;
		unsigned int uEndIdx = uBeginIdx + m_uCellsMaxCountInPage;

		for( unsigned int idx = uBeginIdx, i = 0; idx < uEndIdx; ++idx, ++i )
		{
			FKCW_UIWidget_GridPageViewCell* pGridCell = vGridCells[i];
			if( idx < m_uGridCellsCount )
			{
				pGridCell->setIdx(idx);
				pGridCell = (FKCW_UIWidget_GridPageViewCell*) executeDataSourceAdapterHandler(pGridCell, idx);
				CCAssert(pGridCell != NULL, "cell can not be nil");
			}
			else
			{
				pGridCell->setIdx(CC_INVALID_INDEX);
				pGridCell->reset();
				pGridCell = (FKCW_UIWidget_GridPageViewCell*) executeDataSourceAdapterHandler(pGridCell, CC_INVALID_INDEX);
				CCAssert(pGridCell != NULL, "cell can not be nil");
			}
		}
	}

	pPageCell->setIdx(page);

	switch(m_eDirection)
	{
	case eScrollViewDirectionHorizontal:
		pPageCell->setAnchorPoint(CCPointZero);
		break;
	default:
		pPageCell->setAnchorPoint(CCPoint(0, 1));
		break;
	}

	pPageCell->setContentSize(m_tCellsSize);
	pPageCell->setPosition(cellPositionFromIndex(page));

	m_pContainer->addChild(pPageCell);
	insertSortableCell(pPageCell, page);
	pPageCell->retain();

	m_sIndices.insert(page);
}
//-------------------------------------------------------------------------