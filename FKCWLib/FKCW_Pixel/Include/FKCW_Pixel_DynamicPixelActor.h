//*************************************************************************
//	创建日期:	2015-1-5
//	文件名称:	FKCW_Pixel_DynamicPixelActor.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_Pixel_DynamicPixelModel.h"
//-------------------------------------------------------------------------
class FKCW_Pixel_DynamicPixelAniFrame:public CCObject
{
public:
	FKCW_Pixel_DynamicPixelAniFrame();
	virtual ~FKCW_Pixel_DynamicPixelAniFrame();

	void				setdpModel(FKCW_Pixel_DynamicPixelModel*dpModel);
	void				setDurTime(float durTime);
	bool				init(FKCW_Pixel_DynamicPixelModel*dpModel,float durTime);
	float				getDurTime()const{return m_durTime;}
	FKCW_Pixel_DynamicPixelModel*	getdpModel()const{return m_dpModel;}

protected:
	float m_durTime;
	FKCW_Pixel_DynamicPixelModel* m_dpModel;
};
//-------------------------------------------------------------------------
class FKCW_Pixel_DynamicPixelAniLayer:public CCObject
{
public:
	FKCW_Pixel_DynamicPixelAniLayer(){}
	virtual	~FKCW_Pixel_DynamicPixelAniLayer();

	void					addAniFrame(FKCW_Pixel_DynamicPixelAniFrame*aniFrame);
	FKCW_Pixel_DynamicPixelAniFrame*	getAniFrameByIndex(int index);
	int						getAniFrameCount()const{return (int)m_aniFrameList.size();}
	float					getAniDurTime()const;
	void					setAniName(const string&aniName){m_aniName=aniName;}
	string					getAniName()const{return m_aniName;}
protected:
	vector<FKCW_Pixel_DynamicPixelAniFrame*>	m_aniFrameList;
	string										m_aniName;
};
//-------------------------------------------------------------------------
class FKCW_Pixel_DynamicPixelAniStack:public CCObject
{
public:
	FKCW_Pixel_DynamicPixelAniStack(){};
	virtual	~FKCW_Pixel_DynamicPixelAniStack();

	void								addAniLayer(FKCW_Pixel_DynamicPixelAniLayer*aniLayer);
	int									getAniLayerCount()const{return (int)m_aniLayerList.size();}
	FKCW_Pixel_DynamicPixelAniLayer*	getAniLayerByIndex(int index);
	FKCW_Pixel_DynamicPixelAniLayer*	getAniLayerByName(const string&aniName);
	int									getAniLayerIndexByName(const string&aniName);
protected:
	vector<FKCW_Pixel_DynamicPixelAniLayer*> m_aniLayerList;
};
//-------------------------------------------------------------------------
class FKCW_Pixel_DynamicPixelActor:public CCNode
{
public:
	FKCW_Pixel_DynamicPixelActor();
	virtual	~FKCW_Pixel_DynamicPixelActor();

	bool								init(const string&texFileName,CCSize perPixelSize=g_defaultPerPixelSize);
	FKCW_Pixel_DynamicPixelAniStack*	getAniStack(){return m_aniStack;}
	void								setAniStack(FKCW_Pixel_DynamicPixelAniStack*aniStack);
	void								setdpModel(FKCW_Pixel_DynamicPixelModel*dpModel);
	FKCW_Pixel_DynamicPixelModel*		getdpModel(){return m_dpModel;}
	void								aniTo(float t,int aniLayerIndex);
	FKCW_Pixel_DynamicPixelModel*		getCurDisplayModel();
	CCRect					getBoundingBoxInLocalSpace();
	CCRect					getBoundingBox();
	CCRect					getCurrentFrameBoundingBoxInLocalSpace();
	CCRect					getCurrentFrameBoundingBox();
	CCRect					getCollisionRectByIndex(int index){return convertRectToParent(m_dpModel->getCollisionRectByIndex(index));}
	CCRect					getCollisionRectByName(const string&name){return convertRectToParent(m_dpModel->getCollisionRectByName(name));}
	CCRect					getCurrentFrameCollisionRectByIndex(int index);
	CCRect					getCurrentFrameCollisionRectByName(const string&name);
	CCRect					convertRectToParent(const CCRect&rectInLocalSpace) {return CCRectApplyAffineTransform(rectInLocalSpace, nodeToParentTransform());}
protected:
	void					init_dft();
protected:
	CCNode*								m_displayNode;
	FKCW_Pixel_DynamicPixelAniStack*	m_aniStack;
	FKCW_Pixel_DynamicPixelModel*		m_dpModel;
};
//-------------------------------------------------------------------------
class FKCW_Pixel_DynamicPixelActorAnimate : public CCActionInterval
{
public:
	static FKCW_Pixel_DynamicPixelActorAnimate* create(float fDuration,int aniLayerIndex);
	bool					initWithDuration(float fDuration,int aniLayerIndex);

	virtual void			startWithTarget(CCNode *pTarget);
	virtual void			update(float time);
protected:
	int					m_aniLayerIndex;
};
//-------------------------------------------------------------------------