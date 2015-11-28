//--------------------------------------------------------------------
#include "../Include/FKCW_Action_Blur.h"
#include "../../FKCW_Shader/Include/FKCW_Shader_ShaderManager.h"
//--------------------------------------------------------------------
FKCW_Action_Blur::FKCW_Action_Blur()
	: m_pOldProgram( NULL )
{

}
//--------------------------------------------------------------------
FKCW_Action_Blur::~FKCW_Action_Blur()
{

}
//--------------------------------------------------------------------
FKCW_Action_Blur* FKCW_Action_Blur::Create(float duration, CCSize startBlurSize, CCSize endBlurSize)
{
	FKCW_Action_Blur* a = new FKCW_Action_Blur();
	a->InitWithBlurSize(duration, startBlurSize, endBlurSize);
	return (FKCW_Action_Blur*)a->autorelease();
}
//--------------------------------------------------------------------
bool FKCW_Action_Blur::InitWithBlurSize(float d, CCSize startBlurSize, CCSize endBlurSize)
{
	setDuration(d);
	m_tagStartBlurSize		= startBlurSize;
	m_tagEndBlurSize		= endBlurSize;
	m_tagDeltaBlurSize		= m_tagEndBlurSize - m_tagStartBlurSize;

	return true;
}
//--------------------------------------------------------------------
void FKCW_Action_Blur::update(float time)
{
	FKCW_Shader_ShaderManager::SetBlur(getTarget()->getContentSize(),
		m_tagStartBlurSize + m_tagDeltaBlurSize * time);
}
//--------------------------------------------------------------------
void FKCW_Action_Blur::startWithTarget(CCNode *pTarget)
{
	CCActionInterval::startWithTarget(pTarget);

	// ±£´æ¾Éprogram
	if(!m_pOldProgram) 
	{
		m_pOldProgram = pTarget->getShaderProgram();
	}

	// ÉèÖÃÐÂprogram
	pTarget->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderBlur));
	FKCW_Shader_ShaderManager::SetBlur(pTarget->getContentSize(), m_tagStartBlurSize);
}
//--------------------------------------------------------------------
void FKCW_Action_Blur::stop()
{
	getTarget()->setShaderProgram(m_pOldProgram);
	CCActionInterval::stop();
}
//--------------------------------------------------------------------
CCObject* FKCW_Action_Blur::copyWithZone(CCZone* pZone)
{
	CCZone* pNewZone = NULL;
	FKCW_Action_Blur* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject) 
	{
		pCopy = (FKCW_Action_Blur*)(pZone->m_pCopyObject);
	} else {
		pCopy = new FKCW_Action_Blur();
		pZone = pNewZone = new CCZone(pCopy);
	}

	CCActionInterval::copyWithZone(pZone);
	pCopy->InitWithBlurSize(m_fDuration, m_tagStartBlurSize, m_tagEndBlurSize);

	CC_SAFE_DELETE(pNewZone);
	return pCopy;
}
//--------------------------------------------------------------------
CCActionInterval* FKCW_Action_Blur::reverse()
{
	return FKCW_Action_Blur::Create(getDuration(), m_tagEndBlurSize, m_tagStartBlurSize);
}
//--------------------------------------------------------------------