//-------------------------------------------------------------------------
#include "StarSprite.h"
//-------------------------------------------------------------------------
#define DEFALUT_POSITION ccp(SCREEN_CENTER_POS_X,SCERRN_HEIGHT)

CStarSprite::CStarSprite(void)
{
}
//-------------------------------------------------------------------------
CStarSprite::~CStarSprite(void)
{
}
//-------------------------------------------------------------------------
CStarSprite* CStarSprite::create(CStarWidget* pRoot,int nType,float fScale)
{
	CStarSprite* pRet = new CStarSprite();
	if (pRet && pRet->init(pRoot,nType,fScale))
	{
		return pRet;
	}
	return NULL;
}
//-------------------------------------------------------------------------
bool CStarSprite::init(CStarWidget* pRoot,int nType,float fScale)
{
	char szBuff[64];
	sprintf(szBuff,"color_%d.png",nType);
	
	// 可用此效果,但性能堪忧
	//m_pStarCCSprite = new FKCW_EffectNode_ShatterSprite();
	//m_pStarCCSprite->autorelease();
	//m_pStarCCSprite->init( szBuff );
	//m_pStarCCSprite->initWithSpriteFrameName(szBuff);

	m_pStarCCSprite = CCSprite::createWithSpriteFrameName(szBuff);
	if (m_pStarCCSprite == NULL)
	{
		return false;
	}
	m_pStarCCSprite->setPosition(DEFALUT_POSITION);
	m_pStarCCSprite->setScale(fScale);
	m_pStarCCSprite->setAnchorPoint(ccp(0.0,0.0));
	pRoot->addChild(m_pStarCCSprite);
	m_nType = nType;

	return true;
}
//-------------------------------------------------------------------------
void CStarSprite::SetIntervalSize(CCSize& intervalSize)
{
	m_IntervalSize = intervalSize;
}
//-------------------------------------------------------------------------
void CStarSprite::SetTargetLocation(SStarLocation & loc)
{
	m_sTargetLoc = loc;//
}
//-------------------------------------------------------------------------
void CStarSprite::SetCurrentLocation(SStarLocation & loc)
{
	m_sCurrentLoc = loc;//SStarLocation(nRow,nCol);
}
//-------------------------------------------------------------------------
void CStarSprite::Clear(bool bIsClearUp)
{
	if (m_pStarCCSprite)
	{
		m_pStarCCSprite->removeAllChildrenWithCleanup(bIsClearUp);
		//m_pStarCCSprite->release();
	}
	m_pStarCCSprite = NULL;
}
//-------------------------------------------------------------------------
void CStarSprite::DoExplode()
{

// 	if (m_pStarCCSprite)
// 	{
// 		m_pStarCCSprite->setOpacity(0);
// 		m_pStarCCSprite->stopAllActions();
// 		FKCW_Action_ShatterAction* shatter = FKCW_Action_ShatterAction::create(4);
// 		m_pStarCCSprite->runAction(shatter);
// 	}

	m_pStarCCSprite->setPosition(DEFALUT_POSITION);

}
//-------------------------------------------------------------------------
void CStarSprite::SetType(int nType)
{
	m_nType = nType;
	char szBuff[64];
	sprintf(szBuff,"color_%d.png",nType);

	if (m_pStarCCSprite)
	{
		CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(szBuff);
		if (pFrame == NULL)
		{
			return;
		}
		m_pStarCCSprite->setDisplayFrame(pFrame);
		m_pStarCCSprite->setAnchorPoint(ccp(0.0,0.0));
	}
}
//-------------------------------------------------------------------------
void CStarSprite::RunToTarget()
{
	CCPoint targetPos = ccp(
		m_sTargetLoc.nRow * m_IntervalSize.width,
		m_sTargetLoc.nCol * m_IntervalSize.height);
	CCPoint currentPos = m_pStarCCSprite->getPosition();
	float dX = fabs(targetPos.x - currentPos.x);
	float dY = fabs(targetPos.y - currentPos.y);
	if (dX < 10.0f && dY < 10.0f)
	{
		return;
	}

	m_sCurrentLoc = m_sTargetLoc;

	CCMoveTo* pMove = CCMoveTo::create(0.5f,targetPos);
	if (m_pStarCCSprite)
	{
		m_pStarCCSprite->runAction(pMove);
	}
}
//-------------------------------------------------------------------------
void CStarSprite::RunToCurrent()
{
	CCPoint cPos = ccp(
		m_sTargetLoc.nRow * m_IntervalSize.width,
		m_sTargetLoc.nCol * m_IntervalSize.height);
	CCPoint currentPos = m_pStarCCSprite->getPosition();
	float dX = fabs(cPos.x - currentPos.x);
	float dY = fabs(cPos.y - currentPos.y);
	if (dX < 10.0f && dY < 10.0f)
	{
		return;
	}
	m_pStarCCSprite->setPosition(cPos);
}
