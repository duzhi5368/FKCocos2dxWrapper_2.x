//*************************************************************************
//	创建日期:	2014-11-18
//	文件名称:	FKCW_EffectNode_RippleSprite.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_EffectNode_Mesh.h"
#include "FKCW_EffectNode_IndexVBO.h"
#include "FKCW_EffectNode_GLProgramWithUnifos.h"
//-------------------------------------------------------------------------
class FKCW_EffectNode_RippleBuffer :public CCObject
{
public:
	vector<vector<float> > mat;
};
//-------------------------------------------------------------------------
class FKCW_EffectNode_RippleSprite : public CCSprite
{
public:
	FKCW_EffectNode_RippleSprite();
	virtual ~FKCW_EffectNode_RippleSprite();

	bool	init(string texFileName,float gridSideLen) ;
	void	draw() ;
	void	drawWire();
	void	update(float t);
	void	doTouch(const CCPoint&touchPoint,float depth,float r);
	float	getGridSideLen()const{return m_gridSideLen;}
	void	setIsDrawDebug(bool value){m_isDrawDebug=value;}
	bool	getIsDrawDebug()const{return m_isDrawDebug;}
protected:
	void	_UpdateOnce();
protected:
	FKCW_EffectNode_IndexVBO*		m_indexVBO;
	FKCW_EffectNode_Mesh*			m_mesh;
	FKCW_EffectNode_RippleBuffer*	m_srcBuffer;
	FKCW_EffectNode_RippleBuffer*	m_dstBuffer;
	float							m_gridSideLen;
	int								m_nRow;
	int								m_nCol;
	vector<Cv2>						m_texCoordList_store;
	float							m_rippleStrength;
	bool							m_isDrawDebug;
	float							m_time;
};
//-------------------------------------------------------------------------