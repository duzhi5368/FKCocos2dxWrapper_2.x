//*************************************************************************
//	创建日期:	2014-12-29
//	文件名称:	FKCW_3D_Mesh.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_3D_SubMesh.h"
//-------------------------------------------------------------------------
class FKCW_3D_Mesh:public FKCW_3D_Node
{
protected:
	vector<FKCW_3D_SubMesh*> m_subMeshList;		//网格列表

public:
	FKCW_3D_Mesh();
	virtual ~FKCW_3D_Mesh();

	int getSubMeshCount()const{return (int)m_subMeshList.size();}
	FKCW_3D_SubMesh*getSubMeshByIndex(int index);

	void setIsDoDepthTest(bool isDoDepthTest);
	void setTexture(CCTexture2D*texture);
	void setMaterial(FKCW_3D_Material*material);
	void setCamera3D(FKCW_3D_Camera*camera3D);
	void setProgram(FKCW_3D_Program *program);
	void setPassUnifoCallback(FKCW_3D_PassUnifoCallbackPtr callback);
	void setLightSource(FKCW_3D_LightSource*light);
	void setDiffuse(float r,float g,float b,float a);
	void setAmbient(float r,float g,float b,float a);
	void setSpecular(float r,float g,float b,float a);
	void setShininess(float shininess);
	void submit(GLenum usage);
	void submitPosition(GLenum usage);
	void submitNormal(GLenum usage);
	void submitColor(GLenum usage);
	void submitTexCoord(GLenum usage);
	void submitIndex(GLenum usage);
	void addSubMesh(FKCW_3D_SubMesh*submesh);
};
//-------------------------------------------------------------------------