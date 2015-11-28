//--------------------------------------------------------------------
#include "../Include/FKCW_Shader_ShaderManager.h"

#include "../Shader/Blur_Vert.h"
#include "../Shader/Blur_Frag.h"
#include "../Shader/Flash_Vert.h"
#include "../Shader/Flash_Frag.h"
#include "../Shader/Laser_Vert.h"
#include "../Shader/Laser_Frag.h"
#include "../Shader/Lighting_Vert.h"
#include "../Shader/Lighting_Frag.h"
#include "../Shader/Matrix_Vert.h"
#include "../Shader/Matrix_Frag.h"
#include "../Shader/Shine_Vert.h"
#include "../Shader/Shine_Frag.h"
#include "../Shader/Bloom_Vert.h"
#include "../Shader/Bloom_Frag.h"
#include "../Shader/CelShading_Vert.h"
#include "../Shader/CelShading_Frag.h"
#include "../Shader/EdgeDetection_Vert.h"
#include "../Shader/EdgeDetection_Frag.h"

#include "../Shader/Color_Vert.h"

#include "../Shader/OldPhoto_Frag.h"
#include "../Shader/Negative_Frag.h"
#include "../Shader/Brightness_Frag.h"
#include "../Shader/Constrast_Frag.h"
#include "../Shader/Saturation_Frag.h"
#include "../Shader/BlackWhite_Frag.h"
#include "../Shader/SimpleEdgeDetection_Frag.h"
#include "../Shader/Emboss_Frag.h"
#include "../Shader/SmallBlur_Frag.h"
#include "../Shader/BigBlur_Frag.h"
#include "../Shader/SmallSharpen_Frag.h"
#include "../Shader/BigSharpen_Frag.h"
#include "../Shader/SimpleSharpen_Frag.h"
#include "../Shader/Dilate_Frag.h"
#include "../Shader/Erode_Frag.h"
#include "../Shader/Laplacian_Frag.h"
#include "../Shader/Sobel_Frag.h"
#include "../Shader/Prewitt_Frag.h"
#include "../Shader/MotionBlur_Frag.h"
#include "../Shader/BigEmboss_Frag.h"
#include "../Shader/ColorRump_Frag.h"
//--------------------------------------------------------------------
// 根据Key加载一个已定制shader
void FKCW_Shader_ShaderManager::LoadCustomShader( const string& p_strKey )
{
	// 先进行查找，若没有则加载
	if(!CCShaderCache::sharedShaderCache()->programForKey(p_strKey.c_str())) 
	{
		// 加载各shader
		CCGLProgram* p = new CCGLProgram();
		p->autorelease();

		if( p_strKey == KeyShaderFlash )
		{
			p->initWithVertexShaderByteArray( ShaderFlashVert, ShaderFlashFrag );
		}
		else if( p_strKey == KeyShaderBlur )
		{
			p->initWithVertexShaderByteArray( ShaderBlurVert, ShaderBlurFrag );
		}
		else if( p_strKey == KeyShaderLaser )
		{
			p->initWithVertexShaderByteArray( ShaderLaserVert, ShaderLaserFrag );
		}
		else if( p_strKey == KeyShaderLighting )
		{
			p->initWithVertexShaderByteArray( ShaderLightingVert, ShaderLightingFrag );
		}
		else if( p_strKey == KeyShaderMatrix )
		{
			p->initWithVertexShaderByteArray( ShaderMatrixVert, ShaderMatrixFrag );
		}
		else if( p_strKey == KeyShaderShine )
		{
			p->initWithVertexShaderByteArray( ShaderShineVert, ShaderShineFrag );
		}
		else if( p_strKey == KeyShaderBloom )
		{
			p->initWithVertexShaderByteArray( ShaderBloomVert, ShaderBloomFrag );
		}
		else if( p_strKey == KeyShaderCelShading )
		{
			p->initWithVertexShaderByteArray( ShaderCelShadingVert, ShaderCelShadingFrag );
		}
		else if( p_strKey == KeyShaderEdgeDetection )
		{
			p->initWithVertexShaderByteArray( ShaderEdgeDetectionVert, ShaderEdgeDetectionFrag );
		}
		else if( p_strKey == KeyShaderOldPhoto )
		{
			p->initWithVertexShaderByteArray( ShaderColorSpriteVert, ShaderOldPhotoFrag );
		}
		else if( p_strKey == KeyShaderNegative )
		{
			p->initWithVertexShaderByteArray( ShaderColorSpriteVert, ShaderNegativeFrag );
		}
		else if( p_strKey == KeyShaderBrightness )
		{
			p->initWithVertexShaderByteArray( ShaderColorSpriteVert, ShaderBrightnessFrag );
		}
		else if( p_strKey == KeyShaderContrast )
		{
			p->initWithVertexShaderByteArray( ShaderColorSpriteVert, ShaderContrastFrag );
		}
		else if( p_strKey == KeyShaderSaturation )
		{
			p->initWithVertexShaderByteArray( ShaderColorSpriteVert, ShaderSaturationFrag );
		}
		else if( p_strKey == KeyShaderBlackWhite )
		{
			p->initWithVertexShaderByteArray( ShaderColorSpriteVert, ShaderBlackWhiteFrag );
		}
		else if( p_strKey == KeyShaderSimpleEdgeDetection )
		{
			p->initWithVertexShaderByteArray( ShaderColorSpriteVert, ShaderSimpleEdgeDetectionFrag );
		}
		else if( p_strKey == KeyShaderEmboss )
		{
			p->initWithVertexShaderByteArray( ShaderColorSpriteVert, ShaderEmbossFrag );
		}
		else if( p_strKey == KeyShaderSmallBlur )
		{
			p->initWithVertexShaderByteArray( ShaderColorSpriteVert, ShaderSmallBlurFrag );
		}
		else if( p_strKey == KeyShaderBigBlur )
		{
			p->initWithVertexShaderByteArray( ShaderColorSpriteVert, ShaderBigBlurFrag );
		}
		else if( p_strKey == KeyShaderSmallSharpen )
		{
			p->initWithVertexShaderByteArray( ShaderColorSpriteVert, ShaderSmallSharpenFrag );
		}
		else if( p_strKey == KeyShaderBigSharpen )
		{
			p->initWithVertexShaderByteArray( ShaderColorSpriteVert, ShaderBigSharpenFrag );
		}
		else if( p_strKey == KeyShaderSimpleSharpen )
		{
			p->initWithVertexShaderByteArray( ShaderColorSpriteVert, ShaderSimpleSharpenFrag );
		}
		else if( p_strKey == KeyShaderDilate )
		{
			p->initWithVertexShaderByteArray( ShaderColorSpriteVert, ShaderDilateFrag );
		}
		else if( p_strKey == KeyShaderErode )
		{
			p->initWithVertexShaderByteArray( ShaderColorSpriteVert, ShaderErodeFrag );
		}
		else if( p_strKey == KeyShaderLaplacian )
		{
			p->initWithVertexShaderByteArray( ShaderColorSpriteVert, ShaderLaplacianFrag );
		}
		else if( p_strKey == KeyShaderSobel )
		{
			p->initWithVertexShaderByteArray( ShaderColorSpriteVert, ShaderSobelFrag );
		}
		else if( p_strKey == KeyShaderPrewitt )
		{
			p->initWithVertexShaderByteArray( ShaderColorSpriteVert, ShaderPrewittFrag );
		}
		else if( p_strKey == KeyShaderMotionBlur )
		{
			p->initWithVertexShaderByteArray( ShaderColorSpriteVert, ShaderMotionBlurFrag );
		}
		else if( p_strKey == KeyShaderBigEmboss )
		{
			p->initWithVertexShaderByteArray( ShaderColorSpriteVert, ShaderBigEmbossFrag );
		}
		else if( p_strKey == KeyShaderColorRamp )
		{
			p->initWithVertexShaderByteArray( ShaderColorSpriteVert, ShaderColorRampFrag );
		}

		// 添加属性
		p->addAttribute( kCCAttributeNamePosition, kCCVertexAttrib_Position );
		p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
		p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);

		// 链接shader
		p->link();

		// 添加参数
		if( p_strKey == KeyShaderFlash )
		{
			sUniform_pos_CC_flashColor		= p->getUniformLocationForName( kCCUniform_flashColor );
			sUniform_pos_CC_flashTime		= p->getUniformLocationForName( kCCUniform_flashTime );
		}
		else if( p_strKey == KeyShaderBlur )
		{
			sUniform_pos_CC_blurSize		= p->getUniformLocationForName( kCCUniform_blurSize );
			sUniform_pos_CC_blurSubtract	= p->getUniformLocationForName( kCCUniform_blurSubtract );
		}
		else if( p_strKey == KeyShaderLighting )
		{
			sUniform_pos_CC_lightingMul		= p->getUniformLocationForName( kCCUniform_lightingMul );
			sUniform_pos_CC_lightingAdd		= p->getUniformLocationForName( kCCUniform_lightingAdd );
		}
		else if( p_strKey == KeyShaderMatrix )
		{
			sUniform_pos_CC_colorMatrix		= p->getUniformLocationForName( kCCUniform_colorMatrix );
		}
		else if( p_strKey == KeyShaderShine )
		{
			sUniform_pos_CC_shineWidth		= p->getUniformLocationForName( kCCUniform_shineWidth );
			sUniform_pos_CC_shineXY1		= p->getUniformLocationForName( kCCUniform_shineXY1 );
			sUniform_pos_CC_shineXY2		= p->getUniformLocationForName( kCCUniform_shineXY2 );
			sUniform_pos_CC_shineColor1		= p->getUniformLocationForName( kCCUniform_shineColor1 );
			sUniform_pos_CC_shineColor2		= p->getUniformLocationForName( kCCUniform_shineColor2 );
			sUniform_pos_CC_shineColor3		= p->getUniformLocationForName( kCCUniform_shineColor3 );
			sUniform_pos_CC_shinePositions	= p->getUniformLocationForName( kCCUniform_shinePositions );
			sUniform_pos_CC_shineTime		= p->getUniformLocationForName( kCCUniform_shineTime );
		}
		else if( p_strKey == KeyShaderCelShading )
		{
			sUniform_pos_CC_celShadingSize	= p->getUniformLocationForName( kCCUniform_celShadingSize );
		}
		else if( p_strKey == KeyShaderEdgeDetection )
		{
			sUniform_pos_CC_edgeDetectionSize = p->getUniformLocationForName( kCCUniform_EdgeDetectionSize );
		}
		else if( p_strKey == KeyShaderBigEmboss )
		{
			sUniform_pos_CC_bigEmbossTime = p->getUniformLocationForName( kCCUniform_bigEmbossTime );
		}

		// 添加默认标准参数
		p->updateUniforms();

		// 添加GLProgram
		CCShaderCache::sharedShaderCache()->addProgram(p, p_strKey.c_str());
	}
}
//--------------------------------------------------------------------
// 获取指定Key的CCGLProgram
CCGLProgram* FKCW_Shader_ShaderManager::ProgramForKey( const string& p_strKey )
{
	LoadCustomShader( p_strKey );
	return CCShaderCache::sharedShaderCache()->programForKey(p_strKey.c_str());
}
//--------------------------------------------------------------------
void FKCW_Shader_ShaderManager::genTexCoordOffsets(GLuint width, GLuint height, GLfloat* pOutTexCoordOff, GLfloat step )
{
	// Note: You can multiply the step to displace the samples further. Do this with diff values horiz and vert and you have directional blur of a sort...
	float xInc = step / (GLfloat)(width);
	float yInc = step / (GLfloat)(height);

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			pOutTexCoordOff[(((i*5)+j)*2)+0] = (-2.0f * xInc) + ((GLfloat)i * xInc);
			pOutTexCoordOff[(((i*5)+j)*2)+1] = (-2.0f * yInc) + ((GLfloat)j * yInc);
		}
	}
}
//--------------------------------------------------------
// 设置闪光效果
void FKCW_Shader_ShaderManager::SetFlash( float r, float g, float b, float t )
{
	LoadCustomShader( KeyShaderFlash );
	CCGLProgram* p = CCShaderCache::sharedShaderCache()->programForKey( KeyShaderFlash );
	p->use();
	p->setUniformLocationWith3f( sUniform_pos_CC_flashColor, r, g, b );
	p->setUniformLocationWith1f( sUniform_pos_CC_flashTime, t );
}
//--------------------------------------------------------------------
// 设置模糊效果
void FKCW_Shader_ShaderManager::SetBlur( CCSize p_tagNodeSize, CCSize p_tagBlurSize, ccColor4F p_tagBlurSubtract )
{
	LoadCustomShader( KeyShaderBlur );
	CCGLProgram* p = CCShaderCache::sharedShaderCache()->programForKey( KeyShaderBlur );
	p->use();
	p->setUniformLocationWith2f( sUniform_pos_CC_blurSize, p_tagBlurSize.width / p_tagNodeSize.width,
		p_tagBlurSize.height / p_tagNodeSize.height );
	p->setUniformLocationWith4f( sUniform_pos_CC_blurSubtract, p_tagBlurSubtract.r, 
		p_tagBlurSubtract.g, p_tagBlurSubtract.b, p_tagBlurSubtract.a );
}
//--------------------------------------------------------------------
// 设置光照效果
void FKCW_Shader_ShaderManager::SetLighting( ccColor4B p_Mul, ccColor3B p_Add )
{
	LoadCustomShader( KeyShaderLighting );
	CCGLProgram* p = CCShaderCache::sharedShaderCache()->programForKey( KeyShaderLighting );
	p->use();
	p->setUniformLocationWith4f( sUniform_pos_CC_lightingMul, p_Mul.r / 255.0f, 
		p_Mul.g / 255.0f, p_Mul.b / 255.0f, p_Mul.a / 255.0f );
	p->setUniformLocationWith3f( sUniform_pos_CC_lightingAdd, p_Add.r / 255.0f,
		p_Add.g / 255.0f, p_Add.b / 255.0f );
}
//--------------------------------------------------------------------
// 设置颜色调整矩阵
void FKCW_Shader_ShaderManager::SetColorMatrix( const kmMat4& p_Mat4 )
{
	LoadCustomShader( KeyShaderMatrix );
	CCGLProgram* p = CCShaderCache::sharedShaderCache()->programForKey( KeyShaderMatrix );
	p->use();
	p->setUniformLocationWith4fv( sUniform_pos_CC_colorMatrix, (GLfloat*)p_Mat4.mat, 1 );
}
//--------------------------------------------------------------------
// 设置灰色图
void FKCW_Shader_ShaderManager::SetGray()
{
	kmMat4		m;
	float v[] = {
		0.299f, 0.299f, 0.299f, 0,
		0.587f, 0.587f, 0.587f, 0,
		0.114f, 0.114f, 0.114f, 0,
		0, 0, 0, 1
	};
	kmMat4Fill(&m, v);
	SetColorMatrix(m);
}
//--------------------------------------------------------------------
// 设置光线效果
// 参数：	p_fWidth 节点宽度
//			p_tagLB 模板左下位置
//			p_tagRT 模板右上位置
//			p_tagColor 变化色1,2,3
//			p_tagGradientPositions 光线变化位置，取值为[0,1]之间。通过这个将光线分段进行色彩变化
//			p_fTime 时间[0,1]
void FKCW_Shader_ShaderManager::SetShine( float p_fWidth, CCPoint p_tagLB, CCPoint p_tagRT,
						 ccColor4B p_tagColor1, ccColor4B p_tagColor2, ccColor4B p_tagColor3,
						 ccVertex3F p_tagGradientPositions, float p_fTime )
{
	LoadCustomShader( KeyShaderShine );
	CCGLProgram* p = CCShaderCache::sharedShaderCache()->programForKey( KeyShaderShine );
	p->use();
	p->setUniformLocationWith1f(sUniform_pos_CC_shineWidth, p_fWidth);
	p->setUniformLocationWith1f(sUniform_pos_CC_shineTime, p_fTime);
	p->setUniformLocationWith2f(sUniform_pos_CC_shineXY1, p_tagLB.x, p_tagLB.y);
	p->setUniformLocationWith2f(sUniform_pos_CC_shineXY2, p_tagRT.x, p_tagRT.y);
	p->setUniformLocationWith4f(sUniform_pos_CC_shineColor1, p_tagColor1.r / 255.0f,
		p_tagColor1.g / 255.0f, p_tagColor1.b / 255.0f, p_tagColor1.a / 255.0f);
	p->setUniformLocationWith4f(sUniform_pos_CC_shineColor2, p_tagColor2.r / 255.0f,
		p_tagColor2.g / 255.0f, p_tagColor2.b / 255.0f, p_tagColor2.a / 255.0f);
	p->setUniformLocationWith4f(sUniform_pos_CC_shineColor3, p_tagColor3.r / 255.0f,
		p_tagColor3.g / 255.0f, p_tagColor3.b / 255.0f, p_tagColor3.a / 255.0f);
	p->setUniformLocationWith3f(sUniform_pos_CC_shinePositions, p_tagGradientPositions.x,
		p_tagGradientPositions.y, p_tagGradientPositions.z);
}
//--------------------------------------------------------------------
// 设置bloom效果
void FKCW_Shader_ShaderManager::SetBloom()
{
	LoadCustomShader( KeyShaderBloom );
	CCGLProgram* p = CCShaderCache::sharedShaderCache()->programForKey( KeyShaderBloom );
	p->use();
}
//--------------------------------------------------------------------
// 设置卡通效果
void FKCW_Shader_ShaderManager::SetCelShading( CCSize p_tagNodeSize )
{
	LoadCustomShader( KeyShaderCelShading );
	CCGLProgram* p = CCShaderCache::sharedShaderCache()->programForKey( KeyShaderCelShading );
	p->use();
	p->setUniformLocationWith2f( sUniform_pos_CC_celShadingSize, p_tagNodeSize.width,
		p_tagNodeSize.height );
}
//--------------------------------------------------------------------
// 设置边缘检测
void FKCW_Shader_ShaderManager::SetEdgeDetectionSize( CCSize p_tagNodeSize )
{
	LoadCustomShader( KeyShaderEdgeDetection );
	CCGLProgram* p = CCShaderCache::sharedShaderCache()->programForKey( KeyShaderEdgeDetection );
	p->use();
	p->setUniformLocationWith2f( sUniform_pos_CC_edgeDetectionSize, p_tagNodeSize.width,
		p_tagNodeSize.height );
}
//--------------------------------------------------------------------
// 设置老照片效果
void FKCW_Shader_ShaderManager::SetOldPhoto()
{
	LoadCustomShader( KeyShaderOldPhoto );
	CCGLProgram* p = CCShaderCache::sharedShaderCache()->programForKey( KeyShaderOldPhoto );
	p->use();
}
//--------------------------------------------------------------------
// 设置负色效果
void FKCW_Shader_ShaderManager::SetNegative()
{
	LoadCustomShader( KeyShaderNegative );
	CCGLProgram* p = CCShaderCache::sharedShaderCache()->programForKey( KeyShaderNegative );
	p->use();
}
//--------------------------------------------------------------------
// 设置高光
void FKCW_Shader_ShaderManager::SetBrightness()
{
	LoadCustomShader( KeyShaderBrightness );
	CCGLProgram* p = CCShaderCache::sharedShaderCache()->programForKey( KeyShaderBrightness );
	p->use();
}
//--------------------------------------------------------------------
// 设置对比效果
void FKCW_Shader_ShaderManager::SetContrast()
{
	LoadCustomShader( KeyShaderContrast );
	CCGLProgram* p = CCShaderCache::sharedShaderCache()->programForKey( KeyShaderContrast );
	p->use();
}
//--------------------------------------------------------------------
// 设置饱和
void FKCW_Shader_ShaderManager::SetSaturation()
{
	LoadCustomShader( KeyShaderSaturation );
	CCGLProgram* p = CCShaderCache::sharedShaderCache()->programForKey( KeyShaderSaturation );
	p->use();
}
//--------------------------------------------------------------------
// 设置黑白化
void FKCW_Shader_ShaderManager::SetBlackWhite()
{
	LoadCustomShader( KeyShaderBlackWhite );
	CCGLProgram* p = CCShaderCache::sharedShaderCache()->programForKey( KeyShaderBlackWhite );
	p->use();
}
//--------------------------------------------------------------------
// 设置简易边缘效果
void FKCW_Shader_ShaderManager::SetSimpleEdgeDetection()
{
	LoadCustomShader( KeyShaderSimpleEdgeDetection );
	CCGLProgram* p = CCShaderCache::sharedShaderCache()->programForKey( KeyShaderSimpleEdgeDetection );
	p->use();
}
//--------------------------------------------------------------------
// 设置浮雕效果
void FKCW_Shader_ShaderManager::SetEmboss()
{
	LoadCustomShader( KeyShaderEmboss );
	CCGLProgram* p = CCShaderCache::sharedShaderCache()->programForKey( KeyShaderEmboss );
	p->use();
}
//--------------------------------------------------------------------
// 轻度模糊效果
void FKCW_Shader_ShaderManager::SetSmallBlur( CCSize p_tagNodeSize )
{
	LoadCustomShader( KeyShaderSmallBlur );
	CCGLProgram* p = CCShaderCache::sharedShaderCache()->programForKey( KeyShaderSmallBlur );
	// 计算纹理偏移
	GLfloat	fTexCoordOffsets[5 * 5 * 2];
	GLuint w = static_cast<GLuint>(p_tagNodeSize.width);
	GLuint h = static_cast<GLuint>(p_tagNodeSize.height);
	genTexCoordOffsets( w, h, fTexCoordOffsets );
	int nLoc = glGetUniformLocation( p->getProgram(), "u_TextureCoordOffset" );
	glUniform2fv(nLoc, 5 * 5, fTexCoordOffsets);

	p->use();
}
//--------------------------------------------------------------------
// 重度模糊效果
void FKCW_Shader_ShaderManager::SetBigBlur( CCSize p_tagNodeSize )
{
	LoadCustomShader( KeyShaderBigBlur );
	CCGLProgram* p = CCShaderCache::sharedShaderCache()->programForKey( KeyShaderBigBlur );
	// 计算纹理偏移
	GLfloat	fTexCoordOffsets[5 * 5 * 2];
	GLuint w = static_cast<GLuint>(p_tagNodeSize.width);
	GLuint h = static_cast<GLuint>(p_tagNodeSize.height);
	genTexCoordOffsets( w, h, fTexCoordOffsets );
	int nLoc = glGetUniformLocation( p->getProgram(), "u_TextureCoordOffset" );
	glUniform2fv(nLoc, 5 * 5, fTexCoordOffsets);
	p->use();
}
//--------------------------------------------------------------------
// 轻度磨砂
void FKCW_Shader_ShaderManager::SetSmallSharpen( CCSize p_tagNodeSize )
{
	LoadCustomShader( KeyShaderSmallSharpen );
	CCGLProgram* p = CCShaderCache::sharedShaderCache()->programForKey( KeyShaderSmallSharpen );
	// 计算纹理偏移
	GLfloat	fTexCoordOffsets[5 * 5 * 2];
	GLuint w = static_cast<GLuint>(p_tagNodeSize.width);
	GLuint h = static_cast<GLuint>(p_tagNodeSize.height);
	genTexCoordOffsets( w, h, fTexCoordOffsets );
	int nLoc = glGetUniformLocation( p->getProgram(), "u_TextureCoordOffset" );
	glUniform2fv(nLoc, 5 * 5, fTexCoordOffsets);
	p->use();
}
//--------------------------------------------------------------------
// 重度磨砂
void FKCW_Shader_ShaderManager::SetBigSharpen( CCSize p_tagNodeSize )
{
	LoadCustomShader( KeyShaderBigSharpen );
	CCGLProgram* p = CCShaderCache::sharedShaderCache()->programForKey( KeyShaderBigSharpen );
	// 计算纹理偏移
	GLfloat	fTexCoordOffsets[5 * 5 * 2];
	GLuint w = static_cast<GLuint>(p_tagNodeSize.width);
	GLuint h = static_cast<GLuint>(p_tagNodeSize.height);
	genTexCoordOffsets( w, h, fTexCoordOffsets );
	int nLoc = glGetUniformLocation( p->getProgram(), "u_TextureCoordOffset" );
	glUniform2fv(nLoc, 5 * 5, fTexCoordOffsets);
	p->use();
}
//--------------------------------------------------------------------
// 简易磨砂
void FKCW_Shader_ShaderManager::SetSimpleSharpen( CCSize p_tagNodeSize )
{
	LoadCustomShader( KeyShaderSimpleSharpen );
	CCGLProgram* p = CCShaderCache::sharedShaderCache()->programForKey( KeyShaderSimpleSharpen );
	// 计算纹理偏移
	GLfloat	fTexCoordOffsets[5 * 5 * 2];
	GLuint w = static_cast<GLuint>(p_tagNodeSize.width);
	GLuint h = static_cast<GLuint>(p_tagNodeSize.height);
	genTexCoordOffsets( w, h, fTexCoordOffsets );
	int nLoc = glGetUniformLocation( p->getProgram(), "u_TextureCoordOffset" );
	glUniform2fv(nLoc, 5 * 5, fTexCoordOffsets);
	p->use();
}
//--------------------------------------------------------------------
// 膨胀
void FKCW_Shader_ShaderManager::SetDilate( CCSize p_tagNodeSize )
{
	LoadCustomShader( KeyShaderDilate );
	CCGLProgram* p = CCShaderCache::sharedShaderCache()->programForKey( KeyShaderDilate );
	// 计算纹理偏移
	GLfloat	fTexCoordOffsets[5 * 5 * 2];
	GLuint w = static_cast<GLuint>(p_tagNodeSize.width);
	GLuint h = static_cast<GLuint>(p_tagNodeSize.height);
	genTexCoordOffsets( w, h, fTexCoordOffsets );
	int nLoc = glGetUniformLocation( p->getProgram(), "u_TextureCoordOffset" );
	glUniform2fv(nLoc, 5 * 5, fTexCoordOffsets);
	p->use();
}
//--------------------------------------------------------------------
// 侵蚀效果
void FKCW_Shader_ShaderManager::SetErode( CCSize p_tagNodeSize )
{
	LoadCustomShader( KeyShaderErode );
	CCGLProgram* p = CCShaderCache::sharedShaderCache()->programForKey( KeyShaderErode );
	// 计算纹理偏移
	GLfloat	fTexCoordOffsets[5 * 5 * 2];
	GLuint w = static_cast<GLuint>(p_tagNodeSize.width);
	GLuint h = static_cast<GLuint>(p_tagNodeSize.height);
	genTexCoordOffsets( w, h, fTexCoordOffsets );
	int nLoc = glGetUniformLocation( p->getProgram(), "u_TextureCoordOffset" );
	glUniform2fv(nLoc, 5 * 5, fTexCoordOffsets);
	p->use();
}
//--------------------------------------------------------------------
// Laplacian描边
void FKCW_Shader_ShaderManager::SetLaplacian( CCSize p_tagNodeSize )
{
	LoadCustomShader( KeyShaderLaplacian );
	CCGLProgram* p = CCShaderCache::sharedShaderCache()->programForKey( KeyShaderLaplacian );
	// 计算纹理偏移
	GLfloat	fTexCoordOffsets[5 * 5 * 2];
	GLuint w = static_cast<GLuint>(p_tagNodeSize.width);
	GLuint h = static_cast<GLuint>(p_tagNodeSize.height);
	genTexCoordOffsets( w, h, fTexCoordOffsets );
	int nLoc = glGetUniformLocation( p->getProgram(), "u_TextureCoordOffset" );
	glUniform2fv(nLoc, 5 * 5, fTexCoordOffsets);
	p->use();
}
//--------------------------------------------------------------------
// Sobel边缘检测
void FKCW_Shader_ShaderManager::SetSobel( CCSize p_tagNodeSize )
{
	LoadCustomShader( KeyShaderSobel );
	CCGLProgram* p = CCShaderCache::sharedShaderCache()->programForKey( KeyShaderSobel );
	// 计算纹理偏移
	GLfloat	fTexCoordOffsets[5 * 5 * 2];
	GLuint w = static_cast<GLuint>(p_tagNodeSize.width);
	GLuint h = static_cast<GLuint>(p_tagNodeSize.height);
	genTexCoordOffsets( w, h, fTexCoordOffsets );
	int nLoc = glGetUniformLocation( p->getProgram(), "u_TextureCoordOffset" );
	glUniform2fv(nLoc, 5 * 5, fTexCoordOffsets);
	p->use();
}
//--------------------------------------------------------------------
// Prewitt边缘检测
void FKCW_Shader_ShaderManager::SetPrewitt( CCSize p_tagNodeSize )
{
	LoadCustomShader( KeyShaderPrewitt );
	CCGLProgram* p = CCShaderCache::sharedShaderCache()->programForKey( KeyShaderPrewitt );
	// 计算纹理偏移
	GLfloat	fTexCoordOffsets[5 * 5 * 2];
	GLuint w = static_cast<GLuint>(p_tagNodeSize.width);
	GLuint h = static_cast<GLuint>(p_tagNodeSize.height);
	genTexCoordOffsets( w, h, fTexCoordOffsets );
	int nLoc = glGetUniformLocation( p->getProgram(), "u_TextureCoordOffset" );
	glUniform2fv(nLoc, 5 * 5, fTexCoordOffsets);
	p->use();
}
//--------------------------------------------------------------------
// 运动模糊
void FKCW_Shader_ShaderManager::SetMotionBlur( CCSize p_tagNodeSize )
{
	LoadCustomShader( KeyShaderMotionBlur );
	CCGLProgram* p = CCShaderCache::sharedShaderCache()->programForKey( KeyShaderMotionBlur );
	// 计算纹理偏移
	GLfloat	fTexCoordOffsets[5 * 5 * 2];
	GLuint w = static_cast<GLuint>(p_tagNodeSize.width);
	GLuint h = static_cast<GLuint>(p_tagNodeSize.height);
	genTexCoordOffsets( w, h, fTexCoordOffsets );
	int nLoc = glGetUniformLocation( p->getProgram(), "u_TextureCoordOffset" );
	glUniform2fv(nLoc, 5 * 5, fTexCoordOffsets);
	p->use();
}
//--------------------------------------------------------------------
// 精细浮雕
void FKCW_Shader_ShaderManager::SetBigEmboss( float t )
{
	LoadCustomShader( KeyShaderBigEmboss );
	CCGLProgram* p = CCShaderCache::sharedShaderCache()->programForKey( KeyShaderBigEmboss );
	p->use();
	p->setUniformLocationWith1f( sUniform_pos_CC_bigEmbossTime, t );
}
//--------------------------------------------------------------------
// 底片效果
void FKCW_Shader_ShaderManager::SetColorRamp()
{
	LoadCustomShader( KeyShaderColorRamp );
	CCGLProgram* p = CCShaderCache::sharedShaderCache()->programForKey( KeyShaderColorRamp );
	// 计算纹理混合
	int nColorRampUniformLocation = glGetUniformLocation(p->getProgram(), "u_colorRampTexture");
	glUniform1i( nColorRampUniformLocation, 1 );

	CCTexture2D* pTex = CCTextureCache::sharedTextureCache()->addImage( "Engine/ColorRamp.png" );
	pTex->setAliasTexParameters();

	p->use();

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, pTex->getName() );
	glActiveTexture(GL_TEXTURE0);
}
//--------------------------------------------------------------------