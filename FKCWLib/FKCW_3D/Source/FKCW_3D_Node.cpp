//-------------------------------------------------------------------------
#include "../Include/FKCW_3D_Node.h"
#include "../Include/FKCW_3D_LightSource.h"
//-------------------------------------------------------------------------
FKCW_3D_Node::FKCW_3D_Node()
{
	m_mat=unitMat();
	m_camera=NULL;
	m_lightSource=NULL;
	m_program=NULL;
	m_passUnifoCallback=NULL;
}
//-------------------------------------------------------------------------
FKCW_3D_Node::~FKCW_3D_Node()
{
	if(m_camera)m_camera->release();
	if(m_lightSource)m_lightSource->release();
	if(m_program)m_program->release();
}
//-------------------------------------------------------------------------
bool FKCW_3D_Node::init()
{
	FKCW_3D_Camera*camera=new FKCW_3D_Camera();
	camera->autorelease();
	camera->init();
	setCamera3D(camera);
	return true;
};
//-------------------------------------------------------------------------
FKCW_3D_Vector4 FKCW_3D_Node::getPosition3D()const 
{
	FKCW_3D_Vector4 position(m_mat.getAt(12),m_mat.getAt(13),m_mat.getAt(14),1);
	return position;
}
//-------------------------------------------------------------------------
FKCW_3D_Vector4 FKCW_3D_Node::getWorldPosition3D() 
{
	return convertToWorldSpace3D(FKCW_3D_Vector4(0, 0, 0, 1));
}
//-------------------------------------------------------------------------
void FKCW_3D_Node::setPosition3D(const FKCW_3D_Vector4&position)
{
	m_mat.setAt(12, position.x());
	m_mat.setAt(13, position.y());
	m_mat.setAt(14, position.z());
}
//-------------------------------------------------------------------------
FKCW_3D_Matrix4 FKCW_3D_Node::nodeToWorldTransform3D()
{
	FKCW_3D_Matrix4 t = this->m_mat;
	for (FKCW_3D_Node *p = (FKCW_3D_Node*)this->getParent(); p != NULL; p = (FKCW_3D_Node*)p->getParent()){
		t = p->nodeToParentTransform3D()*t;
		if(p->isSceneNode3D())break;//seneNode's matrix already count in
	}
	return t;
}
//-------------------------------------------------------------------------
FKCW_3D_Matrix4 FKCW_3D_Node::worldToNodeTransform3D()
{
	FKCW_3D_Matrix4 nodeToWorld=nodeToWorldTransform3D();
	FKCW_3D_Matrix4 worldToNode=inverse(nodeToWorld);
	return worldToNode;
}
//-------------------------------------------------------------------------
FKCW_3D_Vector4 FKCW_3D_Node::convertToWorldSpace3D(const FKCW_3D_Vector4&v)
{
	FKCW_3D_Matrix4 nodeToWorldTransform=nodeToWorldTransform3D();
	FKCW_3D_Vector4 rs=nodeToWorldTransform*v;
	return rs;
}
//-------------------------------------------------------------------------
FKCW_3D_Vector4 FKCW_3D_Node::convertToNodeSpace3D(const FKCW_3D_Vector4&v)
{
	FKCW_3D_Matrix4 worldToNodeTransform=worldToNodeTransform3D();
	FKCW_3D_Vector4 rs=worldToNodeTransform*v;
	return rs;
}
//-------------------------------------------------------------------------
FKCW_3D_Matrix4 FKCW_3D_Node::nodeToParentTransform3D()
{
	return m_mat;
}
//-------------------------------------------------------------------------
void FKCW_3D_Node::transform3D()
{   // visit()中调用
	//for ordinary FKCW_3D_Node, we only mult matrix, not locate camrea
	kmMat4 transfrom4x4;
	memcpy(transfrom4x4.mat, m_mat.getArray(), 16*sizeof(float));
	kmGLMultMatrix( &transfrom4x4 );
}
//-------------------------------------------------------------------------
void FKCW_3D_Node::rotate3D(const FKCW_3D_Vector4& axis,float cosA,float sinA)
{
	// axis必须是个单元向量
	assert(axis.w()==0);
	assert(getLength2(axis)==1);
	FKCW_3D_Matrix4 RMat=calculateRotationMatrix(axis, cosA, sinA);
	m_mat=m_mat*RMat;
};
//-------------------------------------------------------------------------
void FKCW_3D_Node::rotateRelativeToParent3D(const FKCW_3D_Vector4&axis,float cosA,float sinA)
{
	// axis必须是个单元向量
	assert(axis.w()==0);
	assert(getLength2(axis)==1);
	FKCW_3D_Matrix4 RMat=calculateRotationMatrix(axis, cosA, sinA);
	m_mat=RMat*m_mat;
}
//-------------------------------------------------------------------------
void FKCW_3D_Node::translate3D(const FKCW_3D_Vector4&translate)
{
	assert(translate.w()==0);
	FKCW_3D_Matrix4 TMat=calculateTranslateMatrix(translate);
	m_mat=m_mat*TMat;
}
//-------------------------------------------------------------------------
void FKCW_3D_Node::translateRelativeToParent3D(const FKCW_3D_Vector4&translate)
{
	assert(translate.w()==0);
	FKCW_3D_Matrix4 TMat=calculateTranslateMatrix(translate);
	m_mat=TMat*m_mat;
}
//-------------------------------------------------------------------------
void FKCW_3D_Node::setRotation3D(const FKCW_3D_Vector4&axis,float cosA,float sinA)
{
	assert(axis.w()==0);
	FKCW_3D_Matrix4 RMat=calculateRotationMatrix(axis, cosA, sinA);
	//replace left up 3x3 sub matrix of m_mat with RMat
	const int indexs[9]={0,1,2,4,5,6,8,9,10};
	for(int i=0;i<9;i++){
		int index=indexs[i];
		m_mat.setAt(index, RMat.getAt(index));
	}
}
//-------------------------------------------------------------------------
void FKCW_3D_Node::rotate3D(const FKCW_3D_Vector4&axis,float A)
{
	float fValue = static_cast<float>(A*M_PI/180.0f);
	rotate3D(axis, cosf(fValue), sinf(fValue));
}
//-------------------------------------------------------------------------
void FKCW_3D_Node::setRotation3D(const FKCW_3D_Vector4&axis,float A)
{
	float fValue = static_cast<float>(A*M_PI/180.0f);
	setRotation3D(axis, cosf(fValue), sinf(fValue));
}
//-------------------------------------------------------------------------
void FKCW_3D_Node::rotateRelativeToParent3D(const FKCW_3D_Vector4&axis,float A)
{
	float fValue = static_cast<float>(A*M_PI/180.0f);
	rotateRelativeToParent3D(axis,cosf(fValue), sinf(fValue));
}
//-------------------------------------------------------------------------
FKCW_3D_Matrix4 FKCW_3D_Node::getRotation3D()
{
	FKCW_3D_Matrix4 RMat=unitMat();
	//replace left up 3x3 sub matrix of RMat with m_mat
	const int indexs[9]={0,1,2,4,5,6,8,9,10};
	for(int i=0;i<9;i++){
		int index=indexs[i];
		RMat.setAt(index, m_mat.getAt(index));
	}
	return RMat;
}
//-------------------------------------------------------------------------
void FKCW_3D_Node::setTransform3D(const FKCW_3D_Matrix4&mat)
{
	m_mat=mat;
}
//-------------------------------------------------------------------------
FKCW_3D_Matrix4 FKCW_3D_Node::getTransform3D()
{
	return m_mat;
}
//-------------------------------------------------------------------------
void FKCW_3D_Node::scale3D(float kx,float ky,float kz)
{
	FKCW_3D_Matrix4 SMat=calculateScaleMatrix(kx, ky, kz);
	m_mat=m_mat*SMat;
}
//-------------------------------------------------------------------------
void FKCW_3D_Node::scaleRelativeToParent3D(float kx,float ky,float kz)
{
	FKCW_3D_Matrix4 SMat=calculateScaleMatrix(kx, ky, kz);
	m_mat=SMat*m_mat;
}
//-------------------------------------------------------------------------
void FKCW_3D_Node::setCamera3D(FKCW_3D_Camera*camera)
{
	assert(camera);
	setRCObject(m_camera, camera);
}
//-------------------------------------------------------------------------
void FKCW_3D_Node::setProgram(FKCW_3D_Program*program)
{
	assert(program);
	setRCObject(m_program, program);
	CCNode::setShaderProgram(m_program);
}
//-------------------------------------------------------------------------
void FKCW_3D_Node::setPassUnifoCallback(FKCW_3D_PassUnifoCallbackPtr passUnifoCallback)
{
	m_passUnifoCallback=passUnifoCallback;
}
//-------------------------------------------------------------------------
void FKCW_3D_Node::setLightSource(FKCW_3D_LightSource*lightSource)
{
	assert(lightSource);
	setRCObject(m_lightSource,lightSource);
}
//-------------------------------------------------------------------------
// 3D渲染，z-Order依然有效
void FKCW_3D_Node::visit()
{
	// 如果不需要被渲染，则直接return，子类也不再被绘制
	if (!m_bVisible)
	{
		return;
	}
	kmGLPushMatrix();

	if (m_pGrid && m_pGrid->isActive())
	{
		m_pGrid->beforeDraw();
	}

	this->transform3D();

	FKCW_3D_Node* pNode = NULL;
	unsigned int i = 0;

	if(m_pChildren && m_pChildren->count() > 0)
	{
		sortAllChildren();

		ccArray *arrayData = m_pChildren->data;
		for( ; i < arrayData->num; i++ )
		{
			pNode = (FKCW_3D_Node*) arrayData->arr[i];

			if ( pNode && pNode->getZOrder() < 0 )
			{
				pNode->visit();
			}
			else
			{
				break;
			}
		}
		// 渲染自己
		this->draw();

		for( ; i < arrayData->num; i++ )
		{
			pNode = (FKCW_3D_Node*) arrayData->arr[i];
			if (pNode)
			{
				pNode->visit();
			}
		}
	}
	else
	{
		this->draw();
	}

	// 等待下一帧
	m_uOrderOfArrival = 0;

	if (m_pGrid && m_pGrid->isActive())
	{
		m_pGrid->afterDraw(this);
	}

	kmGLPopMatrix();
}
//-------------------------------------------------------------------------