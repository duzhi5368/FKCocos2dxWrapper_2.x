//*************************************************************************
//	创建日期:	2014-11-11
//	文件名称:	FKCW_UI_TableView.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
/*  使用范例：
	FKCW_UI_TabelView_Controller* _tableViewController = new FKCW_UI_TabelView_Controller( CCSizeMake( 50, 100 ) );
	CCTableView* _tableView = CCTableView::create( _tableViewController, CCSizeMake( 200, 150 ) );
	_tableView->setDelegate( _tableViewController );
	_tableView->setPosition( ccp( 200,200 ) );
	FKCW_UI_TableView_ScrollBar* _scrollBar;
	bool isV = false;
	if( isV )
	{
		_tableView->setDirection( kCCScrollViewDirectionVertical );
	_	scrollBar = FKCW_UI_TableView_ScrollBar::create( _tableView, eScrollBarType_VerticalIn );
	}
	else
	{
		_tableView->setDirection( kCCScrollViewDirectionHorizontal );
		_scrollBar = FKCW_UI_TableView_ScrollBar::create( _tableView, eScrollBarType_HorizontalIn );
	}
	_tableView->setVerticalFillOrder( kCCTableViewFillTopDown );
	_tableView->reloadData();
	this->addChild( _tableView );
*/
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_UI_Macros.h"
#include "cocos-ext.h"
//-------------------------------------------------------------------------
USING_NS_CC_EXT;
//-------------------------------------------------------------------------
enum ENUM_ScrollBarType
{
	eScrollBarType_VerticalIn	= 1,
	eScrollBarType_VerticalOut	= 2,
	eScrollBarType_HorizontalIn	= 3,
	eScrollBarType_HorizontalOut= 4,
};
class FKCW_UI_TableView_ScrollBar : public CCScale9Sprite
{
private:
	int				m_nType;
	float			m_fFadeDelayTime;
	float			m_fSvFixSize, m_fNowContentSize, m_fBeforeContentSize, m_fBarSize;
	bool			m_bIsCanFade;
	CCScrollView*	m_pTarget;

public:
	FKCW_UI_TableView_ScrollBar( CCScrollView* table, int type );
	virtual ~FKCW_UI_TableView_ScrollBar();
	static FKCW_UI_TableView_ScrollBar* Create( CCScrollView* table, int type );

	void SetCanFadeBar( bool isFade ) { m_bIsCanFade = isFade; }
	void SetFadeDelayTime( float fadeDelayTime ) { m_fFadeDelayTime = fadeDelayTime; }

	void FadeOut( float dt );
	void FadeIn( float dt );
	void RefreshPosition();
	void RefreshScale();
	void Refresh();
};
//-------------------------------------------------------------------------
class FKCW_UI_TabelView_Cell : public cocos2d::extension::CCTableViewCell
{
private:
	CCLabelTTF*					m_pLabel;
public:
	FKCW_UI_TabelView_Cell();
	~FKCW_UI_TabelView_Cell();
public:
	virtual void	draw();
	CCLabelTTF*		GetLabel() const { return m_pLabel; }
};
//-------------------------------------------------------------------------
class FKCW_UI_TabelView_Controller : public CCTableViewDataSource, public CCTableViewDelegate
{
private:
	CCSize							m_tagCellSize;
	FKCW_UI_TableView_ScrollBar*	m_pBar;
	vector<int>						m_vecData;
public:
	FKCW_UI_TabelView_Controller( CCSize size );
	~FKCW_UI_TabelView_Controller();

	CCSize GetCellSize() { return m_tagCellSize; }
	void SetCellSize( CCSize size ) { m_tagCellSize = size; }

	void LaunchScrollBar( FKCW_UI_TableView_ScrollBar *bar ) { m_pBar = bar; m_pBar->Refresh(); }

	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	virtual CCSize cellSizeForTable(CCTableView *table);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);

	virtual void scrollViewDidScroll(CCScrollView* view);
	virtual void scrollViewDidZoom(CCScrollView* view);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual void tableCellHighlight(CCTableView* table, CCTableViewCell* cell);
	virtual void tableCellUnhighlight(CCTableView* table, CCTableViewCell* cell);
};
//-------------------------------------------------------------------------