//-------------------------------------------------------------------------
#include "../Include/FKCW_UIWidget_GridView.h"
//-------------------------------------------------------------------------
#define CGRIDVIEW_AUTO_RELOCATE_SPPED 350.0f
//-------------------------------------------------------------------------
FKCW_UIWidget_GridViewCell::FKCW_UIWidget_GridViewCell()
	: m_uRow(CC_INVALID_INDEX)
	, m_uIdx(CC_INVALID_INDEX)
{

}
//-------------------------------------------------------------------------
FKCW_UIWidget_GridViewCell::~FKCW_UIWidget_GridViewCell()
{

}
//-------------------------------------------------------------------------
FKCW_UIWidget_GridView::FKCW_UIWidget_GridView()
	: m_tCellsSize(CCSizeZero)
	, m_uCellsCount(0)
	, m_uColumns(0)
	, m_uRows(0)
	, m_bAutoRelocate(false)
{
	m_eDirection = eScrollViewDirectionVertical;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_GridView::~FKCW_UIWidget_GridView()
{
	removeAllFromUsed();
	removeAllFromFreed();
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_GridView::setCountOfCell(unsigned int uCellsCount)
{
	m_uCellsCount = uCellsCount;
}
//-------------------------------------------------------------------------
unsigned int FKCW_UIWidget_GridView::getCountOfCell() const
{
	return m_uCellsCount;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_GridView::setSizeOfCell(const CCSize& tCellsSize)
{
	m_tCellsSize = tCellsSize;
}
//-------------------------------------------------------------------------
const CCSize& FKCW_UIWidget_GridView::getSizeOfCell() const
{
	return m_tCellsSize;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_GridView::setColumns(unsigned int uColumns)
{
	m_uColumns = uColumns;
}
//-------------------------------------------------------------------------
unsigned int FKCW_UIWidget_GridView::getColumns() const
{
	return m_uColumns;
}
//-------------------------------------------------------------------------
unsigned int FKCW_UIWidget_GridView::getRows() const
{
	return m_uRows;
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_GridView::isAutoRelocate() const
{
	return m_bAutoRelocate;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_GridView::setAutoRelocate(bool bAuto)
{
	m_bAutoRelocate = bAuto;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_GridView* FKCW_UIWidget_GridView::create(const CCSize& tViewSize)
{
	FKCW_UIWidget_GridView * pRet = new FKCW_UIWidget_GridView();
	if( pRet && pRet->initWithSize(tViewSize) )
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_GridView* FKCW_UIWidget_GridView::create(const CCSize& tViewSize, const CCSize& tCellSize, unsigned int uCellCount,
							 CCObject* pListener, SEL_DataSoucreAdapterHandler pHandler)
{
	FKCW_UIWidget_GridView * pRet = new FKCW_UIWidget_GridView();
	if( pRet && pRet->initWithParams(tViewSize, tCellSize, uCellCount, pListener, pHandler))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
bool FKCW_UIWidget_GridView::initWithParams(const CCSize& tViewSize, const CCSize& tCellSize, unsigned int uCellCount,
							   CCObject* pListener, SEL_DataSoucreAdapterHandler pHandler)
{
	if( initWithSize(tViewSize) )
	{
		setSizeOfCell(tCellSize);
		setCountOfCell(uCellCount);
		setDataSourceAdapter(pListener, pHandler);
		return true;
	}
	return false;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_GridView::removeAllFromUsed()
{
	if( !m_lCellsUsed.empty() )
	{
		list<FKCW_UIWidget_GridViewCell*>::iterator iter = m_lCellsUsed.begin();
		for(; iter != m_lCellsUsed.end(); ++iter)
		{
			m_pContainer->removeChild((*iter));
			(*iter)->release();
		}
		m_lCellsUsed.clear();
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_GridView::removeAllFromFreed()
{
	if( !m_lCellsFreed.empty() )
	{
		list<FKCW_UIWidget_GridViewCell*>::iterator iter = m_lCellsFreed.begin();
		for(; iter != m_lCellsFreed.end(); ++iter)
		{
			(*iter)->release();
		}
		m_lCellsFreed.clear();
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_GridView::insertSortableCell(FKCW_UIWidget_GridViewCell* pCell, unsigned int idx)
{
	if( m_lCellsUsed.empty() )
	{
		m_lCellsUsed.push_back(pCell);
	}
	else
	{
		list<FKCW_UIWidget_GridViewCell*>::iterator iter = m_lCellsUsed.begin();
		for(; iter != m_lCellsUsed.end(); ++iter)
		{
			if( (*iter)->getIdx() > idx )
			{
				m_lCellsUsed.insert(iter, pCell);
				return;
			}
		}
		m_lCellsUsed.push_back(pCell);
		return;
	}
}
//-------------------------------------------------------------------------
const CCPoint& FKCW_UIWidget_GridView::cellPositionFromIndex(unsigned int idx)
{
	if( idx == CC_INVALID_INDEX )
	{
		return m_vPositions[0];
	}

	return m_vPositions[idx];
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_GridView::updateCellAtIndex(unsigned int idx, unsigned int row)
{
	if( m_uCellsCount == 0 )
		return;

	FKCW_UIWidget_GridViewCell* pCell = (FKCW_UIWidget_GridViewCell*) executeDataSourceAdapterHandler(dequeueCell(), idx);
#if 1
	CCAssert(pCell != NULL, "cell can not be nil");
#endif

	pCell->setIdx(idx);
	pCell->setRow(row);
	pCell->setAnchorPoint(CCPoint(0, 1));
	pCell->setContentSize(m_tCellsSize);
	pCell->setPosition(cellPositionFromIndex(idx));

	m_pContainer->addChild(pCell);
	insertSortableCell(pCell, idx);
	pCell->retain();

	m_sIndices.insert(idx);
}
//-------------------------------------------------------------------------
unsigned int FKCW_UIWidget_GridView::cellBeginRowFromOffset(const CCPoint& offset)
{
	float ofy = offset.y + m_pContainer->getContentSize().height;
	float xos = ofy - m_obContentSize.height;
	int row = (int)(xos / m_tCellsSize.height);

	row = MAX(row, 0);
	row = MIN((int)m_uRows - 1, row);

	return (unsigned int)row;
}
//-------------------------------------------------------------------------
unsigned int FKCW_UIWidget_GridView::cellEndRowFromOffset(const CCPoint& offset)
{
	float ofy = offset.y + m_pContainer->getContentSize().height;
	int row = (int)(ofy / m_tCellsSize.height);

	row = MAX(row, 0);
	row = MIN((int)m_uRows - 1, row);

	return (unsigned int)row;
}
//-------------------------------------------------------------------------
unsigned int FKCW_UIWidget_GridView::cellFirstIndexFromRow(unsigned int row)
{
	return m_uColumns * row;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_GridView::updatePositions()
{
	if( m_uCellsCount == 0 )
		return;

	m_uRows = m_uCellsCount % m_uColumns == 0 ? m_uCellsCount / m_uColumns : m_uCellsCount / m_uColumns + 1;
	float fHeight = MAX(m_uRows * m_tCellsSize.height, m_obContentSize.height);
	float fWidth = m_uColumns * m_tCellsSize.width;
	setContainerSize(CCSizeMake(fWidth, fHeight));

	float nX = 0.0f;
	float nY = fHeight;

	for(unsigned int idx = 0; idx < m_uCellsCount; ++idx)
	{
		if( idx != 0 && idx % m_uColumns == 0 )
		{
			nX = 0.0f;
			nY = nY - m_tCellsSize.height;
		}
		m_vPositions.push_back(CCPoint(nX, nY));
		nX += m_tCellsSize.width;
	}
}
//-------------------------------------------------------------------------
CCArray* FKCW_UIWidget_GridView::getCells()
{
	CCArray* pArray = new CCArray();
	pArray->initWithCapacity(10);

	if( !m_lCellsUsed.empty() )
	{
		list<FKCW_UIWidget_GridViewCell*>::iterator iter = m_lCellsUsed.begin();
		for(; iter != m_lCellsUsed.end(); ++iter)
		{
			pArray->addObject(*iter);
		}
	}

	pArray->autorelease();
	return pArray;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_GridViewCell* FKCW_UIWidget_GridView::cellAtIndex(unsigned int idx)
{
	if( m_sIndices.find(idx) == m_sIndices.end() )
	{
		return NULL;
	}

	list<FKCW_UIWidget_GridViewCell*>::iterator iter = m_lCellsUsed.begin();
	for(; iter != m_lCellsUsed.end() ; ++iter)
	{
		if( (*iter)->getIdx() == idx )
		{
			return (*iter);
		}
	}
	return NULL;
}
//-------------------------------------------------------------------------
FKCW_UIWidget_GridViewCell* FKCW_UIWidget_GridView::dequeueCell()
{
	FKCW_UIWidget_GridViewCell* pCell = NULL;
	if( m_lCellsFreed.empty() )
	{
		return NULL;
	}
	else
	{
		pCell = m_lCellsFreed.front();
		m_lCellsFreed.pop_front();
		pCell->autorelease();
	}
	return pCell;
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_GridView::reloadData()
{
#if FKCW_WIDGET_USING_LUA
	CCAssert(
		(m_nDataSourceAdapterScriptHandler != 0) || 
		(m_pDataSourceAdapterListener && m_pDataSourceAdapterHandler), "reloadData");
#else
	CCAssert(m_pDataSourceAdapterListener && m_pDataSourceAdapterHandler, "reloadData");
#endif
	CCAssert((int)m_tCellsSize.width != 0 && (int)m_tCellsSize.height != 0, "reloadData");
	CCAssert(m_eDirection == eScrollViewDirectionVertical, "reloadData");
	CCAssert(m_uColumns != 0, "reloadData");

	list<FKCW_UIWidget_GridViewCell*>::iterator iter = m_lCellsUsed.begin();
	while(iter != m_lCellsUsed.end())
	{
		FKCW_UIWidget_GridViewCell* pCell = (*iter);
		m_lCellsFreed.push_back(pCell);
		iter = m_lCellsUsed.erase(iter);
		m_pContainer->removeChild(pCell, true);
		pCell->reset();
	}

	m_sIndices.clear();
	m_vPositions.clear();
	this->updatePositions();
	this->setContentOffsetToTop();
	this->onScrolling();

	relocateContainer();
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_GridView::onScrolling()
{
	unsigned uBeginRow = 0, uEndRow = 0;

	uBeginRow = cellBeginRowFromOffset( getContentOffset() );
	uEndRow = cellEndRowFromOffset( getContentOffset() );

	while( !m_lCellsUsed.empty() )
	{
		FKCW_UIWidget_GridViewCell* pCell = m_lCellsUsed.front();
		unsigned int uRow = pCell->getRow();
		unsigned int uIdx = pCell->getIdx();

		if( uRow < uBeginRow )
		{
			m_sIndices.erase(uIdx);
			m_lCellsUsed.pop_front();
			m_lCellsFreed.push_back(pCell);
			pCell->reset();
			m_pContainer->removeChild(pCell, true);
		}
		else
		{
			break;
		}
	}

	while( !m_lCellsUsed.empty() )
	{
		FKCW_UIWidget_GridViewCell* pCell = m_lCellsUsed.back();
		unsigned int uRow = pCell->getRow();
		unsigned int uIdx = pCell->getIdx();

		if( uRow > uEndRow && uRow < m_uRows )
		{
			m_sIndices.erase(uIdx);
			m_lCellsUsed.pop_back();
			m_lCellsFreed.push_back(pCell);
			pCell->reset();
			m_pContainer->removeChild(pCell, true);
		}
		else
		{
			break;
		}
	}

	for( unsigned int row = uBeginRow; row <= uEndRow && row < m_uRows; ++row )
	{
		unsigned int nCellBeginIndex = cellFirstIndexFromRow(row);
		unsigned int nCellEndIndx = nCellBeginIndex + m_uColumns;
		unsigned int idx = nCellBeginIndex;

		for(; idx < nCellEndIndx && idx < m_uCellsCount; ++idx)
		{
			if( m_sIndices.find(idx) != m_sIndices.end() )
			{
				continue;
			}
			updateCellAtIndex(idx, row);
		}
	}
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_GridView::onDeaccelerateScrollEnded()
{
	onDraggingScrollEnded();
}
//-------------------------------------------------------------------------
void FKCW_UIWidget_GridView::onDraggingScrollEnded()
{
	if( m_uCellsCount == 0 )
		return;

	if( m_bAutoRelocate )
	{
		CCPoint tOffset = getContentOffset();
		unsigned int uRow = cellBeginRowFromOffset(tOffset);
		CCPoint tAPoint = cellPositionFromIndex(cellFirstIndexFromRow(uRow));
		CCPoint tBPoint = CCPoint(0, tAPoint.y - m_tCellsSize.height);
		CCPoint tContentPoint = CCPoint(0, m_obContentSize.height);
		tOffset = tOffset - tContentPoint;
		tAPoint.x = 0;

		float fADistance = tOffset.getDistance(-tAPoint);
		float fBDistance = tOffset.getDistance(-tBPoint);

		if( fADistance < fBDistance )
		{
			float fDuration = fabs(fADistance) / CGRIDVIEW_AUTO_RELOCATE_SPPED;
			setContentOffsetInDuration(-tAPoint + tContentPoint, fDuration);
		}
		else
		{
			float fDuration = fabs(fBDistance) / CGRIDVIEW_AUTO_RELOCATE_SPPED;
			setContentOffsetInDuration(-tBPoint + tContentPoint, fDuration);
		}
	}
}
//-------------------------------------------------------------------------