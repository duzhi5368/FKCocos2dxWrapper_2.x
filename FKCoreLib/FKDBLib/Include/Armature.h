//*************************************************************************
//	创建日期:	2014-8-14   15:13
//	文件名称:	Armature.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "BaseMacro.h"
#include "EventDispatcher.h"
#include "Object.h"
#include <string>
#include <vector>
using std::string;
using std::vector;
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------

class CFrame;
class CSlot;
class CBone;
class CEvent;
class CAnimation;
class CTimeLineState;
class CAnimationState;
class CDragonBonesObject;
class CArmature : public CEventDispatcher, public IObject
{
public:
	string				m_szName;
	IObject*			m_pDisplay;
	bool				m_bIsSlotsZOrderChanged;
	vector<CSlot*>		m_vecSlotList;
	vector<CBone*>		m_vecBoneList;
	vector<CEvent*>		m_vecEventList;
	CAnimation*			m_pAnimation;
public:
	CArmature( IObject* p_pDisplay );
	virtual ~CArmature();
public:
	void				Clear();
	void				Update( f32 p_fPassedTime );
	void				UpdateSlotsZOrder();
	void				ArriveAtFrame( CFrame* p_pFrame, CTimeLineState* p_pTimeLineState, 
		CAnimationState* p_pAnimationState, bool p_bIsCross );
public:
	vector<CSlot*>		GetSlots();
	vector<CBone*>		GetBones();
	CSlot*				GetSlotByName( const string& p_szSlotName );
	CSlot*				GetSlotByDisplay( IObject* p_pDisplay );
	void				RemoveSlot( CSlot* p_pSlot );
	void				RemoveSlotByName( const string& p_szSlotName );
	CBone*				GetBoneByName( const string& p_szBoneName );
	CBone*				GetBoneByDisplay( IObject* p_pDisplay );
	void				RemoveBone( CBone* p_pBone );
	void				RemoveBoneByName( const string& p_szBoneName );
	void				AddBone( CBone* p_pBone, const string& p_szParentname = "" );
	void				AddChild( CDragonBonesObject* p_pObject, const string& p_szParentName = "");
	void				AddDragonBonesObject( CDragonBonesObject* p_pObject );
	void				RemoveDragonBonesObject( CDragonBonesObject* p_pObject );
public:
	static bool			SortBoneFunc( const std::pair<int, CBone*>&a, const std::pair<int, CBone*>&b );
	void				SortBoneList();
	static bool			SortSlotFunc( CSlot* p_pSlotA, CSlot* p_pSlotB );
};

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------