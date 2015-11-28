//*************************************************************************
//	创建日期:	2014-11-28
//	文件名称:	FKCW_RenderEx_NumberCountNode.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_RenderEx_Macros.h"
#include "cocos-ext.h"
//-------------------------------------------------------------------------
USING_NS_CC_EXT;
//-------------------------------------------------------------------------
/**
 * @brief 一个数字标签的增强功能类
 *
 * 功能：让数字标签按设定的增量一逐步增加
 *       数字会每隔m_timeDelta时间,增加m_numberDelta,直到总共增加m_increment
 *
 * 应用范围：可以用于分数的显示
 * 可以管理 cocos2d 的 CCLabelAtlas 和
 * 从 cocostudio 导出的 ui::LabelAtlas
 */
class FKCW_RenderEx_NumberCountNode : public CCObject
{
public:
	FKCW_RenderEx_NumberCountNode();
	~FKCW_RenderEx_NumberCountNode();

	// 设置纹理图片 【0123456789】这样的图
	void setLabelAtlas(CCLabelAtlas* ccLabelAtlas);
	void setLabelAtlas(ui::LabelAtlas* uiLabelAtlas);

	// 设置数字的增量m_increment（可理解为速度）
	void setIncrement(int increment)   {m_increment   = increment;}
	// 增加数字的增量m_increment（在原来的m_increment上再加increment）
	void addIncrement(int increment)   {m_increment += increment;}

	/**
	 * 设置数字每一步增加的增量的绝对值m_numberDelta
	 * 数字会每隔m_timeDelta时间,增加m_numberDelta,直到总共增加m_increment
	 * 可理解为加速度
	 */
	void setNumberDelta(int delta)     {m_numberDelta = abs(delta);}
	
	/**
	 * 设置数字每一步增加的时间间隔m_timeDelta
	 * 数字会每隔m_timeDelta时间,增加m_numberDelta,直到总共增加m_increment
	 */
	void setTimeDelta(float timeDelta) {m_timeDelta   = timeDelta;}

	// 获取当前数字
	int	getNumber(){return m_number;}
	/**
	 * 设置数字
	 *@param number 目标数字
	 *@param bGradually 是不是由当前数字一步一步增加到目标数字
	 */
	bool setNumber(int number, bool bGradually);

	// schedule方法,数字增加的计算就在这里实现
	void updateNumber(float fInterval);

private:
	CCLabelAtlas*	m_pLabelAtlas;
	int				m_number;
	int				m_increment;
	int				m_numberDelta;
	float			m_timeDelta;
	float			m_timeEscape;
};