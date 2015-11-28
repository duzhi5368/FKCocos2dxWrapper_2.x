//--------------------------------------------------------------------
#include "../Include/FKCW_Action_Shine.h"
#include "../../FKCW_Shader/Include/FKCW_Shader_ShaderManager.h"
//--------------------------------------------------------------------
FKCW_Action_Shine::FKCW_Action_Shine()
	: m_pOldProgram( NULL )
{

}
//--------------------------------------------------------------------
FKCW_Action_Shine::~FKCW_Action_Shine()
{

}
//--------------------------------------------------------------------
FKCW_Action_Shine* FKCW_Action_Shine::Create(float duration, ccColor4B color1, ccColor4B color2, ccColor4B color3, ccVertex3F gradientPositions)
{
	FKCW_Action_Shine* s = new FKCW_Action_Shine();
	if(s->InitWithColors(duration, color1, color2, color3, gradientPositions)) 
	{
		s->autorelease();
		return s;
	}
	CC_SAFE_RELEASE(s);
	return NULL;
}
//--------------------------------------------------------------------
bool FKCW_Action_Shine::InitWithColors(float duration, ccColor4B color1, ccColor4B color2, ccColor4B color3, ccVertex3F gradientPositions)
{
	if(!CCActionInterval::initWithDuration(duration)) 
	{
		return false;
	}

	m_tagColor1 = color1;
	m_tagColor2 = color2;
	m_tagColor3 = color3;
	m_tagGradientPositions = gradientPositions;

	return true;
}
//--------------------------------------------------------------------
void FKCW_Action_Shine::update(float time)
{
	FKCW_Shader_ShaderManager::SetShine(m_tagSize.width,
		ccp(0, m_tagSize.height),
		ccp(m_tagSize.height, 0),
		m_tagColor1,
		m_tagColor2,
		m_tagColor3,
		m_tagGradientPositions,
		time);
}
//--------------------------------------------------------------------
void FKCW_Action_Shine::startWithTarget(CCNode *pTarget)
{
	CCActionInterval::startWithTarget(pTarget);

	// 保存旧Program
	if(!m_pOldProgram) 
	{
		m_pOldProgram = pTarget->getShaderProgram();
	}

	// 计算速度
	m_tagSize = pTarget->getContentSize();

	// 设置新Program
	pTarget->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderShine ));
	FKCW_Shader_ShaderManager::SetShine(m_tagSize.width,
		ccp(0, m_tagSize.height),
		ccp(m_tagSize.height, 0),
		m_tagColor1,
		m_tagColor2,
		m_tagColor3,
		m_tagGradientPositions,
		0);
}
//--------------------------------------------------------------------
void FKCW_Action_Shine::stop()
{
	if(getTarget()) 
	{
		getTarget()->setShaderProgram(m_pOldProgram);
	}
	CCActionInterval::stop();
}
//--------------------------------------------------------------------
CCObject* FKCW_Action_Shine::copyWithZone(CCZone* pZone)
{
	CCZone* pNewZone = NULL;
	FKCW_Action_Shine* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject) 
	{
		pCopy = (FKCW_Action_Shine*)(pZone->m_pCopyObject);
	}
	else 
	{
		pCopy = new FKCW_Action_Shine();
		pZone = pNewZone = new CCZone(pCopy);
	}

	CCActionInterval::copyWithZone(pZone);
	pCopy->InitWithColors(m_fDuration, m_tagColor1, m_tagColor2, m_tagColor3, m_tagGradientPositions);

	CC_SAFE_DELETE(pNewZone);
	return pCopy;
}
//--------------------------------------------------------------------
CCActionInterval* FKCW_Action_Shine::reverse()
{
	return (CCActionInterval*)copyWithZone(NULL)->autorelease();
}
//--------------------------------------------------------------------