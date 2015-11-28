//-------------------------------------------------------------------------
#include "../Include/FKCW_3D_DefaultPassUniformCallback.h"
#include "../Include/FKCW_3D_SubMesh.h"
#include "../Include/FKCW_3D_Node.h"
#include "../Include/FKCW_3D_LightSource.h"
//-------------------------------------------------------------------------
void PassUnifoCallback_TexOnly(FKCW_3D_Node* node,FKCW_3D_Program* program)
{
	assert(program->getName()=="TexOnly");
}
//-------------------------------------------------------------------------
void PassUnifoCallback_ClassicLighting(FKCW_3D_Node* node,FKCW_3D_Program* program)
{
	assert(program->getName()=="ClassicLighting");
	FKCW_3D_SubMesh* sprite=(FKCW_3D_SubMesh*)node;
	FKCW_3D_Matrix4 modelMat;
	{
		FKCW_3D_Matrix4 VMatInverse,VMMat;
		VMatInverse=sprite->getCamera3D()->calculateViewMatInverse();
		VMMat=FKCW_3D_MatStackInfoGetor::sharedMatStackInfoGetor()->getTopMat(eFKCW3D_ModelViewStack);
		modelMat=VMatInverse*VMMat;
	}

	FKCW_3D_Matrix4 normalMat=transpose(inverse(modelMat));
	FKCW_3D_Vector4 lightPosWorld=sprite->getLightSource()->getWorldPosition3D();
	FKCW_3D_Vector4 eyePosWorld=sprite->getCamera3D()->getEyePos();
	FKCW_3D_Vector4 diffuseMaterial=sprite->getMaterial()->getDiffuse();
	FKCW_3D_Vector4 diffuseLight=sprite->getLightSource()->getDiffuse();
	FKCW_3D_Vector4 ambientMaterial=sprite->getMaterial()->getAmbient();
	FKCW_3D_Vector4 ambientLight=sprite->getLightSource()->getAmbient();
	FKCW_3D_Vector4 specularMaterial=sprite->getMaterial()->getSpecular();
	FKCW_3D_Vector4 specularLight=sprite->getLightSource()->getSpecular();
	float shininess=sprite->getMaterial()->getShininess();

	program->passUnifoValueMatrixNfv("modelMat",modelMat.convertToVector());
	program->passUnifoValueMatrixNfv("normalMat",normalMat.convertToVector());
	program->passUnifoValueNfv("lightPos_world",lightPosWorld.convertToVector());
	program->passUnifoValueNfv("eyePos_world",eyePosWorld.convertToVector());
	program->passUnifoValueNfv("diffuseMaterial", diffuseMaterial.convertToVector());
	program->passUnifoValueNfv("diffuseLight",diffuseLight.convertToVector());
	program->passUnifoValueNfv("ambientMaterial", ambientMaterial.convertToVector());
	program->passUnifoValueNfv("ambientLight", ambientLight.convertToVector());
	program->passUnifoValueNfv("specularMaterial", specularMaterial.convertToVector());
	program->passUnifoValueNfv("specularLight", specularLight.convertToVector());
	program->passUnifoValue1f("shininess", shininess);
};
