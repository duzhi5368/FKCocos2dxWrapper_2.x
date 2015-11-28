//-------------------------------------------------------------------------
#include "FKSpriteObj.h"
#include "FKSpriteEx.h"
//-------------------------------------------------------------------------
FKSpriteObj::FKSpriteObj( RENDER_OBJ_ID id )
	: m_pSprite( NULL )
	, m_unID( id )
{
	m_strSpriteName.clear();
	m_strPicFileName.clear();
}
//-------------------------------------------------------------------------
FKSpriteObj::~FKSpriteObj()
{

}
//-------------------------------------------------------------------------
// 创建FKSpriteObj对象
bool FKSpriteObj::Create( const char* szPicFileName, const CCRect& rect, CCPoint ptOffset )
{
	if( szPicFileName == NULL )
		return false;

	if( rect.getMinX() == 0 && rect.getMinY() == 0 && 
		rect.getMaxX() == 0 && rect.getMaxY() == 0 )
	{
		m_pSprite = FKSpriteEx::Create( szPicFileName );
	}
	else
	{
		m_pSprite = FKSpriteEx::Create( szPicFileName, rect );
	}

	if( m_pSprite == NULL )
		return false;

	addChild( m_pSprite );
	m_pSprite->setPosition( ptOffset );

	m_strPicFileName = szPicFileName;

	static int nPicFileIndex = 0;
	++nPicFileIndex;
	char szBuf[1024] = {0};
	sprintf( szBuf, "%s_%d_%d_%d_%d_%d", szPicFileName, rect.getMinX(),
		rect.getMinY(), rect.getMaxX(), rect.getMaxY(), nPicFileIndex );
	m_strSpriteName = szBuf;

	return true;
}
//-------------------------------------------------------------------------
// 更新
void FKSpriteObj::update( float dt )
{

}
//-------------------------------------------------------------------------
// 离开退出
void FKSpriteObj::onExit()
{
	if( m_pSprite )
	{
		removeChild( m_pSprite, true );
		m_pSprite = NULL;
	}
}
//-------------------------------------------------------------------------
// 获取ID
RENDER_OBJ_ID FKSpriteObj::GetID()
{
	return m_unID;
}
//-------------------------------------------------------------------------
// 设置ID
void FKSpriteObj::SetID( RENDER_OBJ_ID unID )
{
	m_unID = unID;
}
//-------------------------------------------------------------------------
// 设置颜色
// 参数：tagColor 颜色值
void FKSpriteObj::SetColor( const ccColor3B& tagColor )
{
	if( m_pSprite )
		m_pSprite->setColor( tagColor );
}
//-------------------------------------------------------------------------
// 设置透明度
// 参数：byAlpha 透明度[0,255]
void FKSpriteObj::SetOpacity( unsigned char byAlpha )
{
	if( m_pSprite )
		m_pSprite->setOpacity( byAlpha );
}
//-------------------------------------------------------------------------
// 获取游戏对象贴图文件名
const std::string& FKSpriteObj::GetFileName()
{
	return m_strPicFileName;
}
//-------------------------------------------------------------------------
// 获取游戏对象特征串名(全局唯一字符串)
const std::string& FKSpriteObj::GetDescName()
{
	return m_strSpriteName;
}
//-------------------------------------------------------------------------
// 碰撞检测
// 参数：pt 检测点
// 返回值：true 表示碰撞，false 表示未碰撞
bool FKSpriteObj::HitTest( const CCPoint& pt )
{
	if( m_pSprite == nullptr )
		return false;

	CCRect rect = m_pSprite->boundingBox();
	return rect.containsPoint( pt );
}
//-------------------------------------------------------------------------
// 碰撞检测
// 参数：rt 检测矩形
// 返回值：true 表示碰撞，false 表示未碰撞
bool FKSpriteObj::HitTest( const CCRect& rt )
{
	if( m_pSprite == nullptr )
		return false;

	CCRect rect = m_pSprite->boundingBox();
	return rect.intersectsRect( rt );
}
//-------------------------------------------------------------------------
// 获取对象包围盒
bool FKSpriteObj::GetBoundingBox( CCRect& rt )
{
	if( m_pSprite == NULL )
		return false;

	rt = m_pSprite->boundingBox();
	rt = CCRectApplyAffineTransform( rt, nodeToParentTransform() );
	return true;
}
//-------------------------------------------------------------------------
// 设置偏移量
void FKSpriteObj::SetOffset( CCPoint& ptOffset )
{
	if( m_pSprite )
		m_pSprite->setPosition( ptOffset );
}
//-------------------------------------------------------------------------
// 获取偏移量
bool FKSpriteObj::GetOffset( CCPoint& ptOffset )
{
	if( m_pSprite )
	{
		ptOffset = m_pSprite->getPosition();
		return true;
	}
	return false;
}
//-------------------------------------------------------------------------
// 获取精灵
CCSprite* FKSpriteObj::GetSprite()
{
	return ( dynamic_cast<CCSprite*>(m_pSprite) );
}
//-------------------------------------------------------------------------