//-------------------------------------------------------------------------
#include "../Include/FKCW_Action_Flash.h"
#include "../../FKCW_Shader/Include/FKCW_Shader_ShaderManager.h"
//-------------------------------------------------------------------------
FKCW_Action_Flash::FKCW_Action_Flash()
	: m_pOldProgram( NULL )
{

}
//-------------------------------------------------------------------------
FKCW_Action_Flash::~FKCW_Action_Flash()
{

}
//-------------------------------------------------------------------------
// 创建一个闪光对象
// 参数：p_fDuration 闪光动作持续时间 p_tagColor 闪光色
FKCW_Action_Flash* FKCW_Action_Flash::Create(float p_fDuration, ccColor3B p_tagColor )
{
	FKCW_Action_Flash* f = new FKCW_Action_Flash();
	f->InitWithDurationAndColor( p_fDuration, p_tagColor );
	return ( FKCW_Action_Flash* )f->autorelease();
}
//-------------------------------------------------------------------------
bool FKCW_Action_Flash::InitWithDurationAndColor(float d, ccColor3B c)
{
	setDuration( d );
	m_tagColor = c;

	m_fR = m_tagColor.r / 255.0f;
	m_fG = m_tagColor.g / 255.0f;
	m_fB = m_tagColor.b / 255.0f;
	return true;
}
//-------------------------------------------------------------------------
void FKCW_Action_Flash::update(float time)
{
	float t;
	if(time > 0.5f) 
	{
		t = 1 - (time - 0.5f) / 0.5f;
	} 
	else 
	{
		t = time / 0.5f;
	}
	// 开启shader
	FKCW_Shader_ShaderManager::SetFlash(m_fR, m_fG, m_fB, t);
}
//-------------------------------------------------------------------------
void FKCW_Action_Flash::startWithTarget(CCNode *pTarget)
{
	CCActionInterval::startWithTarget(pTarget);

	// 保存旧的Program
	if(!m_pOldProgram) 
	{
		m_pOldProgram = pTarget->getShaderProgram();
	}

	// 设置新program
	pTarget->setShaderProgram(FKCW_Shader_ShaderManager::ProgramForKey( KeyShaderFlash ));
}
//-------------------------------------------------------------------------
void FKCW_Action_Flash::stop()
{
	if(getTarget()) 
	{
		getTarget()->setShaderProgram(m_pOldProgram);
	}
	CCActionInterval::stop();
}
//-------------------------------------------------------------------------
CCObject* FKCW_Action_Flash::copyWithZone(CCZone* pZone)
{
	CCZone* pNewZone = NULL;
	FKCW_Action_Flash* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject) 
	{
		// 如果子类调用
		pCopy = (FKCW_Action_Flash*)(pZone->m_pCopyObject);
	} 
	else 
	{
		pCopy = new FKCW_Action_Flash();
		pZone = pNewZone = new CCZone(pCopy);
	}

	CCActionInterval::copyWithZone(pZone);
	pCopy->InitWithDurationAndColor(m_fDuration, m_tagColor);

	CC_SAFE_DELETE(pNewZone);
	return pCopy;
}
//-------------------------------------------------------------------------
CCActionInterval* FKCW_Action_Flash::reverse()
{
	return FKCW_Action_Flash::Create(getDuration(), m_tagColor);
}
//-------------------------------------------------------------------------