//*************************************************************************
//	创建日期:	2014-12-14
//	文件名称:	FKCW_3D_IndexVBO3D.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_3D_Macro.h"
#include "FKCW_3D_CommonFunc.h"
//--------------------------------------------------------------------
class FKCW_3D_IndexVBO3D:public CCObject
{
public:
	FKCW_3D_IndexVBO3D();
	virtual ~FKCW_3D_IndexVBO3D();
public:
	bool			init();
	static void		enableAttribArray_position(bool isEnable);
	static void		enableAttribArray_texCoord(bool isEnable);
	static void		enableAttribArray_normal(bool isEnable);
	static void		enableAttribArray_color(bool isEnable);
	static bool		isEnabledAttribArray_position();
	static bool		isEnabledAttribArray_texCoord();
	static bool		isEnabledAttribArray_normal();
	static bool		isEnabledAttribArray_color();
	static void		bindTexture(GLuint textureUnit, GLuint textureId);
	void			submitPosition(const float posArray[],const int posArrayLen,GLenum usage);
	void			submitTexCoord(const float texCoordArray[],const int texCoordArrayLen,GLenum usage);
	void			submitNormal(const float normalArray[],const int normalArrayLen,GLenum usage);
	void			submitColor(const float colorArray[],const int colorArrayLen,GLenum usage);
	void			submitIndex(const short indexArray[],const int indexArrayLen,GLenum usage);
	void			setPointers();
	void			draw(GLenum mode);
protected:
	void			_GenBuffers();
protected:
	GLuint			m_posBuffer;
	GLuint			m_texCoordBuffer;
	GLuint			m_normalBuffer;
	GLuint			m_colorBuffer;
	GLuint			m_indexBuffer;
	int				m_indexArrayLen;
	int				m_posArrayLen;
	int				m_texCoordArrayLen;
	int				m_normalArrayLen;
	int				m_colorArrayLen;
};