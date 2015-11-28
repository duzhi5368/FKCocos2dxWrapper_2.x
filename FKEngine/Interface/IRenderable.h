//*************************************************************************
//	创建日期:	2015-1-17
//	文件名称:	IRenderable.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../../FKCWLib/FKCocos2dxWrapper.h"
#include "IRenderObj.h"
#include "../../FKCoreLib/FKDBLib/Interface/IAnimation.h"
//-------------------------------------------------------------------------
// 渲染对象类型
enum ENUM_RenderObjType
{
	eROT_Sprite		= 0,		// 普通的静态精灵对象
	eROT_DB			= 1,		// DragonBones动画对象
	eROT_Effect		= 2,		// 特效对象
	eROT_Ani		= 3,		// 传统帧动画对象
	eROT_Armature	= 4,		// cocosStudio动画对象

	eROT_Max,
};
//-------------------------------------------------------------------------
class IEffectProp;
class IRenderable : public CCNode
{
public:
	// 获取渲染对象类型
	virtual ENUM_RenderObjType	GetRenderObjType() = 0;
	// 获取ID
	virtual RENDER_OBJ_ID		GetID() = 0;
	// 设置ID
	virtual void				SetID( RENDER_OBJ_ID unID ) = 0;

	// 设置颜色
	// 参数：tagColor 颜色值
	virtual void				SetColor( const ccColor3B& tagColor ) = 0;
	// 设置透明度
	// 参数：byAlpha 透明度[0,255]
	virtual void				SetOpacity( unsigned char byAlpha ) = 0;
	// 获取游戏对象贴图文件名
	virtual const std::string&	GetFileName() = 0;
	// 获取游戏对象特征串名(全局唯一字符串)
	virtual const std::string&	GetDescName() = 0;

	// 碰撞检测
	// 参数：pt 检测点
	// 返回值：true 表示碰撞，false 表示未碰撞
	virtual bool				HitTest( const CCPoint& pt ) = 0;
	// 碰撞检测
	// 参数：rt 检测矩形
	// 返回值：true 表示碰撞，false 表示未碰撞
	virtual bool				HitTest( const CCRect& rt ) = 0;
	// 获取对象包围盒
	virtual bool				GetBoundingBox( CCRect& rt ) = 0;
	// 设置偏移量
	virtual void				SetOffset( CCPoint& ptOffset ) = 0;
	// 获取偏移量
	virtual bool				GetOffset( CCPoint& ptOffset ) = 0;

	// 获取精灵
	virtual CCSprite*			GetSprite() = 0;
	// 获取动画对象
	virtual IAnimation*			GetAnimation() = 0;
	// 获取特效数据
	virtual IEffectProp*		GetEffectProp() = 0;
};
//-------------------------------------------------------------------------
// 静态精灵对象
class ISprite : public IRenderable
{
public:
	// 获取渲染对象类型
	virtual ENUM_RenderObjType	GetRenderObjType(){ return eROT_Sprite; }
	// 获取动画对象
	virtual IAnimation*			GetAnimation(){return NULL;};
	// 获取特效数据
	virtual IEffectProp*		GetEffectProp(){return NULL;};
};
//-------------------------------------------------------------------------
// DB动画对象
class IDragonBonesSprite : public IRenderable
{
public:
	// 获取渲染对象类型
	virtual ENUM_RenderObjType	GetRenderObjType(){ return eROT_DB; }
	// 获取精灵
	virtual CCSprite*			GetSprite(){return NULL;};
	// 获取特效数据
	virtual IEffectProp*		GetEffectProp(){return NULL;};
};
//-------------------------------------------------------------------------
// 帧动画对象
class IFrameSprite : public IRenderable
{
public:
	// 获取渲染对象类型
	virtual ENUM_RenderObjType	GetRenderObjType(){ return eROT_Ani; }
	// 获取特效数据
	virtual IEffectProp*		GetEffectProp(){return NULL;};
};
//-------------------------------------------------------------------------
// cocosBuilder骨骼动画对象
class IArmatureSprite : public IRenderable
{
public:
	// 获取渲染对象类型
	virtual ENUM_RenderObjType	GetRenderObjType(){ return eROT_Armature; }
	// 获取精灵
	virtual CCSprite*			GetSprite(){return NULL;};
	// 获取特效数据
	virtual IEffectProp*		GetEffectProp(){return NULL;};
};
//-------------------------------------------------------------------------
// 特效对象
class IEffect : public IRenderable
{
public:
	// 获取渲染对象类型
	virtual ENUM_RenderObjType	GetRenderObjType(){ return eROT_Effect; }
	// 获取精灵
	virtual CCSprite*			GetSprite(){return NULL;};
	// 获取动画对象
	virtual IAnimation*			GetAnimation(){return NULL;};
};
//-------------------------------------------------------------------------