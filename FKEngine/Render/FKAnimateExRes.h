//*************************************************************************
//	创建日期:	2015-1-22
//	文件名称:	FKAnimateExRes.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../FKEngineCommonHead.h"
//-------------------------------------------------------------------------
// 动画回调数据
struct SAnimationCallback
{
	float	m_fCallbackTime;		// 回调事件
	string	m_strName;				// 回调名称
	SAnimationCallback()
		: m_fCallbackTime( 0.0f )
	{
		m_strName.clear();
	}
};
//-------------------------------------------------------------------------
// 动画信息
struct SAnimate
{
	string	m_strName;				// 动画名
	float	m_fDelay;				// 动画延时
	bool	m_bIsFlipX;				// 是否X轴对称
	bool	m_bIsFlipY;				// 是否Y轴对称
	vector<string>	m_vecSpriteFrame;	// 精灵动画帧
	SAnimate()
		: m_fDelay( 0.0f )
		, m_bIsFlipX( false )
		, m_bIsFlipY( false )
	{
		m_strName.clear();
		m_vecSpriteFrame.clear();
	}
};
//-------------------------------------------------------------------------
// 处理器
class FKAnimateSaxDelegator : public CCSAXDelegator
{
public:
	enum ENUM_State
	{
		eState_None		= 0,
		eState_Plist,
		eState_Animation,
		eState_Name,
		eState_Delay,
		eState_FlipX,
		eState_FlipY,
		eState_Sprite_Frame,
	};
public:
	virtual void startElement(void *ctx, const char *name, const char **atts) ;
	virtual void endElement(void *ctx, const char *name) ;
	virtual void textHandler(void *ctx, const char *s, int len) ;
public:
	vector<string>		m_vecPlists;		// 全部的Plist列表名称
	vector<SAnimate>	m_vecAnimates;		// 全部的动作列数据
	ENUM_State			m_eCurState;		// 当前处理的标签状态
};
//-------------------------------------------------------------------------
class FKAnimateExRes : public CCObject
{
public:
	// 动画信息
	struct SAnimationInfo
	{
		CCAnimation*	m_pAnimation;
		bool			m_bIsFlipX;
		bool			m_bIsFlipY;
		vector<SAnimationCallback>	m_vecCallbacks;

		SAnimationInfo()
			: m_pAnimation( NULL )
			, m_bIsFlipY( false )
			, m_bIsFlipX( false )
		{
			m_vecCallbacks.clear();
		}
	};
public:
	FKAnimateExRes();
	~FKAnimateExRes();
public:
	// 释放
	virtual void		release();
public:
	// 加载一个资源
	bool				LoadRes( const char* szResName );
	// 获取资源名称
	const string&		GetResFileName();
	// 获取动画数据
	SAnimationInfo*		GetAnimation( const char* szAnimationName );
	// 获取动画名列表
	set<string>&		GetAniNameList();
	// 判断资源是否全部顺利加载
	bool				IsValid();
protected:
	// 加载回调数据
	void				_LoadCallback( const char* szResName );
	// 加载贴图
	void				_LoadTextureAsync( const char* szResName );
	// 加载资源
	void				_LoadRes( const char* szResName );
	// 图片加载完毕回调
	void				_OnImageLoadOver( CCObject* pObj );
private:
	typedef map<string, SAnimationInfo>		AnimationMap;	// 动画列表
	typedef map<string, bool>				PlistTextureMap;// 贴图是否加载成功标示

	bool							m_bIsVaild;				// 是否资源全部加载完毕
	string							m_strAniResName;		// 动画资源名

	map<string, SAnimate>			m_mapAnimateNameToAnimate;
	map<string, vector<string> >	m_mapXMLPathToPlists;
	set<string>						m_setAnimationName;		// 动画列表
	PlistTextureMap					m_mapTextureLoadInfo;	// 资源加载状况
	AnimationMap					m_mapAnimation;			// 动画列表

	CCSAXParser						m_SaxParser;			// xml解释器
	FKAnimateSaxDelegator			m_SaxDelegator;			// 自定义xml处理器
};