//-------------------------------------------------------------------------
#include "../Include/FKCW_UI_SimpleRookieGuiderLayer.h"
#include "../../FKCW_Action/Include/FKCW_Action_TreeFadeIn.h"
//-------------------------------------------------------------------------
FKCW_UI_SimpleRookieGuiderLayer::FKCW_UI_SimpleRookieGuiderLayer()
	: m_pArrow( NULL )
	, m_pHint( NULL )
{

}
//-------------------------------------------------------------------------
FKCW_UI_SimpleRookieGuiderLayer::~FKCW_UI_SimpleRookieGuiderLayer()
{

}
//-------------------------------------------------------------------------
FKCW_UI_SimpleRookieGuiderLayer* FKCW_UI_SimpleRookieGuiderLayer::Create()
{
	FKCW_UI_SimpleRookieGuiderLayer* g = new FKCW_UI_SimpleRookieGuiderLayer();
	g->init();
	return (FKCW_UI_SimpleRookieGuiderLayer*)g->autorelease();
}
//-------------------------------------------------------------------------
// 覆盖父类函数
void FKCW_UI_SimpleRookieGuiderLayer::FadeIn( float duration, float delay )
{
	FKCW_UI_RookieGuideLayer::FadeIn(duration, delay);

	// 其他的也要渐显
	FKCW_Action_TreeFadeIn* fadeIn = FKCW_Action_TreeFadeIn::Create(duration);
	fadeIn->ExcludeNode(m_pContent, false);
	runAction(CCSequence::createWithTwoActions(CCDelayTime::create(delay),
		fadeIn));
}
//-------------------------------------------------------------------------
// 将箭头对准的区域中心，有特定的角度和距离
// 参数：regionIndex 区域的索引 distance 区域中心和箭头精灵中心的举例 degree 区域中心指向箭头中间的角度向量（正值为逆时针）
//		arrowPresetRotation 箭头自身的旋转角度（正值为逆时针）
void FKCW_UI_SimpleRookieGuiderLayer::PointToRegionCenter(int regionIndex, float distance, float degree, float arrowPresetRotation )
{
	if(regionIndex < 0 || regionIndex >= static_cast<int>(m_vecRegions.size()) || !m_pArrow)
		return;

	// 获取指定区域中心
	SRegion& reg = m_vecRegions.at(regionIndex);
	CCRect& r = reg.m_tagRect;
	CCPoint center = ccpAdd(r.origin, ccp(r.size.width / 2, r.size.height / 2));

	// 获取箭头精灵中心
	float radian = CC_DEGREES_TO_RADIANS(degree);
	center.x += distance * cosf(radian);
	center.y += distance * sinf(radian);

	// 设置箭头位置
	m_pArrow->setPosition(center);
	m_pArrow->setRotation(-(degree - 180 - arrowPresetRotation));
}
//-------------------------------------------------------------------------
// 计算提示信息位置（相对于箭头的坐标）
void FKCW_UI_SimpleRookieGuiderLayer::ShiftHint(float dx, float dy)
{
	if(m_pArrow)
		m_pHint->setPosition(ccpAdd(m_pArrow->getPosition(), ccp(dx, dy)));
}
//-------------------------------------------------------------------------
void FKCW_UI_SimpleRookieGuiderLayer::setArrow(CCSprite* arrow) 
{
	if(m_pArrow) 
	{
		m_pArrow->removeFromParent();
		m_pArrow = NULL;
	}
	m_pArrow = arrow;
	m_pArrow->setAnchorPoint(ccp(0.5f, 0.5f));
	m_pArrow->ignoreAnchorPointForPosition(false);
	addChild(m_pArrow, 1);
}
//-------------------------------------------------------------------------
void FKCW_UI_SimpleRookieGuiderLayer::setHint(CCNode* hint) 
{
	if(m_pHint) 
	{
		m_pHint->removeFromParent();
		m_pHint = NULL;
	}
	m_pHint = hint;
	m_pHint->setAnchorPoint(ccp(0.5f, 0.5f));
	m_pHint->ignoreAnchorPointForPosition(false);
	addChild(m_pHint, 2);
}
//-------------------------------------------------------------------------