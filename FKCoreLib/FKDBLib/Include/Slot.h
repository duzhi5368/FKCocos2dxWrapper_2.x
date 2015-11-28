//*************************************************************************
//	创建日期:	2014-8-13   14:37
//	文件名称:	Slot.h
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
using std::string;
using std::vector;
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------

class CDisplayData;
class IDisplayBridge;
class CArmature;
class IObject;
class CSlot : public CDragonBonesObject
{
public:
	vector<CDisplayData*>		m_vecDisplayDataList;
	vector<IObject*>			m_vecDisplayList;
	IDisplayBridge*				m_pDisplayBridge;
	f32							m_fOriginZOrder;
	f32							m_fTweenZOrder;
	f32							m_fOffsetZOrder;
	bool						m_bIsDisplayOnStage;
	bool						m_bIsHideDisplay;
	int							m_nDisplayIndex;
private:
	string						m_szBlendMode;
public:
	CSlot(IDisplayBridge* p_pDisplayBridge);
	~CSlot();
public:
	f32			GetZOrder();
	void		SetZOrder( f32 p_fValue );
	string		GetBlendMode();
	void		SetBlendMode( const string& p_szMode );
	IObject*	GetDisplay();
	void		SetDisplayValue( IObject* p_pValue );
	CArmature*	GetChildArmature();
	void		SetChildArmature( CArmature* p_pArmature );
	const vector<IObject*>& GetDisplayList();
	void		SetDisplayList( const vector<IObject*>& p_Value );
	void		SetDisplay( IObject* p_pDisplay );
	void		ChangeDisplay( int p_nDisplayIndex );	
	void		UpdateVisible( bool p_bValue );
	void		SetVisible( bool p_bValue );
	void		SetArmature(  CArmature* p_pValue  );
public:
	void		Clear();
	void		Update();
	void		UpdateChlidArmatureAnimation();
};

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------