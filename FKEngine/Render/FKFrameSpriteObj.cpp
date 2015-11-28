//-------------------------------------------------------------------------
#include "FKFrameSpriteObj.h"
#include "FKSpriteEx.h"
#include "FKAnimateExResMgr.h"
//-------------------------------------------------------------------------
FKFrameSpriteObj::FKFrameSpriteObj( RENDER_OBJ_ID id )
	: m_unID( id )
	, m_pSprite( NULL )
{
	m_strPicFileName.clear();
}
//-------------------------------------------------------------------------
FKFrameSpriteObj::~FKFrameSpriteObj()
{

}
//-------------------------------------------------------------------------
// 创建FKSprite对象
bool FKFrameSpriteObj::Create( const char* szPicFileName, CCPoint ptOffset )
{
	if( szPicFileName == NULL )
		return false;

	m_pSprite = FKSpriteEx::Create();
	if( m_pSprite == NULL )
		return false;

	FKAnimateExRes* pAniRes = FKAnimateExResMgr::GetInstance().GetAnimateExRes( szPicFileName );
	m_Animate.SetAnimateExRes( pAniRes );
	m_Animate.SetTargetNode( m_pSprite );

	addChild( m_pSprite );
	m_pSprite->setPosition( ptOffset );

	m_strPicFileName = szPicFileName;
	return true;
}
//-------------------------------------------------------------------------
// 更新
void FKFrameSpriteObj::update( float dt )
{
	m_Animate.Update( dt );
}
//-------------------------------------------------------------------------
// 离开退出
void FKFrameSpriteObj::onExit()
{
	if( m_pSprite )
	{
		removeChild( m_pSprite );
		m_pSprite = NULL;
	}
}
//-------------------------------------------------------------------------
// 获取ID
RENDER_OBJ_ID FKFrameSpriteObj::GetID()
{
	return m_unID;
}
//-------------------------------------------------------------------------
// 设置ID
void FKFrameSpriteObj::SetID( RENDER_OBJ_ID unID )
{
	m_unID = unID;
}
//-------------------------------------------------------------------------
// 设置颜色
// 参数：tagColor 颜色值
void FKFrameSpriteObj::SetColor( const ccColor3B& tagColor )
{
	if( m_pSprite )
		m_pSprite->setColor( tagColor );
}
//-------------------------------------------------------------------------
// 设置透明度
// 参数：byAlpha 透明度[0,255]
void FKFrameSpriteObj::SetOpacity( unsigned char byAlpha )
{
	if( m_pSprite )
		m_pSprite->setOpacity( byAlpha );
}
//-------------------------------------------------------------------------
// 获取游戏对象贴图文件名
const std::string& FKFrameSpriteObj::GetFileName()
{
	return m_strPicFileName;
}
//-------------------------------------------------------------------------
// 获取游戏对象特征串名(全局唯一字符串)
const std::string& FKFrameSpriteObj::GetDescName()
{
	return m_strPicFileName;
}
//-------------------------------------------------------------------------
// 碰撞检测
// 参数：pt 检测点
// 返回值：true 表示碰撞，false 表示未碰撞
bool FKFrameSpriteObj::HitTest( const CCPoint& pt )
{
	if( m_pSprite == NULL )
		return false;

	CCRect rect = m_pSprite->boundingBox();
	return rect.containsPoint( pt );
}
//-------------------------------------------------------------------------
// 碰撞检测
// 参数：rt 检测矩形
// 返回值：true 表示碰撞，false 表示未碰撞
bool FKFrameSpriteObj::HitTest( const CCRect& rt )
{
	if( m_pSprite == NULL )
		return false;

	CCRect rect = m_pSprite->boundingBox();
	return rect.intersectsRect( rect );
}
//-------------------------------------------------------------------------
// 获取对象包围盒
bool FKFrameSpriteObj::GetBoundingBox( CCRect& rt )
{
	if( m_pSprite )
	{
		rt = m_pSprite->boundingBox();
		rt = CCRectApplyAffineTransform( rt, nodeToParentTransform() );
		return true;
	}
	return false;
}
//-------------------------------------------------------------------------
// 设置偏移量
void FKFrameSpriteObj::SetOffset( CCPoint& ptOffset )
{
	if( m_pSprite )
		m_pSprite->setPosition( ptOffset );
}
//-------------------------------------------------------------------------
// 获取偏移量
bool FKFrameSpriteObj::GetOffset( CCPoint& ptOffset )
{
	if( m_pSprite )
	{
		ptOffset = m_pSprite->getPosition();
		return true;
	}
	return false;
}
//-------------------------------------------------------------------------
// 获取动画对象
IAnimation* FKFrameSpriteObj::GetAnimation()
{
	return (IAnimation*)&m_Animate;
}
//-------------------------------------------------------------------------
// 获取精灵
CCSprite* FKFrameSpriteObj::GetSprite()
{
	return (CCSprite*)m_pSprite;
}
//-------------------------------------------------------------------------