//*************************************************************************
//	创建日期:	2015-1-9
//	文件名称:	FKCW_Action_Suck.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_Action_Macros.h"
//-------------------------------------------------------------------------
class FKCW_Action_Suck : public CCGrid3DAction
{
public:
	static FKCW_Action_Suck* create( CCSize p_tagSize, float p_fTime, CCPoint p_tagPos ); 
	FKCW_Action_Suck* initWithGrid( CCSize p_tagSize, float p_fTime, CCPoint p_tagPos );
	virtual void update(float time);
private:
	CCPoint m_tagToPos;
	float	m_fAmplitudeRate;
};
//-------------------------------------------------------------------------