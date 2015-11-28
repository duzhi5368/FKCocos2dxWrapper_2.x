//-------------------------------------------------------------------------
#include "FKAnimateEx.h"
//-------------------------------------------------------------------------
FKAnimateEx::FKAnimateEx()
	: m_bIsResValid( false )
{
	m_szCurAniName.clear();
	Reset();
}
//-------------------------------------------------------------------------
FKAnimateEx::~FKAnimateEx()
{
	CC_SAFE_RELEASE( m_pAniRes );
}
//-------------------------------------------------------------------------
// 重置
void FKAnimateEx::Reset()
{
	m_fTime				= 0.0f;
	m_fSpeed			= 1.0f;
	m_nLoopTime			= -1;
	m_nNextFrame		= 0;
	m_bIsNewLoop		= true;
	m_pTarget			= NULL;
	m_eState			= eAnimState_Normal;
	m_pCurAnimation		= NULL;
	m_pAniCallback		= NULL;
	m_pAniCallbackContext = NULL;

	m_vecAniCallbacks.clear();
	m_vecFrameTimes.clear();

	CC_SAFE_RELEASE( m_pAniRes );
}
//-------------------------------------------------------------------------
// 设置动画资源
void FKAnimateEx::SetAnimateExRes( FKAnimateExRes* pAniRes )
{
	CC_SAFE_RELEASE( m_pAniRes );

	m_pAniRes = pAniRes;
	if( m_pAniRes )
		m_bIsResValid = m_pAniRes->IsValid();
}
//-------------------------------------------------------------------------
// 设置目标对象
void FKAnimateEx::SetTargetNode( CCNode* pTarget )
{
	m_pTarget = pTarget;
}
//-------------------------------------------------------------------------
// 帧更新
void FKAnimateEx::Update( float dt )
{
	if( m_pAniRes == NULL )
		return;
	if( !m_pAniRes->IsValid() )
		return;
	if( m_pAniRes->IsValid() && !m_bIsResValid )
	{
		Play( m_szCurAniName, m_fSpeed, m_nLoopTime );
		m_bIsResValid = true;
	}

	if( m_pCurAnimation == NULL )
	{
		set<string>& AniNameSet = m_pAniRes->GetAniNameList();
		if( AniNameSet.empty() )
			return;

		set<string>::iterator IteBegin = AniNameSet.begin();
		Play( *IteBegin, m_fSpeed, m_nLoopTime );
		if( m_pCurAnimation == NULL )
			return;

		m_szCurAniName = (*IteBegin);
	}

	switch( m_eState )
	{
	case eAnimState_Normal:
		{
			float fDuration = m_pCurAnimation->getDuration();
			CCArray* pFrames = m_pCurAnimation->getFrames();
			int nFrames = static_cast<int>( pFrames->count() );
			float fLastTime = m_fTime;
			m_fTime += dt * m_fSpeed;
			
			if( m_fTime >= fDuration )
			{
				// 循环播放
				if( m_nLoopTime == -1 )
				{
					m_nNextFrame = 0;
					m_fTime = 0.0f;
				}
				else
				{
					m_nNextFrame = 0;
					m_fTime = 0.0f;
					m_nLoopTime--;
					if( m_nLoopTime == 0 )
					{
						m_eState = eAnimState_Stop;
						// 动作回调
						_OnAniCallback( fLastTime, m_fTime, fDuration );
						return;
					}
				}
			}

			CCSpriteFrame* pFrameToDisplay = NULL;
			for( int i = m_nNextFrame; i < nFrames; ++i )
			{
				float fSplitTime = m_vecFrameTimes.at( i );
				if( fSplitTime <= m_fTime )
				{
					CCAnimationFrame* pFrame = (CCAnimationFrame*)pFrames->objectAtIndex(i);
					pFrameToDisplay = pFrame->getSpriteFrame();
					((CCSprite*)m_pTarget)->setDisplayFrame( pFrameToDisplay );

					CCDictionary* pDict = pFrame->getUserInfo();
					if( pDict )
					{
						// 动作播放回调
					}
					m_nNextFrame = i + 1;
					break;
				}
			}

			// 动画回调
			_OnAniCallback( fLastTime, m_fTime, fDuration );
		}
		break;
	case eAnimState_Pause:
		break;
	case eAnimState_Stop:
		break;
	}
}
//-------------------------------------------------------------------------
// 设置动画播放回调函数
void FKAnimateEx::SetAniCallback( IAnimationCallback* p_pAniCallBack, void* p_pContext )
{
	m_pAniCallback = p_pAniCallBack;
	m_pAniCallbackContext = p_pContext;
}
//-------------------------------------------------------------------------
// 播放动画，若该动作处于停止状态，则继续播放
void FKAnimateEx::Play( const std::string& p_szAniName, float p_fSpeed, int p_nLoops, bool p_bForceUpdate )
{
	if( m_pTarget == NULL || m_pAniRes == NULL )
		return;

	if( !m_pAniRes->IsValid() )
	{
		m_szCurAniName	= p_szAniName;
		m_fSpeed		= p_fSpeed;
		m_nLoopTime		= p_nLoops;
		return;
	}

	FKAnimateExRes::SAnimationInfo* pInfo = m_pAniRes->GetAnimation( p_szAniName.c_str() );
	if( pInfo == NULL )
		return;

	CCAnimation* pCurAnimation = m_pCurAnimation;
	m_pCurAnimation = pInfo->m_pAnimation;
	if( m_pCurAnimation == NULL )
	{
		m_pCurAnimation = pCurAnimation;
		return;
	}

	m_szCurAniName = p_szAniName;
	if( m_nLoopTime > 0 )
	{
		m_nLoopTime = p_nLoops + m_pCurAnimation->getLoops();
	}
	else
	{
		m_nLoopTime = p_nLoops;
	}

	if( m_pCurAnimation == pCurAnimation )
	{
		if( p_bForceUpdate )
		{
			m_fSpeed = p_fSpeed;
			m_fTime = 0.0f;
			m_nNextFrame = 0;
			m_bIsNewLoop = true;
			m_eState = eAnimState_Normal;
		}
		return;
	}

	m_vecAniCallbacks.clear();
	unsigned int unSize = pInfo->m_vecCallbacks.size();
	for( unsigned int i = 0; i < unSize; ++i )
	{
		m_vecAniCallbacks.push_back( pInfo->m_vecCallbacks[i] );
	}

	m_fSpeed = p_fSpeed;
	m_fTime = 0.0f;
	m_nNextFrame = 0;
	m_bIsNewLoop = true;
	m_eState = eAnimState_Normal;
	m_vecFrameTimes.clear();

	CCSprite* pSprite = dynamic_cast<CCSprite*>( m_pTarget );
	if( pSprite != NULL )
	{
		pSprite->setFlipX( pInfo->m_bIsFlipX );
		pSprite->setFlipY( pInfo->m_bIsFlipY );
	}

	// 设置时间分割
	float fSingleDuration = m_pCurAnimation->getDuration();
	CCArray* pFrames = m_pCurAnimation->getFrames();
	CCARRAY_VERIFY_TYPE( pFrames, CCAnimationFrame* );

	m_vecFrameTimes.reserve( pFrames->count() );

	float fAccumUnitsOfTime = 0;
	float fNewUnitOfTime = fSingleDuration / m_pCurAnimation->getTotalDelayUnits();

	CCObject* pObj = NULL;
	CCARRAY_FOREACH( pFrames, pObj )
	{
		CCAnimationFrame* pFrame = (CCAnimationFrame*)pObj;
		float fValue = (fAccumUnitsOfTime * fNewUnitOfTime); //  / fSingleDuration
		fAccumUnitsOfTime += pFrame->getDelayUnits();
		m_vecFrameTimes.push_back( fValue );
	}
}
//-------------------------------------------------------------------------
// 停止动画
void FKAnimateEx::Stop()
{
	m_eState = eAnimState_Stop;
}
//-------------------------------------------------------------------------
// 暂停动画
void FKAnimateEx::Pause()
{
	m_eState = eAnimState_Pause;
}
//-------------------------------------------------------------------------
// 是否动画在播放
bool FKAnimateEx::IsPlaying()
{
	return m_eState == eAnimState_Normal;
}
//-------------------------------------------------------------------------
// 是否动画在暂停状态
bool FKAnimateEx::IsPause()
{
	return m_eState == eAnimState_Pause;
}
//-------------------------------------------------------------------------
// 是否存在指定名称的动作
bool FKAnimateEx::IsAniExit( const string& p_szAniName )
{
	if( m_pAniRes == NULL )
		return false;

	set<string>& AniSet = m_pAniRes->GetAniNameList();
	return ( AniSet.find( p_szAniName ) != AniSet.end() );
}
//-------------------------------------------------------------------------
// 获取动作列表
void FKAnimateEx::GetAnimationList( FK_OUT list<string>& p_listAnimations )
{
	if( m_pAniRes == NULL )
		return;
	set<string>& AniSet = m_pAniRes->GetAniNameList();
	if( AniSet.empty() )
		return;
	set<string>::iterator Ite = AniSet.begin();
	for( ; Ite != AniSet.end(); ++Ite )
	{
		p_listAnimations.push_back( *Ite );
	}
}
//-------------------------------------------------------------------------
// 设置/获取播放速度
void FKAnimateEx::SetSpeed( float p_fSpeed )
{
	m_fSpeed = p_fSpeed;
}
//-------------------------------------------------------------------------
float FKAnimateEx::GetSpeed()
{
	return m_fSpeed;
}
//-------------------------------------------------------------------------
// 动画回调
void FKAnimateEx::_OnAniCallback( float fLastTime, float fCurTime, float fDuration )
{
	if( m_pAniCallback == NULL )
		return;

	SAnimationCallbackInfo tagAnimationCallbackInfo;
	for( unsigned int i = 0; i < m_vecAniCallbacks.size(); ++i )
	{
		float t = m_vecAniCallbacks[i].m_fCallbackTime;
		if( t >= fLastTime && t <= fCurTime )
		{
			strcpy( tagAnimationCallbackInfo.m_szName, m_vecAniCallbacks[i].m_strName.c_str() );
			strcpy( tagAnimationCallbackInfo.m_szActName, m_szCurAniName.c_str() );
			tagAnimationCallbackInfo.m_fFrameTime = t;
			m_pAniCallback->OnAnimaitonCallback( tagAnimationCallbackInfo, m_pAniCallbackContext );
			return;
		}
	}

	if( fabs(fLastTime) > FK_EPSILON )
	{
		strcpy( tagAnimationCallbackInfo.m_szName, "start" );
		strcpy( tagAnimationCallbackInfo.m_szActName, m_szCurAniName.c_str() );
		tagAnimationCallbackInfo.m_fFrameTime = 0.0f;
		m_pAniCallback->OnAnimaitonCallback( tagAnimationCallbackInfo, m_pAniCallbackContext );
		return;
	}

	if( fCurTime < fLastTime )
	{
		strcpy( tagAnimationCallbackInfo.m_szName, "end" );
		strcpy( tagAnimationCallbackInfo.m_szActName, m_szCurAniName.c_str() );
		tagAnimationCallbackInfo.m_fFrameTime = fDuration;
		m_pAniCallback->OnAnimaitonCallback( tagAnimationCallbackInfo, m_pAniCallbackContext );
		return;
	}
}
//-------------------------------------------------------------------------