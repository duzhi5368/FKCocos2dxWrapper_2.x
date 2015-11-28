//*************************************************************************
//	创建日期:	2015-1-8
//	文件名称:	FKCW_RenderEx_SwayNode.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	摇曳的精灵
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_RenderEx_Macros.h"
//-------------------------------------------------------------------------
class FKCW_RenderEx_SwayNode : public CCNode
{
public:
	FKCW_RenderEx_SwayNode();
	~FKCW_RenderEx_SwayNode();

	static FKCW_RenderEx_SwayNode* create( const char* p_szSpriteFile );
	virtual bool init( const char* p_szSpriteFile );

	// 该函数每帧都会被调用
	void UpdatePos( float delta );
protected:
	int			m_nTimeUniformLocation;
	float		m_fTime;
	CCSprite*	m_pSprite;
};