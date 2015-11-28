//*************************************************************************
//	创建日期:	2014-12-29
//	文件名称:	FKCW_3D_Actor.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_3D_Model.h"
//-------------------------------------------------------------------------
class FKCW_3D_Actor:public FKCW_3D_Node
{
protected:
	FKCW_3D_Model* m_model;
public:
	FKCW_3D_Actor();
	virtual~FKCW_3D_Actor();
	void init_dft();

	void setModel(FKCW_3D_Model*model);
	FKCW_3D_Model*getModel()const {return m_model;}
	void addMesh(FKCW_3D_Mesh*mesh);
	FKCW_3D_Mesh* getMeshByIndex(int index){return m_model->getMeshByIndex(index);}
	int getMeshCount(){return m_model->getMeshCount();}
	void setLightSource(FKCW_3D_LightSource*light);
	void setProgram(FKCW_3D_Program *program);
	void setPassUnifoCallback(FKCW_3D_PassUnifoCallbackPtr callback);
	void setIsDoDepthTest(bool value){m_model->setIsDoDepthTest(value);}
	void setCamera3D(FKCW_3D_Camera*camera3D);
	void submitPosition(GLenum usage){m_model->submitPosition(usage);}
	void submitNormal(GLenum usage){m_model->submitNormal(usage);}
	void submitColor(GLenum usage){m_model->submitColor(usage);}
	void submitTexCoord(GLenum usage){m_model->submitTexCoord(usage);}
	void submitIndex(GLenum usage){m_model->submitIndex(usage);}
	void submit(GLenum usage);
};
//-------------------------------------------------------------------------