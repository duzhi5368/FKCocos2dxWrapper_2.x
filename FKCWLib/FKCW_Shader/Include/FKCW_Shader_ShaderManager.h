/********************************************************************
	created:	2014/10/29
	file base:	FKCW_Shader_ShaderManager
	author:		FreeKnight
	
	purpose:	
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_Shader_Macros.h"
//--------------------------------------------------------------------
// shader Key 枚举
#define KeyShaderFlash					"ShaderFlash"
#define KeyShaderBlur					"ShaderBlur"
#define KeyShaderLaser					"ShaderLaser"
#define KeyShaderLighting				"ShaderLighting"
#define KeyShaderMatrix					"ShaderMatrix"
#define KeyShaderShine					"ShaderShine"
#define KeyShaderBloom					"ShaderBloom"
#define KeyShaderCelShading				"ShaderCelShading"
#define KeyShaderEdgeDetection			"ShaderEdgeDetection"
#define KeyShaderOldPhoto				"ShaderOldPhoto"
#define KeyShaderNegative				"ShaderNegative"
#define KeyShaderBrightness				"ShaderBrightness"
#define KeyShaderContrast				"ShaderContrast"
#define KeyShaderSaturation				"ShaderSaturation"
#define KeyShaderBlackWhite				"ShaderBlackWhite"
#define KeyShaderSimpleEdgeDetection	"ShaderSimpleEdgeDetection"
#define KeyShaderEmboss					"ShaderEmboss"
#define KeyShaderSmallBlur				"ShaderSmallBlur"
#define KeyShaderBigBlur				"ShaderBigBlur"
#define KeyShaderSmallSharpen			"ShaderSmallSharpen"
#define KeyShaderBigSharpen				"ShaderBigSharpen"
#define KeyShaderSimpleSharpen			"ShaderSimpleSharpen"
#define KeyShaderDilate					"ShaderDilate"
#define KeyShaderErode					"ShaderErode"
#define KeyShaderLaplacian				"ShaderLaplacian"
#define KeyShaderSobel					"ShaderSobel"
#define KeyShaderPrewitt				"ShaderPrewitt"
#define KeyShaderMotionBlur				"ShaderMotionBlur"
#define KeyShaderBigEmboss				"ShaderBigEmboss"
#define KeyShaderColorRamp				"ShaderColorRamp"
//--------------------------------------------------------------------
class FKCW_Shader_ShaderManager
{
private:
	// 根据Key加载一个已定制shader
	static void				LoadCustomShader( const string& p_strKey );
public:
	// 获取指定Key的CCGLProgram
	static CCGLProgram*		ProgramForKey( const string& p_strKey );
private:
	static void genTexCoordOffsets(GLuint width,
		GLuint height, GLfloat* pOutTexCoordOff, GLfloat step = 1.0f );
public:
	// 设置闪光效果
	static void		SetFlash( float r, float g, float b, float t );
	// 设置模糊效果
	static void		SetBlur( CCSize p_tagNodeSize, CCSize p_tagBlurSize, 
		ccColor4F p_tagBlurSubtract = cc4fTRANSPARENT );
	// 设置光照效果
	static void		SetLighting( ccColor4B p_Mul, ccColor3B p_Add );
	// 设置颜色调整矩阵
	static void		SetColorMatrix( const kmMat4& p_Mat4 );
	// 设置灰色图
	static void		SetGray();
	// 设置光线效果
	// 参数：	p_fWidth 节点宽度
	//			p_tagLB 模板左下位置
	//			p_tagRT 模板右上位置
	//			p_tagColor 变化色1,2,3
	//			p_tagGradientPositions 光线变化位置，取值为[0,1]之间。通过这个将光线分段进行色彩变化
	//			p_fTime 时间[0,1]
	static void		SetShine( float p_fWidth, CCPoint p_tagLB, CCPoint p_tagRT,
		ccColor4B p_tagColor1, ccColor4B p_tagColor2, ccColor4B p_tagColor3,
		ccVertex3F p_tagGradientPositions, float p_fTime );
	// 设置bloom效果
	static void		SetBloom();
	// 设置卡通效果
	static void		SetCelShading( CCSize p_tagNodeSize );
	// 设置边缘检测
	static void		SetEdgeDetectionSize( CCSize p_tagNodeSize );
	// 设置老照片效果
	static void		SetOldPhoto();
	// 设置负色效果
	static void		SetNegative();
	// 设置高光
	static void		SetBrightness();
	// 设置对比效果
	static void		SetContrast();
	// 设置饱和
	static void		SetSaturation();
	// 设置黑白化
	static void		SetBlackWhite();
	// 设置简易边缘效果
	static void		SetSimpleEdgeDetection();
	// 设置浮雕效果
	static void		SetEmboss();
	// 轻度模糊效果
	static void		SetSmallBlur( CCSize p_tagNodeSize );
	// 重度模糊效果
	static void		SetBigBlur( CCSize p_tagNodeSize );
	// 轻度磨砂
	static void		SetSmallSharpen( CCSize p_tagNodeSize );
	// 重度磨砂
	static void		SetBigSharpen( CCSize p_tagNodeSize );
	// 简易磨砂
	static void		SetSimpleSharpen( CCSize p_tagNodeSize );
	// 膨胀
	static void		SetDilate( CCSize p_tagNodeSize );
	// 侵蚀效果
	static void		SetErode( CCSize p_tagNodeSize );
	// Laplacian描边
	static void		SetLaplacian( CCSize p_tagNodeSize );
	// Sobel边缘检测
	static void		SetSobel( CCSize p_tagNodeSize );
	// Prewitt边缘检测
	static void		SetPrewitt( CCSize p_tagNodeSize );
	// 运动模糊
	static void		SetMotionBlur( CCSize p_tagNodeSize );
	// 精细浮雕
	static void		SetBigEmboss( float t );
	// 底片效果
	static void		SetColorRamp();
};