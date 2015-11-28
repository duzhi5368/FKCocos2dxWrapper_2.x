//-------------------------------------------------------------------------
#include "../Include/FKCW_EffectNode_IndexVBO.h"
//-------------------------------------------------------------------------
FKCW_EffectNode_IndexVBO::FKCW_EffectNode_IndexVBO()
{
	m_unPosBuffer=0 ;
	m_unTexCoordBuffer=0 ;
	m_unIndexBuffer=0 ;
	m_unColorBuffer=0;
	m_nIndexArrayLen=0;
	m_nPosArrayLen=0;
	m_nTexCoordArrayLen=0;
	m_nColorArrayLen=0;
	__Init();
}
//-------------------------------------------------------------------------
FKCW_EffectNode_IndexVBO::~FKCW_EffectNode_IndexVBO()
{
	if(m_unPosBuffer!=0)glDeleteBuffers(1, &m_unPosBuffer) ;
	if(m_unTexCoordBuffer!=0)glDeleteBuffers(1, &m_unTexCoordBuffer) ;
	if(m_unIndexBuffer!=0)glDeleteBuffers(1, &m_unIndexBuffer) ;
	if(m_unColorBuffer!=0)glDeleteBuffers(1, &m_unColorBuffer) ;
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_IndexVBO::EnableAttribArrayPosition(bool isEnable)
{
	if(isEnable)glEnableVertexAttribArray(kCCVertexAttrib_Position);
	else glDisableVertexAttribArray(kCCVertexAttrib_Position);
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_IndexVBO::EnableAttribArrayTexCoord(bool isEnable)
{
	if(isEnable)glEnableVertexAttribArray(kCCVertexAttrib_TexCoords);
	else glDisableVertexAttribArray(kCCVertexAttrib_TexCoords);
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_IndexVBO::EnableAttribArrayColor(bool isEnable)
{
	if(isEnable)glEnableVertexAttribArray(kCCVertexAttrib_Color);
	else glDisableVertexAttribArray(kCCVertexAttrib_Color);
}
//-------------------------------------------------------------------------
bool FKCW_EffectNode_IndexVBO::IsEnabledAttribArrayPosition()
{
	GLint isEnabled;
	glGetVertexAttribiv(kCCVertexAttrib_Position,GL_VERTEX_ATTRIB_ARRAY_ENABLED,&isEnabled);
	return (isEnabled != 0);
}
//-------------------------------------------------------------------------
bool FKCW_EffectNode_IndexVBO::IsEnabledAttribArrayTexCoord()
{
	GLint isEnabled;
	glGetVertexAttribiv(kCCVertexAttrib_TexCoords,GL_VERTEX_ATTRIB_ARRAY_ENABLED,&isEnabled);
	return (isEnabled != 0);
}
//-------------------------------------------------------------------------
bool FKCW_EffectNode_IndexVBO::IsEnabledAttribArrayColor()
{
	GLint isEnabled;
	glGetVertexAttribiv(kCCVertexAttrib_Color,GL_VERTEX_ATTRIB_ARRAY_ENABLED,&isEnabled);
	return (isEnabled != 0);
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_IndexVBO::SubmitPos(const vector<Cv2> &pointList,GLenum usage)
{
	m_nPosArrayLen=((int)pointList.size())*2;
	if(m_nPosArrayLen==0)return;
	glBindBuffer(GL_ARRAY_BUFFER, m_unPosBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m_nPosArrayLen, &pointList.front(), usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_IndexVBO::SubmitTexCoord(const vector<Cv2> &texCoordList,GLenum usage)
{
	m_nTexCoordArrayLen=((int)texCoordList.size())*2;
	if(m_nTexCoordArrayLen==0)return;
	glBindBuffer(GL_ARRAY_BUFFER, m_unTexCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m_nTexCoordArrayLen, &texCoordList.front(), usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_IndexVBO::SubmitColor(const vector<Cv4> &colorList,GLenum usage)
{
	m_nColorArrayLen=((int)colorList.size())*4;
	if(m_nColorArrayLen==0)return;
	glBindBuffer(GL_ARRAY_BUFFER, m_unColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m_nColorArrayLen, &colorList.front(), usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_IndexVBO::SubmitIndex(const vector<CIDTriangle> &IDtriList,GLenum usage)
{
	m_nIndexArrayLen=((int)IDtriList.size())*3;
	if(m_nIndexArrayLen==0)return;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(short)*m_nIndexArrayLen, &IDtriList.front(), usage);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_IndexVBO::SubmitIndex(const vector<CIDLine> &IDlineList,GLenum usage)
{
	m_nIndexArrayLen=((int)IDlineList.size())*2;
	if(m_nIndexArrayLen==0)return;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(short)*m_nIndexArrayLen, &IDlineList.front(), usage);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_IndexVBO::SubmitIndex(const vector<short> &indexList,GLenum usage)
{
	m_nIndexArrayLen=((int)indexList.size())*1;
	if(m_nIndexArrayLen==0)return;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(short)*m_nIndexArrayLen, &indexList.front(), usage);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_IndexVBO::SetPointerPosition()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_unPosBuffer);
	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_IndexVBO::SetPointerTexCoord()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_unTexCoordBuffer);
	glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_IndexVBO::SetPointerColor()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_unColorBuffer);
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_IndexVBO::Draw(GLenum mode)
{
	if(m_nIndexArrayLen==0)return;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIndexBuffer);
	glDrawElements(mode, m_nIndexArrayLen, GL_UNSIGNED_SHORT, 0) ;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_IndexVBO::__Init()
{
	__GenBuffers();
}
//-------------------------------------------------------------------------
void FKCW_EffectNode_IndexVBO::__GenBuffers()
{
	if(m_unPosBuffer==0)glGenBuffers(1, &m_unPosBuffer);
	if(m_unTexCoordBuffer==0)glGenBuffers(1, &m_unTexCoordBuffer);
	if(m_unIndexBuffer==0)glGenBuffers(1, &m_unIndexBuffer);
	if(m_unColorBuffer==0)glGenBuffers(1, &m_unColorBuffer);
}
//-------------------------------------------------------------------------