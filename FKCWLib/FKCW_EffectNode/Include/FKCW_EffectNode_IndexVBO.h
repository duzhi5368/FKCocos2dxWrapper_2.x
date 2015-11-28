//*************************************************************************
//	创建日期:	2014-11-15
//	文件名称:	FKCW_EffectNode_IndexVBO.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_EffectNode_Macros.h"
//-------------------------------------------------------------------------
class FKCW_EffectNode_IndexVBO :public CCObject
{

public:
	FKCW_EffectNode_IndexVBO();
	virtual ~FKCW_EffectNode_IndexVBO();

	static void EnableAttribArrayPosition(bool isEnable);
	static void EnableAttribArrayTexCoord(bool isEnable);
	static void EnableAttribArrayColor(bool isEnable);
	static bool IsEnabledAttribArrayPosition();
	static bool IsEnabledAttribArrayTexCoord();
	static bool IsEnabledAttribArrayColor();

	void		SubmitPos(const vector<Cv2> &pointList,GLenum usage);
	void		SubmitTexCoord(const vector<Cv2> &texCoordList,GLenum usage);
	void		SubmitColor(const vector<Cv4> &colorList,GLenum usage);
	void		SubmitIndex(const vector<CIDTriangle> &IDtriList,GLenum usage);
	void		SubmitIndex(const vector<CIDLine> &IDlineList,GLenum usage);
	void		SubmitIndex(const vector<short> &indexList,GLenum usage);
	void		SetPointerPosition();
	void		SetPointerTexCoord();
	void		SetPointerColor();
	void		Draw(GLenum mode);
private:
	void		__Init();
	void		__GenBuffers();
protected:
	GLuint		m_unPosBuffer ;
	GLuint		m_unTexCoordBuffer ;
	GLuint		m_unIndexBuffer ;
	GLuint		m_unColorBuffer ;
	int			m_nIndexArrayLen;
	int			m_nPosArrayLen;
	int			m_nTexCoordArrayLen;
	int			m_nColorArrayLen;
};
