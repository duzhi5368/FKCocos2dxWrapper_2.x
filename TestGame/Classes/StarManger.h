//*************************************************************************
//	创建日期:	2015-3-26
//	文件名称:	StarManger.h
//  创 建 人:   邓观福 Rect 	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../../FKEngine/FKEngineCommonHead.h"
#include "StarWidget.h"
#include "StarSprite.h"
#include "MsgBoxMgr.h"
//-------------------------------------------------------------------------

class CStarManger : 
	public IStarTouchHandle
	,public ITimerHander
	,public IMsgBoxHandle
{
public:
	CStarManger(void);
	~CStarManger(void);

public:
	
	void				init(FKCW_UIWidget_WidgetWindow* pRoot,CCSize& ccRootSize,int nRow = 10,int nCol = 10);
	void				Start();
	void				Stop();
	void				ClearAStar(int nRow,int nCol);
	void				ClearAll(bool bIsClearUp = false);
	virtual void		OnTouchEnd(CCPoint& pos);
	virtual void		OnTimer( unsigned int unTimerID );
	virtual void		OnMessageBoxCallBack(int nMessageKey);

private:
	void				__CreateAnimeCache();
	void				__Run();
	void				__Reset();
	void				__CheckByStar(CStarSprite* pStar);
	void				__CheckUp(CStarSprite* pStar);
	void				__CheckDown(CStarSprite* pStar);
	void				__CheckLeft(CStarSprite* pStar);
	void				__CheckRight(CStarSprite* pStar);
	void				__ShowEffect(int nTotal);
	void				__ShowSpecialEffect();
	void				__ColStarRun();
	void				__RowStarRun();
	void				__NextRunEnd(int nTimerID);
	void				__StarRunFinish();
	bool				__CheckStarConnect();
	bool				__CheckMissionState();
	bool				__CheckGameEnd();
	void				__ShowGameOverUI();
	void				__ShowGameSuccessUI();

private:

	FKCW_UIWidget_WidgetWindow*		m_pRootWindow;
	std::map<int,CStarSprite*>		m_mapAllStar;
	std::map<int,CStarSprite*>		m_mapSelectStar;
	std::map<int,CStarSprite*>		m_mapCheckedStar;
	std::map<int,CStarSprite*>		m_mapDelStar;
	std::vector<int>				m_vecRowList;
	CStarWidget*					m_pStarLayer;
	CCSize							m_RootSize;
	CCSize							m_IntervalSize;
	CCPoint							m_BasePos;
	int								m_nBaseRow;
	int								m_nBaseCol;
	int								m_nStartCol;
	int								m_nCurrentComboStarTotal;
	bool							m_bIsBusying;


};

