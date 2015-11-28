//*************************************************************************
//	创建日期:	2014-12-29
//	文件名称:	FKCW_3D_SubMeshData.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_3D_Macro.h"
#include "FKCW_3D_Vector.h"
#include "FKCW_3D_IDTriangle.h"
#include "FKCW_3D_CommonFunc.h"
//-------------------------------------------------------------------------
class FKCW_3D_SubMeshData:public CCObject
{
public:
	FKCW_3D_SubMeshData(){};
	virtual~FKCW_3D_SubMeshData(){};

	bool init(){return true;}

	void initPositionList(const float positionArray[],int positionArrayLen);
	void initTexCoordList(const float texCoordArray[],int texCoordArrayLen);
	void initNormalList(const float normalArray[],int normalArrayLen);
	void initColorList(const float colorArray[],int colorArrayLen);
	void initIDtriList(const short indexArray[],int indexArrayLen);

	void initPositionList(const vector<FKCW_3D_Vector4>&positionList){m_positionList=positionList;};
	void initTexCoordList(const vector<FKCW_3D_Vector2>&texCoordList){m_texCoordList=texCoordList;};
	void initNormalList(const vector<FKCW_3D_Vector4>&normalList){m_normalList=normalList;};
	void initColorList(const vector<FKCW_3D_Vector4>&colorList){m_colorList=colorList;};
	void initIDtriList(const vector<FKCW_3D_IDTriangle>&IDtriList){m_IDtriList=IDtriList;};

	int getPositionArrayLen()const{return (int)m_positionList.size()*4;}
	int getTexCoordArrayLen()const{return (int)m_texCoordList.size()*2;}
	int getNormalArrayLen()const{return (int)m_normalList.size()*4;}
	int getColorArrayLen()const{return (int)m_colorList.size()*4;}
	int getIndexArrayLen()const{return (int)m_IDtriList.size()*3;}

	const float*getPositionArray(){return m_positionList.front().getArray();}
	const float*getTexCoordArray(){return m_texCoordList.front().getArray();}
	const float*getNormalArray(){return m_normalList.front().getArray();}
	const float*getColorArray(){return m_colorList.front().getArray();}
	const short*getIndexArray(){return m_IDtriList.front().getArray();}
public:
	vector<FKCW_3D_Vector4> m_positionList;
	vector<FKCW_3D_Vector2> m_texCoordList;
	vector<FKCW_3D_Vector4> m_normalList;
	vector<FKCW_3D_Vector4> m_colorList;
	vector<FKCW_3D_IDTriangle> m_IDtriList;
};