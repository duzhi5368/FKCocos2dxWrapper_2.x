//*************************************************************************
//	创建日期:	2014-11-15
//	文件名称:	FKCW_EffectNode_2DSoftShadow.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	动态2D软阴影
/*
	使用说明：
	创建root节点，绑定数个带软阴影的节点精灵，绑定一个灯光节点对象。
	将root节点绑定到需要显示的层上，OK。
*/
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_EffectNode_Mesh.h"
#include "FKCW_EffectNode_IndexVBO.h"
#include "FKCW_EffectNode_GLProgramWithUnifos.h"
//-------------------------------------------------------------------------
// 边
class FKCW_EffectNode_2DSoftShadow_Edge
{
public:
	CCPoint m_start;
	CCPoint m_end;
	FKCW_EffectNode_2DSoftShadow_Edge(){}
	FKCW_EffectNode_2DSoftShadow_Edge(const CCPoint&start,const CCPoint&end);
};
//-------------------------------------------------------------------------
// 光线节点
class FKCW_EffectNode_2DSoftShadow_LightNode:public CCNode
{
public:
	FKCW_EffectNode_2DSoftShadow_LightNode();
	FKCW_EffectNode_2DSoftShadow_LightNode(float r);
	bool	init(float r);
	void	draw();
	float	getR()const{return m_fR;}
	void	setIsDrawDebug(bool value){m_bIsDebugDraw=value;}
	bool	getIsDrawDebug()const{return m_bIsDebugDraw;}
protected:
	float	m_fR;
	bool	m_bIsDebugDraw;
};
//-------------------------------------------------------------------------
// 点类型 
class FKCW_Effect_2DSoftShadow_PointType
{
public:
	bool m_isLeftUmbraPoint;
	CCPoint m_leftUmbraT;
	bool m_isRightUmbraPoint;
	CCPoint m_rightUmbraT;
	bool m_isLeftPenumbraPoint;
	CCPoint m_leftPenumbraT;
	bool m_isRightPenumbraPoint;
	CCPoint m_rightPenumbraT;
public:
	FKCW_Effect_2DSoftShadow_PointType();
};
//-------------------------------------------------------------------------
// 面的类型
enum ENUM_SideType
{
	eLeftSide=0,
	eRightSide,
	eOn,
};
//-------------------------------------------------------------------------
// 带2D软阴影的对象
class FKCW_Effect_2DSoftShadow_ShadowObj:public CCSprite
{
public:
	FKCW_Effect_2DSoftShadow_ShadowObj();
	virtual ~FKCW_Effect_2DSoftShadow_ShadowObj();

	bool	init(const vector<CCPoint>& polygon);
	void	update(float dt);
	bool	updateShadow();
	void	draw();
	void	setLight(FKCW_EffectNode_2DSoftShadow_LightNode*light);
	void	setOneDivObjCount(float value){m_oneDivObjCount=value;}
	float	getOneDivObjCount()const{return m_oneDivObjCount;}
	void	setIsDrawDebug(bool value){m_isDrawDebug=value;}
	bool	getIsDrawDebug()const{return m_isDrawDebug;}
	void	setIsDrawNonDebug(bool value){m_isDrawNonDebug=value;}
	bool	getIsDrawNonDebug()const{return m_isDrawNonDebug;}
protected:
	CCPoint _GetLightPosLocal();
	void	_MakeFullWindowRectMesh();
	void	_UpdateMesh();
	void	_Submit(GLenum usage);
protected:
	vector<CCPoint>							m_polygon;
	vector<FKCW_Effect_2DSoftShadow_PointType> m_pointTypeList;
	vector<CCPoint>							m_leftPenumbraLine;// 该值有三个元素，第一个是起始点，第二个是中间点，第三个是终止点
	vector<CCPoint>							m_rightPenumbraLine;
	vector<CCPoint>							m_leftUmbraLine;
	vector<CCPoint>							m_rightUmbraLine;
	int										m_leftPenumbraPointID;
	int										m_rightPenumbraPointID;
	int										m_leftUmbraPointID;
	int										m_rightUmbraPointID;
	CCPoint									m_intersectPoint;
	FKCW_EffectNode_2DSoftShadow_LightNode*	m_light;
	float									m_shadowLength;
	CCTexture2D*							m_finTexture;
	FKCW_EffectNode_Mesh*					m_mesh;
	FKCW_EffectNode_IndexVBO*				m_indexVBO;
	FKCW_EffectNode_GLProgramWithUnifos*	m_program;
	float									m_oneDivObjCount;
	bool									m_isDrawDebug;
	bool									m_isDrawNonDebug;
	bool									m_isUpdateShadowSucc;
};
//-------------------------------------------------------------------------
// 场景根节点
class C2DSoftShadowRoot:public CCNode
{
public:
	C2DSoftShadowRoot()
	{
		m_shadowRT=NULL;
		m_program=NULL;
		m_light=NULL;
		m_isDrawDebug=false;
		m_shadowDarkness=0.5;
	}
	virtual~C2DSoftShadowRoot(){
		if(m_shadowRT)m_shadowRT->release();
		if(m_program)m_program->release();
	}
	bool				init();
	void				setLight(FKCW_EffectNode_2DSoftShadow_LightNode*light);
	void				addObj(FKCW_Effect_2DSoftShadow_ShadowObj*obj);
	void				visit();
	void				setIsDrawDebug(bool value){m_isDrawDebug=value;}
	bool				getIsDrawDebug()const{return m_isDrawDebug;}
	void				setShadowDarkness(float value){m_shadowDarkness=value;}
	float				getShadowDarkness()const{return m_shadowDarkness;}
	CCRenderTexture*	getShadowRT(){return m_shadowRT;}
protected:
	CCRenderTexture*							m_shadowRT;
	FKCW_EffectNode_GLProgramWithUnifos*		m_program;
	vector<FKCW_Effect_2DSoftShadow_ShadowObj*> m_objList;
	FKCW_EffectNode_2DSoftShadow_LightNode*		m_light;
	bool										m_isDrawDebug;
	float										m_shadowDarkness;
};
//-------------------------------------------------------------------------