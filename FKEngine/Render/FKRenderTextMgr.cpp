//-------------------------------------------------------------------------
#include "FKRenderTextMgr.h"
//-------------------------------------------------------------------------
FKRenderTextMgr::FKRenderTextMgr()
{

}
//-------------------------------------------------------------------------
FKRenderTextMgr::~FKRenderTextMgr()
{

}
//-------------------------------------------------------------------------
// 创建渲染文字
// 参数：szString 文字
// 参数：szFontName 字体名称
// 参数：nFontSize 字体大小
// 参数：tagDimentions 字体尺寸
// 参数：bIsBmpFont 是否是BMP文字
IRenderText* FKRenderTextMgr::CreateRenderText( const char* szString, const char* szFontName, 
								 int nFontSize,  CCSize tagDimentions, bool bIsBMPFont )
{
	static RENDER_OBJ_ID s_RenderTextID = 0;
	if( ++s_RenderTextID == 0 )
		s_RenderTextID++;

	FKRenderText* pRenderText = new FKRenderText( s_RenderTextID );
	if( pRenderText == NULL )
		return NULL;

	if( bIsBMPFont )
		pRenderText->CreateByBMP( szString, szFontName, nFontSize, tagDimentions );
	if( !bIsBMPFont )
		pRenderText->Create( szString, szFontName, nFontSize, tagDimentions );

	m_mapRenderText[s_RenderTextID] = ( IRenderText* )pRenderText;

	return pRenderText;
}
//-------------------------------------------------------------------------
// 删除渲染文字
// 参数：pRenderText 需要删除的渲染文字
bool FKRenderTextMgr::RemoveRenderText( IRenderText* pRenderText )
{
	if( pRenderText == NULL )
		return false;
	RENDER_OBJ_ID id = ((FKRenderText*)pRenderText)->GetID();
	RenderTextMap::iterator Ite = m_mapRenderText.find( id );
	if( Ite == m_mapRenderText.end() )
		return false;
	m_mapRenderText.erase( id );

	pRenderText->Release();
	return true;
}
//-------------------------------------------------------------------------
// 删除渲染文字
// 参数：id 唯一ID
bool FKRenderTextMgr::RemoveRenderTextByID( RENDER_OBJ_ID id )
{
	if( id == 0 )
		return false;
	RenderTextMap::iterator Ite = m_mapRenderText.find( id );
	if( Ite == m_mapRenderText.end() )
		return false;
	IRenderText* pRenderText = Ite->second;
	m_mapRenderText.erase( id );

	pRenderText->Release();
	return true;
}
//-------------------------------------------------------------------------