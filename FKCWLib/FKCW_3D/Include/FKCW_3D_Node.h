//*************************************************************************
//	创建日期:	2014-11-27
//	文件名称:	FKCW_3D_Node.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_3D_Macro.h"
#include "FKCW_3D_Math.h"
#include "FKCW_3D_Vector.h"
#include "FKCW_3D_CommonFunc.h"
#include "FKCW_3D_GLMath.h"
#include "FKCW_3D_Camera.h"
#include "FKCW_3D_DefaultPrograms.h"
#include "FKCW_3D_Programs.h"
#include "FKCW_3D_DefaultPassUniformCallback.h"
//-------------------------------------------------------------------------
class FKCW_3D_Node;
typedef void(*FKCW_3D_PassUnifoCallbackPtr)(FKCW_3D_Node*, FKCW_3D_Program*);
class FKCW_3D_LightSource;
//-------------------------------------------------------------------------
class FKCW_3D_Node:public CCNode
{
public:
	FKCW_3D_Node();
	virtual~FKCW_3D_Node();
	static FKCW_3D_Node*create();
	virtual bool init();
	virtual void visit();
	virtual bool isSceneNode3D(){return false;}
	// transform
	virtual void setTransform3D(const FKCW_3D_Matrix4&mat);
	virtual FKCW_3D_Matrix4 getTransform3D();
	virtual FKCW_3D_Matrix4 nodeToWorldTransform3D();
	virtual FKCW_3D_Matrix4 worldToNodeTransform3D();
	virtual FKCW_3D_Matrix4 nodeToParentTransform3D();
	virtual FKCW_3D_Vector4 convertToWorldSpace3D(const FKCW_3D_Vector4&v);
	virtual FKCW_3D_Vector4 convertToNodeSpace3D(const FKCW_3D_Vector4&v);
	// translate
	virtual void setPosition3D(const FKCW_3D_Vector4&position);
	virtual FKCW_3D_Vector4 getPosition3D()const;
	virtual FKCW_3D_Vector4 getWorldPosition3D();
	virtual void translate3D(const FKCW_3D_Vector4&translate);
	virtual void translateRelativeToParent3D(const FKCW_3D_Vector4&translate);
	// rotate
	virtual void setRotation3D(const FKCW_3D_Vector4&axis,float cosA,float sinA);
	virtual void setRotation3D(const FKCW_3D_Vector4&axis,float A);
	virtual FKCW_3D_Matrix4 getRotation3D();
	virtual void rotate3D(const FKCW_3D_Vector4&axis,float cosA,float sinA);
	virtual void rotate3D(const FKCW_3D_Vector4&axis,float A);
	virtual void rotateRelativeToParent3D(const FKCW_3D_Vector4&axis,float cosA,float sinA);
	virtual void rotateRelativeToParent3D(const FKCW_3D_Vector4&axis,float A);
	// scale
	virtual void scale3D(float kx,float ky,float kz);
	virtual void scaleRelativeToParent3D(float kx,float ky,float kz);
	FKCW_3D_Camera* getCamera3D(){return m_camera;}
	void setCamera3D(FKCW_3D_Camera*camera);
	// light, passUnifoCallback, program
	FKCW_3D_LightSource*getLightSource(){return m_lightSource;}
	void setLightSource(FKCW_3D_LightSource*lightSource);
	void setPassUnifoCallback(FKCW_3D_PassUnifoCallbackPtr passUnifoCallback);
	void setProgram(FKCW_3D_Program*program);
	void setNodeName(string name){m_nodeName=name;}
	string getNodeName()const{return m_nodeName;}
protected:
	virtual void transform3D();
protected:
	FKCW_3D_Matrix4					m_mat;
	FKCW_3D_Camera*					m_camera;
	FKCW_3D_LightSource*			m_lightSource;
	FKCW_3D_Program*				m_program;
	FKCW_3D_PassUnifoCallbackPtr	m_passUnifoCallback;
	string							m_nodeName;
};
//-------------------------------------------------------------------------