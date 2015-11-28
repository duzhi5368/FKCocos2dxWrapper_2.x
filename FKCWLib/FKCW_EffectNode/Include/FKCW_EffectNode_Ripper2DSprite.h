//*************************************************************************
//	创建日期:	2014-11-18
//	文件名称:	FKCW_EffectNode_Ripper2DSprite.h
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
class FKCW_EffectNode_Ripple2DSprite:public CCSprite
{
public:
	FKCW_EffectNode_Ripple2DSprite();
	virtual~FKCW_EffectNode_Ripple2DSprite();

	void init(const string&texFileName);
	// 参数：若h_press大于0，则表示在水面上按下；若h_press小于0，则表示在水中按下
	// 参数：x 是在父空间中x坐标
	void pressAtX(float x,float h_press,float r_press=120);
	void draw();
	void drawWire();
	void update(float dt);
	bool getIsDrawDebug()const {return m_isDrawDebug;}
	void setIsDrawDebug(bool value){m_isDrawDebug=value;}
protected:
	void _InitMesh();
	void _UpdateMesh();
	void _UpdateRipple();
protected:
	vector<CCPoint>				m_surfacePointList_reduced;
	int							m_reduceStep;
	vector<CCPoint>				m_surfacePointList;
	vector<CCPoint>				m_surfacePointList_back;		// 后台缓冲
	float						m_dx;
	FKCW_EffectNode_Mesh*		m_mesh;
	FKCW_EffectNode_IndexVBO*	m_indexVBO;
	bool						m_isDrawDebug;
};