//-------------------------------------------------------------------------
#include "../Include/FKCW_3D_MatStackInfoGetor.h"
//-------------------------------------------------------------------------
static FKCW_3D_MatStackInfoGetor *s_sharedMatStackInfoGetor = NULL;
//-------------------------------------------------------------------------
FKCW_3D_MatStackInfoGetor::FKCW_3D_MatStackInfoGetor()
{

}
//-------------------------------------------------------------------------
FKCW_3D_MatStackInfoGetor::~FKCW_3D_MatStackInfoGetor()
{

}
//-------------------------------------------------------------------------
FKCW_3D_MatStackInfoGetor* FKCW_3D_MatStackInfoGetor::sharedMatStackInfoGetor()
{
	if (!s_sharedMatStackInfoGetor)
	{
		s_sharedMatStackInfoGetor = new FKCW_3D_MatStackInfoGetor();
		s_sharedMatStackInfoGetor->init();
	}
	return s_sharedMatStackInfoGetor;
}
//-------------------------------------------------------------------------
bool FKCW_3D_MatStackInfoGetor::init()
{
	return true;
}
//-------------------------------------------------------------------------
FKCW_3D_Matrix4 FKCW_3D_MatStackInfoGetor::getTopMat(ENUM_FKCW_3D_MatStackName eMatStackName)
{
	kmMat4 Mat;
	switch (eMatStackName) 
	{
	case eFKCW3D_ModelViewStack:
		kmGLGetMatrix(KM_GL_MODELVIEW,&Mat);
		break;
	case eFKCW3D_ProjectionStack:
		kmGLGetMatrix(KM_GL_PROJECTION,&Mat);
		break;
	case eFKCW3D_TextureStack:
		kmGLGetMatrix(KM_GL_TEXTURE,&Mat);
		break;
	default:
		assert(false);
		break;
	}
	return FKCW_3D_Matrix4(Mat.mat);
}
//-------------------------------------------------------------------------