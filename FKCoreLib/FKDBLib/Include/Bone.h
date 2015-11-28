//*************************************************************************
//	创建日期:	2014-8-14   16:19
//	文件名称:	Bone.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "BaseMacro.h"
#include "DargonBonesObject.h"
#include <vector>
#include <string>
#include <list>
using std::string;
using std::vector;
using std::list;
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------

class CSlot;
class CFrame;
class IObject;
class CArmature;
class CTimeLineState;
class CAnimationState;
class CBone : public CDragonBonesObject
{
public:
	int				m_nScaleMode;
	CPoint			m_TweenPivot;
	CSlot*			m_pSlot;
	string			m_szDisplayController;
	vector< CDragonBonesObject* >  m_vecChildren;
public:
	CBone();
	virtual ~CBone();
public:
	void			Clear();
	bool			IsContains( CDragonBonesObject* p_pChild );
	void			AddChild( CDragonBonesObject* p_pChild );
	void			RemoveChild( CDragonBonesObject* p_pChild );
public:
	CArmature*		GetChildArmature();
	void			SetArmature( CArmature* p_pArmature );
	IObject*		GetDisplay();
	void			SetDisplay( IObject* p_pValue );
	void			SetVisible( bool p_bVisible );
	list<CSlot*>	GetSlots();
	void			ArriveAtFrame( CFrame* p_pFrame, CTimeLineState* p_pTimeLineState, 
		CAnimationState* p_pAnimationState, bool p_bIsCross );
	void			UpdateColor( f32 p_fA, f32 p_fR, f32 p_fG, f32 p_fB,
		f32 p_fAMultiplier, f32 p_fRMultiplier, f32 p_fGMultiplier, f32 p_fBMultiplier, bool p_bIsColorChanged );
};

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------