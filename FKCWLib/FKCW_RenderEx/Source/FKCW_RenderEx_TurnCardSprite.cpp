//-------------------------------------------------------------------------
#include "../Include/FKCW_RenderEx_TurnCardSprite.h"
//-------------------------------------------------------------------------
#define kInAngleZ        270 //里面卡牌的起始Z轴角度
#define kInDeltaZ        90  //里面卡牌旋转的Z轴角度差

#define kOutAngleZ       0   //封面卡牌的起始Z轴角度
#define kOutDeltaZ       90  //封面卡牌旋转的Z轴角度差
//-------------------------------------------------------------------------
enum {
	tag_inCard = 1,
	tag_outCard
};
//-------------------------------------------------------------------------
FKCW_RenderEx_TurnCardSprite::FKCW_RenderEx_TurnCardSprite()
{

}
//-------------------------------------------------------------------------
FKCW_RenderEx_TurnCardSprite::~FKCW_RenderEx_TurnCardSprite()
{
	m_pOpenAnimIn->release();
	m_pOpenAnimOut->release();
}
//-------------------------------------------------------------------------
FKCW_RenderEx_TurnCardSprite* FKCW_RenderEx_TurnCardSprite::create(const char* inCardImageName, const char* outCardImageName, float duration)
{
	FKCW_RenderEx_TurnCardSprite *pSprite = new FKCW_RenderEx_TurnCardSprite();
	if (pSprite && pSprite->init(inCardImageName, outCardImageName, duration))
	{
		pSprite->autorelease();
		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);
	return NULL;
}
//-------------------------------------------------------------------------
bool FKCW_RenderEx_TurnCardSprite::init(const char* inCardImageName, const char* outCardImageName, float duration)
{
	if (!CCSprite::init())
	{
		return false;
	}
	__InitData(inCardImageName, outCardImageName, duration);
	return true;
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_TurnCardSprite::__InitData(const char* inCardImageName, const char* outCardImageName, float duration)
{
	m_bIsOpened = false;

	CCSprite* inCard = CCSprite::create(inCardImageName);
	inCard->setPosition(CCPointZero);
	inCard->setVisible(false);
	inCard->setTag(tag_inCard);
	addChild(inCard);

	CCSprite* outCard = CCSprite::create(outCardImageName);
	outCard->setPosition(CCPointZero);
	outCard->setTag(tag_outCard);
	addChild(outCard);

	m_pOpenAnimIn = (CCActionInterval*)CCSequence::create(CCDelayTime::create(duration * 0.5f),
		CCShow::create(),
		CCOrbitCamera::create(duration * 0.5f, 1, 0, kInAngleZ, kInDeltaZ, 0, 0),
		NULL);
	m_pOpenAnimIn->retain();

	m_pOpenAnimOut = (CCActionInterval *)CCSequence::create(CCOrbitCamera::create(duration * 0.5f, 1, 0, kOutAngleZ, kOutDeltaZ, 0, 0),
		CCHide::create(),
		CCDelayTime::create(duration * 0.5f),
		NULL);
	m_pOpenAnimOut->retain();
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_TurnCardSprite::OpenCard()
{
	CCSprite* inCard = (CCSprite*)getChildByTag(tag_inCard);
	CCSprite* outCard = (CCSprite*)getChildByTag(tag_outCard);
	inCard->runAction(m_pOpenAnimIn);
	outCard->runAction(m_pOpenAnimOut);
}
//-------------------------------------------------------------------------