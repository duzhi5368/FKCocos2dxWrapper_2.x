//-------------------------------------------------------------------------
#include "../Include/FKCW_3D_Mesh.h"
//-------------------------------------------------------------------------
FKCW_3D_Mesh::FKCW_3D_Mesh()
{

}
//-------------------------------------------------------------------------
FKCW_3D_Mesh::~FKCW_3D_Mesh()
{

}
//-------------------------------------------------------------------------
FKCW_3D_SubMesh*FKCW_3D_Mesh::getSubMeshByIndex(int index)
{
	assert(index>=0&&index<(int)m_subMeshList.size());
	return m_subMeshList[index];
}
//-------------------------------------------------------------------------
void FKCW_3D_Mesh::setIsDoDepthTest(bool isDoDepthTest)
{
	int nSubMesh=(int)m_subMeshList.size();
	for(int i=0;i<nSubMesh;i++){
		FKCW_3D_SubMesh*psubMesh=m_subMeshList[i];
		if(psubMesh==NULL)continue;
		psubMesh->setIsDoDepthTest(isDoDepthTest);
	}
}
//-------------------------------------------------------------------------
void FKCW_3D_Mesh::setCamera3D(FKCW_3D_Camera*camera3D)
{
	this->FKCW_3D_Node::setCamera3D(camera3D);

	int nSubMesh=(int)m_subMeshList.size();
	for(int i=0;i<nSubMesh;i++){
		FKCW_3D_SubMesh*psubMesh=m_subMeshList[i];
		if(psubMesh==NULL)continue;
		psubMesh->setCamera3D(camera3D);
	}
}
//-------------------------------------------------------------------------
void FKCW_3D_Mesh::setProgram(FKCW_3D_Program *program)
{
	assert(program);
	//    this->FKCW_3D_Node::setProgram(program);

	int nSubMesh=(int)m_subMeshList.size();
	for(int i=0;i<nSubMesh;i++){
		FKCW_3D_SubMesh*psubMesh=m_subMeshList[i];
		if(psubMesh==NULL)continue;
		psubMesh->setProgram(program);
	}
}
//-------------------------------------------------------------------------
void FKCW_3D_Mesh::setPassUnifoCallback(FKCW_3D_PassUnifoCallbackPtr callback)
{
	assert(callback);
	//  this->FKCW_3D_Node::setPassUnifoCallback(callback);

	int nSubMesh=(int)m_subMeshList.size();
	for(int i=0;i<nSubMesh;i++){
		FKCW_3D_SubMesh*psubMesh=m_subMeshList[i];
		if(psubMesh==NULL)continue;
		psubMesh->setPassUnifoCallback(callback);
	}
}
//-------------------------------------------------------------------------
void FKCW_3D_Mesh::setLightSource(FKCW_3D_LightSource*light)
{
	//  this->FKCW_3D_Node::setLightSource(light);

	int nSubMesh=(int)m_subMeshList.size();
	for(int i=0;i<nSubMesh;i++){
		FKCW_3D_SubMesh*psubMesh=m_subMeshList[i];
		if(psubMesh==NULL)continue;
		psubMesh->setLightSource(light);
	}
}
//-------------------------------------------------------------------------
void FKCW_3D_Mesh::setDiffuse(float r,float g,float b,float a)
{
	int nSubMesh=(int)m_subMeshList.size();
	for(int i=0;i<nSubMesh;i++){
		FKCW_3D_SubMesh*psubMesh=m_subMeshList[i];
		if(psubMesh==NULL)continue;
		psubMesh->getMaterial()->setDiffuse(FKCW_3D_Vector4(r, g, b,a));
	}
}
//-------------------------------------------------------------------------
void FKCW_3D_Mesh::setTexture(CCTexture2D*texture)
{
	int nSubMesh=(int)m_subMeshList.size();
	for(int i=0;i<nSubMesh;i++){
		FKCW_3D_SubMesh*psubMesh=m_subMeshList[i];
		if(psubMesh==NULL)continue;
		psubMesh->setTexture(texture);
	}
}
//-------------------------------------------------------------------------
void FKCW_3D_Mesh::setMaterial(FKCW_3D_Material*material)
{
	int nSubMesh=(int)m_subMeshList.size();
	for(int i=0;i<nSubMesh;i++){
		FKCW_3D_SubMesh*psubMesh=m_subMeshList[i];
		if(psubMesh==NULL)continue;
		psubMesh->setMaterial(material);
	}
}
//-------------------------------------------------------------------------
void FKCW_3D_Mesh::setAmbient(float r,float g,float b,float a)
{
	int nSubMesh=(int)m_subMeshList.size();
	for(int i=0;i<nSubMesh;i++){
		FKCW_3D_SubMesh*psubMesh=m_subMeshList[i];
		if(psubMesh==NULL)continue;
		psubMesh->getMaterial()->setAmbient(FKCW_3D_Vector4(r, g, b,a));
	}
}
//-------------------------------------------------------------------------
void FKCW_3D_Mesh::setSpecular(float r,float g,float b,float a)
{
	int nSubMesh=(int)m_subMeshList.size();
	for(int i=0;i<nSubMesh;i++){
		FKCW_3D_SubMesh*psubMesh=m_subMeshList[i];
		if(psubMesh==NULL)continue;
		psubMesh->getMaterial()->setSpecular(FKCW_3D_Vector4(r, g, b, a));
	}
}
//-------------------------------------------------------------------------
void FKCW_3D_Mesh::setShininess(float shininess)
{
	int nSubMesh=(int)m_subMeshList.size();
	for(int i=0;i<nSubMesh;i++){
		FKCW_3D_SubMesh*psubMesh=m_subMeshList[i];
		if(psubMesh==NULL)continue;
		psubMesh->getMaterial()->setShininess(shininess);
	}
}
//-------------------------------------------------------------------------
void FKCW_3D_Mesh::submit(GLenum usage)
{
	int nSubMesh=(int)m_subMeshList.size();
	for(int i=0;i<nSubMesh;i++){
		FKCW_3D_SubMesh*psubMesh=m_subMeshList[i];
		if(psubMesh)psubMesh->submit(usage);
	}
}
//-------------------------------------------------------------------------
void FKCW_3D_Mesh::submitPosition(GLenum usage)
{
	int nSubMesh=(int)m_subMeshList.size();
	for(int i=0;i<nSubMesh;i++){
		FKCW_3D_SubMesh*psubMesh=m_subMeshList[i];
		if(psubMesh)psubMesh->submitPosition(usage);
	}
}
//-------------------------------------------------------------------------
void FKCW_3D_Mesh::submitNormal(GLenum usage)
{
	int nSubMesh=(int)m_subMeshList.size();
	for(int i=0;i<nSubMesh;i++){
		FKCW_3D_SubMesh*psubMesh=m_subMeshList[i];
		if(psubMesh)psubMesh->submitNormal(usage);
	}
}
//-------------------------------------------------------------------------
void FKCW_3D_Mesh::submitColor(GLenum usage)
{
	int nSubMesh=(int)m_subMeshList.size();
	for(int i=0;i<nSubMesh;i++){
		FKCW_3D_SubMesh*psubMesh=m_subMeshList[i];
		if(psubMesh)psubMesh->submitColor(usage);
	}
}
//-------------------------------------------------------------------------
void FKCW_3D_Mesh::submitTexCoord(GLenum usage)
{
	int nSubMesh=(int)m_subMeshList.size();
	for(int i=0;i<nSubMesh;i++){
		FKCW_3D_SubMesh*psubMesh=m_subMeshList[i];
		if(psubMesh)psubMesh->submitTexCoord(usage);
	}
}
//-------------------------------------------------------------------------
void FKCW_3D_Mesh::submitIndex(GLenum usage)
{
	int nSubMesh=(int)m_subMeshList.size();
	for(int i=0;i<nSubMesh;i++){
		FKCW_3D_SubMesh*psubMesh=m_subMeshList[i];
		if(psubMesh)psubMesh->submitIndex(usage);
	}
}
//-------------------------------------------------------------------------
void FKCW_3D_Mesh::addSubMesh(FKCW_3D_SubMesh*submesh)
{
	assert(submesh);
	m_subMeshList.push_back(submesh);
	//submesh->setName("?");
	this->addChild(submesh);
}
//-------------------------------------------------------------------------