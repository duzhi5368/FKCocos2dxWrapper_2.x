//*************************************************************************
//	创建日期:	2014-12-4
//	文件名称:	FKCW_UIWidget_TableView.h
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
#include <set>
using std::vector;
using std::set;
//-------------------------------------------------------------------------
class FKCW_UIWidget_TableView;
class FKCW_UIWidget_TableViewCell;
//-------------------------------------------------------------------------
class FKCW_UIWidget_TableViewCell : public FKCW_UIWidget_Layout
{
public:
    FKCW_UIWidget_TableViewCell();
    virtual ~FKCW_UIWidget_TableViewCell();
    unsigned int getIdx() { return m_uIdx; }
    void setIdx(unsigned int uIdx) { m_uIdx = uIdx; }
    virtual void reset(){ m_uIdx = CC_INVALID_INDEX; }
    
protected:
    unsigned int m_uIdx;
};
//-------------------------------------------------------------------------
class FKCW_UIWidget_TableView : public FKCW_UIWidget_ScrollView, public FKCW_UIWidget_DataSourceAdapterProtocol
{
public:
    FKCW_UIWidget_TableView();
    virtual ~FKCW_UIWidget_TableView();
	bool initWithParams(const CCSize& tViewSize, const CCSize& tCellSize, unsigned int uCellCount, 
		CCObject* pListener, SEL_DataSoucreAdapterHandler pHandler);
    static FKCW_UIWidget_TableView* create(const CCSize& tViewSize);
	static FKCW_UIWidget_TableView* create(const CCSize& tViewSize, const CCSize& tCellSize, unsigned int uCellCount, 
		CCObject* pListener, SEL_DataSoucreAdapterHandler pHandler);

    void setCountOfCell(unsigned int uCellsCount);
    unsigned int getCountOfCell() const;
    void setSizeOfCell(const CCSize& tCellsSize);
    const CCSize& getSizeOfCell() const;
	bool isAutoRelocate() const;
	void setAutoRelocate(bool bAuto);
	float getAutoRelocateSpeed() const;
	void setAutoRelocateSpeed(float fSpeed);
	CCArray* getCells();
	FKCW_UIWidget_TableViewCell* cellAtIndex(unsigned int idx);
    FKCW_UIWidget_TableViewCell* dequeueCell();
	void reloadData();
    
protected:
    virtual void onScrolling();
	virtual void onDeaccelerateScrollEnded();
	virtual void onDraggingScrollEnded();

protected:
	void removeAllFromUsed();
    void removeAllFromFreed();
	void insertSortableCell(FKCW_UIWidget_TableViewCell* pCell, unsigned int idx);
	unsigned int cellBeginIndexFromOffset(const CCPoint& offset);
	unsigned int cellEndIndexFromOffset(const CCPoint& offset);
	CCPoint cellPositionFromIndex(unsigned int idx);
	virtual void updateCellAtIndex(unsigned int idx);
	void updatePositions();

protected:
    CCSize m_tCellsSize;
    unsigned int m_uCellsCount;
	bool m_bAutoRelocate;
	float m_fAutoRelocateSpeed;
    
	set<unsigned int> m_sIndices;
	vector<float> m_vPositions;
	vector<FKCW_UIWidget_TableViewCell*> m_vCellsUsed;
	vector<FKCW_UIWidget_TableViewCell*> m_vCellsFreed;
};
