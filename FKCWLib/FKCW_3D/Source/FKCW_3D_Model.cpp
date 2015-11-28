//-------------------------------------------------------------------------
#include "../Include/FKCW_3D_Model.h"
//-------------------------------------------------------------------------
FKCW_3D_Model::FKCW_3D_Model()
{
	init_dft();
}
//-------------------------------------------------------------------------
FKCW_3D_Model::~FKCW_3D_Model()
{

}
//-------------------------------------------------------------------------
void FKCW_3D_Model::init_dft()
{

}
//-------------------------------------------------------------------------
FKCW_3D_Mesh* FKCW_3D_Model::getMeshByIndex(int index)
{
	assert(index>=0&&index<(int)m_meshList.size());
	return m_meshList[index];
}
//-------------------------------------------------------------------------
void FKCW_3D_Model::setLightSource(FKCW_3D_LightSource*light)
{
	this->FKCW_3D_Node::setLightSource(light);
	int nMesh=(int)m_meshList.size();
	for(int i=0;i<nMesh;i++){
		if(m_meshList[i]==NULL)continue;
		m_meshList[i]->setLightSource(light);
	}
}
//-------------------------------------------------------------------------
void FKCW_3D_Model::setProgram(FKCW_3D_Program *program)
{
	assert(program);
	this->FKCW_3D_Node::setProgram(program);

	int n=(int)m_meshList.size();
	for(int i=0;i<n;i++){
		FKCW_3D_Mesh*p=m_meshList[i];
		if(p==NULL)continue;
		p->setProgram(program);
	}
}
//-------------------------------------------------------------------------
void FKCW_3D_Model::setPassUnifoCallback(FKCW_3D_PassUnifoCallbackPtr callback)
{
	assert(callback);
	this->FKCW_3D_Node::setPassUnifoCallback(callback);

	int n=(int)m_meshList.size();
	for(int i=0;i<n;i++){
		FKCW_3D_Mesh*p=m_meshList[i];
		if(p==NULL)continue;
		p->setPassUnifoCallback(callback);
	}
}
//-------------------------------------------------------------------------
void FKCW_3D_Model::setCamera3D(FKCW_3D_Camera*camera3D)
{
	this->FKCW_3D_Node::setCamera3D(camera3D);

	int n=(int)m_meshList.size();
	for(int i=0;i<n;i++){
		FKCW_3D_Mesh*p=m_meshList[i];
		if(p==NULL)continue;
		p->setCamera3D(camera3D);
	}
}
//-------------------------------------------------------------------------
void FKCW_3D_Model::setIsDoDepthTest(bool value)
{
	int n=(int)m_meshList.size();
	for(int i=0;i<n;i++){
		FKCW_3D_Mesh*p=m_meshList[i];
		if(p==NULL)continue;
		p->setIsDoDepthTest(value);
	}
}
//-------------------------------------------------------------------------
void FKCW_3D_Model::addMesh(FKCW_3D_Mesh*mesh)
{
	assert(mesh);
	m_meshList.push_back(mesh);
	//  mesh->setName("?");
	this->addChild(mesh);
}
//-------------------------------------------------------------------------
void FKCW_3D_Model::submit(GLenum usage)
{
	int nMesh=(int)getMeshCount();
	for(int i=0;i<nMesh;i++){
		FKCW_3D_Mesh*mesh=m_meshList[i];
		mesh->submit(usage);
	}
}
//-------------------------------------------------------------------------
void FKCW_3D_Model::submitPosition(GLenum usage)
{
	int nMesh=(int)getMeshCount();
	for(int i=0;i<nMesh;i++){
		FKCW_3D_Mesh*mesh=m_meshList[i];
		mesh->submitPosition(usage);
	}
}
//-------------------------------------------------------------------------
void FKCW_3D_Model::submitNormal(GLenum usage)
{
	int nMesh=(int)getMeshCount();
	for(int i=0;i<nMesh;i++){
		FKCW_3D_Mesh*mesh=m_meshList[i];
		mesh->submitNormal(usage);
	}
}
//-------------------------------------------------------------------------
void FKCW_3D_Model::submitColor(GLenum usage)
{
	int nMesh=(int)getMeshCount();
	for(int i=0;i<nMesh;i++){
		FKCW_3D_Mesh*mesh=m_meshList[i];
		mesh->submitColor(usage);
	}
}
//-------------------------------------------------------------------------
void FKCW_3D_Model::submitTexCoord(GLenum usage)
{
	int nMesh=(int)getMeshCount();
	for(int i=0;i<nMesh;i++){
		FKCW_3D_Mesh*mesh=m_meshList[i];
		mesh->submitTexCoord(usage);
	}
}
//-------------------------------------------------------------------------
void FKCW_3D_Model::submitIndex(GLenum usage)
{
	int nMesh=(int)getMeshCount();
	for(int i=0;i<nMesh;i++)
	{
		FKCW_3D_Mesh*mesh=m_meshList[i];
		mesh->submitIndex(usage);
	}
}
//-------------------------------------------------------------------------