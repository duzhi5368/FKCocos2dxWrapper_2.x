//-------------------------------------------------------------------------
#include "../Include/Cocos2dxDBAnimation.h"
#include "../Include/Cocos2dxDBNode.h"
#include <list>
#include <vector>
//-------------------------------------------------------------------------
/// 安全字符串拷贝
static char* sstrcpyn(char* dest, size_t destSize, const char* src )
{
#ifdef WIN32
	return lstrcpynA(dest, src, destSize);
#else
	/// 保证不会越界
	memcpy( dest, src, destSize );
	dest[destSize-1] = 0;
	return dest;
#endif
}
//-------------------------------------------------------------------------
CCocos2dxDBAnimation::CCocos2dxDBAnimation()
	: m_nState( 0 )
	, m_fSpeed( 0.0f )
	, m_pArmature( NULL )
	, m_pAnimation( NULL )
	, m_pAniCallback( NULL )
	, m_pCallbackContext( NULL )
{

}
//-------------------------------------------------------------------------
CCocos2dxDBAnimation::~CCocos2dxDBAnimation()
{

}
//-------------------------------------------------------------------------
void CCocos2dxDBAnimation::SetDBNode( CCocos2dxDBNode* p_pDBNode )
{
	if( p_pDBNode == NULL )
		return;

	m_pDBNode			= p_pDBNode;
	m_pAnimation		= p_pDBNode->GetAnimation();
	m_pArmature			= p_pDBNode->GetArmature();

	// 添加内部注册
	if( m_pDBNode == NULL )
		return;

	m_pDBNode->AddEventListener( CAnimateEvent::LOOP_COMPLETE,		"mykey1", this, callfuncND_selector(CCocos2dxDBAnimation::OnDBEventCallFunc) );
	m_pDBNode->AddEventListener( CAnimateEvent::FADE_IN,			"mykey2", this, callfuncND_selector(CCocos2dxDBAnimation::OnDBEventCallFunc) );
	m_pDBNode->AddEventListener( CAnimateEvent::START,				"mykey3", this, callfuncND_selector(CCocos2dxDBAnimation::OnDBEventCallFunc) );
	m_pDBNode->AddEventListener( CAnimateEvent::COMPLETE,			"mykey4", this, callfuncND_selector(CCocos2dxDBAnimation::OnDBEventCallFunc) );
	m_pDBNode->AddEventListener( CFrameEvent::ANIMATION_FRAME_EVENT,"mykey5", this, callfuncND_selector(CCocos2dxDBAnimation::OnDBEventCallFunc) );
	m_pDBNode->AddEventListener( CSoundEvent::SOUND,				"mykey6", this, callfuncND_selector(CCocos2dxDBAnimation::OnDBEventCallFunc) );

	// 播放第一个动画
	std::list< string > listAnimationName;
	GetAnimationList( listAnimationName );
	if( m_szCurActionName == "" && !listAnimationName.empty() )
	{
		Play( *listAnimationName.begin() );
	}
}
//-------------------------------------------------------------------------
// 设置动画播放回调函数
void CCocos2dxDBAnimation::SetAniCallback( IAnimationCallback* p_pAniCallBack, void* p_pContext )
{
	m_pAniCallback = p_pAniCallBack;
	m_pCallbackContext = p_pContext;
}
//-------------------------------------------------------------------------
// 播放动画，若该动作处于停止状态，则继续播放
void CCocos2dxDBAnimation::Play( const std::string& p_szAniName, float p_fSpeed,  int p_nLoops, bool p_bForceUpdate )
{
	if( m_pAnimation == NULL )
		return;

	// CCLOG( "CCocos2dxDBAnimation::Play - %s", p_szAniName.c_str() );
	float fLoops = NaN;
	if( p_nLoops < 0 )
	{
		fLoops = NaN;
	}
	else
	{
		fLoops = static_cast<float>( p_nLoops );
	}
	m_nState = 0;
	m_pAnimation->GotoAndPlay( p_szAniName.c_str(), -1.0f, -1.0f, fLoops );
	m_pAnimation->m_fTimeScale = p_fSpeed;
	m_szCurActionName = p_szAniName;
}
//-------------------------------------------------------------------------
// 停止动画
void CCocos2dxDBAnimation::Stop()
{
	if( m_pAnimation == NULL )
		return;

	return m_pAnimation->Stop();
}
//-------------------------------------------------------------------------
// 暂停动画
void CCocos2dxDBAnimation::Pause()
{
	if( m_pAnimation == NULL )
		return;

	m_nState = 1;
	return m_pAnimation->Stop();
}
//-------------------------------------------------------------------------
// 是否动画在播放
bool CCocos2dxDBAnimation::IsPlaying()
{
	return m_nState == 0;
}
//-------------------------------------------------------------------------
// 是否动画在暂停状态
bool CCocos2dxDBAnimation::IsPause()
{
	return m_nState == 1;
}
//-------------------------------------------------------------------------
// 是否存在指定名称的动作
bool CCocos2dxDBAnimation::IsAniExit( const string& p_szAniName )
{
	if( m_pAnimation == NULL )
		return false;

	return m_pAnimation->IsHasAnimation( p_szAniName.c_str() );
}
//-------------------------------------------------------------------------
// 获取动作列表
void CCocos2dxDBAnimation::GetAnimationList( FK_OUT list<string>& p_listAnimations )
{
	if( m_pAnimation == NULL )
		return;

	const vector<string>& vecAnimation = m_pAnimation->m_vecAnimationList;
	if( vecAnimation.empty() )
		return;

	unsigned int unSize = vecAnimation.size();
	for( unsigned int i = 0; i < unSize; ++i )
	{
		p_listAnimations.push_back( vecAnimation[i] );
	}
}
//-------------------------------------------------------------------------
// 设置/获取播放速度
void CCocos2dxDBAnimation::SetSpeed( float p_fSpeed )
{
	if( m_pAnimation == NULL )
		return;

	m_pAnimation->m_fTimeScale = p_fSpeed;
}
//-------------------------------------------------------------------------
float CCocos2dxDBAnimation::GetSpeed()
{
	if( m_pAnimation == NULL )
		return 1.0f;

	return m_pAnimation->m_fTimeScale;
}
//-------------------------------------------------------------------------
// 设置动画循环次数
void CCocos2dxDBAnimation::SetAniLoop( int nLoop )
{
	if( m_pAnimation )
	{
		if( m_pAnimation->m_pLastAnimationState )
			m_pAnimation->m_pLastAnimationState->m_nLoop = nLoop;
	}
}
//-------------------------------------------------------------------------
// 动作回调函数
void CCocos2dxDBAnimation::OnMovementEventCallFunc( extension::CCArmature* p_pArmature,
	extension::MovementEventType p_eMovementEventType, const char* p_szEventName )
{
	if( m_pAniCallback == NULL )
		return;

