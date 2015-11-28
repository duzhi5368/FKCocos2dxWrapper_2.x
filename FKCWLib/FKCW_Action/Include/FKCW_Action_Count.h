/********************************************************************
	created:	2014/11/03
	file base:	FKCW_Action_Count
	author:		FreeKnight
	
	purpose:	数字翻转动作
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKCW_Action_Macros.h"
//--------------------------------------------------------------------
// 增加或减少一个数字标签，翻转为另一个
// 注：动作对象必须实现CCLabelProtocol或者cocostudio的Labels.
//  例如：UILabelBMFont 和 UILabel
class FKCW_Action_Count : public CCActionInterval
{
private:
	enum ENUM_LabelType
	{
		eLabelType_Protocol,
		eLabelType_Label,
		eLabelType_BitmapLabel,
	};
private:
	int				m_nFrom;		// 原值
	int				m_nTo;			// 目标值
	string			m_strPattern;	// 格式模板
	char*			m_szBuf;		// 缓冲区
	ENUM_LabelType	m_eType;		// 标签类型
public:
	FKCW_Action_Count();
	virtual ~FKCW_Action_Count();

	// 创建一个翻转数字动作实例
	// 参数：pattern Label文字格式，必须包含一个 %d 格式
	static FKCW_Action_Count* Create(float duration, int from, int to, const string& pattern = "%d");
	bool InitWithDuration(float d, int from, int to, const string& pattern);

	virtual void update(float time);
	virtual void startWithTarget(CCNode *pTarget);
	virtual CCObject* copyWithZone(CCZone* pZone);
	virtual CCActionInterval* reverse();
};