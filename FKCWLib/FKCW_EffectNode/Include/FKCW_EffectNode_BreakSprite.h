//*************************************************************************
//	创建日期:	2014-11-17
//	文件名称:	FKCW_EffectNode_BreakSprite.h
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
// 三角型
class FKCW_RenderTriangle
{
public:
	Cv2 m_pos[3];
	Cv2 m_texCoord[3];
	Cv4 m_color[3];
	float m_fallOffDelay;
	float m_vy;
public:
	FKCW_RenderTriangle();

	void setPositions(const Cv2&pos0,const Cv2&pos1,const Cv2&pos2);
	void setColors(const Cv4&color0,const Cv4&color1,const Cv4&color2);
	void setTexCoords(const Cv2&texCoord0,const Cv2&texCoord1,const Cv2&texCoord2);
};
//-------------------------------------------------------------------------
// 破碎精灵的三个状态
enum ENUM_BreakSpriteState
{
	eBreakSpriteState_Well = 0,			// 正常状态
	eBreakSpriteState_Crack,			// 破碎状态
	eBreakSpriteState_FallOff,			// 跌落状态
};
//-------------------------------------------------------------------------
class FKCW_Action_FallOffAction;
class FKCW_EffectNode_BreakSprite : public CCSprite
{
public:
	friend class FKCW_Action_FallOffAction;
public:
	FKCW_EffectNode_BreakSprite();
	virtual ~FKCW_EffectNode_BreakSprite();

	bool	init(const string&texFileName) ;
	void	draw() ;
	void	drawWire();
	void	doCrack(const CCPoint&touchPoint);
	void	generateDelayTimes(float maxDelayTime){_GenerateDelayTimeForTris(0,maxDelayTime);}
	float	getGridSideLenMax()const{return m_gridSideLenMax;}
	void	setGridSideLenMax(float value){m_gridSideLenMax=value;}
	float	getGridSideLenMin()const{return m_gridSideLenMin;}
	void	setGridSideLenMin(float value){m_gridSideLenMin=value;}
	ENUM_BreakSpriteState getState()const{return m_state;}
	void	reSet();
	void	setIsDrawDebug(bool value){m_isDrawDebug=value;}
	bool	getIsDrawDebug()const{return m_isDrawDebug;}
protected:
	int		_GetPointInRangeEqualsToTargetPoint(const CCPoint&targetPoint,const vector<CCPoint>&pointList,int imin,int imax,float eps);
	void	_UpdateMeshAndSubmitChange();
	void	_GenerateDelayTimeForTris(float minDelayTime,float maxDelayTime);
	void	_UpdateBreakAction(float time,float dt,float ay);
	void	_Clear();
protected:
	float									m_gridSideLenMax;
	float									m_gridSideLenMin;
	FKCW_EffectNode_GLProgramWithUnifos*	m_program;
	FKCW_EffectNode_Mesh*					m_mesh;
	FKCW_EffectNode_MeshWire*				m_meshWire;
	FKCW_EffectNode_IndexVBO*				m_indexVBO;
	FKCW_EffectNode_IndexVBO*				m_indexVBO_wire;
	int										m_nRow;		// 碎片格子总行数
	int										m_nCol;		// 碎片格子总列数
	int										m_nRowPoint;// 点的总行数, m_nRowPoint=m_nRow+1
	int										m_nColPoint;// 点的总列数, m_nColPoint=m_nCol+1
	vector<CCPoint>							m_pointList;
	vector<CIDquad>							m_IDquadList;
	vector<FKCW_RenderTriangle>				m_triList;
	ENUM_BreakSpriteState					m_state;
	bool									m_isDrawDebug;
};
//-------------------------------------------------------------------------
#define FKCW_Action_FallOff_AY (-60*10)
class FKCW_Action_FallOffAction : public CCActionInterval
{
public:
	FKCW_Action_FallOffAction();
	static FKCW_Action_FallOffAction* create(float fDuration,float ay=FKCW_Action_FallOff_AY);
	bool initWithDuration(float fDuration,float ay=FKCW_Action_FallOff_AY);

	virtual void startWithTarget(CCNode *pTarget);
	virtual void update(float time);
protected:
	float m_timeFoe;
	float m_timeCur;
	float m_ay;
};
//-------------------------------------------------------------------------