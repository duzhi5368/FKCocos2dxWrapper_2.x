//*************************************************************************
//	创建日期:	2014-11-17
//	文件名称:	FKCW_EffectNode_LightingBoltNode.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	闪电精灵节点
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_EffectNode_Mesh.h"
#include "FKCW_EffectNode_IndexVBO.h"
#include "FKCW_EffectNode_GLProgramWithUnifos.h"
//-------------------------------------------------------------------------
class FKCW_EffectNode_LightingSeg
{
public:
	CCPoint									startPoint;
	CCPoint									endPoint;
	CCPoint									branchEndPoint;			// 只有种子段才保存该值
	int										branchID;				// 本段所属的分支ID。若该值为0，则表示挂接在主分支上
	int										branchStartGenerationID;// 本段所属分支的起始生成ID
	int										generationID;			// 本段的起始生成ID,源段的起始生成ID为0
	FKCW_EffectNode_LightingSeg*			father;
	vector<FKCW_EffectNode_LightingSeg*>	childList;
	bool									needReGenerate;			// 若分支将本段视为根段，是否要进行重新计算起始生成ID
public:
	FKCW_EffectNode_LightingSeg();
	FKCW_EffectNode_LightingSeg(CCPoint start,CCPoint end);
	virtual~FKCW_EffectNode_LightingSeg();

	void	init(CCPoint start,CCPoint end);
	bool	addChildRef(FKCW_EffectNode_LightingSeg*child);
	bool	removeChildRef(FKCW_EffectNode_LightingSeg*child);
	float	getLen()const;
	CCPoint getDir()const;
	CCPoint getVec()const;
	CCPoint getRightNormal()const;
	CCPoint getMidPoint()const;
protected:
	void	_InitParameter();
};
//-------------------------------------------------------------------------
class FKCW_EffectNode_LightingBoltSprite:public CCSprite
{
public:
	FKCW_EffectNode_LightingBoltSprite();
	virtual ~FKCW_EffectNode_LightingBoltSprite();

	void									updateNGeneration();
	void									draw();
	bool									init();
	void									setStartAndEnd(CCPoint start,CCPoint end);
	void									genLighting();
	void									doJitter();
	bool									getIsDrawDebug()const{return m_isDrawDebug;}
	void									setIsDrawDebug(bool value){m_isDrawDebug=value;}
protected:
	void									initProgram();
	void									genSeges();
	void									genMesh();
	vector<FKCW_EffectNode_LightingSeg*>	genSegesFromBranchSeg(FKCW_EffectNode_LightingSeg* branchSeg,int nGeneration);
	vector<FKCW_EffectNode_LightingSeg*>	splitSeg(FKCW_EffectNode_LightingSeg* seg,int generationID);
	void									releaseNeedReGenBranch(vector<FKCW_EffectNode_LightingSeg*>& branchList,vector<FKCW_EffectNode_LightingSeg*>& segList);
	void									releaseNeedReGenBranch_inn(FKCW_EffectNode_LightingSeg* seg,vector<FKCW_EffectNode_LightingSeg*>& segList);
	void									debugDraw_segWireFrame(float lineWidth);
	void									debugDraw_segLineAndDot(float lineWidth,float pointSize);
protected:
	FKCW_EffectNode_GLProgramWithUnifos*	m_program;
	CCPoint									m_startPoint_parentSpace;	// 父空间起始点
	CCPoint									m_endPoint_parentSpace;		// 父空间终止点
	CCPoint									m_startPoint;				// 本地空间起始点
	CCPoint									m_endPoint;					// 本地空间终止点
	vector<FKCW_EffectNode_LightingSeg*>	m_segList;					// 闪电列表
	int										m_nGeneration;				// 分割多少个分支段
	FKCW_EffectNode_LightingSeg*			m_headSeg;					// 闪电树主干头部段
	FKCW_EffectNode_LightingSeg*			m_tailSeg;					// 闪电树主干尾部段
	FKCW_EffectNode_Mesh*					m_mesh;
	FKCW_EffectNode_IndexVBO*				m_indexVBO;
	float									m_k_offset;
	float									m_mainSegLen;
	float									m_percentageOfJitterBranch;
	float									m_opacityZoom;
	bool									m_isDrawDebug;
};
//-------------------------------------------------------------------------
class FKCW_Action_FlashAction;
class FKCW_EffectNode_LightingBoltNode:public CCNode
{
public:
    friend class FKCW_Action_FlashAction;
public:
    FKCW_EffectNode_LightingBoltNode(){
        m_lbSprite=NULL;
        m_lbSprite2=NULL;
		m_isDrawDebug=false;
    }
    virtual~FKCW_EffectNode_LightingBoltNode(){}
    bool init(const CCPoint&start,const CCPoint&end);
    void setStartAndEnd(const CCPoint&start,const CCPoint&end){
        m_start=start;
        m_end=end;
        m_lbSprite->setStartAndEnd(m_start, m_end);
        m_lbSprite2->setStartAndEnd(m_start, m_end);
    }
    CCPoint getStart()const{return m_start;}
    CCPoint getEnd()const{return m_end;}
	bool getIsDrawDebug()const{
		assert(m_lbSprite->getIsDrawDebug()==m_isDrawDebug);
		assert(m_lbSprite2->getIsDrawDebug()==m_isDrawDebug);
		return m_isDrawDebug;
	}
	void setIsDrawDebug(bool value){
		m_isDrawDebug=value;
		m_lbSprite->setIsDrawDebug(value);
		m_lbSprite2->setIsDrawDebug(value);
	}
protected:
    CCPoint m_start;				// 父控件起始节点
    CCPoint m_end;					// 父控件终止节点
    FKCW_EffectNode_LightingBoltSprite*m_lbSprite;
    FKCW_EffectNode_LightingBoltSprite*m_lbSprite2;
	bool m_isDrawDebug;
};
//-------------------------------------------------------------------------
class FKCW_Action_FlashAction : public CCActionInterval
{
public:
	FKCW_Action_FlashAction();
	virtual~FKCW_Action_FlashAction();

	// 参数：fDuration -1表示无尽循环
	static FKCW_Action_FlashAction* create(float fDuration,float oneFlashTime);
	// 参数：fDuration -1表示无尽循环
	bool			initWithDuration(float fDuration,float oneFlashTime);
	virtual void	stop();
	virtual void	startWithTarget(CCNode *pTarget);
	virtual void	update(float time);
	virtual bool	isDone();
protected:
	void			_StartAnimation(CCNode* pTarget);
	void			_StopAnimation(CCNode* pTarget);
	void			_CallBack_flashForever(CCNode* sender);
	void			_CallBack_genLightning(CCNode* sender);
	void			_CallBack_jitter(CCNode* sender);
	void			_CallBack_resetOpacity(CCNode* sender);
	void			_InitMembers();
protected:
	float			m_timeFoe;
	float			m_timeCur;
	float			m_oneFlashTime;
	float			m_jitterTimeOfOneFlash;
	float			m_jitterInterval;
	bool			m_isDoJitter;
	GLubyte			m_startOpacity;	// 单词闪光开始不透明度
	GLubyte			m_endOpacity;	// 单次闪光终止不透明度
};
//-------------------------------------------------------------------------