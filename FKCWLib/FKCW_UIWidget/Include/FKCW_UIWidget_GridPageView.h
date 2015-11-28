//*************************************************************************
//	创建日期:	2014-12-5
//	文件名称:	FKCW_UIWidget_GridPageView.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "cocos2d.h"
#include "FKCW_UIWidget_Widget.h"
#include "FKCW_UIWidget_Protocol.h"
#include "FKCW_UIWidget_TableView.h"
//-------------------------------------------------------------------------
USING_NS_CC;
//-------------------------------------------------------------------------
class FKCW_UIWidget_GridPageView;
class FKCW_UIWidget_GridPageViewCell;
class FKCW_UIWidget_GridPageViewPage;
//-------------------------------------------------------------------------
class FKCW_UIWidget_GridPageViewPage : public FKCW_UIWidget_TableViewCell
{
public:
	FKCW_UIWidget_GridPageViewPage();
	virtual ~FKCW_UIWidget_GridPageViewPage();

protected:
	friend class FKCW_UIWidget_GridPageView;
	vector<FKCW_UIWidget_GridPageViewCell*>& getGirdCells();
	vector<FKCW_UIWidget_GridPageViewCell*> m_vGirdCells;
};
//-------------------------------------------------------------------------
class FKCW_UIWidget_GridPageViewCell : public FKCW_UIWidget_TableViewCell
{
public:
	FKCW_UIWidget_GridPageViewCell();
	virtual ~FKCW_UIWidget_GridPageViewCell();
};
//-------------------------------------------------------------------------
class FKCW_UIWidget_GridPageView : public FKCW_UIWidget_TableView, public FKCW_UIWidget_PageChangeableProtocol
{
public:
	FKCW_UIWidget_GridPageView();
	virtual ~FKCW_UIWidget_GridPageView();
	static FKCW_UIWidget_GridPageView* create(const CCSize& tViewSize);
	static FKCW_UIWidget_GridPageView* create(const CCSize& tViewSize, const CCSize& tCellSize, unsigned int uCellCount, 
		CCObject* pListener, SEL_DataSoucreAdapterHandler pHandler);

public:
	void setCountOfCell(unsigned int uCellsCount);
    unsigned int getCountOfCell() const;
    void setSizeOfCell(const CCSize& tCellsSize);
    const CCSize& getSizeOfCell() const;
	void setRows(unsigned int uRows);
	unsigned int getRows() const;
	void setColumns(unsigned int uColumns);
	unsigned int getColumns() const;
	void reloadData();

protected:
	virtual void onScrolling();
	void updateGridCellsPosition();
	void updatePageCount();
	virtual void updateCellAtIndex(unsigned int idx);

protected:
	CCSize m_tGridCellsSize;
	unsigned int m_uGridCellsCount;
	unsigned int m_uColumns;
	unsigned int m_uRows;
	unsigned int m_uCellsMaxCountInPage;

	vector<CCPoint> m_vGridCellsPosition;
};