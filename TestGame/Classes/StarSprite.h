//*************************************************************************
//	创建日期:	2015-3-26
//	文件名称:	StarSprite.h
//  创 建 人:   邓观福 Rect 	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../../FKEngine/FKEngineCommonHead.h"
#include "StarWidget.h"
//-------------------------------------------------------------------------

struct SStarLocation
{
	int nRow;
	int nCol;
	SStarLocation()
	{
		nRow = -1;
		nCol = -1;
	}
	SStarLocation(int row,int col)
	{
		nRow = row;
		nCol = col;
	}
};

class CStarSprite
{
public:
	CStarSprite(void);
	~CStarSprite(void);

public:
	static CStarSprite* create(CStarWidget* pRoot,int nType,float fScale = 1.0f);

public:
	bool	init(CStarWidget* pRoot,int nType,float fScale);
	void	SetIntervalSize(CCSize& intervalSize);
	void	SetTargetLocation(SStarLocation & loc);
	SStarLocation& GetTargetLocation(){ return m_sTargetLoc;};
	void	SetCurrentLocation(SStarLocation & loc);
	SStarLocation& GetCurrentLocation(){ return m_sCurrentLoc; };
	void	SetType(int nType);
	int		GetType(){return m_nType;};
	void	DoExplode();
	void	RunToTarget();
	void	RunToCurrent();
	void	Clear(bool bIsClearUp = false);

private:

	//FKCW_EffectNode_ShatterSprite*	  m_pStarCCSprite;
	CCSprite*	  m_pStarCCSprite;
	CCSize		  m_IntervalSize;
	SStarLocation m_sCurrentLoc;
	SStarLocation m_sTargetLoc;
	int			  m_nType;
};

