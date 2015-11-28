//-------------------------------------------------------------------------
#include "../Include/Cocos2dxDBFactory.h"
#include "../Include/Cocos2dxDBTextureAtlas.h"
#include "../Include/Cocos2dxDBObj.h"
#include "../Include/Cocos2dxDBDisplayBridge.h"
#include "../Include/Cocos2dxDBAtlasNode.h"
//-------------------------------------------------------------------------
CCocos2dxDBFactory*		CCocos2dxDBFactory::ms_Cocos2dxDBFactory = NULL;
//-------------------------------------------------------------------------
CCocos2dxDBFactory::CCocos2dxDBFactory()
{

}
//-------------------------------------------------------------------------
CCocos2dxDBFactory::~CCocos2dxDBFactory()
{

}
//-------------------------------------------------------------------------
CCocos2dxDBFactory*	CCocos2dxDBFactory::GetInstance()
{
	if( ms_Cocos2dxDBFactory == NULL )
	{
		ms_Cocos2dxDBFactory = new CCocos2dxDBFactory();
	}
	return ms_Cocos2dxDBFactory;
}
//-------------------------------------------------------------------------
void CCocos2dxDBFactory::DestroyInstance()
{
	if( ms_Cocos2dxDBFactory )
	{
		ms_Cocos2dxDBFactory->Clear();
		delete ms_Cocos2dxDBFactory;
	}
}
//-------------------------------------------------------------------------
ITextureAtlas* CCocos2dxDBFactory::GenerateTextureAtlas( IObject* p_pContent, CTextureAtlasData* p_pTextureAtlasData )
{
	return NULL;
}
//-------------------------------------------------------------------------
CArmature* CCocos2dxDBFactory::GenerateArmature( const string& p_szPicName )
{
#if USE_BATCH_NODE == 1
	CCSprite* pNode = CCSprite::create( /*p_szPicName.c_str()*/ );
	pNode->setCascadeOpacityEnabled( true );
	//pNode->setScale( 1000.0f );
	//pNode->setOpacity( static_cast<GLubyte>(0) );
#else
	CCSprite* pNode = CCSprite::create();
	pNode->setCascadeOpacityEnabled( true );
#endif
	return new CArmature( new CCocos2dxDBObj( pNode ) );
}
//-------------------------------------------------------------------------
CSlot* CCocos2dxDBFactory::GenerateSlot()
{
	return new CSlot( new CCocos2dxDBDisplayBridge() );
}
//-------------------------------------------------------------------------
IObject* CCocos2dxDBFactory::GenerateDisplay( ITextureAtlas* p_pTextureAtlas, const string& p_szFullName, f32 p_fPivotX, f32 p_fPivotY )
{
	CCocos2dxDBTextureAtlas* pTextureAtlas = dynamic_cast<CCocos2dxDBTextureAtlas*>( p_pTextureAtlas );
	CCRect tagRect;
	CRect tagRegion = pTextureAtlas->GetRegion( p_szFullName );
#if USE_BATCH_NODE == 1
	tagRect.origin.x = static_cast<f32>(tagRegion.m_nX);
	tagRect.origin.y = static_cast<f32>(tagRegion.m_nY);
	tagRect.size.width		= static_cast<f32>(tagRegion.m_nWidth);
	tagRect.size.height		= static_cast<f32>(tagRegion.m_nHeight);
#else
	tagRect.origin.x		= 0.0f;
	tagRect.origin.y		= 0.0f;
	tagRect.size.width		= static_cast<f32>(tagRegion.m_nWidth);
	tagRect.size.height		= static_cast<f32>(tagRegion.m_nHeight);
#endif

	CCocos2dxDBAtlasNode* pAtlasNode = CCocos2dxDBAtlasNode::Create( pTextureAtlas->GetTextureAtlas(),
		pTextureAtlas->GetQuadIndex( p_szFullName ), tagRect );
	pAtlasNode->setCascadeColorEnabled( true );
	pAtlasNode->setAnchorPoint( CCPoint(p_fPivotX / static_cast<f32>(tagRegion.m_nWidth), 
		(tagRegion.m_nHeight - p_fPivotY) / static_cast<f32>(tagRegion.m_nHeight) ) );
	pAtlasNode->setContentSize( CCSize( static_cast<f32>(tagRegion.m_nWidth), 
		static_cast<f32>( tagRegion.m_nHeight ) ) );
	return new CCocos2dxDBObj( pAtlasNode );
}
//-------------------------------------------------------------------------
void CCocos2dxDBFactory::LoadSkeletonFile( const string& p_szSkeletonFile, const string& p_szName )
{
	if( IsExistSkeletonDataInDic( p_szName ) )
		return;

	CDargonBonesXMLParser					tagParser;
	unsigned long							ulDummySize = 0;
	XMLDocument								tagDoc;

	unsigned char* szSkeletonData = cocos2d::CCFileUtils::sharedFileUtils()->getFileData( p_szSkeletonFile.c_str(), "rb", &ulDummySize );
	tagDoc.Parse( reinterpret_cast<char*>(szSkeletonData), ulDummySize );
	delete [] szSkeletonData;

	CSkeletonData* pSketeton = tagParser.ParseSkeletonFromXML( tagDoc.RootElement() );
	// CCLOG("AddSkeletonData %s", pSketeton->m_szName.c_str() );
	AddSkeletonData( pSketeton );
}
//-------------------------------------------------------------------------
void CCocos2dxDBFactory::LoadTextureAtlasFile( const string& p_szTextureAtlasFile, const string& p_szName )
{
	if( IsExistTextureDataInDic( p_szTextureAtlasFile ) )
		return;

	CDargonBonesXMLParser					tagParser;
	unsigned long							ulDummySize = 0;
	XMLDocument								tagDoc;

	unsigned char* szTextureData = cocos2d::CCFileUtils::sharedFileUtils()->getFileData( p_szTextureAtlasFile.c_str(), "rb", &ulDummySize );
	tagDoc.Parse( reinterpret_cast<char*>(szTextureData), ulDummySize );
	delete [] szTextureData;

	// 添加资源路径
	int nPos = p_szTextureAtlasFile.find_last_of("/");
	if( nPos != string::npos )
	{
		string szBasePath = p_szTextureAtlasFile.substr( 0, nPos + 1 );
		string szImgPath = tagDoc.RootElement()->Attribute( ConstString::A_IMAGE_PATH.c_str() );
		string szNewImgPath = szBasePath + szImgPath;
		
		tagDoc.RootElement()->SetAttribute( ConstString::A_IMAGE_PATH.c_str(), szNewImgPath.c_str() );
	}

	CTextureAtlasData* pTextureAtlas = tagParser.ParseTextureFromXML( tagDoc.RootElement() );
	// CCLOG("AddTextureAtlas %s", pTextureAtlas->m_szName.c_str() );
	AddTextureAtlas( new CCocos2dxDBTextureAtlas( pTextureAtlas ) );
}
//-------------------------------------------------------------------------