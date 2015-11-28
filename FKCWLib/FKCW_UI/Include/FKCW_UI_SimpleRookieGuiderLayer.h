//*************************************************************************
//	创建日期:	2014-11-6
//	文件名称:	FKCW_UI_SimpleRookieGuiderLayer.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_UI_RookieGuideLayer.h"
#include "FKCW_UI_Macros.h"
//-------------------------------------------------------------------------
// 一个简单的自定义新手引导层，仅显示一个标签和箭头
class FKCW_UI_SimpleRookieGuiderLayer : public FKCW_UI_RookieGuideLayer
{
private:
	CC_SYNTHESIZE_SETTER(CCSprite*, m_pArrow, Arrow);		// 箭头精灵
	CC_SYNTHESIZE_SETTER(CCNode*, m_pHint, Hint);			
protected:
	FKCW_UI_SimpleRookieGuiderLayer();
public:
	virtual ~FKCW_UI_SimpleRookieGuiderLayer();
	static FKCW_UI_SimpleRookieGuiderLayer* Create();
	// 覆盖父类函数
	virtual void	FadeIn( float duration, float delay = 0  );
	// 将箭头对准的区域中心，有特定的角度和距离
	// 参数：regionIndex 区域的索引 distance 区域中心和箭头精灵中心的举例 degree 区域中心指向箭头中间的角度向量（正值为逆时针）
	//		arrowPresetRotation 箭头自身的旋转角度（正值为逆时针）
	void			PointToRegionCenter(int regionIndex, float distance, float degree, float arrowPresetRotation = 0);
	// 计算提示信息位置（相对于箭头的坐标）
	void			ShiftHint(float dx, float dy);
};