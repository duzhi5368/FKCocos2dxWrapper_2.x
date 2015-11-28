//-------------------------------------------------------------------------
#include "../Include/FKCW_3D_SubMesh.h"
#include "../Include/FKCW_3D_LightSource.h"
//-------------------------------------------------------------------------
static unsigned char s_2x2WhiteImage[] = 
{
	// RGBA8888
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF
};

#define CC_2x2_WHITE_IMAGE_KEY  "s_2x2WhiteImage"
//-------------------------------------------------------------------------
FKCW_3D_SubMesh::FKCW_3D_SubMesh()
{
	m_texture=NULL;
	m_subMeshData=NULL;
	m_indexVBO=NULL;
	m_lightSource=NULL;
	m_material=NULL;
	m_isDoDepthTest=true;
	m_blendFunc.src=GL_ONE;
	m_blendFunc.dst=GL_ZERO;
}
//-------------------------------------------------------------------------
FKCW_3D_SubMesh::~FKCW_3D_SubMesh()
{
	if(m_texture)m_texture->release();
	if(m_subMeshData)m_subMeshData->release();
	if(m_indexVBO)m_indexVBO->release();
	if(m_material)m_material->release();
}
//-------------------------------------------------------------------------
bool FKCW_3D_SubMesh::init()
{
	this->setAnchorPoint(ccp(0,0));
	//call parent init
	FKCW_3D_Node::init();
	//texture
	CCImage* image = new CCImage();
	bool isOK = image->initWithImageData(s_2x2WhiteImage, sizeof(s_2x2WhiteImage), CCImage::kFmtRawData, 2, 2, 8);
	CCAssert(isOK, "The 2x2 empty texture was created unsuccessfully.");

	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addUIImage(image, CC_2x2_WHITE_IMAGE_KEY);
	setTexture(texture);
	//material
	FKCW_3D_Material*material=new FKCW_3D_Material();
	material->autorelease();
	material->init();
	setMaterial(material);
	//subMeshData
	FKCW_3D_SubMeshData*subMeshData=new FKCW_3D_SubMeshData();//empty subMeshData
	subMeshData->autorelease();
	subMeshData->init();
	setSubMeshData(subMeshData);
	//program
	FKCW_3D_Program* program=GetProgramTexOnly();
	setProgram(program);
	//uniformPassor
	m_passUnifoCallback=PassUnifoCallback_TexOnly;
	//lightSource
	FKCW_3D_LightSource*lightSource=new FKCW_3D_LightSource();
	lightSource->autorelease();
	lightSource->init();
	setLightSource(lightSource);
	//indexVBO
	FKCW_3D_IndexVBO3D*indexVBO=new FKCW_3D_IndexVBO3D();
	indexVBO->autorelease();
	indexVBO->init();
	setIndexVBO(indexVBO);
	return true;
}
//-------------------------------------------------------------------------
void FKCW_3D_SubMesh::setTexture(CCTexture2D*texture)
{
	assert(texture);
	setRCObject(m_texture, texture);
}
//-------------------------------------------------------------------------
void FKCW_3D_SubMesh::setMaterial(FKCW_3D_Material*material)
{
	assert(material);
	setRCObject(m_material, material);
}
//-------------------------------------------------------------------------
void FKCW_3D_SubMesh::setSubMeshData(FKCW_3D_SubMeshData*subMeshData)
{
	assert(subMeshData);
	setRCObject(m_subMeshData, subMeshData);
};
//-------------------------------------------------------------------------
void FKCW_3D_SubMesh::setIndexVBO(FKCW_3D_IndexVBO3D*indexVBO)
{
	assert(indexVBO);
	setRCObject(m_indexVBO, indexVBO);
}
//-------------------------------------------------------------------------
void FKCW_3D_SubMesh::submit(GLenum usage)
{
	submitPosition(usage);
	submitNormal(usage);
	submitColor(usage);
	submitTexCoord(usage);
	submitIndex(usage);
}
//-------------------------------------------------------------------------
void FKCW_3D_SubMesh::submitPosition(GLenum usage)
{
	assert(m_subMeshData);
	assert(m_indexVBO);
	m_indexVBO->submitPosition(m_subMeshData->getPositionArray(),m_subMeshData->getPositionArrayLen(),usage);
}
//-------------------------------------------------------------------------
void FKCW_3D_SubMesh::submitNormal(GLenum usage)
{
	assert(m_subMeshData);
	assert(m_indexVBO);
	m_indexVBO->submitNormal(m_subMeshData->getNormalArray(),m_subMeshData->getNormalArrayLen(),usage);
}
//-------------------------------------------------------------------------
void FKCW_3D_SubMesh::submitColor(GLenum usage)
{
	assert(m_subMeshData);
	assert(m_indexVBO);
	m_indexVBO->submitColor(m_subMeshData->getColorArray(),m_subMeshData->getColorArrayLen(),usage);
}
//-------------------------------------------------------------------------
void FKCW_3D_SubMesh::submitTexCoord(GLenum usage)
{
	assert(m_subMeshData);
	assert(m_indexVBO);
	m_indexVBO->submitTexCoord(m_subMeshData->getTexCoordArray(),m_subMeshData->getTexCoordArrayLen(),usage);
}
//-------------------------------------------------------------------------
void FKCW_3D_SubMesh::submitIndex(GLenum usage)
{
	assert(m_subMeshData);
	assert(m_indexVBO);
	m_indexVBO->submitIndex(m_subMeshData->getIndexArray(),m_subMeshData->getIndexArrayLen(),usage);
}
//-------------------------------------------------------------------------
void FKCW_3D_SubMesh::draw()
{
	assert(m_subMeshData&&m_texture&&m_indexVBO
		&&m_program&&m_lightSource&&m_camera);
	//enable array attribute
	bool isPostionAttribEnabled=FKCW_3D_IndexVBO3D::isEnabledAttribArray_position();
	bool isTexCoordAttribEnabled=FKCW_3D_IndexVBO3D::isEnabledAttribArray_texCoord();
	bool isNormalAttribEnabled=FKCW_3D_IndexVBO3D::isEnabledAttribArray_normal();
	bool isColorAttribEnabled=FKCW_3D_IndexVBO3D::isEnabledAttribArray_color();
	FKCW_3D_IndexVBO3D::enableAttribArray_position(true);
	FKCW_3D_IndexVBO3D::enableAttribArray_texCoord(true);
	FKCW_3D_IndexVBO3D::enableAttribArray_normal(true);
	FKCW_3D_IndexVBO3D::enableAttribArray_color(true);
	//apply state
	//for performance sake, we only apply state, not restore
	{
		//set depthTest
		CCDirector::sharedDirector()->setDepthTest(m_isDoDepthTest);
		//set blend function
		ccGLBlendFunc(m_blendFunc.src, m_blendFunc.dst);
	}

	//enable server state (i don't know what this means :( )
	ccGLEnable(m_eGLServerState);
	//pass values for cocos2d-x build-in uniforms
	m_pShaderProgram->use();
	m_pShaderProgram->setUniformsForBuiltins();
	//pass values for my own uniforms
	m_passUnifoCallback(this, m_program);
	//attach texture to texture attach point
	FKCW_3D_IndexVBO3D::bindTexture(0, m_texture->getName());
	//draw
	m_indexVBO->setPointers();
	m_indexVBO->draw(GL_TRIANGLES);
	FKCW_3D_IndexVBO3D::bindTexture(0, 0);
	//recover array attribute state
	FKCW_3D_IndexVBO3D::enableAttribArray_position(isPostionAttribEnabled);
	FKCW_3D_IndexVBO3D::enableAttribArray_texCoord(isTexCoordAttribEnabled);
	FKCW_3D_IndexVBO3D::enableAttribArray_normal(isNormalAttribEnabled);
	FKCW_3D_IndexVBO3D::enableAttribArray_color(isColorAttribEnabled);
}
//-------------------------------------------------------------------------
bool FKCW_3D_SubMesh::getIsDoDepthTest()const 
{
	return m_isDoDepthTest;
}
//-------------------------------------------------------------------------
void FKCW_3D_SubMesh::setIsDoDepthTest(bool isDoDepthTest)
{
	m_isDoDepthTest=isDoDepthTest;
}
//-------------------------------------------------------------------------