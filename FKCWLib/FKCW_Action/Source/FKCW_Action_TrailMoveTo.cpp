//--------------------------------------------------------------------
#include "../Include/FKCW_Action_TrailMoveTo.h"
#include "../../FKCW_Shader/Include/FKCW_Shader_ShaderManager.h"
#include "../../FKCW_Base/Include/FKCW_Base_Utils.h"
#include "../../FKCW_RenderEx/Include/FKCW_RenderEx_PredrawArmature.h"
#include "../../FKCW_RenderEx/Include/FKCW_RenderEx_PredrawSprite.h"
//--------------------------------------------------------------------
FKCW_Action_TrailMoveTo::FKCW_Action_TrailMoveTo()
	: m_fDistance( 0 )
	, m_nVisibleArmIndex( -1 )
	, m_nAnimationIndex( -1 )
{

}
//--------------------------------------------------------------------
// 当动作结束，清理全部拖尾轨迹
void FKCW_Action_TrailMoveTo::CleanTrails()
{
	// 运动目标在父节点中的位置
	CCPoint finalPos = m_pTarget->getPosition();
	// 运动目标在父节点的源位置
	CCPoint origin = FKCW_Base_Utils::GetOrigin(m_pTarget);
	// 运动目标锚点位置
	CCPoint finalLocalPos = m_pTarget->getAnchorPointInPoints();

	// 其父节点
	CCNode* parent = m_pTarget->getParent();

	// 处理每一个拖尾轨迹
	CCObject* obj;
	CCARRAY_FOREACH(&m_vecTrails, obj) 
	{
		CCNode* trail = (CCNode*)obj;

		if(m_bIsRemoveWhenDone) 
		{
			RemoveTrail(trail);
		} else 
		{
			// 轨迹当前位置
			CCPoint pos = trail->getPosition();

			// 轨迹持续时间
			float distance = 0;
			if(m_eTrailMode == eTrailMode_Armature)
				distance = ccpLength(ccpSub(finalPos, pos));
			else
				distance = ccpLength(ccpSub(finalLocalPos, pos));
			float duration = distance / m_fSpeed;

			// 如果不是Armature,则向源移动
			if(m_eTrailMode != eTrailMode_Armature)
			{
				// 在父节点中的最终位置
				pos = ccpAdd(origin, pos);

				// 移动拖尾到目标父节点处
				trail->retain();
				trail->removeFromParent();
				trail->setPosition(pos);
				trail->setScaleX(m_pTarget->getScaleX());
				trail->setScaleY(m_pTarget->getScaleY());
				parent->addChild(trail);
				trail->release();
			}

			// 执行动作保证所有拖尾到达目标当前位置
			trail->runAction(CCSequence::createWithTwoActions(CCMoveTo::create(duration, finalPos),
				CCCallFuncO::create(this, callfuncO_selector(FKCW_Action_TrailMoveTo::RemoveTrail), trail)));
		}
	}

	// 清除拖尾列表
	m_vecTrails.removeAllObjects();
}
//--------------------------------------------------------------------
// 预渲染
void FKCW_Action_TrailMoveTo::OnPreDraw( CCObject* p_pSender )
{
	 FKCW_Shader_ShaderManager::SetLighting(m_tagTrailColorScale, m_tagTrailColor);
}
//--------------------------------------------------------------------
// 移除一个拖尾
void FKCW_Action_TrailMoveTo::RemoveTrail( CCNode* p_pTrail )
{
	if(m_eTrailMode == eTrailMode_Armature) 
	{
		((FKCW_RenderEx_PredrawArmature*)p_pTrail)->setPreDrawFunction(NULL);
	}
	p_pTrail->removeFromParent();
}
//--------------------------------------------------------------------
FKCW_Action_TrailMoveTo::~FKCW_Action_TrailMoveTo()
{

}
//--------------------------------------------------------------------
FKCW_Action_TrailMoveTo* FKCW_Action_TrailMoveTo::CreateWithSpriteFrameName(float duration, const CCPoint& position, const string& trailFrameName,
		float trailDistance, int trailSegments, ccColor3B trailColor, ccColor4B trailColorScale, bool removeWhenDone)
{
	FKCW_Action_TrailMoveTo* m = new FKCW_Action_TrailMoveTo();
	if(m->InitWithDurationAndSpriteTrail(duration, position, trailFrameName, trailDistance, trailSegments, trailColor, trailColorScale, removeWhenDone)) 
	{
		m->m_eTrailMode = eTrailMode_SpriteFrameName;
		return (FKCW_Action_TrailMoveTo*)m->autorelease();
	}
	CC_SAFE_RELEASE(m);
	return NULL;
}
//--------------------------------------------------------------------
FKCW_Action_TrailMoveTo* FKCW_Action_TrailMoveTo::CreateWithFileName(float duration, const CCPoint& position, const string& fileName,
		float trailDistance, int trailSegments, ccColor3B trailColor, ccColor4B trailColorScale, bool removeWhenDone)
{
	FKCW_Action_TrailMoveTo* m = new FKCW_Action_TrailMoveTo();
	if(m->InitWithDurationAndSpriteTrail(duration, position, fileName, trailDistance, trailSegments, trailColor, trailColorScale, removeWhenDone)) 
	{
		m->m_eTrailMode = eTrailMode_FileName;
		return (FKCW_Action_TrailMoveTo*)m->autorelease();
	}
	CC_SAFE_RELEASE(m);
	return NULL;
}
//--------------------------------------------------------------------
FKCW_Action_TrailMoveTo* FKCW_Action_TrailMoveTo::CreateWithArmature(float duration, const CCPoint& position, const string& armatureName, int animationIndex,
		float trailDistance, int trailSegments, ccColor3B trailColor, ccColor4B trailColorScale, bool removeWhenDone)
{
	FKCW_Action_TrailMoveTo* m = new FKCW_Action_TrailMoveTo();
	if(m->InitWithDurationAndArmatureTrail(duration, position, armatureName, 
		animationIndex, trailDistance, trailSegments, trailColor, trailColorScale, removeWhenDone)) 
	{
		m->m_eTrailMode = eTrailMode_Armature;
		return (FKCW_Action_TrailMoveTo*)m->autorelease();
	}
	CC_SAFE_RELEASE(m);
	return NULL;
}
//--------------------------------------------------------------------
FKCW_Action_TrailMoveTo* FKCW_Action_TrailMoveTo::CreateWithArmature(float duration, const CCPoint& position, const string& armatureName, const string& animationName,
		float trailDistance, int trailSegments, ccColor3B trailColor, ccColor4B trailColorScale, bool removeWhenDone)
{
	FKCW_Action_TrailMoveTo* m = new FKCW_Action_TrailMoveTo();
	if(m->InitWithDurationAndArmatureTrail(duration, position, armatureName, animationName,
		trailDistance, trailSegments, trailColor, trailColorScale, removeWhenDone)) 
	{
		m->m_eTrailMode = eTrailMode_Armature;
		return (FKCW_Action_TrailMoveTo*)m->autorelease();
	}
	CC_SAFE_RELEASE(m);
	return NULL;
}
//--------------------------------------------------------------------
bool FKCW_Action_TrailMoveTo::InitWithDurationAndSpriteTrail(float duration, const CCPoint& position, const string& spriteName,
	float trailDistance, int trailSegments, ccColor3B trailColor,ccColor4B trailColorScale, bool removeWhenDone)
{
	if(!CCMoveTo::initWithDuration(duration, position))
		return false;

	m_strSpriteName			= spriteName;
	m_fTrailDistance		= trailDistance;
	m_nTrailSegments		= trailSegments;
	m_tagTrailColor			= trailColor;
	m_tagTrailColorScale	= trailColorScale;
	m_fAlphaStep			= 1.0f / (m_nTrailSegments + 1);
	m_bIsRemoveWhenDone		= removeWhenDone;
	m_fSpeed				= ccpLength(ccpSub(m_endPosition, m_startPosition)) / m_fDuration;

	return true;
}
//--------------------------------------------------------------------
bool FKCW_Action_TrailMoveTo::InitWithDurationAndArmatureTrail(float duration, const CCPoint& position, const string& armatureName,
	const string& animationName, float trailDistance, int trailSegments,ccColor3B trailColor, ccColor4B trailColorScale, bool removeWhenDone)
{
	if(!CCMoveTo::initWithDuration(duration, position))
		return false;

	m_strSpriteName			= armatureName;
	m_strAnimationName		= animationName;
	m_fTrailDistance		= trailDistance;
	m_nTrailSegments		= trailSegments;
	m_tagTrailColor			= trailColor;
	m_tagTrailColorScale	= trailColorScale;
	m_fAlphaStep			= 1.0f / (m_nTrailSegments + 1);
	m_bIsRemoveWhenDone		= removeWhenDone;
	m_fSpeed				= ccpLength(ccpSub(m_endPosition, m_startPosition)) / m_fDuration;

	return true;
}
//--------------------------------------------------------------------
bool FKCW_Action_TrailMoveTo::InitWithDurationAndArmatureTrail(float duration, const CCPoint& position, const string& armatureName,
	int animationIndex, float trailDistance, int trailSegments,ccColor3B trailColor, ccColor4B trailColorScale, bool removeWhenDone)
{
	if(!CCMoveTo::initWithDuration(duration, position))
		return false;

	m_strSpriteName			= armatureName;
	m_nAnimationIndex		= animationIndex;
	m_fTrailDistance		= trailDistance;
	m_nTrailSegments		= trailSegments;
	m_tagTrailColor			= trailColor;
	m_tagTrailColorScale	= trailColorScale;
	m_fAlphaStep			= 1.0f / (m_nTrailSegments + 1);
	m_bIsRemoveWhenDone		= removeWhenDone;
	m_fSpeed				= ccpLength(ccpSub(m_endPosition, m_startPosition)) / m_fDuration;

	return true;
}
//--------------------------------------------------------------------
CCObject* FKCW_Action_TrailMoveTo::copyWithZone(CCZone* pZone)
{
	CCZone* pNewZone = NULL;
	FKCW_Action_TrailMoveTo* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject) 
	{
		pCopy = (FKCW_Action_TrailMoveTo*)(pZone->m_pCopyObject);
	} else {
		pCopy = new FKCW_Action_TrailMoveTo();
		pZone = pNewZone = new CCZone(pCopy);
	}

	CCMoveBy::copyWithZone(pZone);

	if(m_eTrailMode == eTrailMode_Armature) 
	{
		if(m_nAnimationIndex >= 0)
			pCopy->InitWithDurationAndArmatureTrail(m_fDuration, m_endPosition, m_strSpriteName, 
			m_nAnimationIndex, m_fTrailDistance, m_nTrailSegments, m_tagTrailColor, m_tagTrailColorScale, m_bIsRemoveWhenDone);
		else	   																
			pCopy->InitWithDurationAndArmatureTrail(m_fDuration, m_endPosition, m_strSpriteName,
			m_strAnimationName, m_fTrailDistance, m_nTrailSegments, m_tagTrailColor, m_tagTrailColorScale, m_bIsRemoveWhenDone);
	} 																			  
	else 																		  
	{																			  
	pCopy->InitWithDurationAndSpriteTrail(m_fDuration, m_endPosition, m_strSpriteName, 
		m_fTrailDistance, m_nTrailSegments, m_tagTrailColor, m_tagTrailColorScale, m_bIsRemoveWhenDone);
	}
	pCopy->m_eTrailMode = m_eTrailMode;

	CC_SAFE_DELETE(pNewZone);
	return pCopy;
}
//--------------------------------------------------------------------
void FKCW_Action_TrailMoveTo::startWithTarget(CCNode* pTarget)
{
	CCMoveTo::startWithTarget(pTarget);

	// 设置其他信息
	m_tagDirection		= ccpSub(m_endPosition, m_startPosition);
	m_tagDirection		= ccpNormalize(m_tagDirection);
	m_fDistance			= 0;
	m_nVisibleArmIndex	= -1;
	m_bIsEnd			= false;

	// 如果Armature模式，则创建Armature拖尾
	if(m_eTrailMode == eTrailMode_Armature) 
	{
		CCNode* parent = m_pTarget->getParent();
		int z = m_pTarget->getZOrder();
		while( m_vecTrails.count() < static_cast<unsigned int>(m_nTrailSegments) ) 
		{
			FKCW_RenderEx_PredrawArmature* trail = FKCW_RenderEx_PredrawArmature::Create(m_strSpriteName.c_str());
			if(m_nAnimationIndex >= 0)
				trail->getAnimation()->playWithIndex(m_nAnimationIndex);
			else
				trail->getAnimation()->play(m_strAnimationName.c_str());
			trail->setPreDrawFunction(CCCallFuncO::create(this, callfuncO_selector(FKCW_Action_TrailMoveTo::OnPreDraw), trail));
			trail->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderLighting ));
			trail->setAnchorPoint(m_pTarget->getAnchorPoint());
			trail->setPosition(m_pTarget->getPosition());
			trail->setScaleX(m_pTarget->getScaleX());
			trail->setScaleY(m_pTarget->getScaleY());
			trail->setVisible(false);

			// 添加到源目标中
			m_vecTrails.addObject(trail);
			parent->addChild(trail, z - m_vecTrails.count());
		}
	}
}
//--------------------------------------------------------------------
void FKCW_Action_TrailMoveTo::update(float time)
{
	CCPoint previousPos = m_pTarget->getPosition();
	CCMoveTo::update(time);
	CCPoint currentPos = m_pTarget->getPosition();

	// 创建拖尾轨迹
	if( m_eTrailMode == eTrailMode_Armature ) 
	{
		if(m_nVisibleArmIndex < (int)m_vecTrails.count() - 1) 
		{
			float d = ccpLength(ccpSub(currentPos, previousPos));
			m_fDistance += d;

			while(m_nVisibleArmIndex < (int)m_vecTrails.count() - 1 && m_fDistance >= m_fTrailDistance) 
			{
				m_nVisibleArmIndex++;
				FKCW_RenderEx_PredrawArmature* trail = (FKCW_RenderEx_PredrawArmature*)m_vecTrails.objectAtIndex(m_nVisibleArmIndex);
				trail->setVisible(true);

				// 相对位置
				m_fDistance -= m_fTrailDistance;
			}
		}
	} 
	else 
	{
		if(m_vecTrails.count() < static_cast<unsigned int>(m_nTrailSegments)) 
		{
			float d = ccpLength(ccpSub(currentPos, previousPos));
			m_fDistance += d;
			while(m_vecTrails.count() < static_cast<unsigned int>(m_nTrailSegments) && m_fDistance >= m_fTrailDistance) 
			{
				FKCW_RenderEx_PredrawSprite* trail = NULL;
				if(m_eTrailMode == eTrailMode_SpriteFrameName)
					trail = FKCW_RenderEx_PredrawSprite::createWithSpriteFrameName(m_strSpriteName.c_str());
				else
					trail = FKCW_RenderEx_PredrawSprite::create(m_strSpriteName.c_str());
				trail->setPreDrawFunction(CCCallFuncO::create(this, callfuncO_selector(FKCW_Action_TrailMoveTo::OnPreDraw), trail));
				trail->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderLighting ));
				trail->setAnchorPoint(m_pTarget->getAnchorPoint());
				trail->setPosition(m_pTarget->getAnchorPointInPoints());

				// 添加到源对象上
				m_vecTrails.addObject(trail);
				int z = static_cast<int>(m_vecTrails.count());
				m_pTarget->addChild(trail, -z);

				// 相对距离
				m_fDistance -= m_fTrailDistance;
			}
		}
	}

	// 更新拖尾轨迹距离和alpha值
	CCObject* obj;
	int index = 0;
	int alpha = 255;
	CCPoint previousTrailPos = m_pTarget->getAnchorPointInPoints();
	if(m_eTrailMode == eTrailMode_Armature)
		previousTrailPos = m_pTarget->getPosition();
	CCARRAY_FOREACH(&m_vecTrails, obj) 
	{
		CCNodeRGBA* trail = dynamic_cast<CCNodeRGBA*>(obj);
		if(!trail || !trail->isVisible())
			continue;

		// 设置位置
		CCPoint pos = m_tagDirection * -m_fTrailDistance;
		pos = ccpAdd(pos, previousTrailPos);
		trail->setPosition(pos);
		previousTrailPos = pos;

		// 设置Alpha
		alpha -= static_cast<int>(m_fAlphaStep * 255.0f);
		trail->setOpacity(MAX(0, alpha));

		index++;
	}

	// 如果移动动作结束，则清理轨迹
	if(time >= 1 && !m_bIsEnd) 
	{
		m_bIsEnd = true;
		CleanTrails();
	}
}
//--------------------------------------------------------------------