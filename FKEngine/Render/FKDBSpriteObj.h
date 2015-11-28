//*************************************************************************
//	创建日期:	2015-1-22
//	文件名称:	FKDBSpriteObj.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../FKEngineCommonHead.h"
#include "../../FKCoreLib/FKDBCocos2dxLib/FKDBCocos2dxLib.h"
//-------------------------------------------------------------------------
class FKDBSpriteObj : public IDragonBonesSprite
{
public:
	FKDBSpriteObj( RENDER_OBJ_ID id );
	~FKDBSpriteObj();
public:
	// 创建FKSprite对象
	bool Create( const char* szPicFileName, CCPoint ptOffset = CCPointZero );
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

	// 获取动画对象
	virtual IAnimation*			GetAnimation();
private:
	CCocos2dxDBSprite*			m_pSprite;				// 实际DB精灵
	RENDER_OBJ_ID				m_unID;
	string						m_strFileName;
	string						m_strDescName;
};