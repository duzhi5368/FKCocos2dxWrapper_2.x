//-------------------------------------------------------------------------
#include "FKRenderObjModel.h"
//-------------------------------------------------------------------------
#define CUR_FILE_VERSION		10001
#define MAX_PATH          260
//-------------------------------------------------------------------------
FKRenderObjModel::FKRenderObjModel()
{
	m_strObjName.clear();
	m_nVersion = CUR_FILE_VERSION;
	m_pOffset = CCPointZero;
	m_mapObjParts.clear();
	m_mapObjEffects.clear();
	m_mapObjLocators.clear();
}
//-------------------------------------------------------------------------
FKRenderObjModel::~FKRenderObjModel()
{

}
//-------------------------------------------------------------------------
// 解析一个文件
bool FKRenderObjModel::LoadFromFKOFile( const string& strFKOFilePathName )
{
	string szFKOFilePathName = strFKOFilePathName;
	if( szFKOFilePathName.empty() )
	{
		szFKOFilePathName = m_strObjName;
	}
	if( szFKOFilePathName.empty() )
		return false;

	szFKOFilePathName = CCFileUtils::sharedFileUtils()->fullPathForFilename( szFKOFilePathName.c_str() );

	const string strTemp = szFKOFilePathName;
	FKCW_IO_AssetInputStream* pStream = FKCW_IO_AssetInputStream::Create( strTemp );
	if( pStream == NULL )
		return false;
	if( !pStream->Open() )
		return false;

	int nHeadFlag = 0;
	pStream->ReadInt( &nHeadFlag );
	if( nHeadFlag != MAKEFOURCC( 'F', 'K', 'O', 0 ) )
		return false;

	pStream->ReadInt( &m_nVersion );

	char szTmp[MAX_PATH];
	memset( szTmp, 0, MAX_PATH );
	pStream->Read( szTmp, sizeof(float) );
	m_pOffset.x = (float)FKCW_Base_Utils::AtoF( szTmp );
	memset( szTmp, 0, MAX_PATH );
	pStream->Read( szTmp, sizeof(float) );
	m_pOffset.y = (float)FKCW_Base_Utils::AtoF( szTmp );

	int nStrLen = 0;
	{
		int nSize = 0;
		pStream->ReadInt( &nSize );
		m_mapObjParts.clear();
		for( int i = 0; i < nSize; ++i )
		{
			SObjPart tagPart;
			memset( szTmp, 0, MAX_PATH );
			nStrLen = 0;
			pStream->ReadInt( &nStrLen );
			pStream->Read( szTmp, nStrLen );
			tagPart.m_strName = string(szTmp);

			memset( szTmp, 0, MAX_PATH );
			nStrLen = 0;
			pStream->ReadInt( &nStrLen );
			pStream->Read( szTmp, nStrLen );
			tagPart.m_strFile = string(szTmp);

			pStream->ReadInt( &tagPart.m_nDepth );

			m_mapObjParts[tagPart.m_strName] = tagPart;
		}
	}
	{
		int nSize = 0;
		pStream->ReadInt( &nSize );
		m_mapObjLocators.clear();
		for( int i = 0; i < nSize; ++i )
		{
			SObjLocator tagLocator;
			memset( szTmp, 0, MAX_PATH );
			nStrLen = 0;
			pStream->ReadInt( &nStrLen );
			pStream->Read( szTmp, nStrLen );
			tagLocator.m_strName = string(szTmp);

			memset( szTmp, 0, MAX_PATH );
			pStream->Read( szTmp, sizeof(float) );
			tagLocator.m_tagOffset.x = (float)FKCW_Base_Utils::AtoF( szTmp );

			memset( szTmp, 0, MAX_PATH );
			pStream->Read( szTmp, sizeof(float) );
			tagLocator.m_tagOffset.y = (float)FKCW_Base_Utils::AtoF( szTmp );

			m_mapObjLocators[tagLocator.m_strName] = tagLocator;
		}
	}
	{
		int nSize = 0;
		pStream->ReadInt( &nSize );
		m_mapObjEffects.clear();
		for( int i = 0; i < nSize; ++i )
		{
			SObjEffect tagEffect;
			memset( szTmp, 0, MAX_PATH );
			nStrLen = 0;
			pStream->ReadInt( &nStrLen );
			pStream->Read( szTmp, nStrLen );
			tagEffect.m_strName = string(szTmp);

			memset( szTmp, 0, MAX_PATH );
			nStrLen = 0;
			pStream->ReadInt( &nStrLen );
			pStream->Read( szTmp, nStrLen );
			tagEffect.m_strFileName = string(szTmp);

			memset( szTmp, 0, MAX_PATH );
			nStrLen = 0;
			pStream->ReadInt( &nStrLen );
			pStream->Read( szTmp, nStrLen );
			tagEffect.m_strLocator = string(szTmp);

			memset( szTmp, 0, MAX_PATH );
			pStream->Read( szTmp, sizeof(float) );
			tagEffect.m_tagOffset.x = (float)FKCW_Base_Utils::AtoF( szTmp );

			memset( szTmp, 0, MAX_PATH );
			pStream->Read( szTmp, sizeof(float) );
			tagEffect.m_tagOffset.y = (float)FKCW_Base_Utils::AtoF( szTmp );

			memset( szTmp, 0, MAX_PATH );
			pStream->Read( szTmp, sizeof(float) );
			tagEffect.m_tagScale.x = (float)FKCW_Base_Utils::AtoF( szTmp );

			memset( szTmp, 0, MAX_PATH );
			pStream->Read( szTmp, sizeof(float) );
			tagEffect.m_tagScale.y = (float)FKCW_Base_Utils::AtoF( szTmp );

			pStream->ReadInt( &tagEffect.m_nDepth );

			pStream->ReadByte( (char*)&tagEffect.m_tagColor.r );
			pStream->ReadByte( (char*)&tagEffect.m_tagColor.g );
			pStream->ReadByte( (char*)&tagEffect.m_tagColor.b );
			pStream->ReadByte( (char*)&tagEffect.m_tagColor.a );

			m_mapObjEffects[tagEffect.m_strName] = tagEffect;
		}
	}
	pStream->Close();
	return true;
}
//-------------------------------------------------------------------------
// 保存本模型到一个文件中
bool FKRenderObjModel::SaveToFKOFile( const string& strFKOFilePathName )
{
	if( m_nVersion == 10001 )
	{
		return __SaveToFKOFile_10001( strFKOFilePathName );
	}
	return false;
}
//-------------------------------------------------------------------------
// 添加一个组件
bool FKRenderObjModel::AddPart( const string& strName, const string& strFile, int nDepth )
{
	SObjPart tagPart;
	tagPart.m_strName = strName;
	tagPart.m_strFile = strFile;
	tagPart.m_nDepth = nDepth;
	m_mapObjParts[strName] = tagPart;
	return true;
}
//-------------------------------------------------------------------------
// 移除一个组件
void FKRenderObjModel::RemovePart( const string& strName )
{
	m_mapObjParts.erase( strName );
}
//-------------------------------------------------------------------------
// 添加一个挂接对象
bool FKRenderObjModel::AddLocator( const string& strLocatorName, CCPoint& ptOffset )
{
	SObjLocator tagLocator;
	tagLocator.m_strName = strLocatorName;
	tagLocator.m_tagOffset = ptOffset;
	m_mapObjLocators[strLocatorName] = tagLocator;
	return true;
}
//-------------------------------------------------------------------------
// 删除一个挂接对象
void FKRenderObjModel::RemoveLocator( const string& strLocatorName )
{
	m_mapObjLocators.erase( strLocatorName );
}
//-------------------------------------------------------------------------
// 添加特效节点
bool FKRenderObjModel::AddEffect( const string& strEffectName, const string& strLocatorName, 
	const string& strEffectFileName, CCPoint& ptOffset, ccColor4B& color, CCPoint& scale, int nDepth )
{
	SObjEffect tagEffect;
	tagEffect.m_strName = strEffectName;
	tagEffect.m_strFileName = strEffectFileName;
	tagEffect.m_strLocator = strLocatorName;
	tagEffect.m_tagOffset = ptOffset;
	tagEffect.m_tagColor = color;
	tagEffect.m_tagScale = scale;
	tagEffect.m_nDepth = nDepth;
	m_mapObjEffects[strEffectName] = tagEffect;
	return true;
}
//-------------------------------------------------------------------------
// 删除特效节点
void FKRenderObjModel::RemoveEffect( const string& strEffectName )
{
	m_mapObjEffects.erase( strEffectName );
}
//-------------------------------------------------------------------------
// 保存10001版本的文件
bool FKRenderObjModel::__SaveToFKOFile_10001( const string& strFKOFilePathName )
{
	int nVersion = CUR_FILE_VERSION;

	string szFKOFilePathName = strFKOFilePathName;
	if( szFKOFilePathName.empty() )
	{
		szFKOFilePathName = m_strObjName;
	}
	if( szFKOFilePathName.empty() )
		return false;

	szFKOFilePathName = CCFileUtils::sharedFileUtils()->fullPathForFilename( szFKOFilePathName.c_str() );
	FKCW_IO_AssetOutputStream* pWriter = FKCW_IO_AssetOutputStream::Create( szFKOFilePathName, false );
	if( pWriter == NULL )
		return false;
	if( !pWriter->Open() )
		return false;
	{
		// 开始实际写入
		int nHeadFlag = MAKEFOURCC( 'F', 'K', 'O', 0 );

		pWriter->Write( (const char*)((void*)&nHeadFlag), sizeof( int ) );
		pWriter->Write( (const char*)((void*)&nVersion), sizeof( int ) );
		pWriter->Write( (const char*)((void*)&m_pOffset.x), sizeof( float ) );
		pWriter->Write( (const char*)((void*)&m_pOffset.y), sizeof( float ) );

		{
			unsigned int unSize = m_mapObjParts.size();
			pWriter->Write( (const char*)((void*)&unSize), sizeof( unsigned int ) );
			ObjPartMap::iterator Ite = m_mapObjParts.begin();
			for( ; Ite != m_mapObjParts.end(); ++Ite )
			{
				int nLen = 0;
				nLen = Ite->second.m_strName.length();
				pWriter->Write( (const char*)((void*)&nLen), 
					sizeof( unsigned int ) );
				pWriter->Write( (const char*)((void*)&(Ite->second.m_strName)), nLen );

				nLen = Ite->second.m_strFile.length();
				pWriter->Write( (const char*)((void*)&nLen), 
					sizeof( unsigned int ) );
				pWriter->Write( (const char*)((void*)&(Ite->second.m_strFile)), nLen );

				pWriter->Write( (const char*)((void*)&(Ite->second.m_nDepth)), 
					sizeof( int ) );
			}
		}

		{
			unsigned int unSize = m_mapObjLocators.size();
			pWriter->Write( (const char*)((void*)&unSize), sizeof( unsigned int ) );
			ObjLocatorMap::iterator Ite = m_mapObjLocators.begin();
			for( ; Ite != m_mapObjLocators.end(); ++Ite )
			{
				int nLen = 0;
				nLen = Ite->second.m_strName.length();
				pWriter->Write( (const char*)((void*)&nLen), 
					sizeof( unsigned int ) );
				pWriter->Write( (const char*)((void*)&(Ite->second.m_strName)), nLen );

				pWriter->Write( (const char*)((void*)&(Ite->second.m_tagOffset.x)), 
					sizeof( float ) );
				pWriter->Write( (const char*)((void*)&(Ite->second.m_tagOffset.y)), 
					sizeof( float ) );
			}
		}

		{
			unsigned int unSize = m_mapObjEffects.size();
			pWriter->Write( (const char*)((void*)&unSize), sizeof( unsigned int ) );
			ObjEffectMap::iterator Ite = m_mapObjEffects.begin();
			for( ; Ite != m_mapObjEffects.end(); ++Ite )
			{
				int nLen = 0;
				nLen = Ite->second.m_strName.length();
				pWriter->Write( (const char*)((void*)&nLen), 
					sizeof( unsigned int ) );
				pWriter->Write( (const char*)((void*)&(Ite->second.m_strName)), nLen );

				nLen = Ite->second.m_strFileName.length();
				pWriter->Write( (const char*)((void*)&nLen), 
					sizeof( unsigned int ) );
				pWriter->Write( (const char*)((void*)&(Ite->second.m_strFileName)), nLen );

				nLen = Ite->second.m_strLocator.length();
				pWriter->Write( (const char*)((void*)&nLen), 
					sizeof( unsigned int ) );
				pWriter->Write( (const char*)((void*)&(Ite->second.m_strLocator)), nLen );

				pWriter->Write( (const char*)((void*)&(Ite->second.m_tagOffset.x)), 
					sizeof( float ) );
				pWriter->Write( (const char*)((void*)&(Ite->second.m_tagOffset.y)), 
					sizeof( float ) );

				pWriter->Write( (const char*)((void*)&(Ite->second.m_tagScale.x)), 
					sizeof( float ) );
				pWriter->Write( (const char*)((void*)&(Ite->second.m_tagScale.y)), 
					sizeof( float ) );

				pWriter->Write( (const char*)((void*)&(Ite->second.m_nDepth)), 
					sizeof( int ) );

				pWriter->Write( (const char*)((void*)&(Ite->second.m_tagColor.r)), 
					sizeof( unsigned char ) );
				pWriter->Write( (const char*)((void*)&(Ite->second.m_tagColor.g)), 
					sizeof( unsigned char ) );
				pWriter->Write( (const char*)((void*)&(Ite->second.m_tagColor.b)), 
					sizeof( unsigned char ) );
				pWriter->Write( (const char*)((void*)&(Ite->second.m_tagColor.a)), 
					sizeof( unsigned char ) );
			}
		}
	}

	pWriter->Close();
	return true;
}
//-------------------------------------------------------------------------