	SAnimationCallbackInfo tagAnimationCallbackInfo;
	tagAnimationCallbackInfo.m_fFrameTime	= 0.0f;

	switch ( p_eMovementEventType )
	{
	case extension::START:
		{
			strcpy( tagAnimationCallbackInfo.m_szName, "start" );
		}
		break;
	case extension::COMPLETE:
	case extension::LOOP_COMPLETE:
		{
			strcpy( tagAnimationCallbackInfo.m_szName, "end" );
		}
		break;
	default:
		break;
	}

	strcpy( tagAnimationCallbackInfo.m_szActName, m_szCurActionName.c_str() );
	m_pAniCallback->OnAnimaitonCallback( tagAnimationCallbackInfo, m_pCallbackContext );
}
//-------------------------------------------------------------------------
// 帧事件回调
void CCocos2dxDBAnimation::OnFrameEventCallFunc( extension::CCBone* p_pBone, 
	const char* p_szEventName, int p_nFrameID, int p_nTime )
{
	if( m_pAniCallback == NULL )
		return;

	SAnimationCallbackInfo tagAnimationCallbackInfo;
	tagAnimationCallbackInfo.m_fFrameTime	= static_cast<float>(p_nTime);
	strcpy( tagAnimationCallbackInfo.m_szActName,  m_szCurActionName.c_str() );
	strcpy( tagAnimationCallbackInfo.m_szActName,  p_szEventName );
	m_pAniCallback->OnAnimaitonCallback( tagAnimationCallbackInfo, m_pCallbackContext );
}
//-------------------------------------------------------------------------
// 骨骼动画回调
void CCocos2dxDBAnimation::OnDBEventCallFunc( CCNode* pNode, void* p_pEvent )
{
	if( p_pEvent == NULL || pNode == NULL )
	{
		return;
	}

	if( m_pAniCallback == NULL )
	{
		return;
	}

	SAnimationCallbackInfo tagAnimationCallbackInfo;
	tagAnimationCallbackInfo.m_fFrameTime	=  0.0f;
	CEvent* pEvent = (CEvent*)p_pEvent;
	string szEventType = pEvent->m_szType;

	if( szEventType == CFrameEvent::ANIMATION_FRAME_EVENT )
	{
		CFrameEvent* pEvent						= ( CFrameEvent* )p_pEvent;
		tagAnimationCallbackInfo.m_fFrameTime	= pEvent->m_pAnimationState->m_fCurrentTime;
		sstrcpyn( tagAnimationCallbackInfo.m_szActName, sizeof( tagAnimationCallbackInfo.m_szActName ), 
			pEvent->m_szFrameLabel.c_str() );
		sstrcpyn( tagAnimationCallbackInfo.m_szName, sizeof( tagAnimationCallbackInfo.m_szName ), 
			pEvent->m_pAnimationState ? pEvent->m_pAnimationState->m_szName.c_str() : "" );

		printf("\nDBEVEN:%s,%s",tagAnimationCallbackInfo.m_szName,tagAnimationCallbackInfo.m_szActName);

		m_pAniCallback->OnAnimaitonCallback( tagAnimationCallbackInfo, m_pCallbackContext );
	}
	else if( szEventType == CAnimateEvent::START )
	{
		CAnimateEvent* pEvent					= ( CAnimateEvent* )p_pEvent;
		tagAnimationCallbackInfo.m_fFrameTime	= 0.0f;
		sstrcpyn( tagAnimationCallbackInfo.m_szActName, sizeof( tagAnimationCallbackInfo.m_szActName ), 
			pEvent->m_pAnimationState->m_szName.c_str() );
		sstrcpyn( tagAnimationCallbackInfo.m_szName, sizeof( tagAnimationCallbackInfo.m_szName ), "start");

		m_pAniCallback->OnAnimaitonCallback( tagAnimationCallbackInfo, m_pCallbackContext );
	}
	else if( szEventType == CAnimateEvent::COMPLETE || szEventType == CAnimateEvent::LOOP_COMPLETE )
	{
		CAnimateEvent* pEvent					= ( CAnimateEvent* )p_pEvent;
		tagAnimationCallbackInfo.m_fFrameTime	= 0.0f;
		sstrcpyn( tagAnimationCallbackInfo.m_szActName, sizeof( tagAnimationCallbackInfo.m_szActName ), 
			pEvent->m_pAnimationState->m_szName.c_str() );
		sstrcpyn( tagAnimationCallbackInfo.m_szName, sizeof( tagAnimationCallbackInfo.m_szName ), "end");

		m_pAniCallback->OnAnimaitonCallback( tagAnimationCallbackInfo, m_pCallbackContext );
	}
	else if( szEventType == CSoundEvent::SOUND )
	{
		CAnimateEvent* pEvent					= ( CAnimateEvent* )p_pEvent;
		tagAnimationCallbackInfo.m_fFrameTime	= 0.0f;
		sstrcpyn( tagAnimationCallbackInfo.m_szActName, sizeof( tagAnimationCallbackInfo.m_szActName ), 
			pEvent->m_pArmature ? pEvent->m_pArmature->m_szName.c_str() : "" );
		sstrcpyn( tagAnimationCallbackInfo.m_szName, sizeof( tagAnimationCallbackInfo.m_szName ), "end");

		m_pAniCallback->OnAnimaitonCallback( tagAnimationCallbackInfo, m_pCallbackContext );
	}
}
//-------------------------------------------------------------------------