//*************************************************************************
//	创建日期:	2014-8-12   13:43
//	文件名称:	DargonBonesXMLParser.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "BaseMacro.h"
#include "SkeletonData.h"
#include "Transform.h"
#include "Frame.h"
#include "TransformFrame.h"
#include "TextureAtlasData.h"
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
class XMLElement;
class CDargonBonesXMLParser
{
	typedef CFrame* (*FrameParserFunc)( const XMLElement* , uint );
public:
	CTextureAtlasData*	ParseTextureFromXML( const XMLElement* p_pRoot, f32 p_fScale = 1.0f );
	CSkeletonData*		ParseSkeletonFromXML( const XMLElement* p_pRoot );
private:
	CArmatureData*		__ParseArmatureFromXML( const XMLElement* p_pArmatureXML, CSkeletonData* p_pData, uint p_unFrameRate );
	CBoneData*			__ParseBoneFromXML( const XMLElement* p_pBoneXML );
	CSkinData*			__ParseSkinFromXML( const XMLElement* p_pSkinXML, CSkeletonData* p_pData );
	CSlotData*			__ParseSlotFromXML( const XMLElement* p_pSlotXML, CSkeletonData* p_pData );
	CDisplayData*		__ParseDisplayFromXML( const XMLElement* p_pDisplayXML, CSkeletonData* p_pData );
	CAnimationData*		__ParseAnimationFromXML( const XMLElement* p_pAnimationXML, CArmatureData* p_pArmatureData, uint p_unFrameRate );
	CTransformTimeLine*	__ParseTranformTimeLine( const XMLElement* p_pTransformLineXML, f32 p_fDuration, uint p_unFrameRate );
private:
	void				__ParseTimeLine( const XMLElement* p_pTimeLineXML, CTimeLine* p_pTimeLine, FrameParserFunc p_pFrameParserFunc, uint p_unFrameRate );
	static void			__ParseTranform( const XMLElement* p_pTransformXML, CTransform* p_pTransform, CPoint* p_pPivot = NULL );
	static CFrame*		__ParseMainFrame( const XMLElement* p_pFrameXML, uint p_unFrameRate );
	static CFrame*		__ParseTransformFrame( const XMLElement* p_pFrameXML, uint p_unFrameRate );
	static void			__ParseFrame( const XMLElement* p_pFrameXML, CFrame* p_pFrame, uint p_unFrameRate );
private:
	static void			TransfromArmatureData( CArmatureData* p_pArmatureData );
	static void			TransfromArmatureDataAnimations( CArmatureData* p_pArmatureData );
	static void			TransfromAnimationData( CAnimationData* p_pAnimationData, CArmatureData* p_pArmatureData );
	static void			GetTimeLineTranfrom( CTransformTimeLine* p_pTimeLine, f32 p_fPos, FK_OUT CTransform& p_ResultTransfrom );
	static void			AddHideTimeLine( CAnimationData* p_pAnimationData, CArmatureData* p_pArmatureData );
};

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------