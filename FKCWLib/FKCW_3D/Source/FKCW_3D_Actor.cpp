//-------------------------------------------------------------------------
#include "../Include/FKCW_3D_Actor.h"
//-------------------------------------------------------------------------
FKCW_3D_Actor::FKCW_3D_Actor()
{
	m_model=NULL;
	init_dft();
}
//-------------------------------------------------------------------------
FKCW_3D_Actor::~FKCW_3D_Actor()
{
}
//-------------------------------------------------------------------------
void FKCW_3D_Actor::init_dft()
{
	FKCW_3D_Model*model=new FKCW_3D_Model();
	model->autorelease();
	model->init();
	model->setNodeName("model");
	setModel(model);
}
//-------------------------------------------------------------------------
void FKCW_3D_Actor::setLightSource(FKCW_3D_LightSource*light)
{
	this->FKCW_3D_Node::setLightSource(light);
	m_model->setLightSource(light);
}
//-------------------------------------------------------------------------
void FKCW_3D_Actor::setProgram(FKCW_3D_Program *program)
{
	this->FKCW_3D_Node::setProgram(program);
	m_model->setProgram(program);
}
//-------------------------------------------------------------------------
void FKCW_3D_Actor::setPassUnifoCallback(FKCW_3D_PassUnifoCallbackPtr callback)
{
	this->FKCW_3D_Node::setPassUnifoCallback(callback);
	m_model->setPassUnifoCallback(callback);
}
//-------------------------------------------------------------------------
void FKCW_3D_Actor::setCamera3D(FKCW_3D_Camera*camera3D)
{
	this->FKCW_3D_Node::setCamera3D(camera3D);
	m_model->setCamera3D(camera3D);
}
//-------------------------------------------------------------------------
void FKCW_3D_Actor::setModel(FKCW_3D_Model*model)
{
	if(m_model==NULL){
		m_model=model;
		addChild(m_model);
	}else{
		m_model->removeFromParent();
		m_model=model;
		addChild(m_model);
	}
}
//-------------------------------------------------------------------------
void FKCW_3D_Actor::addMesh(FKCW_3D_Mesh*mesh)
{
	assert(mesh);
	m_model->addMesh(mesh);
}
//-------------------------------------------------------------------------
void FKCW_3D_Actor::submit(GLenum usage)
{
	m_model->submit(usage);
}
//-------------------------------------------------------------------------