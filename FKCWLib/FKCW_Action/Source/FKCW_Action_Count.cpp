//--------------------------------------------------------------------
#include "../Include/FKCW_Action_Count.h"
#include "cocos-ext.h"
//--------------------------------------------------------------------
using namespace cocos2d::ui;
//--------------------------------------------------------------------
FKCW_Action_Count::FKCW_Action_Count()
	: m_nFrom( 0 )
	, m_nTo( 0 )
	, m_szBuf( NULL )
{

}
//--------------------------------------------------------------------
FKCW_Action_Count::~FKCW_Action_Count()
{
	CC_SAFE_FREE( m_szBuf );
}
//--------------------------------------------------------------------
// 创建一个翻转数字动作实例
// 参数：pattern Label文字格式，必须包含一个 %d 格式
FKCW_Action_Count* FKCW_Action_Count::Create(float duration, int from, int to, const string& pattern)
{
	FKCW_Action_Count* a = new FKCW_Action_Count();
	if(a->InitWithDuration(duration, from, to, pattern)) 
	{
		a->autorelease();
		return a;
	}
	CC_SAFE_RELEASE(a);
	return NULL;
}
//--------------------------------------------------------------------
bool FKCW_Action_Count::InitWithDuration(float d, int from, int to, const string& pattern)
{
	setDuration(d);
	m_nFrom			= from;
	m_nTo			= to;
	m_strPattern	= pattern;
	m_szBuf			= (char*)malloc(sizeof(char) * (m_strPattern.length() + 32));

	return true;
}
//--------------------------------------------------------------------
void FKCW_Action_Count::update(float time)
{
	int v = static_cast<int>(m_nFrom + (m_nTo - m_nFrom) * time);
	sprintf(m_szBuf, m_strPattern.c_str(), v);
	switch (m_eType) 
	{
	case eLabelType_Label:
		dynamic_cast<Label*>(m_pTarget)->setText(m_szBuf);
		break;
	case eLabelType_BitmapLabel:
		dynamic_cast<LabelBMFont*>(m_pTarget)->setText(m_szBuf);
		break;
	default:
		dynamic_cast<CCLabelProtocol*>(m_pTarget)->setString(m_szBuf);
		break;
	}
}
//--------------------------------------------------------------------
void FKCW_Action_Count::startWithTarget(CCNode *pTarget)
{
	CCActionInterval::startWithTarget(pTarget);

	if(dynamic_cast<CCLabelProtocol*>(pTarget))
	{
		m_eType = eLabelType_Protocol;
	} 
	else if(dynamic_cast<Label*>(pTarget)) 
	{
		m_eType = eLabelType_Label;
	} 
	else if(dynamic_cast<LabelBMFont*>(pTarget)) 
	{
		m_eType = eLabelType_BitmapLabel;
	} 
	else 
	{
		CCAssert(false, "CCCount only support CCLabelProtocol or CocoStudio Label/LabelBMFont");
	}
}
//--------------------------------------------------------------------
CCObject* FKCW_Action_Count::copyWithZone(CCZone* pZone)
{
	CCZone* pNewZone = NULL;
	FKCW_Action_Count* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject)
	{
		pCopy = (FKCW_Action_Count*)(pZone->m_pCopyObject);
	} 
	else 
	{
		pCopy = new FKCW_Action_Count();
		pZone = pNewZone = new CCZone(pCopy);
	}

	CCActionInterval::copyWithZone(pZone);
	pCopy->InitWithDuration(m_fDuration, m_nFrom, m_nTo, m_strPattern);

	CC_SAFE_DELETE(pNewZone);
	return pCopy;
}
//--------------------------------------------------------------------
CCActionInterval* FKCW_Action_Count::reverse()
{
	return FKCW_Action_Count::Create(getDuration(), m_nTo, m_nFrom, m_strPattern);
}
//--------------------------------------------------------------------