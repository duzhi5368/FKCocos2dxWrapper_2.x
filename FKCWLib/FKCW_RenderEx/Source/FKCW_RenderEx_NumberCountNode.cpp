//-------------------------------------------------------------------------
#include "../Include/FKCW_RenderEx_NumberCountNode.h"
//-------------------------------------------------------------------------
void FKCW_RenderEx_NumberCountNode::setLabelAtlas(CCLabelAtlas* ccLabelAtlas)   
{
	m_pLabelAtlas = ccLabelAtlas;
	m_pLabelAtlas->setString(CCString::createWithFormat("%d",m_number)->getCString());
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_NumberCountNode::setLabelAtlas(ui::LabelAtlas* uiLabelAtlas) 
{
	m_pLabelAtlas = (CCLabelAtlas*)uiLabelAtlas->getVirtualRenderer();
	m_pLabelAtlas->setString(CCString::createWithFormat("%d",m_number)->getCString());
}
//-------------------------------------------------------------------------
FKCW_RenderEx_NumberCountNode::FKCW_RenderEx_NumberCountNode():
	m_pLabelAtlas(0),
	m_number(0),
	m_increment(0),
	m_numberDelta(1),
	m_timeDelta(0.01f)
{
	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(FKCW_RenderEx_NumberCountNode::updateNumber),this,0.0f,false);
}
//-------------------------------------------------------------------------
FKCW_RenderEx_NumberCountNode::~FKCW_RenderEx_NumberCountNode()
{
	CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(FKCW_RenderEx_NumberCountNode::updateNumber),this);
}
//-------------------------------------------------------------------------
bool FKCW_RenderEx_NumberCountNode::setNumber(int number, bool bGradually)
{
	if(!m_pLabelAtlas)
		return false;

	if(!bGradually)
	{
		m_number = number;
		m_pLabelAtlas->setString(CCString::createWithFormat("%d",number)->getCString());
	}
	else
	{
		m_increment = number - m_number;
	}

	m_timeEscape = 0.0f;

	return true;
}
//-------------------------------------------------------------------------
void FKCW_RenderEx_NumberCountNode::updateNumber(float fInterval)
{
	if(m_increment==0 || !m_pLabelAtlas)
		return ;

	if(m_timeEscape > m_timeDelta)
	{
		if(m_increment>0)
		{
			if(m_increment - m_numberDelta > 0)
			{
				m_number     = m_number + m_numberDelta;
				m_increment -= m_numberDelta;
			}
			else
			{
				m_number    = m_number + m_increment;
				m_increment = 0;
			}
		}
		else
		{
			if(m_increment + m_numberDelta < 0)
			{
				m_number     = m_number - m_numberDelta;
				m_increment += m_numberDelta;
			}
			else
			{
				m_number    = m_number + m_increment;
				m_increment = 0;
			}
		}

		m_pLabelAtlas->setString(CCString::createWithFormat("%d",m_number)->getCString());
		m_timeEscape = 0.0f;
	}
	else
		m_timeEscape+=fInterval;
}
//-------------------------------------------------------------------------