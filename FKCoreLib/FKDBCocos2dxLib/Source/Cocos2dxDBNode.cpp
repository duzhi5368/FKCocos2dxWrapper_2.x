//-------------------------------------------------------------------------
#include "../Include/Cocos2dxDBNode.h"
#include "../Include/Cocos2dxDBFactory.h"
#include "../Include/Cocos2dxDBObj.h"
#include "../Include/Cocos2dxDBAtlasNode.h"
//-------------------------------------------------------------------------
CArmature* CCocos2dxDBNode::BuildAramture( const char* p_szSkeletonXMLFile,
						 const char* p_szTextureXMLFile,
						 const char* p_szTexturePicFile,
						 const char* p_szDragonBonesName,
						 const char* p_szArmatureName,
						 const char* p_szAnimationName )
{
 	CCocos2dxDBFactory* pFactory = CCocos2dxDBFactory::GetInstance();
	pFactory->LoadSkeletonFile( p_szSkeletonXMLFile, p_szDragonBonesName );
	pFactory->LoadTextureAtlasFile( p_szTextureXMLFile, p_szDragonBonesName );
	return pFactory->BuildArmature( p_szArmatureName, p_szAnimationName, p_szTexturePicFile, p_szDragonBonesName );
}
//-------------------------------------------------------------------------
CCocos2dxDBNode* CCocos2dxDBNode::Create( CArmature* p_pArmature )
{
	CCocos2dxDBNode* pNewNode = new CCocos2dxDBNode();
	if( pNewNode && pNewNode->init() )
	{
		pNewNode->__InitWithArmature( p_pArmature );
		pNewNode->autorelease();
		return pNewNode;
	}
	CC_SAFE_DELETE( pNewNode );
	return NULL;
}
//-------------------------------------------------------------------------
CCocos2dxDBNode* CCocos2dxDBNode::Create( const char* p_szSkeletonXMLFile,
						const char* p_szDragonBonesName,
						const char* p_szArmatureName )
{
	return CCocos2dxDBNode::Create( p_szSkeletonXMLFile, "texture.xml", p_szDragonBonesName, p_szArmatureName, "" );
}
//-------------------------------------------------------------------------
CCocos2dxDBNode* CCocos2dxDBNode::Create( const char* p_szSkeletonXMLFile,
							   const char* p_szTextureXMLFile,
							   const char* p_szTexturePicFile,
							   const char* p_szDragonBonesName,
							   const char* p_szArmatureName,
							   const char* p_szAnimationName )
{
	CCocos2dxDBNode* pNewNode = new CCocos2dxDBNode();

#if USE_BATCH_NODE == 1
	if( pNewNode && pNewNode->initWithFile( p_szTexturePicFile, MAX_BATCH_NUM ) )
#else
	if( pNewNode && pNewNode->init() )
#endif
	{
		CArmature* pArmature = CCocos2dxDBNode::BuildAramture( p_szSkeletonXMLFile,
			p_szTextureXMLFile, p_szTexturePicFile, p_szDragonBonesName, p_szArmatureName, p_szAnimationName );
		if( pArmature == NULL )
			return NULL;
		pNewNode->__InitWithArmature( pArmature );
		pNewNode->autorelease();
		return pNewNode;
	}
	CC_SAFE_DELETE( pNewNode );
	return NULL;
}
//-------------------------------------------------------------------------
CCocos2dxDBNode::CCocos2dxDBNode()
	: m_pArmature( NULL )
	, m_CallbackFun( NULL )
	, m_pCaller( NULL )
{

}
//-------------------------------------------------------------------------
CCNode* CCocos2dxDBNode::GetDisplayNode()
{
	return (static_cast<CCocos2dxDBObj*>( m_pArmature->m_pDisplay ))->GetNode();
}
//-------------------------------------------------------------------------
CArmature* CCocos2dxDBNode::GetArmature()
{
	return m_pArmature;
}
//-------------------------------------------------------------------------
CAnimation* CCocos2dxDBNode::GetAnimation()
{
	return m_pArmature->m_pAnimation;
}
//-------------------------------------------------------------------------
void CCocos2dxDBNode::GotoAndPlay(const string& p_szAnimationName, f32 p_fFadeInTime, f32 p_fDuration, f32 p_fLoop, 
								  uint p_unLayer, const string& p_szGroup, const string& p_szFadeOutMode, 
								  bool p_bDisplayControl, bool p_bPauseFadeOut, bool p_bPauseFadeIn )
								
