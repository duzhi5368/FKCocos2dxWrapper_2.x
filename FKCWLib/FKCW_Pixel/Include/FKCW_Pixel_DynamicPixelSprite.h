//*************************************************************************
//	创建日期:	2015-1-5
//	文件名称:	FKCW_Pixel_DynamicPixelSprite.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_Pixel_Macros.h"
#include "FKCW_Pixel_BaseStruct.h"
//-------------------------------------------------------------------------
class FKCW_Pixel_DynamicPixelSprite;
class FKCW_Pixel_FragSprite:public CCSprite
{
public:
	FKCW_Pixel_FragSprite();
	virtual ~FKCW_Pixel_FragSprite();
public:
	bool				init(const string&texFileName);
	bool				init(CCTexture2D*texture);

	void				setIJ(int i,int j){m_ij=FKCW_Pixel_IJ(i,j);}
	int					getI()const{return m_ij.getI();}
	int					getJ()const{return m_ij.getJ();}
	FKCW_Pixel_IJ		getIJ()const{return m_ij;}
	void				storeInitPosition(const CCPoint&point){m_initPosition=point;}
	CCPoint				getInitPosition()const{return m_initPosition;}
protected:
	FKCW_Pixel_IJ m_ij;
	CCPoint m_initPosition;
};
//-------------------------------------------------------------------------
class FKCW_Pixel_DynamicPixelSprite:public CCNode
{
public:
	FKCW_Pixel_DynamicPixelSprite();
	virtual ~FKCW_Pixel_DynamicPixelSprite();

	bool							init(const string&texFileName,CCSize perPixelSize=g_defaultPerPixelSize);
	void							noise();
	void							relocateToOrigin();
	CCSpriteBatchNode*				getFragRoot();
	int								getFragMatRowCount()const;
	int								getFragMatColCount()const;
	int								getFragMatElementCount()const;
	FKCW_Pixel_FragSprite*			getFragByIndex(int i,int j);
	vector<FKCW_Pixel_FragSprite*>	getShellFrags();
	vector<FKCW_Pixel_FragSprite*>	getFragsInRect(const CCRect& rectInParentSpace);
	CCPoint							getFragPosition(FKCW_Pixel_FragSprite* frag);
	CCPoint							getFragPosition(int i,int j);
	CCRect							getFragRect(FKCW_Pixel_FragSprite* frag);
	CCRect							getMinRectForFrags(const vector<FKCW_Pixel_FragSprite*>& fragList);
	CCRect							getFragRect(int i,int j);
	bool							getIsHasFrag(int i,int j);
	bool							getIsHasFrag(FKCW_Pixel_FragSprite* frag);
	vector<FKCW_Pixel_IJ>			getFragsIndexInRect(const CCRect& rectInParentSpace);
	vector<FKCW_Pixel_IJ>			getFragsIndexInRectGeneral(const CCRect& rectInParentSpace);
	CCRect							getBoundingBoxInLocalSpace()const;
	CCRect							getBoundingBox();
	CCPoint							downDirRayIntersectWithMe(const CCPoint& rayStartPoint,float rayLen,float rayWidth=0);
	CCPoint							upDirRayIntersectWithMe(const CCPoint& rayStartPoint,float rayLen,float rayWidth=0);
	CCPoint							leftDirRayIntersectWithMe(const CCPoint& rayStartPoint,float rayLen,float rayWidth=0);
	CCPoint							rightDirRayIntersectWithMe(const CCPoint& rayStartPoint,float rayLen,float rayWidth=0);
	void							draw();
	void							killFrag(int i,int j);
	void							killFrag(FKCW_Pixel_FragSprite* frag);
protected:
	vector<vector<FKCW_Pixel_FragSprite*> >	m_fragMat;
	CCSize									m_perPixelSize;
	CCSpriteBatchNode*						m_fragRoot;
	CCRect									m_boundingBoxInLocalSpace;
};