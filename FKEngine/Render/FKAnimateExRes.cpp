//-------------------------------------------------------------------------
#include "FKAnimateExRes.h"
#include "FKAnimateExResMgr.h"
//-------------------------------------------------------------------------
void FKAnimateSaxDelegator::startElement(void *ctx, const char *name, const char **atts)
{
	string tag( name );
	if (tag=="plist")
	{
		m_eCurState=eState_Plist;
	} 
	else if (tag=="animation")
	{
		m_eCurState=eState_Animation;
		m_vecAnimates.push_back(SAnimate());
	}
	else if (tag=="name")
	{
		m_eCurState=eState_Name;
	}
	else if (tag=="delay")
	{
		m_eCurState=eState_Delay;
	}
	else if (tag=="spriteFrame")
	{
		m_eCurState=eState_Sprite_Frame;
	}
	else if (tag=="flipX")
	{
		m_eCurState=eState_FlipX;
	}
	else if (tag=="flipY")
	{
		m_eCurState=eState_FlipY;
	}
	else
	{
		m_eCurState=eState_None;
	}
}
//-------------------------------------------------------------------------
void FKAnimateSaxDelegator::endElement(void *ctx, const char *name)
{
	m_eCurState=eState_None;
}
//-------------------------------------------------------------------------
void FKAnimateSaxDelegator::textHandler(void *ctx, const char *s, int len)
{
	if(m_eCurState==eState_None)
		return;

	string	strText((char*)s, 0, len);
	int		nIndex = 0;
	float	fDelay = 0.0f;

	switch ( m_eCurState )
	{
	case eState_Plist:
		m_vecPlists.push_back( strText );
		break;
	case eState_Animation:
		break;
	case eState_Name:
		{
			if( m_vecAnimates.size() == 0 )
				break;
			nIndex = m_vecAnimates.size() - 1;
			m_vecAnimates[nIndex].m_strName = strText;
		}
		break;
	case eState_Delay:
		{
			if( m_vecAnimates.size() == 0 )
				break;
			nIndex = m_vecAnimates.size() - 1;
			fDelay = (float)FKCW_Base_Utils::AtoF(strText);
			m_vecAnimates[nIndex].m_fDelay = fDelay;
		}
		break;
	case eState_FlipX:
		{
			if( m_vecAnimates.size() == 0 )
				break;
			nIndex = m_vecAnimates.size() - 1;
			m_vecAnimates[nIndex].m_bIsFlipX = (strText == "true");
		}
		break;
	case eState_FlipY:
		{
			if( m_vecAnimates.size() == 0 )
				break;
			nIndex = m_vecAnimates.size() - 1;
			m_vecAnimates[nIndex].m_bIsFlipY = (strText == "true");
		}
		break;
	case eState_Sprite_Frame:
		{
			if( m_vecAnimates.size() == 0 )
				break;
			nIndex = m_vecAnimates.size() - 1;
			m_vecAnimates[nIndex].m_vecSpriteFrame.push_back( strText );
		}
		break;
	default:
		break;
	}
}
//-------------------------------------------------------------------------
// FKAnimateExRes
//-------------------------------------------------------------------------
FKAnimateExRes::FKAnimateExRes()
	: m_bIsVaild( false )
{
	m_strAniResName.clear();
	m_mapAnimation.clear();
	m_mapAnimateNameToAnimate.clear();
	m_mapXMLPathToPlists.clear();
	m_setAnimationName.clear();
	m_mapTextureLoadInfo.clear();
}
//-------------------------------------------------------------------------
FKAnimateExRes::~FKAnimateExRes()
{
	AnimationMap::iterator Ite = m_mapAnimation.begin();
	for( ; Ite != m_mapAnimation.end(); ++Ite )
	{
		if( Ite->second.m_pAnimation )
		{
			Ite->second.m_pAnimation->release();
			Ite->second.m_pAnimation = NULL;
		}
	}
	m_mapAnimation.clear();
}
//-------------------------------------------------------------------------
// 释放
void FKAnimateExRes::release()
{
	FKAnimateExResMgr::GetInstance().ReleaseAnimationExRes( this );
	CCObject::release();
}
//-------------------------------------------------------------------------
// 加载一个资源
bool FKAnimateExRes::LoadRes( const char* szResName )
{
	if( szResName == NULL )
		return false;

	m_strAniResName = szResName;

	_LoadTextureAsync( szResName );

	return true;
}
//-------------------------------------------------------------------------
// 获取资源名称
const string& FKAnimateExRes::GetResFileName()
{
	return m_strAniResName;
}
//-------------------------------------------------------------------------
// 获取动画数据
FKAnimateExRes::SAnimationInfo* FKAnimateExRes::GetAnimation( const char* szAnimationName )
{
	if( !IsValid() )
		return NULL;
	if( m_mapAnimation.empty() || m_mapAnimation.size() == 0 )
		return NULL;

	AnimationMap::iterator Ite;
	if( szAnimationName == NULL )
	{
		Ite = m_mapAnimation.begin();
		return &Ite->second;
	}

	Ite = m_mapAnimation.find( szAnimationName );
	if( Ite != m_mapAnimation.end() )
		return &Ite->second;

	return NULL;
}
//-------------------------------------------------------------------------
// 获取动画名列表
set<string>& FKAnimateExRes::GetAniNameList()
{
	return m_setAnimationName;
}
//-------------------------------------------------------------------------
// 判断资源是否全部顺利加载
bool FKAnimateExRes::IsValid()
{
	return m_bIsVaild;
}
//-------------------------------------------------------------------------
// 加载回调数据
void FKAnimateExRes::_LoadCallback( const char* szResName )
{
	// 暂时去除回调函数加载
	return;
}
//-------------------------------------------------------------------------
// 加载贴图
void FKAnimateExRes::_LoadTextureAsync( const char* szResName )
{
	string strPathName = CCFileUtils::sharedFileUtils()->fullPathForFilename( szResName );
	if( !m_SaxParser.init("UTF-8") )
		return;
	m_SaxParser.setDelegator( &m_SaxDelegator );
	if( !m_SaxParser.parse( strPathName.c_str() ) )
		return;

	vector<string>	vecPlists = m_SaxDelegator.m_vecPlists;
	for( unsigned int i = 0; i < vecPlists.size(); ++i )
	{
		string strFullPath = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile(
			vecPlists[i].c_str(), strPathName.c_str() );
		CCDictionary *pDict = CCDictionary::createWithContentsOfFileThreadSafe( strFullPath.c_str() );
		if( pDict == NULL )
			continue;

		string strTexturePath("");
		CCDictionary* pMetadataDict = (CCDictionary*)pDict->objectForKey("metadata");
		if( pMetadataDict )
			strTexturePath = pMetadataDict->valueForKey("textureFileName")->getCString();

		if( !strTexturePath.empty() )
		{
			strTexturePath = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile( 
				strTexturePath.c_str(), strPathName.c_str() );
		}
		else
		{
			strTexturePath = vecPlists[i].c_str();

			size_t unStartPos = strTexturePath.find_last_of(".");
			strTexturePath = strTexturePath.erase( unStartPos );

			strTexturePath = strTexturePath.append(".png");
		}

		pDict->release();

		m_mapTextureLoadInfo[strTexturePath] = false;
		CCTextureCache::sharedTextureCache()->addImageAsync(
			strTexturePath.c_str(),this, callfuncO_selector(FKAnimateExRes::_OnImageLoadOver) );
	}
}
//-------------------------------------------------------------------------
// 加载资源
void FKAnimateExRes::_LoadRes( const char* szResName )
{
	string strPathName = CCFileUtils::sharedFileUtils()->fullPathForFilename( szResName );

	// 加载Plist
	vector<string>	vecPlists = m_SaxDelegator.m_vecPlists;
	for( unsigned int i = 0; i < vecPlists.size(); ++i )
	{
		string strPlistPath = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile(
			vecPlists[i].c_str(), strPathName.c_str() );
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile( strPlistPath.c_str() );
	}

	// 加载Animate
	vector<SAnimate> vecAnimates = m_SaxDelegator.m_vecAnimates;
	CCArray* pSpriteFramesArray = CCArray::create();
	for( unsigned int i = 0; i < vecAnimates.size(); ++i )
	{
		SAnimate tagAnimate = vecAnimates[i];
		vector<string>	vecSpriteFrames = tagAnimate.m_vecSpriteFrame;

		for( unsigned int j = 0; j < vecSpriteFrames.size(); ++j )
		{
			CCSpriteFrame* pSpriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(vecSpriteFrames[j].c_str());
			if( pSpriteFrame != NULL )
			{
				pSpriteFramesArray->addObject( pSpriteFrame );
			}
		}

		CCAnimation* pAnimation = CCAnimation::createWithSpriteFrames( pSpriteFramesArray, tagAnimate.m_fDelay );

		SAnimationInfo tagInfo;
		tagInfo.m_pAnimation	= pAnimation;
		tagInfo.m_bIsFlipX		= tagAnimate.m_bIsFlipX;
		tagInfo.m_bIsFlipY		= tagAnimate.m_bIsFlipY;
		m_mapAnimation[tagAnimate.m_strName] = tagInfo;

		pAnimation->retain();
		pSpriteFramesArray->removeAllObjects();
		m_setAnimationName.insert( tagAnimate.m_strName );
	}

	// 保存Animate
	for( unsigned int i = 0; i < vecAnimates.size(); ++i )
	{
		SAnimate tagAnimate = vecAnimates[i];
		m_mapAnimateNameToAnimate[tagAnimate.m_strName] = tagAnimate;
	}
	// 保存Plist
	m_mapXMLPathToPlists[szResName] = vecPlists;

	// 加载回调函数
	_LoadCallback( szResName );

	m_bIsVaild = true;
}
//-------------------------------------------------------------------------
// 图片加载完毕回调
void FKAnimateExRes::_OnImageLoadOver( CCObject* pObj )
{
	CCTexture2D* pTex = (CCTexture2D*)pObj;
	if( pTex )
	{
		const char* szTextureName = CCTextureCache::sharedTextureCache()->GetTextureResName( pTex );
		if( szTextureName == NULL )
			return;

		PlistTextureMap::iterator Ite = m_mapTextureLoadInfo.find( szTextureName );
		if( Ite != m_mapTextureLoadInfo.end() )
			Ite->second = true;
	}

	bool bIsAllLoadOver = true;
	PlistTextureMap::iterator Ite = m_mapTextureLoadInfo.begin();
	for( ; Ite != m_mapTextureLoadInfo.end(); ++Ite )
	{
		if( !Ite->second )
		{
			bIsAllLoadOver = false;
			break;
		}
	}

	if( bIsAllLoadOver )
		_LoadRes( m_strAniResName.c_str() );
}
//-------------------------------------------------------------------------