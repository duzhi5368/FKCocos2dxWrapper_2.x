//*************************************************************************
//	创建日期:	2014-11-6
//	文件名称:	FKCW_UI_RookieGuideLayer.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_UI_Macros.h"
//-------------------------------------------------------------------------
// 新手引导层
// 该类提供了一个基本的新手指南的框架。我们应当继承它并为其添加视觉元素。
// 本类负责触摸时间和一般的UI渲染。
// 另外，本类将会将预设的可点击区域设为透明。
class FKCW_UI_RookieGuideLayer : public CCLayer
{
protected:
	struct SRegion
	{
		CCRect			m_tagRect;				// 可点击区域
		CCCallFunc*		m_pFunc;				// 区域触摸回调函数，注意，若函数为NULL，则该区域不可被点击
		bool			m_pRemoveOnTouch;		// 是否触碰后删除本区域（当回调函数不为空时，本值无效 ）
	public:
		SRegion()
		{
			m_tagRect = CCRectZero;
			m_pFunc = NULL;
			m_pRemoveOnTouch = false;
		}
	};
	typedef vector<SRegion>		RegionList;
protected:
	SRegion*			m_pClickedRegion;									// 用户刚点击过的区域
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(RegionList, m_vecRegions, Regions);	// 可点击区域列表
	CC_SYNTHESIZE_PASS_BY_REF_SETTER(ccColor4B, m_tagBgColor, BgColor);		// 背景颜色
	CC_SYNTHESIZE_BOOL(m_bIsAnyTouchMode, AnyTouchMode);					// 任意触摸模式（true则表示触摸任意地方都可以结束该新手引导）
	CC_SYNTHESIZE_BOOL(m_bIsShouldCheckRegion, ShouldCheckRegion);			// 无触摸模式（true则表示没有任何区域接受点击检查，层会抛弃全部触摸事件）
	CC_SYNTHESIZE(CCClippingNode*, m_pClipper, Clipper);					// 遮罩层
	CC_SYNTHESIZE(CCLayerColor*, m_pContent, Content);						// 内容层
	CC_SYNTHESIZE(CCDrawNode*, m_pStencil, Stencil);						// 深度节点
	CC_SYNTHESIZE_RETAIN(CCCallFunc*, m_pOnExitFunc, OnExitFunc);			// 进入本层的回调函数
	CC_SYNTHESIZE_RETAIN(CCCallFunc*, m_pOnEnterFunc, OnEnterFunc);			// 离开本层的回调函数
protected:
	FKCW_UI_RookieGuideLayer();

	// 当淡入显示后会被调用，子类可以复写该函数并执行自定义行为
	virtual void	_OnFadeInDone();
public:
	virtual ~FKCW_UI_RookieGuideLayer();
	static FKCW_UI_RookieGuideLayer* Create();

	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

	// 淡入显示整个新手引导层
	// 注：该函数需要在设置背景色之后调用
	// 参数：duration 淡入持续时间  delay 淡入动作之前的延迟时间
	virtual void	FadeIn( float duration, float delay = 0 );
	// 添加一个可点击区域
	// 注: 该区域应当是世界空间坐标。
	//		若该区域可点击，则当它被点击之后，新手引导层将自动删除它。
	// 参数：r 世界空间坐标，即响应区域
	//		func 区域被点击后的回调函数。若该值为NULL，则区域为不可点击
	//		removeOnTouch 当回调函数不为NULL时，本值无效。当回调函数为NULL时，该值若为true，
	//				表示，当区域被点击，则移除本区域。
	 void			AddRegion(const CCRect& r, CCCallFunc* func, bool removeOnTouch = false);
	 // 通过一个节点添加一个可点击区域
	 // 注：该区域大小将与节点的包围盒矩形相同
	 void			AddRegion(CCNode* n, CCCallFunc* func, bool removeOnTouch = false);
	 // 设置一个区域触摸后移除（若其有触摸回调，将删除该回调）
	 void			SetRegionRemoveOnTouch(int index);
	 // 获取指定区域矩形
	 const CCRect&	GetRegionRect(int index);
	 // 无效化一个区域，直到超过指定时间
	 void			EnableRegionCheckAfter(float seconds);
};