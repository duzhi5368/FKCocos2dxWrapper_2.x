//-------------------------------------------------------------------------
#include "StarManger.h"
#include "GameResource.h"
#include "GameCommon.h"
#include "AudioBridge.h"
//-------------------------------------------------------------------------

#define BEGIN_RUN_TIMEID 1000
#define COL_RUN_TIMEID 1001
#define ROW_RUN_TIMEID 1002
#define STAR_COMBO_TIMEID 1003

CStarManger::CStarManger(void)
	:m_pRootWindow(NULL)
	,m_nBaseRow(0)
	,m_nBaseCol(0)
	,m_pStarLayer(NULL)
	,m_bIsBusying(false)
{
	m_mapAllStar.clear();
	m_mapSelectStar.clear();
	m_mapDelStar.clear();
	m_mapCheckedStar.clear();
}

//-------------------------------------------------------------------------
CStarManger::~CStarManger(void)
{
	m_mapAllStar.clear();
	m_mapSelectStar.clear();
	m_mapDelStar.clear();
	m_mapCheckedStar.clear();
}
//-------------------------------------------------------------------------
// 设置星星依赖的基本数据
// pRoot 星星AddChild的根节点
// ccRootSize  节点中装载星星的矩阵
// nRow 默认星星行数 
// nCol 默认星星列数
void CStarManger::init(FKCW_UIWidget_WidgetWindow* pRoot,CCSize& ccRootSize,int nRow ,int nCol)
{
	if (pRoot == NULL)
	{
		return;
	}
	m_pRootWindow = pRoot;
	m_RootSize = ccRootSize;
	m_nBaseRow = nRow;
	m_nBaseCol = nCol;
	

	ClearAll(true);

	m_pStarLayer = CStarWidget::create(cc4BLACK);
	m_pStarLayer->setOpacity(160);
	m_pStarLayer->setPosition(SCREEN_CENTER_POS);
	m_pStarLayer->setContentSize(m_RootSize);
	m_pStarLayer->setTouchEnabled(true);
	m_pStarLayer->SetTouchHandel(this);
	m_pRootWindow->addChild(m_pStarLayer);

	m_BasePos.x = (SCERRN_WIDTH - m_RootSize.width)/2;
	m_BasePos.y = (SCERRN_HEIGHT - m_RootSize.height)/2;
	float fX = m_RootSize.width / 10;
	float fY = m_RootSize.height / 10;
	m_IntervalSize = ccz(fX,fY);

	float fScale = fX / 85.0f;

	
	for (int r = 0 ; r < m_nBaseRow;r++)
	{
		for (int c = 0 ; c < m_nBaseCol; c++)
		{
			int nRand = (rand() % (5-1+1))+ 1; 
			CStarSprite* pStar = CStarSprite::create(m_pStarLayer,nRand,fScale);
			if (pStar == NULL)
			{
				return;
			}
			SStarLocation sSl(r,c);
			pStar->SetCurrentLocation(sSl);
			pStar->SetTargetLocation(sSl);
			pStar->SetIntervalSize(m_IntervalSize);
			m_mapAllStar[10*r + c] = pStar;
			m_mapSelectStar[10*r + c] = pStar;
			
		}
	}

	__CreateAnimeCache();
	
}
//-------------------------------------------------------------------------
void CStarManger::__CreateAnimeCache()
{
	CCAnimationCache* pAnimeCache = CCAnimationCache::sharedAnimationCache();
	if (pAnimeCache == NULL)
	{
		return;
	}

	if (pAnimeCache->animationByName(s_szEffectName) != NULL)
	{
		return;
	}

	CCSpriteFrameCache * pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	if (pFrameCache == NULL)
	{
		return;
	}

	CCArray *animFrames = CCArray::create(); 
	char str[64] = {0};  
	for (int i = 1; i <= 5; ++i) {  
		sprintf(str, "effects_%d.png", i);  
		CCSpriteFrame *frame = pFrameCache->spriteFrameByName(str); 
		animFrames->addObject(frame);  
	}  
	CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.1f);  
	animation->setRestoreOriginalFrame(true); 
	pAnimeCache->addAnimation(animation, s_szEffectName);
}
//-------------------------------------------------------------------------
void CStarManger::OnTouchEnd(CCPoint& pos)
{
	
	int nRow = static_cast<int>((pos.x - m_BasePos.x)/m_IntervalSize.width);
	int nCol = static_cast<int>((pos.y - m_BasePos.y)/m_IntervalSize.height);

	FKLOG("(nRow,nCol) = (%d,%d)",nRow,nCol);

	if (m_bIsBusying)
	{
		FKLOG("I am Busying");
		return;
	}
	m_bIsBusying = true;

	m_mapSelectStar.clear();
	m_mapCheckedStar.clear();

	int nKey = nRow *10 + nCol;
	std::map<int,CStarSprite*>::iterator itor = m_mapAllStar.find(nKey);
	if (itor != m_mapAllStar.end())
	{
		CAudioBridge::GetInstance().PlayEffect(eSound_StarClick);

		CStarSprite* pTouchStar = itor->second;
		if (pTouchStar)
		{
			__CheckByStar(pTouchStar);
		}
		FKLOG("Click in ! StarType = %d",pTouchStar->GetType());
	}
	
	int nTotal = (int)m_mapCheckedStar.size();
	FKLOG("hava [%d] star ",nTotal);

	if (m_mapCheckedStar.empty())
	{
		m_bIsBusying = false;
		return;
	}

	// 音效和特效
	__ShowEffect(nTotal);
	

	CCPoint posTemp;
	int nDelKey;
	CStarSprite* pDelStar = NULL;
	std::map<int,CStarSprite*>::iterator itDel = m_mapAllStar.end();
	std::map<int,CStarSprite*>::iterator it = m_mapCheckedStar.begin();
	for (; it != m_mapCheckedStar.end(); ++it)
	{
		FKLOG("key = %d",it->first);
		nDelKey = it->first;
		pDelStar = it->second;
		if (pDelStar)
		{
			pDelStar->DoExplode();
			posTemp.x = pDelStar->GetTargetLocation().nRow * m_IntervalSize.width;
			posTemp.y = pDelStar->GetTargetLocation().nCol * m_IntervalSize.height;
			m_pStarLayer->DoEffect(posTemp,s_szEffectName,s_szEffectFirstFarmeName);
		}

		itDel = m_mapAllStar.find(nDelKey);
		if (itDel != m_mapAllStar.end())
		{
			m_mapAllStar.erase(itDel);
		}
		
	}

	m_vecRowList.clear();
	__RowStarRun();
	
}
//------------------------------------------------------------------------
void CStarManger::__ShowEffect(int nTotal)
{
	
	CAudioBridge::GetInstance().PlayEffect(eSound_StarCombo);
	m_nCurrentComboStarTotal = nTotal;

	if (nTotal > 4)
	{
		__ShowSpecialEffect();
	}


}
//-------------------------------------------------------------------------
void CStarManger::__ShowSpecialEffect()
{
	if (m_nCurrentComboStarTotal < 5)
	{
		return;
	}

	string strEffectFrameName = " ";
	switch (m_nCurrentComboStarTotal)
	{
	case 5:
		CAudioBridge::GetInstance().PlayEffect(eSound_StarGood);
		strEffectFrameName = s_szGoodFrame;
		break;
	case 6:
		CAudioBridge::GetInstance().PlayEffect(eSound_StarAmazing);
		strEffectFrameName = s_szAmazingFrame;
		break;
	default:
		CAudioBridge::GetInstance().PlayEffect(eSound_StarUnbeliveable);
		strEffectFrameName = s_szunbelievaFrame;
		break;
	}

	if (strEffectFrameName == " ")
	{
		return;
	}

	if (m_pStarLayer)
	{
		m_pStarLayer->DoSpecialEffect(strEffectFrameName);
	}
}
//-------------------------------------------------------------------------
// 定时器时间触发回调
void CStarManger::OnTimer( unsigned int unTimerID )
{
	switch (unTimerID)
	{
	case BEGIN_RUN_TIMEID:
		__Run();
		break;

	case COL_RUN_TIMEID:
		__ColStarRun();
		break;

	case ROW_RUN_TIMEID:
		__StarRunFinish();
		break;

	default:
		break;
	}
}
//-------------------------------------------------------------------------
void CStarManger::OnMessageBoxCallBack(int nMessageKey)
{
	if (nMessageKey == eNormalType_GameSuccess)
	{
		FKLOG("m_mapAllStar len = %d",(int)m_mapAllStar.size());
		FKLOG("m_mapDelStar len = %d",(int)m_mapDelStar.size());
		
		__Reset();

	}
}
//-------------------------------------------------------------------------
void CStarManger::__Reset()
{
	int nTotalStar = (int)m_mapAllStar.size() + (int)m_mapDelStar.size();
	if (nTotalStar != (m_nBaseCol * m_nBaseRow))
	{
		FKLOG("__Reset Error");
		return;
	}

	
	std::map<int,CStarSprite*>::iterator itor = m_mapAllStar.begin();
	for (; itor != m_mapAllStar.end();++itor)
	{
		itor->second->DoExplode();
		m_mapDelStar[m_mapDelStar.size()] = itor->second;
	}
	m_mapAllStar.clear();
	m_mapSelectStar.clear();

	int nKey = -1;
	SStarLocation sStrLoc;
	CStarSprite* pStar = NULL;
	for (int r = 0 ; r < m_nBaseRow ; ++r)
	{
		for (int c = 0 ; c < m_nBaseCol; ++c)
		{
			if (m_mapDelStar.empty())
			{
				return;
			}

			nKey = 10 * r + c;
			sStrLoc = SStarLocation(r,c);
			itor = m_mapDelStar.begin();
			pStar = itor->second;
			pStar->SetCurrentLocation(sStrLoc);
			pStar->SetTargetLocation(sStrLoc);
			pStar->SetIntervalSize(m_IntervalSize);
			int nRand = (rand() % (5-1+1))+ 1;
			pStar->SetType(nRand);
			m_mapAllStar[10*r + c] = pStar;
			m_mapSelectStar[10*r + c] = pStar;
			m_mapDelStar.erase(itor);
		}
	}
	Start();
}
//-------------------------------------------------------------------------
void CStarManger::__Run()
{
	if (m_pRootWindow == NULL || m_nBaseRow == 0 || m_nBaseCol == 0)
	{
		return;
	}

	if (m_nStartCol >= m_nBaseCol)
	{
		ITimerSystem* pTimer = GetTimerSystem();
		if (pTimer != NULL)
		{
			pTimer->DestroyTimer(BEGIN_RUN_TIMEID,this);
		}
		m_bIsBusying = false;
		m_mapSelectStar.clear();
		return;
	}

	CAudioBridge::GetInstance().PlayEffect(eSound_StarOut);

	for (int r = 0; r < m_nBaseRow; ++r)
	{
		int nKey = 10 * r + m_nStartCol;
		std::map<int,CStarSprite*>::iterator itor = m_mapSelectStar.find(nKey);
		if (itor == m_mapSelectStar.end())
		{
			continue;
		}

		CStarSprite* pStar = itor->second;
		if (pStar)
		{
			pStar->RunToTarget();
		}

	}
	m_nStartCol++;
}
//-------------------------------------------------------------------------
void CStarManger::Start()
{
	m_nStartCol = 0;
	m_bIsBusying = true;
	ITimerSystem* pTimer = GetTimerSystem();
	if (pTimer != NULL)
	{
		pTimer->CreateTimer(BEGIN_RUN_TIMEID,300,this,m_nBaseCol + 1);
	}
}
//-------------------------------------------------------------------------
void CStarManger::Stop()
{

}
//-------------------------------------------------------------------------
void CStarManger::ClearAStar(int nRow,int nCol)
{

}
//-------------------------------------------------------------------------
void CStarManger::ClearAll(bool bIsClearUp)
{
	std::map<int,CStarSprite*>::iterator itor = m_mapAllStar.begin();
	for (; itor != m_mapAllStar.end(); ++itor)
	{
		CStarSprite* pStar = itor->second;
		if (pStar)
		{
			pStar->Clear(bIsClearUp);
		}
	}

	itor = m_mapDelStar.begin();
	for (; itor != m_mapDelStar.end(); ++itor)
	{
		CStarSprite* pStar = itor->second;
		if (pStar)
		{
			pStar->Clear(bIsClearUp);
		}
	}

	m_mapAllStar.clear();
	m_mapSelectStar.clear();
	m_mapDelStar.clear();

	if (m_pStarLayer)
	{
		m_pStarLayer->removeFromParentAndCleanup(bIsClearUp);
	}
	m_pStarLayer = NULL;

	ITimerSystem* pTimer = GetTimerSystem();
	if (pTimer != NULL)
	{
		pTimer->DestroyTimer(BEGIN_RUN_TIMEID,this);
		pTimer->DestroyTimer(COL_RUN_TIMEID,this);
		pTimer->DestroyTimer(ROW_RUN_TIMEID,this);
		pTimer->DestroyTimer(STAR_COMBO_TIMEID,this);
		
	}
}
//-------------------------------------------------------------------------
void CStarManger::__CheckByStar(CStarSprite* pStar)
{
	if (pStar == NULL)
	{
		return;
	}

	SStarLocation sStarLoc = pStar->GetCurrentLocation();
	int nKey = sStarLoc.nRow  * 10 + sStarLoc.nCol;
	std::map<int,CStarSprite*>::iterator itor = m_mapCheckedStar.find(nKey);
	if (itor != m_mapCheckedStar.end())
	{
		return;
	}
	__CheckUp(pStar);
	__CheckDown(pStar);
	__CheckLeft(pStar);
	__CheckRight(pStar);
	
}
//-------------------------------------------------------------------------
void CStarManger::__CheckUp(CStarSprite* pStar)
{
	if (pStar == NULL)
	{
		return;
	}
	int nType = pStar->GetType();
	SStarLocation sStarLoc = pStar->GetCurrentLocation();
	if ((sStarLoc.nRow + 1) == m_nBaseRow)
	{
		return;
	}
	int nKey = (sStarLoc.nRow ) * 10 + sStarLoc.nCol;
	int nNextKey = (sStarLoc.nRow + 1) * 10 + sStarLoc.nCol;

	std::map<int,CStarSprite*>::iterator itor = m_mapAllStar.find(nNextKey);
	if (itor != m_mapAllStar.end())
	{
		CStarSprite* pNextStar = itor->second;
		if (pNextStar && pNextStar->GetType() == pStar->GetType())
		{
			m_mapCheckedStar[nKey] = pStar;
			__CheckByStar(pNextStar);
			m_mapCheckedStar[nNextKey] = pNextStar;
		}
		
	}
}
//-------------------------------------------------------------------------
void CStarManger::__CheckDown(CStarSprite* pStar)
{
	if (pStar == NULL)
	{
		return;
	}
	int nType = pStar->GetType();
	SStarLocation sStarLoc = pStar->GetCurrentLocation();
	if (sStarLoc.nRow <= 0)
	{
		return;
	}
	int nKey = (sStarLoc.nRow ) * 10 + sStarLoc.nCol;
	int nNextKey = (sStarLoc.nRow - 1) * 10 + sStarLoc.nCol;

	std::map<int,CStarSprite*>::iterator itor = m_mapAllStar.find(nNextKey);
	if (itor != m_mapAllStar.end())
	{
		CStarSprite* pNextStar = itor->second;
		if (pNextStar && pNextStar->GetType() == pStar->GetType())
		{
			m_mapCheckedStar[nKey] = pStar;
			__CheckByStar(pNextStar);
			m_mapCheckedStar[nNextKey] = pNextStar;
		}

	}
}
//-------------------------------------------------------------------------
void CStarManger::__CheckLeft(CStarSprite* pStar)
{
	if (pStar == NULL)
	{
		return;
	}
	int nType = pStar->GetType();
	SStarLocation sStarLoc = pStar->GetCurrentLocation();
	if (sStarLoc.nCol <= 0)
	{
		return;
	}
	int nKey = sStarLoc.nRow * 10 + sStarLoc.nCol;
	int nNextKey = sStarLoc.nRow * 10 + sStarLoc.nCol - 1;

	std::map<int,CStarSprite*>::iterator itor = m_mapAllStar.find(nNextKey);
	if (itor != m_mapAllStar.end())
	{
		CStarSprite* pNextStar = itor->second;
		if (pNextStar && pNextStar->GetType() == pStar->GetType())
		{
			m_mapCheckedStar[nKey] = pStar;
			__CheckByStar(pNextStar);
			m_mapCheckedStar[nNextKey] = pNextStar;
		}

	}
}
//-------------------------------------------------------------------------
void CStarManger::__CheckRight(CStarSprite* pStar)
{
	if (pStar == NULL)
	{
		return;
	}
	int nType = pStar->GetType();
	SStarLocation sStarLoc = pStar->GetCurrentLocation();
	if ((sStarLoc.nCol + 1) >= m_nBaseCol )
	{
		return;
	}
	int nKey = sStarLoc.nRow * 10 + sStarLoc.nCol;
	int nNextKey = sStarLoc.nRow * 10 + sStarLoc.nCol + 1;

	std::map<int,CStarSprite*>::iterator itor = m_mapAllStar.find(nNextKey);
	if (itor != m_mapAllStar.end())
	{
		CStarSprite* pNextStar = itor->second;
		if (pNextStar && pNextStar->GetType() == pStar->GetType())
		{
			m_mapCheckedStar[nKey] = pStar;
			__CheckByStar(pNextStar);
			m_mapCheckedStar[nNextKey] = pNextStar;
		}

	}
}
//-------------------------------------------------------------------------
void CStarManger::__ColStarRun()
{
	if (m_vecRowList.empty())
	{
		__StarRunFinish();
		return;
	}
	sort(m_vecRowList.begin(),m_vecRowList.end());

	int nRowCurrent = m_nBaseRow;
	std::vector<int>::iterator itorVec = m_vecRowList.begin();
	for (; itorVec != m_vecRowList.end(); ++itorVec)
	{
		FKLOG("__ColStarRun row = %d",*itorVec);
		nRowCurrent = min(*itorVec,nRowCurrent);
	}
	FKLOG("__ColStarRun nRowCurrent = %d",nRowCurrent);

	if ((nRowCurrent + 1) >= m_nBaseRow)
	{
		return;
	}

	int nDistance = 0;
	int nOldKey = -1;
	int nNewKey = -1;
	std::map<int,CStarSprite*>::iterator itorMap = m_mapAllStar.end();
	CStarSprite* pStar = NULL;
	SStarLocation sStarLocationNew;

	for (int nRowNow = (nRowCurrent + 1) ; nRowNow < m_nBaseRow ; ++nRowNow)
	{
		for (int col = 0 ; col < m_nBaseCol; ++col)
		{
			nNewKey = 10 * (nDistance + nRowCurrent) + col;
			nOldKey = 10 * nRowNow + col;
			itorMap = m_mapAllStar.find(nOldKey);
			if (itorMap != m_mapAllStar.end())
			{
				sStarLocationNew = SStarLocation(nDistance + nRowCurrent,col);
				pStar = itorMap->second;
				pStar->SetTargetLocation(sStarLocationNew);
				pStar->RunToTarget();
				m_mapAllStar.erase(itorMap);
				m_mapAllStar[nNewKey] = pStar;
			}
			else
			{
				if (col == 0)
				{
					nDistance--;
					break;
				}
			}

		}

		nDistance++;
	}

	m_vecRowList.clear();
	__NextRunEnd(ROW_RUN_TIMEID);
}
//-------------------------------------------------------------------------
void CStarManger::__RowStarRun()
{
	if (m_mapCheckedStar.empty())
	{
		__NextRunEnd(COL_RUN_TIMEID);
		return;
	}

	int nColMin = m_nBaseCol;
	int nColMax = -1;
	int nRowCurrent;
	int nKey = -1;
	std::map<int,CStarSprite*>::iterator itor = m_mapCheckedStar.begin();
	if (itor == m_mapCheckedStar.end())
	{
		return;
	}
	
	SStarLocation sStarLocBase = itor->second->GetCurrentLocation();
	nRowCurrent = sStarLocBase.nRow;
	SStarLocation sStarLocIndex;
	for (; itor != m_mapCheckedStar.end(); ++ itor)
	{
		sStarLocIndex = itor->second->GetCurrentLocation();
		if (nRowCurrent == sStarLocIndex.nRow)
		{
			nColMin = min(nColMin,sStarLocIndex.nCol);
			nColMax = max(nColMax,sStarLocIndex.nCol);
		}
	}

	int nDistance = nColMax - nColMin + 1;
	int nSub = 0;
	CStarSprite* pStar = NULL;

	for (int col = nColMin ; col <= nColMax ; ++col)
	{
		nKey = 10 * nRowCurrent + col;
		itor = m_mapCheckedStar.find(nKey);
		if (itor != m_mapCheckedStar.end())
		{
			//m_mapDelStar[itor->first] = itor->second;
			m_mapDelStar[m_mapDelStar.size()] = itor->second;
			m_mapCheckedStar.erase(itor);
		}
		else
		{
			int nKeyNew = -1;
			itor = m_mapAllStar.find(nKey);
			if (itor != m_mapAllStar.end())
			{
				nKeyNew = 10 * nRowCurrent + nColMin + nSub;
				pStar = itor->second;
				sStarLocIndex = SStarLocation(nRowCurrent,nColMin + nSub);
				pStar->SetTargetLocation(sStarLocIndex);
				pStar->RunToTarget();
				//m_mapDelStar[itor->first] = pStar;
				m_mapAllStar.erase(itor);
				m_mapAllStar[nKeyNew] = pStar;
				nSub++;
				nDistance--;
			}
		}
		
	}

	bool isNoStarAtThisCol = true;
	if ((nColMax + 1) < m_nBaseCol)
	{
		int nKeyNew = -1;
		//int nDistance = nColMax - nColMin + 1;
		
		for (int col = (nColMax + 1) ; col < m_nBaseCol; ++col)
		{
			nKey = 10 * nRowCurrent + col;
			nKeyNew = nKey - nDistance;
			itor = m_mapAllStar.find(nKey);
			if (itor != m_mapAllStar.end())
			{
				pStar = itor->second;
				sStarLocIndex = SStarLocation(nRowCurrent,col - nDistance);
				pStar->SetTargetLocation(sStarLocIndex);
				pStar->RunToTarget();
				m_mapAllStar.erase(itor);
				m_mapAllStar[nKeyNew] = pStar;
				isNoStarAtThisCol = false;
			}
		}
	}

	if (isNoStarAtThisCol && nColMin == 0)
	{
		m_vecRowList.push_back(nRowCurrent);
	}
	__RowStarRun();
	
}
//-------------------------------------------------------------------------
void CStarManger::__NextRunEnd(int nTimerID)
{
	ITimerSystem* pTimer = GetTimerSystem();
	if (pTimer != NULL)
	{
		pTimer->CreateTimer(nTimerID,650,this,1);
	}
}
//-------------------------------------------------------------------------
void CStarManger::__StarRunFinish()
{

	m_vecRowList.clear();
	bool done = __CheckGameEnd();
	if (!done)
	{
		m_bIsBusying = false;
	}
	
}
//-------------------------------------------------------------------------
bool CStarManger::__CheckGameEnd()
{
	
	bool done = __CheckStarConnect();
	if (done)
	{
		return false;
	}

	done = __CheckMissionState();
	if (done)
	{
		__ShowGameSuccessUI();
	}
	else
	{
		__ShowGameOverUI();
	}

	return true;
}
//-------------------------------------------------------------------------
// 检查是否还有相同星星彼此项链
bool CStarManger::__CheckStarConnect()
{
	if (m_mapAllStar.empty())
	{
		return false;
	}

	m_mapCheckedStar.clear();

	std::map<int,CStarSprite*>::iterator itor = m_mapAllStar.begin();
	CStarSprite* pStar = NULL;
	for (; itor != m_mapAllStar.end(); ++itor)
	{
		pStar = itor->second;
		__CheckByStar(pStar);
		if (!m_mapCheckedStar.empty())
		{
			return true;
		}
	}

	return false;
}
//-------------------------------------------------------------------------
// 检查分数是否完成
bool CStarManger::__CheckMissionState()
{
	FKLOG("__CheckMissionState default true");
	return true;
}
//-------------------------------------------------------------------------
void CStarManger::__ShowGameOverUI()
{
	FKLOG("__ShowGameOverUI Show GameOver");
}
//-------------------------------------------------------------------------
void CStarManger::__ShowGameSuccessUI()
{
	FKLOG("__ShowGameOverUI Show Success");
	CMsgBoxMgr::instance().Show(this,m_pRootWindow,eNormalType_GameSuccess);
}
//-------------------------------------------------------------------------