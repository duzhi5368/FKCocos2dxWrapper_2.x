//*************************************************************************
//	创建日期:	2014-12-29
//	文件名称:	FKCW_3D_SubMesh.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_3D_Macro.h"
#include "FKCW_3D_Matrix4.h"
#include "FKCW_3D_IndexVBO3D.h"
#include "FKCW_3D_SubMeshData.h"
#include "FKCW_3D_CommonFunc.h"
#include "FKCW_3D_DefaultPrograms.h"
#include "FKCW_3D_Material.h"
#include "FKCW_3D_Camera.h"
#include "FKCW_3D_Node.h"
//-------------------------------------------------------------------------
class FKCW_3D_SubMesh : public FKCW_3D_Node
{
public:
	FKCW_3D_SubMesh();
	virtual ~FKCW_3D_SubMesh();

	bool init();
	void setTexture(CCTexture2D*texture);
	void setMaterial(FKCW_3D_Material*material);
	void setSubMeshData(FKCW_3D_SubMeshData*subMeshData);
	FKCW_3D_SubMeshData*getSubMeshData(){return m_subMeshData;}
	void setIndexVBO(FKCW_3D_IndexVBO3D*indexVBO);

	void submit(GLenum usage);
	void submitPosition(GLenum usage);
	void submitNormal(GLenum usage);
	void submitColor(GLenum usage);
	void submitTexCoord(GLenum usage);
	void submitIndex(GLenum usage);
	void draw();

	FKCW_3D_Material*getMaterial(){return m_material;}
	bool getIsDoDepthTest()const;
	void setIsDoDepthTest(bool isDoDepthTest);
	void setBlendFunc(ccBlendFunc blendFunc){m_blendFunc = blendFunc;}
	ccBlendFunc getBlendFunc(){return m_blendFunc;}
protected:
	CCTexture2D* m_texture;
	FKCW_3D_Material* m_material;
	FKCW_3D_SubMeshData* m_subMeshData;
	FKCW_3D_IndexVBO3D* m_indexVBO;
	bool m_isDoDepthTest;
	ccBlendFunc m_blendFunc;
};
//-------------------------------------------------------------------------