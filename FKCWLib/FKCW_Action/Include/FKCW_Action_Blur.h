/********************************************************************
	created:	2014/11/03
	file base:	FKCW_Action_Blur
	author:		FreeKnight
	
	purpose:	模糊动作
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_Action_Macros.h"
//--------------------------------------------------------------------
class FKCW_Action_Blur : public CCActionInterval
{
private:
	 CCGLProgram*				m_pOldProgram;
	 CCSize						m_tagStartBlurSize;		// 开始模糊的大小
	 CCSize						m_tagEndBlurSize;		// 结束模糊的大小
	 CCSize						m_tagDeltaBlurSize;		// 增量模糊大小
protected:
	FKCW_Action_Blur();
public:
	virtual ~FKCW_Action_Blur();

	static	FKCW_Action_Blur* Create(float duration, CCSize startBlurSize, CCSize endBlurSize);

	bool	InitWithBlurSize(float d, CCSize startBlurSize, CCSize endBlurSize);

	virtual void update(float time);
	virtual void startWithTarget(CCNode *pTarget);
	virtual void stop();
	virtual CCObject* copyWithZone(CCZone* pZone);
	virtual CCActionInterval* reverse();
};
//--------------------------------------------------------------------