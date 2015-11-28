//-------------------------------------------------------------------------
#include "StarWidget.h"
#include "StarManger.h"
//-------------------------------------------------------------------------
CStarWidget::CStarWidget()
{
	m_pTouchHandel = NULL;
}
//-------------------------------------------------------------------------
CStarWidget::~CStarWidget()
{
	m_pTouchHandel = NULL;
}
//-------------------------------------------------------------------------
void CStarWidget::SetTouchHandel(IStarTouchHandle* pHandle)
{
	m_pTouchHandel = pHandle;
}
//-------------------------------------------------------------------------
CStarWidget* CStarWidget::create(const ccColor4B& color,IStarTouchHandle* pTouchHandle)
{
	CStarWidget* pRet = new CStarWidget();
	if( pRet && pRet->initWithColor(color) )
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//-------------------------------------------------------------------------
void CStarWidget::onTouchMoved(CCTouch* pTouch, float fDuration)
{
	FKCW_UIWidget_ColorView::onTouchMoved(pTouch,fDuration);
}
//-------------------------------------------------------------------------
void CStarWidget::onTouchEnded(CCTouch* pTouch, float fDuration)
{
	CCPoint pos = pTouch->getLocation();
	if (m_pTouchHandel)
	{
		m_pTouchHandel->OnTouchEnd(pos);
	}
	
	FKCW_UIWidget_ColorView::onTouchEnded(pTouch,fDuration);
}
//-------------------------------------------------------------------------
void CStarWidget::onTouchCancelled(CCTouch* pTouch, float fDuration)
{
	FKCW_UIWidget_ColorView::onTouchCancelled(pTouch,fDuration);
}
//-------------------------------------------------------------------------
void CStarWidget::DoEffect(CCPoint&pos,string strEffectName,string strEffectFirstFarmeName)
{
	CCAnimationCache* pAnimeCache = CCAnimationCache::sharedAnimationCache();
	if (pAnimeCache == NULL)
	{
		return;
	}

	CCAnimation* pAnime = pAnimeCache->animationByName(strEffectName.c_str());
	if ( pAnime == NULL)
	{
		return;
	}

	CCAnimate * pAnimate = CCAnimate::create(pAnime);
	CCSprite* spTemp = CCSprite::createWithSpriteFrameName(strEffectFirstFarmeName.c_str());
	spTemp->setAnchorPoint(ccp(0.5,0.5));
	addChild(spTemp);
	spTemp->setPosition(pos);
	CCFiniteTimeAction* actions= CCSequence::create(pAnimate,  
		CCCallFuncN::create(this,callfuncN_selector(CStarWidget::OnEffectEnd)),NULL);
	spTemp->runAction(actions);
}
//-------------------------------------------------------------------------
void CStarWidget::DoSpecialEffect(string strFrameName)
{
	CCSprite* spTemp = CCSprite::createWithSpriteFrameName(strFrameName.c_str());
	if (spTemp == NULL)
	{
		return;
	}
	spTemp->setAnchorPoint(ccp(0.5,0.5));
	addChild(spTemp);
	spTemp->setPosition(SCREEN_CENTER_POS);
	spTemp->setScale(0.01f);
	CCScaleTo* pScale = CCScaleTo::create(0.8f,1.0f,1.0f);

	CCFiniteTimeAction* actions= CCSequence::create(pScale,  
		CCCallFuncN::create(this,callfuncN_selector(CStarWidget::OnEffectEnd)),NULL);
	spTemp->runAction(actions);
}
//-------------------------------------------------------------------------
void CStarWidget::OnEffectEnd(CCNode* pSender)
{
	CCSprite* sp =  dynamic_cast<CCSprite*>(pSender);
	if (sp)
	{
		sp->removeFromParentAndCleanup(true);
	}
}