//*************************************************************************
//	创建日期:	2014-11-28
//	文件名称:	FKCW_RenderEx_SimpleShaderColorPainter.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_RenderEx_Macros.h"
//-------------------------------------------------------------------------
const GLchar* const szColorChangeSource =
				"#ifdef GL_ES													  \n \
				precision mediump float;										  \n \
				#endif															  \n \
				uniform sampler2D u_texture;									  \n \
				varying vec2 v_texCoord;										  \n \
				varying vec4 v_fragmentColor;									  \n \
				uniform mat4 matrixEffect;										  \n \
				void main(void)													  \n \
				{																  \n \
					gl_FragColor = texture2D(u_texture, v_texCoord)*matrixEffect*v_fragmentColor; \n \
				}";
//-------------------------------------------------------------------------
enum EffectSelect
{
	ES_NONE              = 0,  //没有特效
	ES_BLACK_AND_WHITE   = 1,  //黑白化
	ES_OLD_PHOTOS        = 2,  //老照片
	ES_INVERT            = 3,  //反相
	ES_BURNS			 = 4,  //灼伤（偏红）
	ES_POISONING		 = 5,  //中毒（偏绿）
	ES_COLD			     = 6,  //寒冷（偏蓝）

	ES_USER_DEFINED //自定义的特效矩阵序号从这里开始
};
//-------------------------------------------------------------------------
struct Matrix44
{
	GLfloat _matrix[16];
};
//-------------------------------------------------------------------------
struct EffectListNode
{
	CCNode* _pNode;
	GLint   _uniformMatrix;
	int		_effectSelect;
};
//-------------------------------------------------------------------------
/**
 * 单例模式的简单的片段渲染效果类
 * 先调用 addEffectNode 增加需要渲染特效的节点
 * 然后可以调用 setEffect 设置特效或 getEffectSelect的到特效选择
 */
class FKCW_RenderEx_SimpleShaderColorPainter
{
public:
	static FKCW_RenderEx_SimpleShaderColorPainter* getInstance();
	static void destroy();

	/**
	 * 增加自定义的特效矩阵
	 *@param matrix 特效矩阵
	 *@return 返回节点在 m_nodeList 的序号,失败则返回-1
	 */
	int addEffectMatrix(const Matrix44& matrix);

	/**
	 * 设置节点的特效，这个方法比较高效
	 *@param nodeIndex 节点序号，即addEffectNode方法的返回值
	 *@param sel 特效的选择
	 */
	bool setEffect(unsigned int nodeIndex,unsigned int sel);

	/**
	 * 设置节点的特效，建议使用更加高效的 bool setEffect(int nodeIndex,int sel)
	 * 必须先调用 addEffectNode 将需要渲染特效的节点加到队列中
	 *@param pNode 节点指针
	 *@param sel 特效的选择
	 */
	bool setEffect(CCNode* pNode,unsigned int sel);
	// 增加需要渲染特效的节点
	int  addEffectNode(CCNode* pNode);
	// 得到节点的特效选择，若没有此节点则返回-1，这个方法比较高效
	int  getEffectSelect(unsigned int nodeInde);
	// 得到节点的特效选择，若没有此节点则返回-1，建议使用更加高效的 int getEffectSelect(int nodeInde)
	int  getEffectSelect(CCNode* pNode);
	
private:
	static FKCW_RenderEx_SimpleShaderColorPainter* s_instance;

	FKCW_RenderEx_SimpleShaderColorPainter();
	~FKCW_RenderEx_SimpleShaderColorPainter(){}

	std::vector<Matrix44>		m_matrices;
	std::vector<EffectListNode> m_nodeList;
};