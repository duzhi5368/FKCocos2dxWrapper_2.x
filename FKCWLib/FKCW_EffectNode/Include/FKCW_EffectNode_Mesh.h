//*************************************************************************
//	创建日期:	2014-11-15
//	文件名称:	FKCW_EffectNode_Mesh.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_EffectNode_Macros.h"
//-------------------------------------------------------------------------
class FKCW_EffectNode_Mesh:public CCObject
{
public:
	vector<Cv2> vlist;
	vector<Cv2> texCoordList;
	vector<Cv4> colorList;
	vector<CIDTriangle> IDtriList;
	void clear(){
		vlist.clear();
		texCoordList.clear();
		colorList.clear();
		IDtriList.clear();
	}
};
//-------------------------------------------------------------------------
class FKCW_EffectNode_MeshWire:public CCObject
{
public:
	vector<Cv2> vlist;
	vector<Cv2> texCoordList;
	vector<Cv4> colorList;
	vector<CIDLine> IDlineList;
	void clear(){
		vlist.clear();
		texCoordList.clear();
		colorList.clear();
		IDlineList.clear();
	}
};
//-------------------------------------------------------------------------
class FKCW_EffectNode_MeshGeneral:public CCObject
{
public:
	vector<Cv2> vlist;
	vector<Cv2> texCoordList;
	vector<Cv4> colorList;
	vector<short> indexList;
	void clear(){
		vlist.clear();
		texCoordList.clear();
		colorList.clear();
		indexList.clear();
	}
};
//-------------------------------------------------------------------------