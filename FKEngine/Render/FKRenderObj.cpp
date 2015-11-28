//-------------------------------------------------------------------------
#include "FKRenderObj.h"
#include "FKRenderObjMgr.h"
#include "FKRenderableMgr.h"
#include "FKRenderObjModel.h"
#include "FKMapObj.h"
#include "FKMapMgr.h"
//-------------------------------------------------------------------------
FKRenderObj::FKRenderObj( RENDER_OBJ_ID id )
	: m_unID( id )
	, m_pUserData( nullptr )
	, m_pRootRenderable( nullptr )
	, m_pModel( nullptr )
	, m_bIsNeedUpdate( false )
	, m_bIsCanbeSelected( false )
	, m_bIsInScene( false )
{
	m_strFKOFileName.clear();
	m_strCurAniName.clear();

	m_tagPos = CCPointZero;
	m_tagOffset = CCPointZero;

	m_mapObjParts.clear();
	m_mapObjLocators.clear();
	m_mapObjEffects.clear();
	m_mapObjLinks.clear();
	m_setObjRenderTexts.clear();
}
//-------------------------------------------------------------------------
FKRenderObj::~FKRenderObj()
{

}
//-------------------------------------------------------------------------
// 更新函数
void FKRenderObj::update( float delta )
{
	// 更新组件
	ObjPartMap::iterator ItePart = m_mapObjParts.begin();
	for( ; ItePart !=  m_mapObjParts.end(); ++ItePart )
	{
		if( ItePart->second )
		{
			ItePart->second->update( delta );
		}
	}
	// 更新特效
	ObjEffectMap::iterator IteEffect = m_mapObjEffects.begin();
	for( ; IteEffect !=  m_mapObjEffects.end(); ++IteEffect )
	{
		if( IteEffect->second )
		{
			IteEffect->second->update( delta );
		}
	}
	// 更新链接对象
	ObjLinkMap::iterator IteLink = m_mapObjLinks.begin();
	for( ; IteLink !=  m_mapObjLinks.end(); ++IteLink )
	{
		if( IteLink->second.m_pLinkObj )
		{
			IteLink->second.m_pLinkObj->update( delta );
		}
	}
}
//-------------------------------------------------------------------------
// 创建函数
bool FKRenderObj::Create( const char* szFKOFileName )
{
	if( szFKOFileName == NULL )
		return false;

	string strPathName, strFileName, strExt;
	string strFKOFileName = szFKOFileName;
	if( !FKCW_Base_Utils::ParseFilePathName( strFKOFileName, strPathName, strFileName, strExt ) )
		return false;

	FKCW_Base_Utils::ToLowerCase( strExt );
	if( strExt != "fko" )
	{
		CCRect tagRect = CCRectZero;
		ISprite* pSprite = FKRenderableMgr::GetInstance().CreateSprite( szFKOFileName, tagRect );
		if( pSprite == NULL )
			return false;

		addChild( pSprite );
		pSprite->release();

		m_strFKOFileName = szFKOFileName;
		m_pRootRenderable = pSprite;
		return true;
	}

	__CreateParts();
	__CreateEffects();

	m_strFKOFileName = szFKOFileName;
	return true;
}
//-------------------------------------------------------------------------
// 释放函数
void FKRenderObj::onExit()
{
	removeAllChildren();

	if( m_pModel )
	{
		m_pModel->release();
		m_pModel = NULL;
	}

	m_mapObjLinks.clear();
}
//-------------------------------------------------------------------------
// 获取ID
RENDER_OBJ_ID FKRenderObj::GetID()
{
	return m_unID;
}
//-------------------------------------------------------------------------
// 设置用户数据
// 参数：pData 自定义用户数据
void FKRenderObj::SetUserData( void* pData )
{
	m_pUserData = pData;
}
//-------------------------------------------------------------------------
// 获取用户数据
void* FKRenderObj::GetUserData()
{
	return m_pUserData;
}
//-------------------------------------------------------------------------
// 获取游戏对象贴图文件名
const std::string& FKRenderObj::GetFileName()
{
	return m_strFKOFileName;
}
//-------------------------------------------------------------------------
// 获取游戏对象特征串名(全局唯一字符串)
const std::string& FKRenderObj::GetDescName()
{
	return m_strFKOFileName;
}
//-------------------------------------------------------------------------
// 清除全部挂接对象，仅保留空对象
void FKRenderObj::Clear()
{
	ObjPartMap::iterator ItePart = m_mapObjParts.begin();
	for( ; ItePart !=  m_mapObjParts.end(); ++ItePart )
	{
		if( ItePart->second )
		{
			removeChild( ItePart->second );
			ItePart->second = NULL;
		}
	}
	m_mapObjParts.clear();
	ObjEffectMap::iterator IteEffect = m_mapObjEffects.begin();
	for( ; IteEffect !=  m_mapObjEffects.end(); ++IteEffect )
	{
		if( IteEffect->second )
		{
			removeChild( IteEffect->second );
			IteEffect->second = NULL;
		}
	}
	m_mapObjEffects.clear();

	RemoveAllLinkObj();
}
//-------------------------------------------------------------------------
// 完全清理
void FKRenderObj::Release()
{
	Clear();
	this->release();
}
//-------------------------------------------------------------------------
// 绑定到场景上
// 参数：pParentNode 父节点
// 参数：nZOrder 深度Z值
// 参数：nTag 标签
void FKRenderObj::Attach( CCNode* pParentNode, int nZOrder, int nTag )
{
	if( pParentNode == NULL )
		return;
	pParentNode->addChild( this, nZOrder, nTag );
}
//-------------------------------------------------------------------------
// 从场景中移除
// 参数：bIsRemove 移除后是否进行删除
void FKRenderObj::Detach( bool bIsRemove )
{
	CCNode* pParentNode = getParent();
	if( pParentNode == NULL )
		return;
	if( !bIsRemove )
		retain();	// 注意，这里就要求外部手动release了
	pParentNode->removeChild( this );
}
//-------------------------------------------------------------------------
// 设置位置
// 参数：tagPoint 位置
void FKRenderObj::SetPos( const CCPoint& tagPoint )
{
	if( m_bIsInScene )
	{
		if( GetRenderSystem() )
		{
			FKMapObj* pCurMap = (FKMapObj*)(GetRenderSystem()->GetCurrentMap());
			if( pCurMap )
				pCurMap->SetObjPos( m_unID, tagPoint );
		}
	}
	else
	{
		setPosition( tagPoint );
	}
	m_tagPos = tagPoint;
}
//-------------------------------------------------------------------------
// 获取位置
// 参数：[out]tagPoint 位置
void FKRenderObj::GetPos( CCPoint& tagPoint )
{
	tagPoint = m_tagPos;
}
//-------------------------------------------------------------------------
// 设置偏移量
void FKRenderObj::SetOffset( CCPoint& tagPoint )
{
	ObjPartMap::iterator Ite = m_mapObjParts.begin();
	for( ; Ite != m_mapObjParts.end(); ++Ite )
	{
		if( Ite->second )
			Ite->second->SetOffset( tagPoint );
	}
	m_tagOffset = tagPoint;
}
//-------------------------------------------------------------------------
// 获取偏移量			 
CCPoint FKRenderObj::GetOffset()
{
	if( m_pRootRenderable == NULL )
		return CCPointZero;
	CCPoint ptRet;
	m_pRootRenderable->GetOffset( ptRet );
	return ptRet;
}
//-------------------------------------------------------------------------
// 设置颜色
// 参数：tagColor 颜色值
void FKRenderObj::SetColor( const ccColor3B& tagColor )
{
	ObjPartMap::iterator ItePart = m_mapObjParts.begin();
	for( ; ItePart !=  m_mapObjParts.end(); ++ItePart )
	{
		if( ItePart->second )
		{
			ItePart->second->SetColor( tagColor );
		}
	}
	ObjEffectMap::iterator IteEffect = m_mapObjEffects.begin();
	for( ; IteEffect !=  m_mapObjEffects.end(); ++IteEffect )
	{
		if( IteEffect->second )
		{
			IteEffect->second->SetColor( tagColor );
		}
	}
	ObjLinkMap::iterator IteLink = m_mapObjLinks.begin();
	for( ; IteLink !=  m_mapObjLinks.end(); ++IteLink )
	{
		if( IteLink->second.m_pLinkObj )
		{
			IteLink->second.m_pLinkObj->SetColor( tagColor );
		}
	}
}
//-------------------------------------------------------------------------
// 设置透明度
// 参数：byAlpha 透明度[0,255]
void FKRenderObj::SetOpacity( unsigned char byAlpha )
{
	ObjPartMap::iterator ItePart = m_mapObjParts.begin();
	for( ; ItePart !=  m_mapObjParts.end(); ++ItePart )
	{
		if( ItePart->second )
		{
			ItePart->second->SetOpacity( byAlpha );
		}
	}
	ObjEffectMap::iterator IteEffect = m_mapObjEffects.begin();
	for( ; IteEffect !=  m_mapObjEffects.end(); ++IteEffect )
	{
		if( IteEffect->second )
		{
			IteEffect->second->SetOpacity( byAlpha );
		}
	}
	ObjLinkMap::iterator IteLink = m_mapObjLinks.begin();
	for( ; IteLink !=  m_mapObjLinks.end(); ++IteLink )
	{
		if( IteLink->second.m_pLinkObj )
		{
			IteLink->second.m_pLinkObj->SetOpacity( byAlpha );
		}
	}
}
//-------------------------------------------------------------------------
// 设置缩放
void FKRenderObj::SetScale( CCPoint& tagPoint )
{
	ObjPartMap::iterator ItePart = m_mapObjParts.begin();
	for( ; ItePart !=  m_mapObjParts.end(); ++ItePart )
	{
		if( ItePart->second )
		{
			ItePart->second->setScaleX( tagPoint.x );
			ItePart->second->setScaleY( tagPoint.y );
		}
	}
	ObjEffectMap::iterator IteEffect = m_mapObjEffects.begin();
	for( ; IteEffect !=  m_mapObjEffects.end(); ++IteEffect )
	{
		if( IteEffect->second )
		{
			IteEffect->second->setScaleX( tagPoint.x );
			IteEffect->second->setScaleY( tagPoint.y );
		}
	}
	ObjLinkMap::iterator IteLink = m_mapObjLinks.begin();
	for( ; IteLink !=  m_mapObjLinks.end(); ++IteLink )
	{
		if( IteLink->second.m_pLinkObj )
		{
			IteLink->second.m_pLinkObj->SetScale( tagPoint );
		}
	}
}
//-------------------------------------------------------------------------
// 是否可见
// 参数：bIsVisible 是否可见
void FKRenderObj::IsVisible( bool bIsVisible )
{
	setVisible( bIsVisible );
}
//-------------------------------------------------------------------------
// 是否允许Update
void FKRenderObj::SetEnableUpdate( bool bIsEnable )
{
	if( m_bIsNeedUpdate == bIsEnable )
		return;

	if( bIsEnable )
		scheduleUpdate();
	else
		unscheduleUpdate();

	m_bIsNeedUpdate = bIsEnable;
}
//-------------------------------------------------------------------------
// 碰撞检测
// 参数：pt 检测点
// 返回值：true 表示碰撞，false 表示未碰撞
bool FKRenderObj::HitTest( const CCPoint& pt )
{
	if( m_pRootRenderable == NULL )
		return false;

	CCRect tagRect;
	m_pRootRenderable->GetBoundingBox( tagRect );
	tagRect = CCRectApplyAffineTransform( tagRect, nodeToParentTransform() );
	return tagRect.containsPoint( pt );
}
//-------------------------------------------------------------------------
// 碰撞检测
// 参数：rt 检测矩形
// 返回值：true 表示碰撞，false 表示未碰撞
bool FKRenderObj::HitTest( const CCRect& rt )
{
	if( m_pRootRenderable == NULL )
		return false;

	CCRect tagRect;
	m_pRootRenderable->GetBoundingBox( tagRect );
	tagRect = CCRectApplyAffineTransform( tagRect, nodeToParentTransform() );
	return tagRect.intersectsRect( rt );
}
//-------------------------------------------------------------------------
// 设置是否可以被选中
// 参数：bIsEnable 是否开启被选中检测
void FKRenderObj::SetEnableSelect( bool bIsEnable )
{
	m_bIsCanbeSelected = bIsEnable;
}
//-------------------------------------------------------------------------
// 挂接对象
RENDER_OBJ_ID FKRenderObj::AddLinkObj( const char* szLinkObjName, const char* szLocator,
								 int nDepth, ccColor4B color )
{
	if( m_pModel == NULL )
		return 0;
	IRenderSystem* pRenderSystem = GetRenderSystem();
	if( pRenderSystem == NULL )
		return 0;

	SLinkObj tagLinkObj;
	tagLinkObj.m_pLinkObj = (FKRenderObj*)pRenderSystem->CreateRenderObj( szLinkObjName );
	if( tagLinkObj.m_pLinkObj == NULL )
		return 0;
	tagLinkObj.m_strLocator = "";

	CCPoint ptLoc = CCPointZero;
	if( szLocator != NULL )
	{
		tagLinkObj.m_strLocator = szLocator;
		FKRenderObjModel::ObjLocatorMap::iterator Ite = m_pModel->m_mapObjLocators.find( tagLinkObj.m_strLocator );
		if( Ite != m_pModel->m_mapObjLocators.end() )
		{
			ptLoc = Ite->second.m_tagOffset;
		}
	}

	addChild( tagLinkObj.m_pLinkObj, nDepth );
	tagLinkObj.m_pLinkObj->release();

	tagLinkObj.m_pLinkObj->SetPos( ptLoc );
	tagLinkObj.m_pLinkObj->SetColor( ccc3(color.r, color.g, color.b) );
	tagLinkObj.m_pLinkObj->SetOpacity( color.a );

	m_mapObjLinks[tagLinkObj.m_pLinkObj->GetID()] = tagLinkObj;
	return tagLinkObj.m_pLinkObj->GetID();
}
//-------------------------------------------------------------------------
// 挂接对象
RENDER_OBJ_ID FKRenderObj::AddLinkObj( IRenderObj* pRenderObj, const char* szLocator,
								  int nDepth, ccColor4B color )
{
	if( m_pModel == NULL || pRenderObj == NULL )
		return 0;
	SLinkObj tagLinkObj;
	tagLinkObj.m_pLinkObj = (FKRenderObj*)pRenderObj;
	tagLinkObj.m_strLocator = "";

	CCPoint ptLoc = CCPointZero;
	if( szLocator != NULL )
	{
		tagLinkObj.m_strLocator = szLocator;
		FKRenderObjModel::ObjLocatorMap::iterator Ite = m_pModel->m_mapObjLocators.find( tagLinkObj.m_strLocator );
		if( Ite != m_pModel->m_mapObjLocators.end() )
		{
			ptLoc = Ite->second.m_tagOffset;
		}
	}

	addChild( tagLinkObj.m_pLinkObj, nDepth );
	tagLinkObj.m_pLinkObj->release();

	tagLinkObj.m_pLinkObj->SetPos( ptLoc );
	tagLinkObj.m_pLinkObj->SetColor( ccc3(color.r, color.g, color.b) );
	tagLinkObj.m_pLinkObj->SetOpacity( color.a );

	m_mapObjLinks[tagLinkObj.m_pLinkObj->GetID()] = tagLinkObj;
	return tagLinkObj.m_pLinkObj->GetID();
}
//-------------------------------------------------------------------------
// 删除一个挂接对象
// 参数：unLinkObjID 挂接对象的ID
bool FKRenderObj::RemoveLinkObj( RENDER_OBJ_ID unLinkObjID )
{
	ObjLinkMap::iterator Ite = m_mapObjLinks.find( unLinkObjID );
	if( Ite == m_mapObjLinks.end() )
		return true;
	if( Ite->second.m_pLinkObj == NULL )
		return true;

	removeChild( Ite->second.m_pLinkObj );
	m_mapObjLinks.erase( Ite );
	return true;
}
//-------------------------------------------------------------------------
// 删除一个挂接对象
// 参数：unLinkObjID 挂接对象的ID
// 参数：bIsRemove 是否删除本挂节对象
IRenderObj* FKRenderObj::RemoveLinkObj( RENDER_OBJ_ID unLinkObjID, bool bIsRemove )
{
	IRenderObj* pRet = NULL;
	ObjLinkMap::iterator Ite = m_mapObjLinks.find( unLinkObjID );
	if( Ite == m_mapObjLinks.end() )
		return NULL;
	if( Ite->second.m_pLinkObj == NULL )
		return NULL;
	if( !bIsRemove )
		Ite->second.m_pLinkObj->retain();

	pRet = Ite->second.m_pLinkObj;
	removeChild( Ite->second.m_pLinkObj );
	m_mapObjLinks.erase( Ite );
	return pRet;
}
//-------------------------------------------------------------------------
// 删除全部挂接对象
void FKRenderObj::RemoveAllLinkObj()
{
	ObjLinkMap::iterator Ite = m_mapObjLinks.begin();
	for( ; Ite != m_mapObjLinks.end(); ++Ite )
	{
		if( Ite->second.m_pLinkObj )
			removeChild( Ite->second.m_pLinkObj );
	}
	m_mapObjLinks.clear();
}
//-------------------------------------------------------------------------
// 获取一个挂节对象
// 参数：RENDER_OBJ_ID 挂节对象ID
IRenderObj* FKRenderObj::GetLinkObjByID( RENDER_OBJ_ID unID )
{
	ObjLinkMap::iterator Ite = m_mapObjLinks.find( unID );
	if( Ite != m_mapObjLinks.end() )
		return Ite->second.m_pLinkObj;
	return NULL;
}
//-------------------------------------------------------------------------
// 更新一个部件
void FKRenderObj::ChangePart( const char* szPartObjName, const char* szPartPlaceName, int nDepth )
{
	if( szPartObjName == NULL || szPartPlaceName == NULL || m_pModel == NULL )
		return;

	string strPartObjName = szPartObjName;
	string strPartPlaceName = szPartPlaceName;
	FKRenderObjModel::ObjPartMap::iterator Ite = m_pModel->m_mapObjParts.find( szPartPlaceName );
	if( Ite == m_pModel->m_mapObjParts.end() )
	{
		m_pModel->AddPart( strPartPlaceName, strPartObjName, nDepth );
		Ite = m_pModel->m_mapObjParts.find( szPartPlaceName );
	}
	else
	{
		if( Ite->second.m_strFile == strPartObjName && Ite->second.m_nDepth == nDepth )
			return;
	}

	float fAniSpeed = 1.0f;
	if( m_pRootRenderable && ( m_pRootRenderable->GetRenderObjType() == eROT_Ani 
		|| m_pRootRenderable->GetRenderObjType() == eROT_Armature 
		|| m_pRootRenderable->GetRenderObjType() == eROT_DB ) )
	{
		IRenderable* pObj = m_pRootRenderable;
		IAnimation* pAni = pObj->GetAnimation();
		if( pAni )
		{
			fAniSpeed = pAni->GetSpeed();
		}
	}

	string strPart = Ite->second.m_strFile;
	Ite->second.m_strFile = szPartObjName;
	Ite->second.m_nDepth = nDepth;
	if( !__CreatePart(Ite->second) )
	{
		// 恢复
		Ite->second.m_strFile = strPart;
	}

	//  播放动画
	Play( m_strCurAniName.c_str(), fAniSpeed, -1, true );
}
//-------------------------------------------------------------------------
// 更新一个特效
void FKRenderObj::ChangeEffect( const string& strEffectName, const string& strLocatorName, 
									 const string& strEffectFileName, CCPoint& tagOffset,
									 ccColor4B& color, CCPoint& tagScale, int nDepth )
{
	if( m_pModel == NULL )
		return;
	FKRenderObjModel::ObjEffectMap::iterator Ite = m_pModel->m_mapObjEffects.find( strEffectName );
	if( Ite == m_pModel->m_mapObjEffects.end() )
	{
		m_pModel->AddEffect( strEffectName, strLocatorName, strEffectFileName,
			tagOffset, color, tagScale, nDepth );
		Ite = m_pModel->m_mapObjEffects.find( strEffectName );
	}

	ObjEffectMap::iterator IteObj = m_mapObjEffects.find( strEffectName );
	if( IteObj != m_mapObjEffects.end() )
	{
		if( IteObj->second != NULL )
		{
			removeChild( IteObj->second );
			IteObj->second = NULL;
		}
	}

	Ite->second.m_strName		= strEffectName;
	Ite->second.m_strFileName	= strEffectFileName;
	Ite->second.m_strLocator	= strLocatorName;
	Ite->second.m_tagOffset		= tagOffset;
	Ite->second.m_tagColor		= color;
	Ite->second.m_tagScale		= tagScale;
	Ite->second.m_nDepth		= nDepth;

	__CreateEffect( Ite->second );
}
//-------------------------------------------------------------------------

