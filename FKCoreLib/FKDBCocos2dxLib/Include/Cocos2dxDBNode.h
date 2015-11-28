//*************************************************************************
//	创建日期:	2014-8-19   16:43
//	文件名称:	Cocos2dxDBNode.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "CommonHead.h"
#include "cocos2d.h"
//-------------------------------------------------------------------------
USING_NS_CC;
//-------------------------------------------------------------------------
#define MAX_BATCH_NUM		300
//-------------------------------------------------------------------------
// cocos2dx的DB渲染Node
#if USE_BATCH_NODE == 1
class CCocos2dxDBNode : public CCSpriteBatchNode
#else  
class CCocos2dxDBNode : public CCNode
#endif
{
public:
	static CArmature* BuildAramture( const char* p_szSkeletonXMLFile,
		const char* p_szTextureXMLFile,
		const char* p_szTexturePicFile,
		const char* p_szDragonBonesName,
		const char* p_szArmatureName,
		const char* p_szAnimationName = "");

	static CCocos2dxDBNode* Create( CArmature* p_pArmature );

	static CCocos2dxDBNode* Create( const char* p_szSkeletonXMLFile,
		const char* p_szDragonBonesName,
		const char* p_szArmatureName );

	static CCocos2dxDBNode* Create( const char* p_szSkeletonXMLFile,
		const char* p_szTextureXMLFile,
		const char* p_szTexturePicFile,
		const char* p_szDragonBonesName,
		const char* p_szArmatureName,
		const char* p_szAnimationName = "");
public:
	CCocos2dxDBNode();
public:
	CCNode*			GetDisplayNode();
	CArmature*		GetArmature();
	CAnimation*		GetAnimation();
	void			GotoAndPlay(const string& p_szAnimationName, f32 p_fFadeInTime = -1,
		f32 p_fDuration = -1, f32 p_fLoop = NaN, uint p_unLayer = 0, const string& p_szGroup = "",
		const string& p_szFadeOutMode = CAnimation::SAME_LAYER_AND_GROUP, bool p_bDisplayControl = true, 
		bool p_bPauseFadeOut = true, bool p_bPauseFadeIn = true );
public:
	void			AddEventListener( const string& p_szType, const string& p_szKey, CCObject* p_pObject, SEL_CallFuncND p_CallbackFunc );
	void			DispatchEvent( CEvent* p_pEvent );
	bool			IsHadEventListener( const string& p_szType )const;
	void			RemoveEventListener( const string& p_szType, const string& p_szKey );
public:
	void			SetBoneTexture( const char* p_szBoneName, const char* p_szTextureName, const char* p_szTextureAtlasName );
public:
	// 重载函数
	virtual void	onExit();
	virtual void	visit();
private:
	void			__InitWithArmature( CArmature* p_pArmature );
	void			__Update( float p_fDelayTime );
	void			__EventBridge( CEvent* p_pEvent );
#if USE_BATCH_NODE == 1
	virtual void	addChild(CCNode *child, int zOrder, int tag);
	virtual void	addChild(CCNode *child);
	virtual void	addChild(CCNode *child, int zOrder);
#endif
private:
	CArmature*		m_pArmature;
	SEL_CallFuncND	m_CallbackFun;
	CCObject*		m_pCaller;
};
//-------------------------------------------------------------------------