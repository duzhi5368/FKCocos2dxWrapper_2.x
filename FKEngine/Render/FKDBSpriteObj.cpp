//-------------------------------------------------------------------------
#include "FKDBSpriteObj.h"
//-------------------------------------------------------------------------
FKDBSpriteObj::FKDBSpriteObj( RENDER_OBJ_ID id )
	: m_unID( id )
	, m_pSprite( NULL )
{
	m_strFileName.clear();
	m_strDescName.clear();
}
//-------------------------------------------------------------------------
FKDBSpriteObj::~FKDBSpriteObj()
{

}
//-------------------------------------------------------------------------
// 创建FKSprite对象
bool FKDBSpriteObj::Create( const char* szPicFileName, CCPoint ptOffset )
{
	m_pSprite = new CCocos2dxDBSprite();
	if( m_pSprite == NULL )
	{
		return false;
	}
	if( !m_pSprite->create( szPicFileName, ptOffset ) )
	{
		CC_SAFE_DELETE( m_pSprite );
		return false;
	}
	m_pSprite->setID( m_unID );
	m_pSprite->setOffset( ptOffset );
	this->addChild( m_pSprite );
	return true;
}
//-------------------------------------------------------------------------
// 更新
void FKDBSpriteObj::update( float dt )
{

}
//-------------------------------------------------------------------------
// 离开退出
void FKDBSpriteObj::onExit()
{
	CCNode::onExit();
	CC_SAFE_DELETE( m_pSprite );
}
//-------------------------------------------------------------------------
// 获取ID
RENDER_OBJ_ID FKDBSpriteObj::GetID()
{
	if( m_pSprite )
		return m_pSprite->getID();
	return m_unID;
}
//-------------------------------------------------------------------------
// 设置ID
void FKDBSpriteObj::SetID( RENDER_OBJ_ID unID )
{
	m_unID = unID;
	if( m_pSprite )
		m_pSprite->setID( unID );
}
//-------------------------------------------------------------------------
// 设置颜色
// 参数：tagColor 颜色值
void FKDBSpriteObj::SetColor( const ccColor3B& tagColor )
{
	if( m_pSprite )
		m_pSprite->setColor( tagColor );
}
//-------------------------------------------------------------------------
// 设置透明度
// 参数：byAlpha 透明度[0,255]
void FKDBSpriteObj::SetOpacity( unsigned char byAlpha )
{
	if( m_pSprite )
		m_pSprite->setOpacity( byAlpha );
}
//-------------------------------------------------------------------------
// 获取游戏对象贴图文件名
const std::string& FKDBSpriteObj::GetFileName()
{
	m_strFileName.clear();
	if( m_pSprite )
		m_strFileName = m_pSprite->getFileName();
	return m_strFileName;
}
//-------------------------------------------------------------------------
// 获取游戏对象特征串名(全局唯一字符串)
const std::string& FKDBSpriteObj::GetDescName()
{
	m_strDescName.clear();
	if( m_pSprite )
		m_pSprite->getFeatureString( m_strDescName );
	return m_strDescName;
}
//-------------------------------------------------------------------------
// 碰撞检测
// 参数：pt 检测点
// 返回值：true 表示碰撞，false 表示未碰撞
bool FKDBSpriteObj::HitTest( const CCPoint& pt )
{
	if( m_pSprite )
		return m_pSprite->hitTest( pt );
	return false;
}
//-------------------------------------------------------------------------
// 碰撞检测
// 参数：rt 检测矩形
// 返回值：true 表示碰撞，false 表示未碰撞
bool FKDBSpriteObj::HitTest( const CCRect& rt )
{
	if( m_pSprite )
		return m_pSprite->hitTest( rt );
	return false;
}
//-------------------------------------------------------------------------
// 获取对象包围盒
bool FKDBSpriteObj::GetBoundingBox( CCRect& rt )
{
	if( m_pSprite )
		return m_pSprite->getBoundingBox( rt );
	return false;
}
//-------------------------------------------------------------------------
// 设置偏移量
void FKDBSpriteObj::SetOffset( CCPoint& ptOffset )
{
	if( m_pSprite )
		m_pSprite->setOffset( ptOffset );
}
//-------------------------------------------------------------------------
// 获取偏移量
bool FKDBSpriteObj::GetOffset( CCPoint& ptOffset )
{
	if( m_pSprite )
	{
		m_pSprite->getOffset( ptOffset );
		return true;
	}
	return false;
}
//-------------------------------------------------------------------------
// 获取动画对象
IAnimation* FKDBSpriteObj::GetAnimation()
{
	if( m_pSprite )
		return m_pSprite->getAnimation();
	return NULL;
}
//-------------------------------------------------------------------------