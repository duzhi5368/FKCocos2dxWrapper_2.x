//-------------------------------------------------------------------------
#include "../Include/FKCW_UI_RookieGuideLayer.h"
#include "../../FKCW_Base/Include/FKCW_Base_ColorConvert.h"
#include "../../FKCW_Base/Include/FKCW_Base_Utils.h"
//-------------------------------------------------------------------------
// 监察动作tag
#define TAG_ENABLE_REGIN_CHECK		1000
//-------------------------------------------------------------------------
FKCW_UI_RookieGuideLayer::FKCW_UI_RookieGuideLayer()
	: m_pClickedRegion( NULL )
	, m_tagBgColor( ccc4(0, 0, 0, 0x7f ))		// 默认灰色
	, m_bIsAnyTouchMode( false )
	, m_bIsShouldCheckRegion( true )
	, m_pStencil( NULL )
	, m_pContent( NULL )
	, m_pClipper( NULL )
	, m_pOnExitFunc( NULL )
	, m_pOnEnterFunc( NULL )
{

}
//-------------------------------------------------------------------------
// 当淡入显示后会被调用，子类可以复写该函数并执行自定义行为
void FKCW_UI_RookieGuideLayer::_OnFadeInDone()
{
	// 不做任何事，交由子类实现就好
}
//-------------------------------------------------------------------------
FKCW_UI_RookieGuideLayer::~FKCW_UI_RookieGuideLayer()
{
	for( RegionList::iterator Ite = m_vecRegions.begin();
		Ite != m_vecRegions.end(); Ite++ )
	{
		SRegion& r = *Ite;
		CC_SAFE_RELEASE( r.m_pFunc );
	}
	CC_SAFE_RELEASE( m_pOnEnterFunc );
	CC_SAFE_RELEASE( m_pOnExitFunc );
}
//-------------------------------------------------------------------------
FKCW_UI_RookieGuideLayer* FKCW_UI_RookieGuideLayer::Create()
{
	FKCW_UI_RookieGuideLayer* l = new FKCW_UI_RookieGuideLayer();
	l->init();
	return (FKCW_UI_RookieGuideLayer*)l->autorelease();
}
//-------------------------------------------------------------------------
bool FKCW_UI_RookieGuideLayer::init()
{
	if(!CCLayer::init()) 
	{
		return false;
	}

	// 深度
	m_pStencil = CCDrawNode::create();
	m_pStencil->setPosition(CCPointZero);

	// 剪裁遮罩
	m_pClipper = CCClippingNode::create();
	m_pClipper->setPosition(CCPointZero);
	m_pClipper->setStencil(m_pStencil);
	m_pClipper->setInverted(true);
	addChild(m_pClipper);

	// 背景颜色层
	m_pContent = CCLayerColor::create(m_tagBgColor);
	m_pClipper->addChild(m_pContent);

	// 开启消息接收，并设置消息接收层级为最高
	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);
	setTouchPriority(-MAX_INT);
	setKeypadEnabled(true);

	return true;
}
//-------------------------------------------------------------------------
void FKCW_UI_RookieGuideLayer::onEnter()
{
	CCLayer::onEnter();

	if(m_pOnEnterFunc)
		m_pOnEnterFunc->execute();
}
//-------------------------------------------------------------------------
void FKCW_UI_RookieGuideLayer::onExit()
{
	if(m_pOnExitFunc)
		m_pOnExitFunc->execute();

	CCLayer::onExit();
}
//-------------------------------------------------------------------------
bool FKCW_UI_RookieGuideLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint loc = pTouch->getLocation();

	// 如果触摸了任何区域, 释放该消息
	if(!m_bIsAnyTouchMode && m_bIsShouldCheckRegion) 
	{
		m_pClickedRegion = NULL;
		for(RegionList::iterator iter = m_vecRegions.begin(); iter != m_vecRegions.end(); iter++) 
		{
			SRegion& r = *iter;
			if(r.m_tagRect.containsPoint(loc)) 
			{
				if(r.m_pFunc) 
				{
					m_pClickedRegion = &r;
					break;
				} 
				else if(r.m_pRemoveOnTouch)
				{
					removeFromParent();
					return false;
				}
			}
		}
	}

	return true;
}
//-------------------------------------------------------------------------
void FKCW_UI_RookieGuideLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{

}
//-------------------------------------------------------------------------
void FKCW_UI_RookieGuideLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	if(m_bIsAnyTouchMode && m_bIsShouldCheckRegion) 
	{
		removeFromParent();
	} 
	else if(m_pClickedRegion) 
	{
		m_pClickedRegion->m_pFunc->execute();
		removeFromParent();
	}
}
//-------------------------------------------------------------------------
void FKCW_UI_RookieGuideLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{

}
//-------------------------------------------------------------------------
// 淡入显示整个新手引导层
// 注：该函数需要在设置背景色之后调用
// 参数：duration 淡入持续时间  delay 淡入动作之前的延迟时间
void FKCW_UI_RookieGuideLayer::FadeIn( float duration, float delay )
{
	FKCW_Base_Utils::SetOpacityRecursively(this, 0);
	m_pContent->runAction(CCSequence::create(CCDelayTime::create(delay),
		CCFadeTo::create(duration, m_tagBgColor.a),
		CCCallFunc::create(this, callfunc_selector(FKCW_UI_RookieGuideLayer::_OnFadeInDone)),
		NULL));
}
//-------------------------------------------------------------------------
// 添加一个可点击区域
// 注: 该区域应当是世界空间坐标。
//		若该区域可点击，则当它被点击之后，新手引导层将自动删除它。
// 参数：r 世界空间坐标，即响应区域
//		func 区域被点击后的回调函数。若该值为NULL，则区域为不可点击
//		removeOnTouch 当回调函数不为NULL时，本值无效。当回调函数为NULL时，该值若为true，
//				表示，当区域被点击，则移除本区域。
void FKCW_UI_RookieGuideLayer::AddRegion(const CCRect& r, CCCallFunc* func, bool removeOnTouch )
{
	// 添加区域
	SRegion tagRegion;
	tagRegion.m_pFunc = func;
	tagRegion.m_pRemoveOnTouch = removeOnTouch;
	tagRegion.m_tagRect = r;
	CC_SAFE_RETAIN(func);
	m_vecRegions.push_back(tagRegion);

	// 更新深度
	CCPoint v[] =
	{
		r.origin,
		ccpAdd(r.origin, ccp(r.size.width, 0)),
		ccpAdd(r.origin, ccp(r.size.width, r.size.height)),
		ccpAdd(r.origin, ccp(0, r.size.height))
	};
	m_pStencil->drawPolygon(v, 4, cc4fGREEN, 0, cc4fTRANSPARENT);
}
//-------------------------------------------------------------------------
// 通过一个节点添加一个可点击区域
// 注：该区域大小将与节点的包围盒矩形相同
void FKCW_UI_RookieGuideLayer::AddRegion(CCNode* n, CCCallFunc* func, bool removeOnTouch )
{
	CCRect b = CCRectMake(0, 0, n->getContentSize().width, n->getContentSize().height);
	CCAffineTransform t = n->nodeToWorldTransform();
	b = CCRectApplyAffineTransform(b, t);
	AddRegion(b, func, removeOnTouch);
}
//-------------------------------------------------------------------------
// 设置一个区域触摸后移除（若其有触摸回调，将删除该回调）
void FKCW_UI_RookieGuideLayer::SetRegionRemoveOnTouch(int index)
{
	if(index < 0 || index >= static_cast<int>(m_vecRegions.size()))
		return;

	SRegion& reg = m_vecRegions.at(index);
	reg.m_pRemoveOnTouch = true;
	CC_SAFE_RELEASE(reg.m_pFunc);
}
//-------------------------------------------------------------------------
// 获取指定区域矩形
const CCRect& FKCW_UI_RookieGuideLayer::GetRegionRect(int index)
{
	if(index < 0 || index >= static_cast<int>(m_vecRegions.size()))
		return CCRectZero;

	SRegion& reg = m_vecRegions.at(index);
	return reg.m_tagRect;
}
//-------------------------------------------------------------------------
// 无效化一个区域，直到超过指定时间
void FKCW_UI_RookieGuideLayer::EnableRegionCheckAfter(float seconds)
{
	// 停止之前的一个动作
	stopActionByTag(TAG_ENABLE_REGIN_CHECK);

	// 执行新动作任务
	setShouldCheckRegion(false);
	CCAction* a = CCSequence::createWithTwoActions(CCDelayTime::create(seconds),
		CCCallFunc::create(this, callfunc_selector(FKCW_UI_RookieGuideLayer::markShouldCheckRegion)));
	a->setTag(TAG_ENABLE_REGIN_CHECK);
	runAction(a);
}
//-------------------------------------------------------------------------
void FKCW_UI_RookieGuideLayer::setBgColor( const ccColor4B& var )
{
	m_tagBgColor = var;
	m_pContent->setColor(ccc3(m_tagBgColor.r, m_tagBgColor.g, m_tagBgColor.b));
	m_pContent->setOpacity(m_tagBgColor.a);
}
//-------------------------------------------------------------------------