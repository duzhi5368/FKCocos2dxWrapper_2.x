//*************************************************************************
//	创建日期:	2014-12-5
//	文件名称:	FKCW_UIWidget_GridView.h
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
#include <list>
#include <set>
using std::list;
using std::set;
using std::vector;
//-------------------------------------------------------------------------
USING_NS_CC;
//-------------------------------------------------------------------------
class FKCW_UIWidget_GridView;
class FKCW_UIWidget_GridViewCell;
//-------------------------------------------------------------------------
class FKCW_UIWidget_GridViewCell : public FKCW_UIWidget_Layout
{
public:
    FKCW_UIWidget_GridViewCell();
	virtual ~FKCW_UIWidget_GridViewCell();
	unsigned int getIdx() const { return m_uIdx; }
	void setIdx(unsigned int uIdx) { m_uIdx = uIdx; }
	unsigned int getRow() const { return m_uRow; }
	void setRow(unsigned int uRow) { m_uRow = uRow; }
	virtual void reset(){ m_uIdx = CC_INVALID_INDEX; }
    
protected:
	unsigned int m_uRow;
    unsigned int m_uIdx;
};
//-------------------------------------------------------------------------
class FKCW_UIWidget_GridView : public FKCW_UIWidget_ScrollView, public FKCW_UIWidget_DataSourceAdapterProtocol
{
public:
	FKCW_UIWidget_GridView();
	virtual ~FKCW_UIWidget_GridView();
	bool initWithParams(const CCSize& tViewSize, const CCSize& tCellSize, unsigned int uCellCount,
		CCObject* pListener, SEL_DataSoucreAdapterHandler pHandler);
	static FKCW_UIWidget_GridView* create(const CCSize& tViewSize);
	static FKCW_UIWidget_GridView* create(const CCSize& tViewSize, const CCSize& tCellSize, unsigned int uCellCount,
		CCObject* pListener, SEL_DataSoucreAdapterHandler pHandler);

	void setCountOfCell(unsigned int uCellsCount);
    unsigned int getCountOfCell() const;
    void setSizeOfCell(const CCSize& tCellsSize);
    const CCSize& getSizeOfCell() const;
	void setColumns(unsigned int uColumns);
	unsigned int getColumns() const;
	unsigned int getRows() const;
	bool isAutoRelocate() const;
	void setAutoRelocate(bool bAuto);
	CCArray* getCells();
	FKCW_UIWidget_GridViewCell* cellAtIndex(unsigned int idx);
	FKCW_UIWidget_GridViewCell* dequeueCell();
	void reloadData();

protected:
	virtual void onScrolling();
	virtual void onDeaccelerateScrollEnded();
	virtual void onDraggingScrollEnded();

protected:
	void removeAllFromUsed();
    void removeAllFromFreed();
	void insertSortableCell(FKCW_UIWidget_GridViewCell* pCell, unsigned int idx);
	void updatePositions();
	void updateCellAtIndex(unsigned int idx, unsigned int row);
	const CCPoint& cellPositionFromIndex(unsigned int idx);
	unsigned int cellBeginRowFromOffset(const CCPoint& offset);
	unsigned int cellEndRowFromOffset(const CCPoint& offset);
	unsigned int cellFirstIndexFromRow(unsigned int row);

protected:
	CCSize m_tCellsSize;
    unsigned int m_uCellsCount;
	unsigned int m_uColumns;
	unsigned int m_uRows;
	bool m_bAutoRelocate;

	set<unsigned int> m_sIndices;
	list<CCPoint> m_lCellsRow;
	list<FKCW_UIWidget_GridViewCell*> m_lCellsUsed;
	list<FKCW_UIWidget_GridViewCell*> m_lCellsFreed;
	vector<CCPoint> m_vPositions;
};