{
	GetAnimation()->GotoAndPlay( p_szAnimationName, p_fFadeInTime, p_fDuration, p_fLoop, p_unLayer, p_szGroup, 
		p_szFadeOutMode, p_bDisplayControl, p_bPauseFadeOut, p_bPauseFadeIn );
}
//-------------------------------------------------------------------------
void CCocos2dxDBNode::AddEventListener( const string& p_szType, const string& p_szKey, CCObject* p_pObject, SEL_CallFuncND p_CallbackFunc )
{
	m_pCaller = p_pObject;
	m_CallbackFun = p_CallbackFunc;
	std::function< void(CEvent*) > f = std::bind( &CCocos2dxDBNode::__EventBridge, this, std::placeholders::_1 );
	m_pArmature->AddEventListener( p_szType, f, p_szKey );
}
//-------------------------------------------------------------------------
void CCocos2dxDBNode::DispatchEvent( CEvent* p_pEvent )
{
	m_pArmature->DispatchEvent( p_pEvent );
}
//-------------------------------------------------------------------------
bool CCocos2dxDBNode::IsHadEventListener( const string& p_szType )const
{
	return m_pArmature->IsHadEventListener( p_szType );
}
//-------------------------------------------------------------------------
void CCocos2dxDBNode::RemoveEventListener( const string& p_szType, const string& p_szKey )
{
	m_pArmature->RemoveEventListener( p_szType, p_szKey );
}
//-------------------------------------------------------------------------
void CCocos2dxDBNode::SetBoneTexture( const char* p_szBoneName, const char* p_szTextureName, const char* p_szTextureAtlasName )
{
	CCocos2dxDBFactory* pFactory = CCocos2dxDBFactory::GetInstance();
	IObject* pClothesObj = pFactory->GetTextureDisplay( p_szTextureName, p_szTextureAtlasName );
	CBone* pBone = GetArmature()->GetBoneByName( p_szBoneName );
	CCocos2dxDBObj* pOldObj = static_cast<CCocos2dxDBObj*>( pBone->GetDisplay() );
	pBone->SetDisplay( pOldObj );
}
//------------------------------------------------------------------------
void CCocos2dxDBNode::onExit()
{
	CC_SAFE_DELETE( m_pArmature );
	this->unscheduleAllSelectors();
}
//-------------------------------------------------------------------------
void CCocos2dxDBNode::__InitWithArmature( CArmature* p_pArmature )
{
	m_pArmature = p_pArmature;
#if USE_BATCH_NODE == 1
	schedule( schedule_selector(CCocos2dxDBNode::__Update), 1.0f / 30.0f );
#else
	schedule( schedule_selector(CCocos2dxDBNode::__Update), 1.0f / 60.0f );
#endif
	addChild( ( static_cast<CCocos2dxDBObj*>(m_pArmature->m_pDisplay) )->GetNode() );
}
//-------------------------------------------------------------------------
void CCocos2dxDBNode::__Update( float p_fDelayTime )
{
	if( m_pArmature )
		m_pArmature->Update( p_fDelayTime );

#if USE_BATCH_NODE == 1
	// 开启Z排序
	reorderBatch(true);
#endif
}
//-------------------------------------------------------------------------
void CCocos2dxDBNode::__EventBridge( CEvent* p_pEvent )
{
	if( m_CallbackFun != NULL )
	{
		( m_pCaller->*m_CallbackFun )( this, p_pEvent );
	}
}
//-------------------------------------------------------------------------
void CCocos2dxDBNode::visit()
{
#if USE_BATCH_NODE == 1
	CCSpriteBatchNode::visit();
#else
	CCNode::visit();
#endif
}
//-------------------------------------------------------------------------
#if USE_BATCH_NODE == 1
//-------------------------------------------------------------------------
void CCocos2dxDBNode::addChild(CCNode *child, int zOrder, int tag)
{
	CCAssert(child != NULL, "child should not be null");
	CCAssert(dynamic_cast<CCSprite*>(child) != NULL, "CCSpriteBatchNode only supports CCSprites as children");
	CCSprite *pSprite = (CCSprite*)(child);
	// check CCSprite is using the same texture id
	// 必须删除这里检查
	// CCAssert(pSprite->getTexture()->getName() == m_pobTextureAtlas->getTexture()->getName(), "CCSprite is not using the same texture id");

	CCNode::addChild(child, zOrder, tag);

	appendChild(pSprite);
}
//-------------------------------------------------------------------------
void CCocos2dxDBNode::addChild(CCNode *child)
{
	CCNode::addChild(child);
}
//-------------------------------------------------------------------------
void CCocos2dxDBNode::addChild(CCNode *child, int zOrder)
{
	CCNode::addChild(child, zOrder);
}
//-------------------------------------------------------------------------
#endif
