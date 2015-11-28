//*************************************************************************
//	创建日期:	2014-8-20   16:11
//	文件名称:	Cocos2dxDBSprite.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "CommonHead.h"
#include "Cocos2dxDBAnimation.h"
//-------------------------------------------------------------------------
USING_NS_CC;
//-------------------------------------------------------------------------
class CCocos2dxDBNode;
class CCocos2dxDBSprite : public CCNode
{
public:
	CCocos2dxDBSprite();
	virtual ~CCocos2dxDBSprite();
public:
	// 创建DragonBones对象
	// params p_szAniFileName DB导出的骨骼xml文件相对路径
	// params p_tagOffset 渲染node和基准点坐标偏移值
	virtual bool			create( const char* p_szAniFileName, CCPoint p_tagOffset = ccp(0,0) );
	virtual void			onExit();

	// 碰撞检查
	virtual bool			hitTest( const CCPoint& p_tagPos );
	virtual bool			hitTest( const CCRect& p_tagRect );

	virtual void			setColor( const ccColor3B& p_Color );
	virtual void			setID( unsigned int p_unID );
	virtual unsigned int	getID();
	virtual void			setOpacity( unsigned char p_ucAlpha );
	virtual void			getFeatureString( FK_OUT string& p_szFeature );
	virtual const string	getFileName();
	virtual CCSprite*		getSprite();
	// 获取DragonBones对象包围盒
	// 注意，这里是遍历其全部动作得到的最大包围盒
	virtual bool			getBoundingBox( FK_OUT CCRect& p_tagBoundBox );
	virtual IAnimation*		getAnimation();
	virtual void			setOffset( CCPoint& p_tagOffset );
	virtual void			getOffset( FK_OUT CCPoint& p_tagOffset );

	CCocos2dxDBNode*		GetDBNode(){return m_pDBNode;}
private:
	string					m_szFileName;			// 骨骼xml文件相对路径
	CCocos2dxDBAnimation	m_tagAnimate;			// 动画管理器
	CCocos2dxDBNode*		m_pDBNode;				// Dragone实际渲染node
};