//*************************************************************************
//	创建日期:	2014-12-29
//	文件名称:	FKCW_3D_Model.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_3D_Mesh.h"
//-------------------------------------------------------------------------
class FKCW_3D_Model:public FKCW_3D_Node
{
protected:
	vector<FKCW_3D_Mesh*> m_meshList;
public:
	FKCW_3D_Model();
	virtual~FKCW_3D_Model();

	void init_dft();
	int getMeshCount()const{return (int)m_meshList.size();}
	FKCW_3D_Mesh*getMeshByIndex(int index);
	void setLightSource(FKCW_3D_LightSource*light);
	void setProgram(FKCW_3D_Program *program);
	void setPassUnifoCallback(FKCW_3D_PassUnifoCallbackPtr callback);
	void setCamera3D(FKCW_3D_Camera*camera3D);
	void setIsDoDepthTest(bool value);
	void addMesh(FKCW_3D_Mesh*mesh);
	void submit(GLenum usage);
	void submitPosition(GLenum usage);
	void submitNormal(GLenum usage);
	void submitColor(GLenum usage);
	void submitTexCoord(GLenum usage);
	void submitIndex(GLenum usage);
};
