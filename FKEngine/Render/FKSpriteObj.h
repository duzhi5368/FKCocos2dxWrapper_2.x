//*************************************************************************
//	创建日期:	2015-1-21
//	文件名称:	FKSpriteObj.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../FKEngineCommonHead.h"
//-------------------------------------------------------------------------
class FKSpriteObj : public ISprite
{
public:
	FKSpriteObj( RENDER_OBJ_ID id );
	~FKSpriteObj();
public:
	// 创建FKSprite对象
	bool Create( const char* szPicFileName, const CCRect& rect, CCPoint ptOffset = CCPointZero );
	// 更新
	virtual void				update( float dt );
	// 离开退出
	virtual void				onExit();
public:
	// 获取ID
	virtual RENDER_OBJ_ID		GetID();
	// 设置ID
	virtual void				SetID( RENDER_OBJ_ID unID );

	// 设置颜色
	// 参数：tagColor 颜色值
	virtual void				SetColor( const ccColor3B& tagColor );
	// 设置透明度
	// 参数：byAlpha 透明度[0,255]
	virtual void				SetOpacity( unsigned char byAlpha );
	// 获取游戏对象贴图文件名
	virtual const std::string&	GetFileName();
	// 获取游戏对象特征串名(全局唯一字符串)
	virtual const std::string&	GetDescName();

	// 碰撞检测
	// 参数：pt 检测点
	// 返回值：true 表示碰撞，false 表示未碰撞
	virtual bool				HitTest( const CCPoint& pt );
	// 碰撞检测
	// 参数：rt 检测矩形
	// 返回值：true 表示碰撞，false 表示未碰撞
	virtual bool				HitTest( const CCRect& rt );
	// 获取对象包围盒
	virtual bool				GetBoundingBox( CCRect& rt );
	// 设置偏移量
	virtual void				SetOffset( CCPoint& ptOffset );
	// 获取偏移量
	virtual bool				GetOffset( CCPoint& ptOffset );
	// 获取精灵
	virtual CCSprite*			GetSprite();
private:
	RENDER_OBJ_ID				m_unID;				// 对象ID
	CCSprite*					m_pSprite;			// 精灵对象
	string						m_strPicFileName;	// 贴图文件名
	string						m_strSpriteName;	// 唯一标示名
};