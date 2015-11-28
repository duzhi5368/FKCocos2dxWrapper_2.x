/********************************************************************
	created:	2014/11/03
	file base:	FKCW_Action_TrailMoveTo
	author:		FreeKnight
	
	purpose:	带移动轨迹的移动动作
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_Action_Macros.h"
//--------------------------------------------------------------------
// 对象移动，并在其身后留下移动轨迹的动作
// 注意：当移动对象是CCArmature时，拖尾轨迹不会添加到主Node中，请保证预留了足够的Z深度值
class FKCW_Action_TrailMoveTo : public CCMoveTo
{
private:
	enum ENUM_TrailMode
	{
		eTrailMode_SpriteFrameName,
		eTrailMode_FileName,
		eTrailMode_Armature,
	};
protected:
	CCArray			m_vecTrails;			// 轨迹
	float			m_fAlphaStep;			// 每步的Alpha
	CCPoint			m_tagDirection;			// 移动方向
	float			m_fSpeed;				// 移动速度
	float			m_fDistance;			// 距离
	bool			m_bIsEnd;				// 是否动作已结束
	ENUM_TrailMode	m_eTrailMode;			// 拖尾方式
	string			m_strSpriteName;		// 拖尾使用的精灵明/图片名/帧名
	int				m_nVisibleArmIndex;		// 当前可见的Armature拖尾索引

	CC_SYNTHESIZE(float, m_fTrailDistance, trailDistance);						// 拖尾间隔距离
	CC_SYNTHESIZE(int, m_nTrailSegments, TrailSegments);						// 拖尾轨迹个数
	CC_SYNTHESIZE_PASS_BY_REF(ccColor3B, m_tagTrailColor, TrailColor);			// 拖尾轨迹颜色
	CC_SYNTHESIZE_PASS_BY_REF(ccColor4B, m_tagTrailColorScale, TrailColorScale);// 拖尾轨迹混合颜色
	CC_SYNTHESIZE(int, m_nAnimationIndex, AnimationIndex);						// 动画索引（若使用动画做轨迹）
	CC_SYNTHESIZE_PASS_BY_REF(string, m_strAnimationName, AnimationName);		// 动画名（若使用动画做轨迹）
	CC_SYNTHESIZE_BOOL(m_bIsRemoveWhenDone, RemoveWhenDone);					// 当该值为true则表示当本移动动作完成时，拖尾将清除
protected:
	FKCW_Action_TrailMoveTo();
	// 当动作结束，清理全部拖尾轨迹
	void			CleanTrails();
	// 预渲染
	void			OnPreDraw( CCObject* p_pSender );
	// 移除一个拖尾
	void			RemoveTrail( CCNode* p_pTrail );
public:
	virtual ~FKCW_Action_TrailMoveTo();

	// 创建一个拖尾轨迹动作实例
	/* 参数：
		duration		持续时间
		position		目标地点
		trailFrameName	在资源集中的拖尾精灵帧名
		trailDistance	各个拖尾轨迹之间的间隔距离
		trailSegments	拖尾轨迹段个数
		trailColor		拖尾轨迹颜色，默认白色表示无颜色变化
		trailColorScale	拖尾轨迹叠加混合颜色，默认白色表示无颜色变化
		removeWhenDone	当该值为true则表示当本移动动作完成时，拖尾将清除
	*/
	static FKCW_Action_TrailMoveTo* CreateWithSpriteFrameName(float duration, const CCPoint& position, const string& trailFrameName,
		float trailDistance, int trailSegments, ccColor3B trailColor = ccWHITE, ccColor4B trailColorScale = cc4BLACK, bool removeWhenDone = false);

	static FKCW_Action_TrailMoveTo* CreateWithFileName(float duration, const CCPoint& position, const string& fileName,
		float trailDistance, int trailSegments, ccColor3B trailColor = ccWHITE, ccColor4B trailColorScale = cc4BLACK, bool removeWhenDone = false);

	static FKCW_Action_TrailMoveTo* CreateWithArmature(float duration, const CCPoint& position, const string& armatureName, int animationIndex,
		float trailDistance, int trailSegments, ccColor3B trailColor = ccWHITE, ccColor4B trailColorScale = cc4BLACK, bool removeWhenDone = false);

	static FKCW_Action_TrailMoveTo* CreateWithArmature(float duration, const CCPoint& position, const string& armatureName, const string& animationName,
		float trailDistance, int trailSegments, ccColor3B trailColor = ccWHITE, ccColor4B trailColorScale = cc4BLACK, bool removeWhenDone = false);


	bool InitWithDurationAndSpriteTrail(float duration, const CCPoint& position, const string& spriteName,
		float trailDistance, int trailSegments, ccColor3B trailColor,
		ccColor4B trailColorScale, bool removeWhenDone);

	bool InitWithDurationAndArmatureTrail(float duration, const CCPoint& position, const string& armatureName,
		const string& animationName, float trailDistance, int trailSegments,
		ccColor3B trailColor, ccColor4B trailColorScale, bool removeWhenDone);

	bool InitWithDurationAndArmatureTrail(float duration, const CCPoint& position, const string& armatureName,
		int animationIndex, float trailDistance, int trailSegments,
		ccColor3B trailColor, ccColor4B trailColorScale, bool removeWhenDone);

	virtual CCObject* copyWithZone(CCZone* pZone);
	virtual void startWithTarget(CCNode* pTarget);
	virtual void update(float time);
};
//--------------------------------------------------------------------