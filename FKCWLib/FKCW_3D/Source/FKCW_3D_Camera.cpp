//-------------------------------------------------------------------------
#include "../Include/FKCW_3D_Camera.h"
//-------------------------------------------------------------------------
FKCW_3D_Camera::FKCW_3D_Camera()
{
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	m_fovy=60;
	m_aspect=winSize.width/winSize.height;
	m_zNear=1;
	m_zFar=1500;

	const float w=winSize.width;
	const float h=winSize.height;
	m_range.init(-w/2, -w/2+w, -h/2, -h/2+h,
		-1024,
		1024);
	m_projectionMode=eFKCW3D_PerspectiveMode;
	m_isViewMatDirty=false;
	m_isViewMatInverseDirty=false;
}
//-------------------------------------------------------------------------
FKCW_3D_Camera::~FKCW_3D_Camera()
{

}
//-------------------------------------------------------------------------
FKCW_3D_Vector4 FKCW_3D_Camera::getEyePos()const
{
	//getEyeXYZ is not const, but i want getEyePos to be const
	//so i convert this from const to nonconst
	float eyex,eyey,eyez;
	((FKCW_3D_Camera*)this)->getEyeXYZ(&eyex, &eyey, &eyez);
	return FKCW_3D_Vector4(eyex,eyey,eyez,1);
}
//-------------------------------------------------------------------------
FKCW_3D_Vector4 FKCW_3D_Camera::getCenter()const
{
	float centerX,centerY,centerZ;
	((FKCW_3D_Camera*)this)->getCenterXYZ(&centerX, &centerY, &centerZ);
	return FKCW_3D_Vector4(centerX,centerY,centerZ,1);
}
//-------------------------------------------------------------------------
FKCW_3D_Vector4 FKCW_3D_Camera::getUp()const
{
	float upX,upY,upZ;
	((FKCW_3D_Camera*)this)->getUpXYZ(&upX, &upY, &upZ);
	return FKCW_3D_Vector4(upX,upY,upZ,0);
}
//-------------------------------------------------------------------------
void FKCW_3D_Camera::setEyePos(const FKCW_3D_Vector4&eyePos)
{
	this->setEyeXYZ(eyePos.x(), eyePos.y(), eyePos.z());
	m_isViewMatDirty=true;
	m_isViewMatInverseDirty=true;
}
//-------------------------------------------------------------------------
void FKCW_3D_Camera::setCenter(const FKCW_3D_Vector4&center)
{
	this->setCenterXYZ(center.x(), center.y(), center.z());
	m_isViewMatDirty=true;
	m_isViewMatInverseDirty=true;
}
//-------------------------------------------------------------------------
void FKCW_3D_Camera::setUp(const FKCW_3D_Vector4&up)
{
	this->setUpXYZ(up.x(), up.y(), up.z());
	m_isViewMatDirty=true;
	m_isViewMatInverseDirty=true;
}
//-------------------------------------------------------------------------
FKCW_3D_Matrix4 FKCW_3D_Camera::calculateViewMat()
{
	FKCW_3D_Matrix4 ret;
	if(m_isViewMatDirty)
	{
		ret=::calculateViewMatrix(getEyePos(), getCenter(), getUp());
		m_viewMatCache=ret;
	}
	else
	{
		ret=m_viewMatCache;
	}
	return ret;
}
//-------------------------------------------------------------------------
FKCW_3D_Matrix4 FKCW_3D_Camera::calculateViewMatInverse()
{
	FKCW_3D_Matrix4 ret;
	if(m_isViewMatInverseDirty)
	{
		ret=::calculateViewMatrixInverse(getEyePos(), getCenter(), getUp());
		m_viewMatInverseCache=ret;
	}
	else
	{
		ret=m_viewMatInverseCache;
	}
	return ret;
}
//-------------------------------------------------------------------------
FKCW_3D_Matrix4 FKCW_3D_Camera::calculateProjectionMat()
{
	FKCW_3D_Matrix4 projectionMat;
	switch (m_projectionMode) 
	{
	case eFKCW3D_PerspectiveMode:
		projectionMat=::calculatePerspectiveProjectionMatrix(m_fovy, m_aspect, m_zNear, m_zFar);
		break;
	case eFKCW3D_OrthographicMode:
		projectionMat=::calculateOrthoProjectionMatrix(m_range.getMinX(), m_range.getMaxX(), m_range.getMinY(), m_range.getMaxY(), m_range.getMinZ(), m_range.getMaxZ());
		break;
	default:
		assert(false);
		break;
	}
	return projectionMat;
}
//-------------------------------------------------------------------------
void FKCW_3D_Camera::applyProjection()
{ 
	FKCW_3D_Matrix4 projectionMat=calculateProjectionMat();
	kmGLMatrixMode(KM_GL_PROJECTION);
	kmGLLoadIdentity();
	kmMat4 projMat;
	memcpy(projMat.mat, projectionMat.getArray(), 16*sizeof(float));
	kmGLMultMatrix(&projMat);
	kmGLMatrixMode(KM_GL_MODELVIEW);
};
//-------------------------------------------------------------------------
void FKCW_3D_Camera::printProjectionMode()const
{
	if(m_projectionMode==eFKCW3D_PerspectiveMode)
	{
		FKLOG("projectionMode: perspectiveMode");
	}
	else if(m_projectionMode==eFKCW3D_OrthographicMode)
	{
		FKLOG("projectionMode: orthographic");
	}
	else
	{
		assert(false);
	}
}
//-------------------------------------------------------------------------