// 设置动画播放速度
void FKRenderObj::SetAnimationSpeed( float fSpeed )
{
	ObjPartMap::iterator ItePart = m_mapObjParts.begin();
	for( ; ItePart !=  m_mapObjParts.end(); ++ItePart )
	{
		if( ItePart->second && ( ItePart->second->GetRenderObjType() == eROT_Ani 
			|| ItePart->second->GetRenderObjType() == eROT_Armature 
			|| ItePart->second->GetRenderObjType() == eROT_DB ))
		{
			IRenderable* pObj = ItePart->second;
			IAnimation* pAni = pObj->GetAnimation();
			if( pAni )
			{
				pAni->SetSpeed( fSpeed );
			}
		}
	}
	ObjEffectMap::iterator IteEffect = m_mapObjEffects.begin();
	for( ; IteEffect !=  m_mapObjEffects.end(); ++IteEffect )
	{
		if( IteEffect->second && ( IteEffect->second->GetRenderObjType() == eROT_Ani 
			|| IteEffect->second->GetRenderObjType() == eROT_Armature 
			|| IteEffect->second->GetRenderObjType() == eROT_DB ))
		{
			IRenderable* pObj = IteEffect->second;
			IAnimation* pAni = pObj->GetAnimation();
			if( pAni )
			{
				pAni->SetSpeed( fSpeed );
			}
		}
	}
	ObjLinkMap::iterator IteLink = m_mapObjLinks.begin();
	for( ; IteLink !=  m_mapObjLinks.end(); ++IteLink )
	{
		if( IteLink->second.m_pLinkObj )
		{
			IteLink->second.m_pLinkObj->SetAnimationSpeed( fSpeed );
		}
	}
}
//-------------------------------------------------------------------------
// 设置动画播放回调
void FKRenderObj::SetAnimationCallback( IAnimationCallback* pCallback, void* pContext )
{
	if( m_pRootRenderable == NULL )
		return;

	if( m_pRootRenderable->GetRenderObjType() == eROT_Ani 
		|| m_pRootRenderable->GetRenderObjType() == eROT_Armature 
		|| m_pRootRenderable->GetRenderObjType() == eROT_DB )
	{
		IRenderable* pObj = m_pRootRenderable;
		IAnimation* pAni = pObj->GetAnimation();
		if( pAni )
		{
			pAni->SetAniCallback( pCallback, pContext );
		}
	}
}
//-------------------------------------------------------------------------
// 播放动画，若动作处于停止状态，则默认继续播放（也可强制从头播放）
// 参数：szAniName 动作名
// 参数：fSpeed 播放速度
// 参数：nLoops 播放重复次数，若为-1表示无限循环
// 参数：bIsForceUpdate 是否强制从头播放
void FKRenderObj::Play( const char* szAniName, float fSpeed, int nLoops, bool bIsForceUpdate )
{
	ObjPartMap::iterator ItePart = m_mapObjParts.begin();
	for( ; ItePart !=  m_mapObjParts.end(); ++ItePart )
	{
		if( ItePart->second && ( ItePart->second->GetRenderObjType() == eROT_Ani 
			|| ItePart->second->GetRenderObjType() == eROT_Armature 
			|| ItePart->second->GetRenderObjType() == eROT_DB ))
		{
			IRenderable* pObj = ItePart->second;
			IAnimation* pAni = pObj->GetAnimation();
			if( pAni )
			{
				pAni->Play( szAniName, fSpeed, nLoops, bIsForceUpdate );
			}
		}
	}
	ObjEffectMap::iterator IteEffect = m_mapObjEffects.begin();
	for( ; IteEffect !=  m_mapObjEffects.end(); ++IteEffect )
	{
		if( IteEffect->second && ( IteEffect->second->GetRenderObjType() == eROT_Ani 
			|| IteEffect->second->GetRenderObjType() == eROT_Armature 
			|| IteEffect->second->GetRenderObjType() == eROT_DB ))
		{
			IRenderable* pObj = IteEffect->second;
			IAnimation* pAni = pObj->GetAnimation();
			if( pAni )
			{
				pAni->Play( szAniName, fSpeed, nLoops, bIsForceUpdate );
			}
		}
	}
	ObjLinkMap::iterator IteLink = m_mapObjLinks.begin();
	for( ; IteLink !=  m_mapObjLinks.end(); ++IteLink )
	{
		if( IteLink->second.m_pLinkObj )
		{
			IteLink->second.m_pLinkObj->Play( szAniName, fSpeed, nLoops, bIsForceUpdate );
		}
	}
}
//-------------------------------------------------------------------------
// 暂停动画播放
void FKRenderObj::Pause()
{
	ObjPartMap::iterator ItePart = m_mapObjParts.begin();
	for( ; ItePart !=  m_mapObjParts.end(); ++ItePart )
	{
		if( ItePart->second && ( ItePart->second->GetRenderObjType() == eROT_Ani 
			|| ItePart->second->GetRenderObjType() == eROT_Armature 
			|| ItePart->second->GetRenderObjType() == eROT_DB ))
		{
			IRenderable* pObj = ItePart->second;
			IAnimation* pAni = pObj->GetAnimation();
			if( pAni )
			{
				pAni->Pause();
			}
		}
	}
	ObjEffectMap::iterator IteEffect = m_mapObjEffects.begin();
	for( ; IteEffect !=  m_mapObjEffects.end(); ++IteEffect )
	{
		if( IteEffect->second && ( IteEffect->second->GetRenderObjType() == eROT_Ani 
			|| IteEffect->second->GetRenderObjType() == eROT_Armature 
			|| IteEffect->second->GetRenderObjType() == eROT_DB ))
		{
			IRenderable* pObj = IteEffect->second;
			IAnimation* pAni = pObj->GetAnimation();
			if( pAni )
			{
				pAni->Pause();
			}
		}
	}
	ObjLinkMap::iterator IteLink = m_mapObjLinks.begin();
	for( ; IteLink !=  m_mapObjLinks.end(); ++IteLink )
	{
		if( IteLink->second.m_pLinkObj )
		{
			IteLink->second.m_pLinkObj->Pause();
		}
	}
}
//-------------------------------------------------------------------------
// 获取动画动作列表
// 参数：[out]AnimationNameList 动画动作列表
void FKRenderObj::GetAnimationNames( list<string>& AnimationNameList )
{
	if( m_pRootRenderable == nullptr )
		return;

	if( m_pRootRenderable->GetRenderObjType() != eROT_Ani 
		&& m_pRootRenderable->GetRenderObjType() != eROT_Armature 
		&& m_pRootRenderable->GetRenderObjType() != eROT_DB )
		return;

	IRenderable* pObj = m_pRootRenderable;
	IAnimation* pAni = pObj->GetAnimation();

	pAni->GetAnimationList( AnimationNameList );
}
//-------------------------------------------------------------------------
// 添加文字渲染对象
// 参数：pRenderText 链接渲染字体
// 参数：szLocator 挂节点名字
// 参数：tagOffset 挂接偏移
bool FKRenderObj::AddLinkRenderText( IRenderText* pRenderText, const char* szLocator,
										  const CCPoint& tagOffset )
{
	if( pRenderText == NULL || m_pRootRenderable == NULL )
		return false;

	CCPoint ptLoc = tagOffset;
	if( (szLocator != "") && (szLocator != NULL) )
	{
		FKRenderObjModel::ObjLocatorMap::iterator Ite = m_pModel->m_mapObjLocators.find( szLocator );
		if( Ite != m_pModel->m_mapObjLocators.end() )
		{
			ptLoc = ptLoc + Ite->second.m_tagOffset;
		}
	}
	pRenderText->SetPos( ptLoc );
	pRenderText->Attach( m_pRootRenderable, 1000 );

	m_setObjRenderTexts.insert( pRenderText );
	return true;
}
//-------------------------------------------------------------------------
// 删除文字渲染对象
// 参数：pRenderText 需要删除的链接渲染字体
void FKRenderObj::RemveLinkRenderText( IRenderText* pRenderText )
{
	IRenderSystem* pSystem = GetRenderSystem();
	if( pSystem == NULL )
		return;
	if( pRenderText == NULL )
		return;

	m_setObjRenderTexts.erase( pRenderText );
	pSystem->RemoveRenderText( pRenderText );
}
//-------------------------------------------------------------------------
// 设置模板数据
void FKRenderObj::SetObjModel( IRenderObjModel* pObjModel )
{
	if( m_pModel )
	{
		m_pModel->release();
	}
	m_pModel = (dynamic_cast<FKRenderObjModel*>(pObjModel));
	if( m_pModel )
	{
		m_pModel->retain();
	}
}
//-------------------------------------------------------------------------
// 获取模板数据
IRenderObjModel* FKRenderObj::GetObjModel()
{
	return (dynamic_cast<IRenderObjModel*>(m_pModel));
}
//-------------------------------------------------------------------------
// 设置是否在场景
void FKRenderObj::SetInScene( bool bIsInScene )
{
	m_bIsInScene = bIsInScene;
}
//-------------------------------------------------------------------------
// 判断是否在场景中
bool FKRenderObj::IsInScene()
{
	return m_bIsInScene;
}
//-------------------------------------------------------------------------
// 是否被选中
bool FKRenderObj::IsSelected()
{
	return m_bIsCanbeSelected;
}
//-------------------------------------------------------------------------
// 创建部件组
void FKRenderObj::__CreateParts()
{
	if( m_pModel == NULL )
		return ;

	FKRenderObjModel::ObjPartMap::iterator Ite = m_pModel->m_mapObjParts.begin();
	for( ; Ite != m_pModel->m_mapObjParts.end(); ++Ite )
	{
		__CreatePart( Ite->second );
	}
}
//-------------------------------------------------------------------------
// 获取部件数据
FKRenderObjModel::ObjPartMap&	FKRenderObj::GetParts()
{
	CCAssert( m_pModel != NULL, "对象模板数据不应当为空" );
	return m_pModel->m_mapObjParts;
}
//-------------------------------------------------------------------------
// 获取绑定点数据
FKRenderObjModel::ObjLocatorMap& FKRenderObj::GetLocators()
{
	CCAssert( m_pModel != NULL, "对象模板数据不应当为空" );
	return m_pModel->m_mapObjLocators;
}
//-------------------------------------------------------------------------
// 获取特效数据
FKRenderObjModel::ObjEffectMap& FKRenderObj::GetEffects()
{
	CCAssert( m_pModel != NULL, "对象模板数据不应当为空" );
	return m_pModel->m_mapObjEffects;
}
//-------------------------------------------------------------------------
// 删除部件
bool FKRenderObj::RemovePart( const string& strName, bool bRemoveData )
{
	if( m_pRootRenderable == nullptr )
		return false;
	if( strName == "Main" )
	{
		if( m_pRootRenderable )
		{
			m_pRootRenderable->GetOffset( m_pModel->m_pOffset );
			m_pRootRenderable = NULL;
		}
	}
	ObjPartMap::iterator ItePart = m_mapObjParts.begin();
	for( ; ItePart !=  m_mapObjParts.end(); ++ItePart )
	{
		if( ItePart->second )
		{
			removeChild( ItePart->second );
		}
	}
	if( m_pModel && bRemoveData )
	{
		m_pModel->RemovePart( strName );
		m_mapObjParts.erase( strName );
	}
	return true;
}
//-------------------------------------------------------------------------
// 删除绑定点
bool FKRenderObj::RemoveLocator( const string& strName, bool bRemoveData )
{
	if( m_pRootRenderable == nullptr )
		return false;
	// 特效列表
	string strEffectName;
	FKRenderObjModel::ObjEffectMap::iterator IteEffect = m_pModel->m_mapObjEffects.begin();
	for( ; IteEffect != m_pModel->m_mapObjEffects.end(); ++IteEffect )
	{
		if( IteEffect->second.m_strLocator == strName )
		{
			strEffectName = IteEffect->second.m_strName;

			ObjEffectMap::iterator IteEffectObj = m_mapObjEffects.find( strEffectName );
			if( IteEffectObj != m_mapObjEffects.end() )
			{
				if( IteEffectObj->second )
				{
					IteEffectObj->second->setPosition( IteEffect->second.m_tagOffset );
				}
			}
		}
	}
	// 挂接对象
	ObjLinkMap::iterator IteLink = m_mapObjLinks.begin();
	ObjLinkMap::iterator IteLink2;
	for( ; IteLink != m_mapObjLinks.end(); )
	{
		IteLink2 = IteLink++;
		if( IteLink2->second.m_strLocator == strName )
		{
			removeChild( IteLink->second.m_pLinkObj );
			m_mapObjLinks.erase( IteLink2 );
		}
	}

	// 自身
	if( bRemoveData && m_pModel )
	{
		m_pModel->RemoveLocator( strName );
	}
	return true;
}
//-------------------------------------------------------------------------
// 删除特效
bool FKRenderObj::RemoveEffect( const string& strName, bool bRemoveData )
{
	if( m_pRootRenderable == nullptr )
		return false;
	ObjEffectMap::iterator IteEffect = m_mapObjEffects.begin();
	for( ; IteEffect !=  m_mapObjEffects.end(); ++IteEffect )
	{
		if( IteEffect->second )
		{
			removeChild( IteEffect->second );
		}
	}
	if( m_pModel && bRemoveData )
	{
		m_pModel->RemoveEffect( strName );
		m_mapObjEffects.erase( strName );
	}
	return true;
}
//-------------------------------------------------------------------------
// 保存到文件中
bool FKRenderObj::SaveToFile( const string& strFileName )
{
	if( m_pModel == NULL )
		return false;

	if( !m_tagOffset.equals(CCPointZero) )
	{
		m_pModel->m_pOffset = GetOffset();
	}

	return m_pModel->SaveToFKOFile( strFileName );
}
//-------------------------------------------------------------------------
// 创建特效组
void FKRenderObj::__CreateEffects()
{
	if( m_pModel == NULL )
		return ;

	FKRenderObjModel::ObjEffectMap::iterator Ite = m_pModel->m_mapObjEffects.begin();
	for( ; Ite != m_pModel->m_mapObjEffects.end(); ++Ite )
	{
		__CreateEffect( Ite->second );
	}
}
//-------------------------------------------------------------------------
// 创建部件
bool FKRenderObj::__CreatePart( FKRenderObjModel::SObjPart& tagPart )
{
	IRenderable* pRenderable = NULL;
	if( m_pModel == NULL )
		return false;

	ObjPartMap::iterator Ite = m_mapObjParts.find( tagPart.m_strName );
	if( Ite != m_mapObjParts.end() )
	{
		removeChild( Ite->second );
		if( tagPart.m_strName == "Main" )
			m_pRootRenderable = NULL;
		Ite->second = NULL;
	}

	string strPathName, strFileName, strExtName;
	FKCW_Base_Utils::ParseFilePathName( tagPart.m_strFile, strPathName, strFileName, strExtName );
	FKCW_Base_Utils::ToLowerCase( strExtName );

	if( strExtName == "xml" )
	{
		// DragonBones骨骼文件
		pRenderable = FKRenderableMgr::GetInstance().CreateDBSprite( tagPart.m_strFile.c_str(), m_pModel->m_pOffset );
	}
	else if( strExtName == "png" || strExtName == "fkp" || strExtName == "jpg" || strExtName == "bmp" )
	{
		// 静态精灵文件
		CCRect tagRect = CCRectZero;
		pRenderable = FKRenderableMgr::GetInstance().CreateSprite( tagPart.m_strFile.c_str(), tagRect, m_pModel->m_pOffset );
	}
	else if( strExtName == "fke" )
	{
		// 特效文件
		pRenderable = FKRenderableMgr::GetInstance().CreateEffect( tagPart.m_strFile.c_str(), m_pModel->m_pOffset );
	}
	else if( strExtName == "fka" )
	{
		// 帧动画文件
		pRenderable = FKRenderableMgr::GetInstance().CreateFrameSprite( tagPart.m_strFile.c_str(), m_pModel->m_pOffset );
	}
	else if( strExtName == "json" || strExtName == "exportjson" )
	{
		// cocosStduio骨骼文件
		pRenderable = FKRenderableMgr::GetInstance().CreateArmaSprite( tagPart.m_strFile.c_str(), m_pModel->m_pOffset );
	}
	else 
	{
		return false;
	}

	if( pRenderable == NULL )
		return false;

	if( tagPart.m_strName == "Main" )
		m_pRootRenderable = pRenderable;

	addChild( pRenderable, tagPart.m_nDepth );
	pRenderable->release();

	m_mapObjParts[tagPart.m_strName] = pRenderable;
	return true;
}
//-------------------------------------------------------------------------
// 创建特效
bool FKRenderObj::__CreateEffect( FKRenderObjModel::SObjEffect& tagEffect )
{
	IRenderable* pRenderable = NULL;
	if( m_pModel == NULL )
		return false;

	string strPathName, strFileName, strExtName;
	FKCW_Base_Utils::ParseFilePathName( tagEffect.m_strFileName, strPathName, strFileName, strExtName );
	FKCW_Base_Utils::ToLowerCase( strExtName );

	if( strExtName == "fke" )
	{
		// 特效类
		pRenderable = FKRenderableMgr::GetInstance().CreateEffect( tagEffect.m_strFileName.c_str() );
	}
	else if( strExtName == "fka" )
	{
		// 帧动画类
		pRenderable = FKRenderableMgr::GetInstance().CreateFrameSprite( tagEffect.m_strFileName.c_str() );
	}
	else if( strExtName == "png" || strExtName == "fkp" || strExtName == "jpg" || strExtName == "bmp" )
	{
		// 静态精灵文件
		CCRect tagRect = CCRectZero;
		pRenderable = FKRenderableMgr::GetInstance().CreateSprite( tagEffect.m_strFileName.c_str(), tagRect );
	}
	else
	{
		return false;
	}

	CCPoint ptLoc = tagEffect.m_tagOffset;
	if(( !tagEffect.m_strLocator.empty() ) && ( tagEffect.m_strLocator != "" ))
	{
		FKRenderObjModel::ObjLocatorMap::iterator Ite = m_pModel->m_mapObjLocators.find( tagEffect.m_strLocator );
		if( Ite != m_pModel->m_mapObjLocators.end() )
		{
			ptLoc = ptLoc + Ite->second.m_tagOffset;
		}
	}

	addChild( pRenderable, tagEffect.m_nDepth );
	pRenderable->release();

	pRenderable->setPosition( ptLoc );
	pRenderable->SetColor( ccc3( tagEffect.m_tagColor.r, tagEffect.m_tagColor.g, tagEffect.m_tagColor.b ) );
	pRenderable->SetOpacity( tagEffect.m_tagColor.a );
	pRenderable->setScaleX( tagEffect.m_tagScale.x );
	pRenderable->setScaleY( tagEffect.m_tagScale.y );

	m_mapObjEffects[ tagEffect.m_strName ] = pRenderable;
	return true;
}
//-------------------------------------------------------------------------