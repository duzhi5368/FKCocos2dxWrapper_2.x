//-------------------------------------------------------------------------
#include "../Include/FKCW_3D_Programs.h"
//-------------------------------------------------------------------------
FKCW_3D_Program::FKCW_3D_Program()
{

}
//-------------------------------------------------------------------------
FKCW_3D_Program::~FKCW_3D_Program()
{

}
//-------------------------------------------------------------------------
bool FKCW_3D_Program::init(string vertFileName,string fragFileName)
{
	GLchar * vertSource = (GLchar*) CCString::createWithContentsOfFile(CCFileUtils::sharedFileUtils()->fullPathForFilename(vertFileName.c_str()).c_str())->getCString();
	GLchar * fragSource = (GLchar*) CCString::createWithContentsOfFile(CCFileUtils::sharedFileUtils()->fullPathForFilename(fragFileName.c_str()).c_str())->getCString();
	assert(vertSource);
	assert(fragSource);
	this->initWithVertexShaderByteArray(vertSource, fragSource);
	//bind attribute
	this->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
	this->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
	this->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
	this->addAttribute(kCCAttributeNameNormal, kCCVertexAttrib_Normals);
	//link  (must after bindAttribute)
	this->link();
	//get cocos2d-x build-in uniforms
	this->updateUniforms();
	//check gl error
	CHECK_GL_ERROR_DEBUG();
	return true;
}
//-------------------------------------------------------------------------
bool FKCW_3D_Program::init(const char* vertShader,const char* fragShader)
{
	this->initWithVertexShaderByteArray(vertShader, fragShader);
	//bind attribute
	this->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
	this->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
	this->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
	this->addAttribute(kCCAttributeNameNormal, kCCVertexAttrib_Normals);
	//link  (must after bindAttribute)
	this->link();
	//get cocos2d-x build-in uniforms
	this->updateUniforms();
	//check gl error
	CHECK_GL_ERROR_DEBUG();
	return true;
}
//-------------------------------------------------------------------------
void FKCW_3D_Program::attachUniform(string unifoName)
{
	m_unifoMap[unifoName] = glGetUniformLocation(this->getProgram(),unifoName.c_str());
}
//-------------------------------------------------------------------------
void FKCW_3D_Program::passUnifoValue1f(string unifoName,GLfloat v)
{
	glUniform1f(m_unifoMap[unifoName],v);
}
//-------------------------------------------------------------------------
void FKCW_3D_Program::passUnifoValue1i(string unifoName,GLint v)
{
	glUniform1i(m_unifoMap[unifoName],v);
}
//-------------------------------------------------------------------------
void FKCW_3D_Program::passUnifoValueMatrixNfv(string unifoName,const GLfloat *array,int arrayLen)
{
	assert(arrayLen==4||arrayLen==9||arrayLen==16);
	switch (arrayLen) {
	case 4:
		glUniformMatrix2fv(m_unifoMap[unifoName],1,GL_FALSE,array);
		break;
	case 9:
		glUniformMatrix3fv(m_unifoMap[unifoName],1,GL_FALSE,array);
		break;
	case 16:
		glUniformMatrix4fv(m_unifoMap[unifoName],1,GL_FALSE,array);
		break;
	default:
		assert(false);
		break;
	}
}
//-------------------------------------------------------------------------
void FKCW_3D_Program::passUnifoValueNfv(string unifoName,const GLfloat*array,int arrayLen)
{
	assert(arrayLen>=1&&arrayLen<=4);
	switch (arrayLen) 
	{
	case 1:
		glUniform1fv(m_unifoMap[unifoName],1,array);
		break;
	case 2:
		glUniform2fv(m_unifoMap[unifoName],1,array);
		break;
	case 3:
		glUniform3fv(m_unifoMap[unifoName],1,array);
		break;
	case 4:
		glUniform4fv(m_unifoMap[unifoName],1,array);
		break;
	default:
		assert(false);
		break;
	}
}
//-------------------------------------------------------------------------
void FKCW_3D_Program::passUnifoValueNiv(string unifoName,const GLint*array,int arrayLen)
{
	assert(arrayLen>=1&&arrayLen<=4);
	switch (arrayLen) 
	{
	case 1:
		glUniform1iv(m_unifoMap[unifoName],1,array);
		break;
	case 2:
		glUniform2iv(m_unifoMap[unifoName],1,array);
		break;
	case 3:
		glUniform3iv(m_unifoMap[unifoName],1,array);
		break;
	case 4:
		glUniform4iv(m_unifoMap[unifoName],1,array);
		break;
	default:
		assert(false);
		break;
	}
}
//-------------------------------------------------------------------------
void FKCW_3D_Program::passUnifoValueMatrixNfv(string unifoName,const vector<GLfloat>&valueList)
{
	passUnifoValueMatrixNfv(unifoName, &valueList.front(), (int)valueList.size());
}
//-------------------------------------------------------------------------
void FKCW_3D_Program::passUnifoValueNfv(string unifoName,const vector<GLfloat>&valueList)
{
	passUnifoValueNfv(unifoName, &valueList.front(), (int)valueList.size());
}
//-------------------------------------------------------------------------
void FKCW_3D_Program::passUnifoValueNiv(string unifoName,const vector<GLint>&valueList)
{
	passUnifoValueNiv(unifoName, &valueList.front(), (int)valueList.size());
}
//-------------------------------------------------------------------------