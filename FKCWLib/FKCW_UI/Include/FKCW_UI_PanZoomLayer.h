//*************************************************************************
//	创建日期:	2014-11-11
//	文件名称:	FKCW_UI_PanZoomLayer.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_UI_Macros.h"
//-------------------------------------------------------------------------
// 缩放手势识别延迟时间
#define kCCLayerPanZoomMultitouchGesturesDetectionDelay 0.5
//-------------------------------------------------------------------------
// Layer两种类型
enum ENUM_LayerPanZoomMode
{
	eLayerPanZoomModeSheet,		// 默认模式
	eLayerPanZoomModeFrame,		// 帧模式（例如拖动内部对象）
};
// 帧模式下的边缘
enum ENUM_LayerPanZoomFrameEdge
{
	eLayerPanZoomFrameEdgeNone,
	eLayerPanZoomFrameEdgeTop,
	eLayerPanZoomFrameEdgeBottom,
	eLayerPanZoomFrameEdgeLeft,
	eLayerPanZoomFrameEdgeRight,
	eLayerPanZoomFrameEdgeTopLeft,
	eLayerPanZoomFrameEdgeBottomLeft,
	eLayerPanZoomFrameEdgeTopRight,
	eLayerPanZoomFrameEdgeBottomRight,
};
//-------------------------------------------------------------------------
class FKCW_UI_LayerPanZoom : public CCLayer
{
public:
	virtual bool init();  
	CREATE_FUNC(FKCW_UI_LayerPanZoom);
public:
	CC_SYNTHESIZE(float, m_fMaxTouchDistanceToClick, MaxTouchDistanceToClick);
	CC_SYNTHESIZE(CCArray*, m_pTouches, Touches);
	CC_SYNTHESIZE(float, m_fTouchDistance, TouchDistance);
	CC_SYNTHESIZE(float, m_fMinSpeed, MinSpeed);
	CC_SYNTHESIZE(float, m_fMaxSpeed, MaxSpeed);
	CC_SYNTHESIZE(float, m_fTopFrameMargin, TopFrameMargin);
	CC_SYNTHESIZE(float, m_fBottomFrameMargin, BottomFrameMargin);
	CC_SYNTHESIZE(float, m_fLeftFrameMargin, LeftFrameMargin);
	CC_SYNTHESIZE(float, m_fRightFrameMargin, RightFrameMargin);
	CC_SYNTHESIZE(float, m_fRubberEffectRecoveryTime, RubberEffectRecoveryTime);

	CCRect						m_tagPanBoundsRect;
	float						m_fMaxScale;
	float						m_fMinScale;
	ENUM_LayerPanZoomMode		m_eMode;
	CCPoint						m_tagPrevSingleTouchPositionInLayer; 
	double						m_dSingleTouchTimestamp;		// 单点触碰的时间，用以判断多点触控与否
	bool						m_bTouchMoveBegan;				// 每次单点触控时间仅会修改本标示一次，
																// 本标志用于调用touchMoveBeganAtPosition函数
	float						m_fRubberEffectRatio;			// 橡胶拉伸效果参数
	bool						m_bRubberEffectRecovering;
	bool						m_bRubberEffectZooming;

	void						ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	void						ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	void						ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	void						ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void				update(float dt);
	void						onEnter();
	void						onExit();

	void						setMaxScale(float maxScale);
	float						getMaxScale();
	void						setMinScale(float minScale);
	float						getMinScale(); 
	void						setRubberEffectRatio(float rubberEffectRatio);
	float						getRubberEffectRatio();
	void						setPanBoundsRect(CCRect m_tagRect);
	void						setPosition(CCPoint m_tagPosition);
	void						setScale(float m_fScale);
	void						recoverPositionAndScale();
	void						recoverEnded();

	float						TopEdgeDistance();
	float						LeftEdgeDistance();
	float						BottomEdgeDistance();    
	float						RightEdgeDistance();
	float						MinPossibleScale();
	ENUM_LayerPanZoomFrameEdge	FrameEdgeWithPoint( cocos2d::CCPoint point);
	float						HorSpeedWithPosition(CCPoint pos);
	float						VertSpeedWithPosition(CCPoint pos);
};