//*************************************************************************
//	创建日期:	2014-11-22
//	文件名称:	FKCW_RenderEx_ZoomControllerLayer.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_RenderEx_Macros.h"
//-------------------------------------------------------------------------
#define MaxZoomControllerLayerHistoryCount 12
//-------------------------------------------------------------------------
struct CCPanZoomTimePointStamp
{
	float time;
	CCPoint pt;
};
//-------------------------------------------------------------------------
class FKCW_RenderEx_ZoomControllerLayer : public CCLayer 
{
public:
	FKCW_RenderEx_ZoomControllerLayer();
	~FKCW_RenderEx_ZoomControllerLayer();

	// 创建一个缩放层，其挂接的节点将支持缩放和滚动
	static FKCW_RenderEx_ZoomControllerLayer* controllerWithNode(CCNode* node);
	bool initWithNode(CCNode* node);

	virtual void registerWithTouchDispatcher(void);
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);

public:
	bool      centerOnPinch;			/*!< Should zoom center on pinch pts, default is YES */
	bool      zoomOnDoubleTap;			/*!< Should we zoom in/out on double-tap */
	float     zoomRate;					/*!< How much to zoom based on movement of pinch */
	float     zoomInLimit;				/*!< The smallest zoom level */
	float     zoomOutLimit;				/*!< The hightest zoom level */
	float     swipeVelocityMultiplier;	/*!< The velocity factor of the swipe's scroll action */
	float     scrollDuration;			/*!< Duration of the scroll action after a swipe */
	float     scrollDamping;			/*!< When scrolling around, this will dampen the movement */
	float     pinchDamping;				/*!< When zooming, this will dampen the zoom */
	float     pinchDistanceThreshold;	/*!< The distance moved before a pinch is recognized */
	float     doubleTapZoomDuration;	/*!< Duration of zoom after double-tap */

public:
	// 允许滑动的最大范围
	void	setBoundingRect(CCRect rect);
	CCRect	getBoundingRect();
	// 用户自己渲染窗口大小（默认情况下是CCDirector的winSize大小）
	void	setWindowRect(CCRect rect);
	CCRect	getWindowRect();
	// 获取当前最佳缩放限制比
	float	getOptimalZoomOutLimit();
	// 聚焦到指定点
	void	centerOnPoint(CCPoint pt, float damping);
	// 移动到指定位置
	void	updatePosition(CCPoint pos);
	// 设置指定点为视图窗口中心点
	void	centerOnPoint(CCPoint pt);
	// 设置指定点为视图窗口中心点，并指定该行为时间
	void	centerOnPoint(CCPoint pt, float duration, float rate);
	// 以指定位为中心点缩小指定时间
	void	zoomInOnPoint(CCPoint pt, float duration);
	// 以指定位为中心点放大指定时间
	void	zoomOutOnPoint(CCPoint pt, float duration);
	// 以指定位为中心点缩放到指定大小
	void	zoomOnPoint(CCPoint pt, float duration, float scale);
	// 开启touch消息获取
	void	enableWithTouchPriority(int priority, bool swallowsTouches);
	// 关闭touch消息获取
	void	disable();
private:
	CCNode*			_node;			// 对象
	CCPoint			_tr;			// 对象包围区
	CCPoint			_bl;			// 对象包围区
	CCPoint			_winTr;			// 窗口大小
	CCPoint			_winBl;			// 窗口大小
	CCPoint			_firstTouch;	// 首次触摸点
	float			_firstLength;	// 移动长度
	float			_oldScale;		// 之前缩放率
	CCDictionary*	_touchesDic;	// 保存触点队列
	// internals
	float			_time;
	int				_timePointStampCounter;
	CCPanZoomTimePointStamp _history[MaxZoomControllerLayerHistoryCount];
	CCAction*		_lastScrollAction;
private:
	void			updateTime(float dt);
	CCPoint			boundPos(CCPoint pos);
	void			recordScrollPoint(CCTouch* touch);
	CCPoint			getHistoricSpeed();

	void			handleDoubleTapAt(CCPoint pt);

	void			beginScroll(CCPoint pos);
	void			moveScroll(CCPoint pos);
	void			endScroll(CCPoint pos);

	void			beginZoom(CCPoint pt, CCPoint pt2);
	void			moveZoom(CCPoint pt, CCPoint pt2);
	void			endZoom(CCPoint pt, CCPoint pt2);
};
//-------------------------------------------------------------------------
// 特殊的缩放动作，保证视图中心聚焦在一个给顶点
class FKCW_RenderEx_ZoomControllerLayerScaleAction : public CCScaleTo 
{
public:
	FKCW_RenderEx_ZoomControllerLayerScaleAction();
	~FKCW_RenderEx_ZoomControllerLayerScaleAction();
	static FKCW_RenderEx_ZoomControllerLayerScaleAction* actionWithDuration(float duration, float s, FKCW_RenderEx_ZoomControllerLayer* controller, CCPoint pt);
	bool initWithDuration(float duration, float s, FKCW_RenderEx_ZoomControllerLayer* controller, CCPoint pt);

	void update(float t);
public:
	FKCW_RenderEx_ZoomControllerLayer*	m_pControllerLayer;
	CCPoint								m_tagCenterPoint;
};
//-------------------------------------------------------------------------