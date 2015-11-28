//-------------------------------------------------------------------------
#include "FKAudioSystem.h"
#include "SimpleAudioEngine.h"
//-------------------------------------------------------------------------
using namespace CocosDenshion;
//-------------------------------------------------------------------------
IAudioSystem*	gs_pAudioSystem = NULL;
FKAudioSystem	gs_FKAudioSystem;
//-------------------------------------------------------------------------
// 获取音频接口
IAudioSystem* GetAudioSystem()
{
	return gs_pAudioSystem;
}
//-------------------------------------------------------------------------
FKAudioSystem::FKAudioSystem()
	: m_fEffectVolume(1.0f)
	, m_fBackgroundMusicVolume(1.0f)
{
	gs_pAudioSystem = this;
}
//-------------------------------------------------------------------------
FKAudioSystem::~FKAudioSystem()
{
	gs_pAudioSystem = NULL;
}
//-------------------------------------------------------------------------
// 释放清理
void FKAudioSystem::Release()
{
	SimpleAudioEngine::end();
}
//-------------------------------------------------------------------------
// 播放背景音乐
void FKAudioSystem::PlayBackgroundMusic(const char* pszFilePath, bool bLoop)
{
	if( pszFilePath == NULL )
		return;

	SimpleAudioEngine::sharedEngine()->playBackgroundMusic( pszFilePath, bLoop );
}
//-------------------------------------------------------------------------
// 停止背景音乐
// 参数：bReleaseData 停止音乐后，是否继续保存音乐缓冲内存，默认值为false
void FKAudioSystem::StopBackgroundMusic(bool bReleaseData)
{
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(bReleaseData);
}
//-------------------------------------------------------------------------
// 获取背景音乐音量
// 返回值：	【0.0f, 1.0f】
float FKAudioSystem::GetBackgroundMusicVolume()
{
	return SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume();
}
//-------------------------------------------------------------------------
// 设置背景音乐音量
// 参数：	【0.0f, 1.0f】
void FKAudioSystem::SetBackgroundMusicVolume(float volume)
{
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume( volume );
}
//-------------------------------------------------------------------------
// 获取音效音量
// 返回值：	【0.0f, 1.0f】
float FKAudioSystem::GetEffectsVolume()
{
	return SimpleAudioEngine::sharedEngine()->getEffectsVolume();
}
//-------------------------------------------------------------------------
// 设置音效音量
// 参数：	【0.0f, 1.0f】
void FKAudioSystem::SetEffectsVolume(float volume)
{
	SimpleAudioEngine::sharedEngine()->setEffectsVolume(volume);
}
//-------------------------------------------------------------------------
// 播放音效
unsigned int FKAudioSystem::PlayEffect(const char* pszFilePath, bool bLoop)
{
	if( pszFilePath == NULL )
	{
		return 0;
	}

	return SimpleAudioEngine::sharedEngine()->playEffect( pszFilePath, bLoop );
}
//-------------------------------------------------------------------------
// Resume指定音效
// 参数：PlayEffect返回的EffectID
void FKAudioSystem::ResumeEffect(unsigned int nSoundId)
{
	SimpleAudioEngine::sharedEngine()->resumeEffect( nSoundId );
}
//-------------------------------------------------------------------------
// Resume全部音效
void FKAudioSystem::ResumeAllEffects()
{
	SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}
//-------------------------------------------------------------------------
// 停止指定音效
// 参数：PlayEffect返回的EffectID
void FKAudioSystem::StopEffect(unsigned int nSoundId)
{
	SimpleAudioEngine::sharedEngine()->stopEffect( nSoundId );
}
//-------------------------------------------------------------------------
// 停止全部音效
void FKAudioSystem::StopAllEffects()
{
	SimpleAudioEngine::sharedEngine()->stopAllEffects();
}
//-------------------------------------------------------------------------
// 系统静音
// 参数：是否静音
void FKAudioSystem::MuteAllSounds( bool bMute )
{
	if( bMute )
	{
		m_fEffectVolume = GetEffectsVolume();
		m_fBackgroundMusicVolume = GetBackgroundMusicVolume();
		SetEffectsVolume( 0.0f );
		SetBackgroundMusicVolume( 0.0f );
	}
	else
	{
		SetEffectsVolume( m_fEffectVolume );
		SetBackgroundMusicVolume( m_fBackgroundMusicVolume );
	}
}
//-------------------------------------------------------------------------