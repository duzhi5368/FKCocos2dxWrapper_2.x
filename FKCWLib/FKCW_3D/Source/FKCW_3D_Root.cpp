//-------------------------------------------------------------------------
#include "../Include/FKCW_3D_Root.h"
//-------------------------------------------------------------------------
FKCW_3D_Root::FKCW_3D_Root()
{
}
//-------------------------------------------------------------------------
FKCW_3D_Root::~FKCW_3D_Root()
{
}
//-------------------------------------------------------------------------
bool FKCW_3D_Root::init()
{
	this->FKCW_3D_Node::init();
	return true;
}
//-------------------------------------------------------------------------
void FKCW_3D_Root::visit()
{
	//store depthTest state
	GLboolean isDoDepthTestOld;
	glGetBooleanv(GL_DEPTH_TEST,&isDoDepthTestOld);
	//push matrix
	kmGLMatrixMode(KM_GL_PROJECTION);
	kmGLPushMatrix();
	kmGLMatrixMode(KM_GL_MODELVIEW);
	kmGLPushMatrix();
	{
		//apply projection matrix
		getCamera3D()->applyProjection();
		//no matter what the modelview is, set it to identity
		kmGLMatrixMode(KM_GL_MODELVIEW);
		kmGLLoadIdentity();
		//apply view matrix
		getCamera3D()->locate();

		//call FKCW_3D_Node's visit
		FKCW_3D_Node::visit();
	}
	//pop matrix
	kmGLMatrixMode(KM_GL_PROJECTION);
	kmGLPopMatrix();
	kmGLMatrixMode(KM_GL_MODELVIEW);
	kmGLPopMatrix();
	//resotre depthTest state
	CCDirector::sharedDirector()->setDepthTest(static_cast<bool>(isDoDepthTestOld));
	//disable array attribute
	// KCW_3D_IndexVBO3d::disableAttribArrays();
}
//-------------------------------------------------------------------------