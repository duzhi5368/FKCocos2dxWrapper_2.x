//*************************************************************************
//	创建日期:	2015-1-19
//	文件名称:	FKRenderableMgr.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../FKEngineCommonHead.h"
//-------------------------------------------------------------------------
class FKRenderableMgr
{
protected:
	FKRenderableMgr();
	~FKRenderableMgr();
public:
	// 单件对象
	static FKRenderableMgr& GetInstance()
	{
		static FKRenderableMgr s_FKRenderableMgrInstance;
		return s_FKRenderableMgrInstance;
	}
public:
	// 获取渲染对象
	IRenderable*			GetRenderable( RENDER_OBJ_ID id );
	// 删除渲染对象
	void					RemoveRenderable( RENDER_OBJ_ID id );

	// 创建静态精灵对象
	ISprite*				CreateSprite( const char* szPicFileName, 
		const CCRect& rect, CCPoint ptOffset = CCPointZero );
	// 创建帧精灵对象
	IFrameSprite*			CreateFrameSprite( const char* szFrameFileName, CCPoint ptOffset = CCPointZero );
	// 创建DB动画对象
	IDragonBonesSprite*		CreateDBSprite( const char* szDBFileName, CCPoint ptOffset = CCPointZero );
	// 创建CocosBuilder骨骼动画对象
	IArmatureSprite*		CreateArmaSprite( const char* szArmaFileName, CCPoint ptOffset = CCPointZero );
	// 创建特效对象
	IEffect*				CreateEffect( const char* szEffectFileName, CCPoint ptOffset = CCPointZero );
private:
	typedef map<RENDER_OBJ_ID, IRenderable*>	RenderableMap;
	RenderableMap			m_mapRenderables;	// 当前被管理渲染对象表
	RENDER_OBJ_ID			m_unCurID;			// 当前渲染对象编号
};