//-------------------------------------------------------------------------
#include "../Include/Cocos2dxDBDisplayBridge.h"
#include "../Include/Cocos2dxDBObj.h"
//-------------------------------------------------------------------------
CCocos2dxDBDisplayBridge::CCocos2dxDBDisplayBridge()
	: m_pDisplayNode( NULL )
	, m_pDisplay( NULL )
{

}
//-------------------------------------------------------------------------
CCocos2dxDBDisplayBridge::~CCocos2dxDBDisplayBridge()
{
	Clear();
}
//-------------------------------------------------------------------------
bool CCocos2dxDBDisplayBridge::GetVisible()
{
	if( m_pDisplayNode == NULL )
		return true;
	if( m_pDisplayNode->GetNode() == NULL )
		return true;
	return m_pDisplayNode->GetNode()->isVisible();
}
//-------------------------------------------------------------------------
void CCocos2dxDBDisplayBridge::SetVisible( bool p_bVisible )
{
	if( m_pDisplayNode == NULL )
		return;
	if( m_pDisplayNode->GetNode() == NULL )
		return;
	m_pDisplayNode->GetNode()->setVisible( p_bVisible );
}
//-------------------------------------------------------------------------
IObject* CCocos2dxDBDisplayBridge::GetDisplay()
{
	return m_pDisplay;
}
//-------------------------------------------------------------------------
void CCocos2dxDBDisplayBridge::SetDisplay( IObject* p_pValue )
{
	CCSprite* pParent = NULL;
	int nZOrder = 0;
	if( m_pDisplayNode )
	{
		pParent = (CCSprite*)m_pDisplayNode->GetNode()->getParent();
		nZOrder = m_pDisplayNode->GetNode()->getZOrder();

		RemoveDisplay();
	}
	m_pDisplay		= p_pValue;
	m_pDisplayNode	= dynamic_cast<CCocos2dxDBObj*>( p_pValue );

	if( m_pDisplayNode && pParent )
	{
		CCocos2dxDBObj tagParentNode( pParent );
		AddDisplay( &tagParentNode, nZOrder );
	}
}
//-------------------------------------------------------------------------
void CCocos2dxDBDisplayBridge::Clear()
{

}
//-------------------------------------------------------------------------
void CCocos2dxDBDisplayBridge::UpdateTransform( CMatrix& p_Matrix, CTransform& p_Transform )
{
	if( m_pDisplayNode == NULL )
		return;

	CCNode* pNode = m_pDisplayNode->GetNode();
	pNode->setPosition( p_Matrix.m_fX, -p_Matrix.m_fY );
	pNode->setScaleX( p_Transform.m_fScaleX );
	pNode->setScaleY( p_Transform.m_fScaleY );
	pNode->setRotationX( CC_RADIANS_TO_DEGREES( p_Transform.m_fSkewX ) );
	pNode->setRotationY( CC_RADIANS_TO_DEGREES( p_Transform.m_fSkewY ) ); 
}
//-------------------------------------------------------------------------
void CCocos2dxDBDisplayBridge::UpdateColor( f32 p_fA, f32 p_fR, f32 p_fG, f32 p_fB, 
		f32 p_fAMultiplier, f32 p_fRMultiplier, f32 p_fGMultiplier, f32 p_fBMultiplier )
{
	if( m_pDisplayNode == NULL )
		return;

	CCNodeRGBA* pNode = dynamic_cast<CCNodeRGBA*>( m_pDisplayNode->GetNode() );
	pNode->setColor( ccc3( static_cast<GLubyte>(p_fRMultiplier * 255), 
		static_cast<GLubyte>( p_fGMultiplier * 255 ), static_cast<GLubyte>( p_fBMultiplier * 255 ) ) );
	pNode->setOpacity( static_cast<GLubyte>(p_fAMultiplier * 255) );
}
//-------------------------------------------------------------------------
void CCocos2dxDBDisplayBridge::UpdateBlendMode( const string& p_szBlendMode )
{
}
//-------------------------------------------------------------------------
void CCocos2dxDBDisplayBridge::AddDisplay( IObject* p_pObject, int p_nIndex )
{
	if( m_pDisplayNode == NULL )
		return;

	CCocos2dxDBObj* pParent = dynamic_cast<CCocos2dxDBObj*>( p_pObject );
	if( pParent )
	{
		if( m_pDisplayNode->GetNode()->getParent() )
		{
			m_pDisplayNode->GetNode()->removeFromParentAndCleanup( false );
		}
		static int s_nZOrder = 0;
		pParent->GetNode()->addChild( m_pDisplayNode->GetNode(), p_nIndex == -1 ? s_nZOrder-- : p_nIndex );
	}
}
//-------------------------------------------------------------------------
void CCocos2dxDBDisplayBridge::RemoveDisplay()
{
	if( m_pDisplayNode && m_pDisplayNode->GetNode()->getParent() )
	{
		m_pDisplayNode->GetNode()->removeFromParent();
	}
}
//-------------------------------------------------------------------------