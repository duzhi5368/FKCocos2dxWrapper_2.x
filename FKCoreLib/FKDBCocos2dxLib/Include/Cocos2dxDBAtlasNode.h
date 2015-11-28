//*************************************************************************
//	创建日期:	2014-8-19   20:01
//	文件名称:	Cocos2dxDBAtlasNode.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "CommonHead.h"
//-------------------------------------------------------------------------
USING_NS_CC;
//-------------------------------------------------------------------------
#if USE_BATCH_NODE
class CCocos2dxDBAtlasNode : public CCSprite, public IObject
#else
class CCocos2dxDBAtlasNode : public CCNodeRGBA, public CCTextureProtocol, public IObject
#endif
{
	friend class CCDirector;
public:
	ccColor3B			m_tagColorUnmodified;
	bool				m_bIsOpacityModifyRGB;
	GLint				m_nUniformColor;
	bool				m_bIsIgnoreContentScaleFactor;
	unsigned int		m_unQuadIndex;
	CC_PROPERTY( CCTextureAtlas*, m_pTextureAtlas, TextureAtlas );
	CC_PROPERTY( ccBlendFunc, m_tagBlendFunc, BlendFunc );
public:
	CCocos2dxDBAtlasNode();
	virtual ~CCocos2dxDBAtlasNode();
public:
	static CCocos2dxDBAtlasNode* Create( CCTextureAtlas* p_pTextureAtlas, unsigned int p_unQuadIndex,
		const CCRect& p_tagSourceRect );
public:
	bool InitWithTextureAtlas( CCTextureAtlas* p_pTextureAtlas, unsigned int p_unQuadIndex, 
		const CCRect& p_tagSourceRect );
	virtual void				draw();
	virtual CCTexture2D*		getTexture();
	virtual void				setTexture(CCTexture2D* p_pTexture);
	virtual bool				isOpacityModifyRGB();
	virtual void				setOpacityModifyRGB( bool p_bIsOpacityModifyRGB );
	virtual const ccColor3B&	getColor();
	virtual void				setColor( const ccColor3B& p_Color );
	virtual void				setOpacity( GLubyte p_ucOpacity );
private:
	void						__UpdateBlendFunc();
	void						__UpdateOpacityModifyRGB();
	void						__SetIgnoreContentScaleFactor( bool p_bIgnoreContentScaleFactor );
};
//-------------------------------------------------------------------------