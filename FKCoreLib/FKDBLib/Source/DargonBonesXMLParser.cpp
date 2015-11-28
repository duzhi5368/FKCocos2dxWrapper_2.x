//-------------------------------------------------------------------------
#include "../Include/DargonBonesXMLParser.h"
#include "../Include/ConstString.h"
#include "../Include/Version.h"
#include "../Include/TinyXML2.h"
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------

CTextureAtlasData* CDargonBonesXMLParser::ParseTextureFromXML( const XMLElement* p_pRoot, f32 p_fScale )
{
	if( p_pRoot == NULL )
		return NULL;

	CTextureAtlasData* pTextureAtlasData = new CTextureAtlasData();
	pTextureAtlasData->m_szName			= p_pRoot->Attribute( ConstString::A_NAME.c_str() );
	pTextureAtlasData->m_szImagePath	= p_pRoot->Attribute( ConstString::A_IMAGE_PATH.c_str() );
	for( const XMLElement* pSubTextureXML = p_pRoot->FirstChildElement( ConstString::SUB_TEXTURE.c_str() );
		pSubTextureXML != NULL;
		pSubTextureXML = pSubTextureXML->NextSiblingElement( ConstString::SUB_TEXTURE.c_str() ) )
	{
		string szSubTextureName			= pSubTextureXML->Attribute( ConstString::A_NAME.c_str() );
		CRect tagSubTextureRect;
		tagSubTextureRect.m_nX			= static_cast< int >( pSubTextureXML->IntAttribute( ConstString::A_X.c_str() ) / p_fScale );
		tagSubTextureRect.m_nY			= static_cast< int >( pSubTextureXML->IntAttribute( ConstString::A_Y.c_str() ) / p_fScale );
		tagSubTextureRect.m_nWidth		= static_cast< int >( pSubTextureXML->IntAttribute( ConstString::A_WIDTH.c_str() ) / p_fScale );
		tagSubTextureRect.m_nHeight		= static_cast< int >( pSubTextureXML->IntAttribute( ConstString::A_HEIGHT.c_str() ) / p_fScale );

		pTextureAtlasData->AddTextureAtlasData( szSubTextureName, tagSubTextureRect );
	}
	return pTextureAtlasData;
}
//-------------------------------------------------------------------------
CSkeletonData* CDargonBonesXMLParser::ParseSkeletonFromXML( const XMLElement* p_pRoot )
{
	if( p_pRoot == NULL )
		return NULL;

	string szVersion = p_pRoot->Attribute( ConstString::A_VERSION.c_str() );
	if( szVersion != Version::DATA_VERSION )
	{
		//throw std::invalid_argument("Nonsupport DB xml version!");
		//android 平台不支持c++11特性
		printf("Nonsupport DB xml version!");
		return NULL;
	}

	uint unFrameRate		= p_pRoot->IntAttribute( ConstString::A_FRAME_RATE.c_str() );

	CSkeletonData* pData = new CSkeletonData();
	pData->m_szName			= p_pRoot->Attribute( ConstString::A_NAME.c_str() );
	for( const XMLElement* pArmatureXML = p_pRoot->FirstChildElement( ConstString::ARMATURE.c_str() );
		pArmatureXML != NULL;
		pArmatureXML = pArmatureXML->NextSiblingElement( ConstString::ARMATURE.c_str() ) )
	{
		pData->AddArmatureData( __ParseArmatureFromXML( pArmatureXML, pData, unFrameRate ) );
	}

	return pData;
}
//-------------------------------------------------------------------------
CArmatureData* CDargonBonesXMLParser::__ParseArmatureFromXML( const XMLElement* p_pArmatureXML, CSkeletonData* p_pData, uint p_unFrameRate )
{
	if( p_pArmatureXML == NULL )
		return NULL;

	CArmatureData* pArmatureData = new CArmatureData();
	pArmatureData->m_szName		= p_pArmatureXML->Attribute( ConstString::A_NAME.c_str() );

	for( const XMLElement* pBoneXML = p_pArmatureXML->FirstChildElement( ConstString::BONE.c_str() );
		pBoneXML != NULL;
		pBoneXML = pBoneXML->NextSiblingElement( ConstString::BONE.c_str() ) )
	{
		pArmatureData->AddBoneData( __ParseBoneFromXML( pBoneXML ) );
	}

	for( const XMLElement* pSkinXML = p_pArmatureXML->FirstChildElement( ConstString::SKIN.c_str() );
		pSkinXML != NULL;
		pSkinXML = pSkinXML->NextSiblingElement( ConstString::SKIN.c_str() ) )
	{
		pArmatureData->AddSkinData( __ParseSkinFromXML( pSkinXML, p_pData ) );
	}
	
	TransfromArmatureData( pArmatureData );
	pArmatureData->SortBoneDataList();

	for( const XMLElement* pAnimationXML = p_pArmatureXML->FirstChildElement( ConstString::ANIMATION.c_str() );
		pAnimationXML != NULL;
		pAnimationXML = pAnimationXML->NextSiblingElement( ConstString::ANIMATION.c_str() ) )
	{
		pArmatureData->AddAnimationData( __ParseAnimationFromXML( pAnimationXML, pArmatureData, p_unFrameRate ) );
	}

	return pArmatureData;
}
//-------------------------------------------------------------------------
CBoneData* CDargonBonesXMLParser::__ParseBoneFromXML( const XMLElement* p_pBoneXML )
{
	CBoneData* pBoneData = new CBoneData();
	pBoneData->m_szName = p_pBoneXML->Attribute( ConstString::A_NAME.c_str() );
	const char* szParent = p_pBoneXML->Attribute( ConstString::A_PARENT.c_str() );
	if( szParent != NULL )
		pBoneData->m_szParent = szParent;
	pBoneData->m_fLength = static_cast<f32>( p_pBoneXML->DoubleAttribute( ConstString::A_LENGTH.c_str() ) );
	const char* szInheritScale = p_pBoneXML->Attribute( ConstString::A_SCALE_MODE.c_str() );
	if( szInheritScale != NULL )
		pBoneData->m_nScaleMode = atoi(szInheritScale);
	const char* szFixedRotation = p_pBoneXML->Attribute( ConstString::A_FIXED_ROTATION.c_str() );
	if( szFixedRotation == NULL )
	{
		pBoneData->m_bIsRotationFixed = false;
	}
	else
	{
		if( strcmp( szFixedRotation, "false" ) == 0 || strcmp( szFixedRotation, "0" ) == 0 ||
			strcmp( szFixedRotation, "no" ) == 0 || strcmp( szFixedRotation, "" ) == 0 )
		{
			pBoneData->m_bIsRotationFixed = false;
		}
		else
		{
			pBoneData->m_bIsRotationFixed = true;
		}
	}

	__ParseTranform( p_pBoneXML->FirstChildElement( ConstString::TRANSFORM.c_str() ), &pBoneData->m_GlobalTransform );
	pBoneData->m_Transform	= pBoneData->m_GlobalTransform;

	return pBoneData;
}
//-------------------------------------------------------------------------
CSkinData* CDargonBonesXMLParser::__ParseSkinFromXML( const XMLElement* p_pSkinXML, CSkeletonData* p_pData )
{
	CSkinData* pSkinData = new CSkinData();
	pSkinData->m_szName = p_pSkinXML->Attribute( ConstString::A_NAME.c_str() );

	for( const XMLElement* pSlotXML = p_pSkinXML->FirstChildElement( ConstString::SLOT.c_str() );
		pSlotXML != NULL;
		pSlotXML = pSlotXML->NextSiblingElement( ConstString::SLOT.c_str() ) )
	{
		pSkinData->AddSlotData( __ParseSlotFromXML( pSlotXML, p_pData ) );
	}
	return pSkinData;
}
//-------------------------------------------------------------------------
CSlotData* CDargonBonesXMLParser::__ParseSlotFromXML( const XMLElement* p_pSlotXML, CSkeletonData* p_pData )
{
	CSlotData* pSlotData = new CSlotData();
	pSlotData->m_szName = p_pSlotXML->Attribute( ConstString::A_NAME.c_str() );
	pSlotData->m_szParent = p_pSlotXML->Attribute( ConstString::A_PARENT.c_str() );
	pSlotData->m_fZOrder = static_cast<f32>( p_pSlotXML->DoubleAttribute( ConstString::A_Z_ORDER.c_str() ) );
	const char* szBlendMode = p_pSlotXML->Attribute( ConstString::A_BLENDMODE.c_str() );
	if( szBlendMode != NULL )
	{
		pSlotData->m_szBlendMode = szBlendMode;
	}
	else
	{
		pSlotData->m_szBlendMode = DEFAULT_BLEND_MODE;
	}

	for( const XMLElement* pDisplayXML = p_pSlotXML->FirstChildElement( ConstString::DISPLAY.c_str() );
		pDisplayXML != NULL;
		pDisplayXML = pDisplayXML->NextSiblingElement( ConstString::DISPLAY.c_str() ) )
	{
		pSlotData->AddDisplayData( __ParseDisplayFromXML( pDisplayXML, p_pData ) );
	}
	return pSlotData;
}
//-------------------------------------------------------------------------
CDisplayData* CDargonBonesXMLParser::__ParseDisplayFromXML( const XMLElement* p_pDisplayXML, CSkeletonData* p_pData )
{
	CDisplayData* pDisplayData = new CDisplayData();
	pDisplayData->m_szName  = p_pDisplayXML->Attribute( ConstString::A_NAME.c_str() );
	pDisplayData->m_szType	= p_pDisplayXML->Attribute( ConstString::A_TYPE.c_str() );
	pDisplayData->m_Pivot	= p_pData->AddSubTexturePivot( 0.0f, 0.0f, pDisplayData->m_szName );
	__ParseTranform( p_pDisplayXML->FirstChildElement( ConstString::TRANSFORM.c_str() ),
		&pDisplayData->m_Transform, &pDisplayData->m_Pivot );
	return pDisplayData;
}
//-------------------------------------------------------------------------
CAnimationData*	CDargonBonesXMLParser::__ParseAnimationFromXML( const XMLElement* p_pAnimationXML, CArmatureData* p_pArmatureData, uint p_unFrameRate )
{
	CAnimationData* pAnimationData = new CAnimationData();
	pAnimationData->m_szName		= p_pAnimationXML->Attribute( ConstString::A_NAME.c_str() );
	pAnimationData->m_unFrameRate	= p_unFrameRate;
	pAnimationData->m_nLoop			= p_pAnimationXML->IntAttribute( ConstString::A_LOOP.c_str() );
	f32 fFadeInTime					= static_cast< f32 >( p_pAnimationXML->DoubleAttribute( ConstString::A_FADE_IN_TIME.c_str() ) );
	pAnimationData->m_fFadeInTime	=  IsNaN( fFadeInTime ) ? 0.0f : fFadeInTime;
	pAnimationData->m_fDuration		= static_cast< f32 >( p_pAnimationXML->DoubleAttribute( ConstString::A_DURATION.c_str() ) ) / static_cast<f32>( p_unFrameRate );
	pAnimationData->m_fScale		= static_cast< f32 >( p_pAnimationXML->DoubleAttribute( ConstString::A_SCALE.c_str() ) );
	if( strcmp( p_pAnimationXML->Attribute( ConstString::A_TWEEN_EASING.c_str()), "NaN" ) == 0 )
	{
		pAnimationData->m_fTweenEasing	= NaN;
	}
	else
	{
		pAnimationData->m_fTweenEasing	= static_cast< f32 >( p_pAnimationXML->DoubleAttribute( ConstString::A_TWEEN_EASING.c_str() ) );
	}

	__ParseTimeLine( p_pAnimationXML, pAnimationData, __ParseMainFrame, p_unFrameRate );

	CTransformTimeLine* pTimeLine = NULL;
	string				szTimeLineName;
	for( const XMLElement* pTimeLineXML = p_pAnimationXML->FirstChildElement( ConstString::TIMELINE.c_str() );
		pTimeLineXML != NULL;
		pTimeLineXML = pTimeLineXML->NextSiblingElement( ConstString::TIMELINE.c_str() ) )
	{
		pTimeLine		= __ParseTranformTimeLine( pTimeLineXML, pAnimationData->m_fDuration, p_unFrameRate );
		szTimeLineName	= pTimeLineXML->Attribute( ConstString::A_NAME.c_str() );
		pAnimationData->AddTimeLine( pTimeLine, szTimeLineName );
	}

	CDargonBonesXMLParser::AddHideTimeLine( pAnimationData, p_pArmatureData );
	CDargonBonesXMLParser::TransfromAnimationData( pAnimationData, p_pArmatureData );

	return pAnimationData;
}
//-------------------------------------------------------------------------
CTransformTimeLine*	CDargonBonesXMLParser::__ParseTranformTimeLine( const XMLElement* p_pTransformLineXML, f32 p_fDuration, uint p_unFrameRate )
{
	CTransformTimeLine* pTimeLine = new CTransformTimeLine();
	pTimeLine->m_fDuration = p_fDuration;

	__ParseTimeLine( p_pTransformLineXML, pTimeLine, __ParseTransformFrame, p_unFrameRate );

	pTimeLine->m_fScale		= static_cast< f32 >( p_pTransformLineXML->DoubleAttribute( ConstString::A_SCALE.c_str() ) );
	pTimeLine->m_fOffset	= static_cast< f32 >( p_pTransformLineXML->DoubleAttribute( ConstString::A_OFFSET.c_str() ) );

	return pTimeLine;
}
//-------------------------------------------------------------------------
void CDargonBonesXMLParser::__ParseTimeLine( const XMLElement* p_pTimeLineXML, CTimeLine* p_pTimeLine, FrameParserFunc p_pFrameParserFunc, uint p_unFrameRate )
{
	f32		fPosition	= 0.0f;
	CFrame* pFrame		= NULL;

	for( const XMLElement* pFrameXML = p_pTimeLineXML->FirstChildElement( ConstString::FRAME.c_str() );
		pFrameXML != NULL;
		pFrameXML = pFrameXML->NextSiblingElement( ConstString::FRAME.c_str() ) )
	{
		pFrame = p_pFrameParserFunc( pFrameXML, p_unFrameRate );
		pFrame->m_fPos		= fPosition;
		p_pTimeLine->AddFrame( pFrame );
		fPosition += pFrame->m_fDuration;
	}

	if( pFrame )
	{
		pFrame->m_fDuration = p_pTimeLine->m_fDuration -  pFrame->m_fPos;
	}
}
//-------------------------------------------------------------------------
void CDargonBonesXMLParser::__ParseTranform( const XMLElement* p_pTransformXML, CTransform* p_pTransform, CPoint* p_pPivot )
{
	if( p_pTransformXML == NULL )
		return;

	if( p_pTransform )
	{
		p_pTransform->m_fPosX		=	static_cast< f32 >( p_pTransformXML->DoubleAttribute( ConstString::A_X.c_str() ) );
		p_pTransform->m_fPosY		=	static_cast< f32 >( p_pTransformXML->DoubleAttribute( ConstString::A_Y.c_str() ) );
		p_pTransform->m_fSkewX		=	static_cast< f32 >( p_pTransformXML->DoubleAttribute( ConstString::A_SKEW_X.c_str() ) ) * ANGLE_TO_RADIN;
		p_pTransform->m_fSkewY		=	static_cast< f32 >( p_pTransformXML->DoubleAttribute( ConstString::A_SKEW_Y.c_str() ) ) * ANGLE_TO_RADIN;
		p_pTransform->m_fScaleX		=	static_cast< f32 >( p_pTransformXML->DoubleAttribute( ConstString::A_SCALE_X.c_str() ) );
		p_pTransform->m_fScaleY		=	static_cast< f32 >( p_pTransformXML->DoubleAttribute( ConstString::A_SCALE_Y.c_str() ) );
	}

	if( p_pPivot )
	{
		p_pPivot->m_fX				=	static_cast< f32 >( p_pTransformXML->DoubleAttribute( ConstString::A_PIVOT_X.c_str() ) );
		p_pPivot->m_fY				=	static_cast< f32 >( p_pTransformXML->DoubleAttribute( ConstString::A_PIVOT_Y.c_str() ) );
	}
}
//-------------------------------------------------------------------------
CFrame* CDargonBonesXMLParser::__ParseMainFrame( const XMLElement* p_pFrameXML, uint p_unFrameRate )
{
	CFrame* pFrame = new CFrame();
	__ParseFrame( p_pFrameXML, pFrame, p_unFrameRate );
	return pFrame;
}
//-------------------------------------------------------------------------
CFrame*	CDargonBonesXMLParser::__ParseTransformFrame( const XMLElement* p_pFrameXML, uint p_unFrameRate )
{
	CTransformFrame* pFrame = new CTransformFrame();

	__ParseFrame( p_pFrameXML, pFrame, p_unFrameRate );

	pFrame->m_bIsVisible = ( static_cast< uint >( p_pFrameXML->IntAttribute( ConstString::A_HIDE.c_str() ) ) != 1 );
	const char* szTweenEasing = p_pFrameXML->Attribute( ConstString::A_TWEEN_EASING.c_str() );
	if( szTweenEasing && strcmp( szTweenEasing, "NaN" ) == 0 )
	{
		pFrame->m_fTweenEasing = NaN;
	}
	else
	{
		pFrame->m_fTweenEasing = static_cast< f32 >( p_pFrameXML->DoubleAttribute( ConstString::A_TWEEN_EASING.c_str() ) );
	}

	pFrame->m_nTweenRotate	= static_cast< int >( p_pFrameXML->DoubleAttribute( ConstString::A_TWEEN_ROTATE.c_str() ) );
	pFrame->m_nDisplayIndex	= static_cast< int >( p_pFrameXML->DoubleAttribute( ConstString::A_DISPLAY_INDEX.c_str() ) );
	pFrame->m_fZOrder		= static_cast< f32 >( p_pFrameXML->DoubleAttribute( ConstString::A_Z_ORDER.c_str() ) );

	__ParseTranform( p_pFrameXML->FirstChildElement( ConstString::TRANSFORM.c_str() ), &pFrame->m_GlobalTransform, &pFrame->m_Pivot );
	pFrame->m_Transform		= pFrame->m_GlobalTransform;

	const XMLElement* pColorTransformXML = p_pFrameXML->FirstChildElement( ConstString::COLOR_TRANSFORM.c_str() );
	if( pColorTransformXML )
	{
		pFrame->m_pColorTransform		= new CColorTransform();

		pFrame->m_pColorTransform->m_cAlphaOffset	= static_cast< c8 >( pColorTransformXML->DoubleAttribute( ConstString::A_ALPHA_OFFSET.c_str() ) );
		pFrame->m_pColorTransform->m_cRedOffset		= static_cast< c8 >( pColorTransformXML->DoubleAttribute( ConstString::A_RED_OFFSET.c_str() ) );
		pFrame->m_pColorTransform->m_cGreenOffset	= static_cast< c8 >( pColorTransformXML->DoubleAttribute( ConstString::A_GREEN_OFFSET.c_str() ) );
		pFrame->m_pColorTransform->m_cBlueOffset	= static_cast< c8 >( pColorTransformXML->DoubleAttribute( ConstString::A_BLUE_OFFSET.c_str() ) );

		pFrame->m_pColorTransform->m_fAlphaMulti	= static_cast< f32 >( pColorTransformXML->DoubleAttribute( ConstString::A_ALPHA_MULTIPLIER.c_str() ) ) * 0.01f;
		pFrame->m_pColorTransform->m_fRedMulti		= static_cast< f32 >( pColorTransformXML->DoubleAttribute( ConstString::A_RED_MULTIPLIER.c_str() ) ) * 0.01f;
		pFrame->m_pColorTransform->m_fGreenMulti	= static_cast< f32 >( pColorTransformXML->DoubleAttribute( ConstString::A_GREEN_MULTIPLIER.c_str() ) ) * 0.01f;
		pFrame->m_pColorTransform->m_fBlueMulti		= static_cast< f32 >( pColorTransformXML->DoubleAttribute( ConstString::A_BLUE_MULTIPLIER.c_str() ) ) * 0.01f;
	}

	return pFrame;
}
//-------------------------------------------------------------------------
void CDargonBonesXMLParser::__ParseFrame( const XMLElement* p_pFrameXML, CFrame* p_pFrame, uint p_unFrameRate )
{
	p_pFrame->m_fDuration		= static_cast< f32 >( p_pFrameXML->DoubleAttribute( ConstString::A_DURATION.c_str() ) ) / static_cast< f32 >( p_unFrameRate );
	const char*	szAction		= p_pFrameXML->Attribute( ConstString::A_ACTION.c_str() );
	if( szAction != NULL )
	{
		 p_pFrame->m_szAction	= szAction;
	}
	const char* szEvent			= p_pFrameXML->Attribute( ConstString::A_EVENT.c_str() );
	if( szEvent )
	{
		p_pFrame->m_szEvent		= szEvent;
	}
	const char* szSound			= p_pFrameXML->Attribute( ConstString::A_SOUND.c_str() );
	if( szSound )
	{
		p_pFrame->m_szSound		= szSound;
	}
}
//-------------------------------------------------------------------------
void CDargonBonesXMLParser::TransfromArmatureData( CArmatureData* p_pArmatureData )
{
	vector<CBoneData*>& vecBoneDataList = p_pArmatureData->m_vecBoneDataList;
	int i = vecBoneDataList.size();
	CBoneData* pBoneData = NULL;
	CBoneData* pParentData = NULL;

	while( i-- )
	{
		pBoneData = vecBoneDataList[i];
		if( !pBoneData->m_szParent.empty() )
		{
			pParentData = p_pArmatureData->GetBoneDataByName( pBoneData->m_szParent );
			if( pParentData )
			{
				pBoneData->m_Transform = pBoneData->m_GlobalTransform;
				CTransform::TransformPointWithParent( pBoneData->m_Transform, pParentData->m_GlobalTransform );
			}
		}
	}
}
//-------------------------------------------------------------------------
void CDargonBonesXMLParser::TransfromArmatureDataAnimations( CArmatureData* p_pArmatureData )
{
	vector<CAnimationData*>& vecAnimationDataList = p_pArmatureData->m_vecAnimationDataList;
	int i = vecAnimationDataList.size();
	while( i-- )
	{
		TransfromAnimationData( vecAnimationDataList[i], p_pArmatureData );
	}
}
//-------------------------------------------------------------------------
void CDargonBonesXMLParser::TransfromAnimationData( CAnimationData* p_pAnimationData, CArmatureData* p_pArmatureData )
{
	CSkinData* pSkinData = p_pArmatureData->GetFirstSkinData();
	if( pSkinData == NULL )
		return;
	vector<CBoneData*>&		vecBoneDataList = p_pArmatureData->m_vecBoneDataList;
	vector<CSlotData*>&		vecSlotDataList = pSkinData->m_vecSlotDataList;

	int i									= vecBoneDataList.size();
	CBoneData*			pBoneData			= NULL;
	CTransformTimeLine*	pTimeLine			= NULL;
	CSlotData*			pSlotData			= NULL;
	CDisplayData*		pDisplayData		= NULL;
	CTransformTimeLine*	pParentTimeLine		= NULL;
	CTransform*			pOriginTranstorm	= NULL;
	CPoint*				pOriginPivot		= NULL;
	CTransformFrame*	pPrevFrame			= NULL;
	CTransformFrame*	pFrame				= NULL;
	uint				unFrameListLen		= 0;

	CTransform			tagTmpTransform1;
	CTransform			tagTmpTransform2;

	while( i-- )
	{
		pSlotData			= NULL;
		pOriginTranstorm	= NULL;
		pPrevFrame			= NULL;
		pOriginPivot		= NULL;

		pBoneData = vecBoneDataList[i];
		pTimeLine = p_pAnimationData->GetTransformTimeLineByName( pBoneData->m_szName );
		if( pTimeLine == NULL )
			continue;
	
		for( unsigned int j = 0; j < vecSlotDataList.size(); ++j )
		{
			pSlotData = vecSlotDataList[j];
			if( pSlotData->m_szParent == pBoneData->m_szName )
				break;
		}

		pParentTimeLine = pBoneData->m_szParent.empty() ? NULL : p_pAnimationData->GetTransformTimeLineByName( pBoneData->m_szParent );
		vector<CFrame*>& vecFrameList = pTimeLine->m_vecFrameList;
		unFrameListLen = vecFrameList.size();
		for( uint j = 0; j < unFrameListLen; ++j )
		{
			pFrame = static_cast<CTransformFrame*>( vecFrameList[j] );
			if( pParentTimeLine )
			{
				tagTmpTransform1	= pFrame->m_GlobalTransform;
				GetTimeLineTranfrom( pParentTimeLine, pFrame->m_fPos, tagTmpTransform2 );
				CTransform::TransformPointWithParent( tagTmpTransform1, tagTmpTransform2 );
				pFrame->m_Transform = tagTmpTransform1;
			}
			else
			{
				pFrame->m_Transform = pFrame->m_GlobalTransform;
			}

			pFrame->m_Transform.m_fPosX			-= pBoneData->m_Transform.m_fPosX;
			pFrame->m_Transform.m_fPosY			-= pBoneData->m_Transform.m_fPosY;
			pFrame->m_Transform.m_fSkewX		-= pBoneData->m_Transform.m_fSkewX;
			pFrame->m_Transform.m_fSkewY		-= pBoneData->m_Transform.m_fSkewY;
			pFrame->m_Transform.m_fScaleX		-= pBoneData->m_Transform.m_fScaleX;
			pFrame->m_Transform.m_fScaleY		-= pBoneData->m_Transform.m_fScaleY;

			if( !pTimeLine->m_bTransformed )
			{
				if( pSlotData )
				{
					pFrame->m_fZOrder			-= pSlotData->m_fZOrder;
				}
			}

			if( pOriginTranstorm == NULL )
			{
				pOriginTranstorm			= &pTimeLine->m_OriginTransform;
				*pOriginTranstorm			= pFrame->m_Transform;
				pOriginTranstorm->m_fSkewX	= FormatRadian( pOriginTranstorm->m_fSkewX );
				pOriginTranstorm->m_fSkewY	= FormatRadian( pOriginTranstorm->m_fSkewY );
				pOriginPivot				= &pTimeLine->m_OriginPiovt;
				pOriginPivot->m_fX			= pFrame->m_Pivot.m_fX;
				pOriginPivot->m_fY			= pFrame->m_Pivot.m_fY;
			}

			pFrame->m_Transform.m_fPosX		-= pOriginTranstorm->m_fPosX;
			pFrame->m_Transform.m_fPosY		-= pOriginTranstorm->m_fPosY;
			pFrame->m_Transform.m_fSkewX	= FormatRadian( pFrame->m_Transform.m_fSkewX - pOriginTranstorm->m_fSkewX );
			pFrame->m_Transform.m_fSkewY	= FormatRadian( pFrame->m_Transform.m_fSkewY - pOriginTranstorm->m_fSkewY );
			pFrame->m_Transform.m_fScaleX	-= pOriginTranstorm->m_fScaleX;
			pFrame->m_Transform.m_fScaleY	-= pOriginTranstorm->m_fScaleY;

			if( !pTimeLine->m_bTransformed )
			{
				pFrame->m_Pivot.m_fX		-= pOriginPivot->m_fX;
				pFrame->m_Pivot.m_fY		-= pOriginPivot->m_fY;
			}

			if( pPrevFrame )
			{
				f32 fTweenX					= pFrame->m_Transform.m_fSkewX - pPrevFrame->m_Transform.m_fSkewX;

				if( pPrevFrame->m_nTweenRotate != 0 )
				{
					if( pPrevFrame->m_nTweenRotate > 0 )
					{
						if( fTweenX < 0 )
						{
							pFrame->m_Transform.m_fSkewX		+= F_PI * 2;
							pFrame->m_Transform.m_fSkewY		+= F_PI * 2;
						}

						if( pPrevFrame->m_nTweenRotate > 1 )
						{
							pFrame->m_Transform.m_fSkewX		+= F_PI * 2 * ( pPrevFrame->m_nTweenRotate - 1 );
							pFrame->m_Transform.m_fSkewY		+= F_PI * 2 * ( pPrevFrame->m_nTweenRotate - 1 );
						}
					}
					else
					{
						if( fTweenX > 0 )
						{
							pFrame->m_Transform.m_fSkewX		-= F_PI * 2;
							pFrame->m_Transform.m_fSkewY		-= F_PI * 2;
						}

						if( pPrevFrame->m_nTweenRotate < 1 )
						{
							pFrame->m_Transform.m_fSkewX		+= F_PI * 2 * ( pPrevFrame->m_nTweenRotate + 1 );
							pFrame->m_Transform.m_fSkewY		+= F_PI * 2 * ( pPrevFrame->m_nTweenRotate + 1 );
						}
					}
				}
				else
				{
					pFrame->m_Transform.m_fSkewX = pPrevFrame->m_Transform.m_fSkewX + FormatRadian( 
						pFrame->m_Transform.m_fSkewX - pPrevFrame->m_Transform.m_fSkewX );
					pFrame->m_Transform.m_fSkewY = pPrevFrame->m_Transform.m_fSkewY + FormatRadian( 
						pFrame->m_Transform.m_fSkewY - pPrevFrame->m_Transform.m_fSkewY );
				}
			}

			pPrevFrame = pFrame;
		}
		pTimeLine->m_bTransformed = true;
	}
}
//-------------------------------------------------------------------------
void CDargonBonesXMLParser::GetTimeLineTranfrom( CTransformTimeLine* p_pTimeLine, f32 p_fPos, CTransform& p_ResultTransfrom )
{
	vector<CFrame*>& vecFrameList = p_pTimeLine->m_vecFrameList;
	int i = vecFrameList.size();
	CTransformFrame* pCurrentFrame = NULL;
	CTransformFrame* pNextFrame = NULL;
	f32 fTweenEasing = 0.0f;
	f32 fProgress = 0.0f;

	while( i-- )
	{
		pCurrentFrame = static_cast<CTransformFrame*>( vecFrameList[i] );

		if( pCurrentFrame->m_fPos <= p_fPos && pCurrentFrame->m_fPos + pCurrentFrame->m_fDuration > p_fPos )
		{
			fTweenEasing = pCurrentFrame->m_fTweenEasing;
			if( i == vecFrameList.size() - 1 || IsNaN( fTweenEasing ) || IsEqual( p_fPos, pCurrentFrame->m_fPos ) )
			{
				p_ResultTransfrom = pCurrentFrame->m_GlobalTransform;
			}
			else
			{
				fProgress = ( p_fPos - pCurrentFrame->m_fPos ) / pCurrentFrame->m_fDuration;
				if( fTweenEasing )
				{
					fProgress = CTransform::GetEaseValue( fProgress, fTweenEasing );
				}

				pNextFrame = static_cast<CTransformFrame*>( vecFrameList[i+1] );

				p_ResultTransfrom.m_fPosX = pCurrentFrame->m_GlobalTransform.m_fPosX + ( pNextFrame->m_GlobalTransform.m_fPosX - 
					pCurrentFrame->m_GlobalTransform.m_fPosX ) * fProgress;
				p_ResultTransfrom.m_fPosY = pCurrentFrame->m_GlobalTransform.m_fPosY + ( pNextFrame->m_GlobalTransform.m_fPosY - 
					pCurrentFrame->m_GlobalTransform.m_fPosY ) * fProgress;
				p_ResultTransfrom.m_fSkewX = FormatRadian( pCurrentFrame->m_GlobalTransform.m_fSkewX + ( pNextFrame->m_GlobalTransform.m_fSkewX - 
					pCurrentFrame->m_GlobalTransform.m_fSkewX ) * fProgress );
				p_ResultTransfrom.m_fSkewY = FormatRadian( pCurrentFrame->m_GlobalTransform.m_fSkewY + ( pNextFrame->m_GlobalTransform.m_fSkewY - 
					pCurrentFrame->m_GlobalTransform.m_fSkewY ) * fProgress );
				p_ResultTransfrom.m_fScaleX = pCurrentFrame->m_GlobalTransform.m_fScaleX + ( pNextFrame->m_GlobalTransform.m_fScaleX - 
					pCurrentFrame->m_GlobalTransform.m_fScaleX ) * fProgress;
				p_ResultTransfrom.m_fScaleY = pCurrentFrame->m_GlobalTransform.m_fScaleY + ( pNextFrame->m_GlobalTransform.m_fScaleY - 
					pCurrentFrame->m_GlobalTransform.m_fScaleY ) * fProgress;
			}
			break;
		}
	}
}
//-------------------------------------------------------------------------
void CDargonBonesXMLParser::AddHideTimeLine( CAnimationData* p_pAnimationData, CArmatureData* p_pArmatureData )
{
	vector< CBoneData* >& vecBoneDataList = p_pArmatureData->m_vecBoneDataList;
	int i = vecBoneDataList.size();
	CBoneData* pBoneData = NULL;
	string szBoneName;

	while( i-- )
	{
		pBoneData	= vecBoneDataList[i];
		szBoneName	= pBoneData->m_szName;
		if( !p_pAnimationData->GetTransformTimeLineByName( szBoneName ) )
		{
			p_pAnimationData->AddTimeLine( &CTransformTimeLine::EMPTY_TIMELINE, szBoneName );
		}
	}
}

//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------