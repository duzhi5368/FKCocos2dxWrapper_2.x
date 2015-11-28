//-------------------------------------------------------------------------
#include "../Include/FKCW_Action_Suck.h"
//-------------------------------------------------------------------------
FKCW_Action_Suck* FKCW_Action_Suck::create( CCSize p_tagSize, float p_fTime, CCPoint p_tagPos )
{
	FKCW_Action_Suck* pRet = new FKCW_Action_Suck();

	if (pRet)
	{
		if (pRet->initWithGrid( p_tagSize, p_fTime, p_tagPos ))
		{
			pRet->autorelease();
		}
		else
		{
			delete pRet;
			pRet = NULL;
		}
	}
	return pRet;
}
//-------------------------------------------------------------------------
FKCW_Action_Suck* FKCW_Action_Suck::initWithGrid( CCSize p_tagSize, float p_fTime, CCPoint p_tagPos )
{
	do 
	{
		CC_BREAK_IF( !CCGrid3DAction::initWithDuration( p_fTime, p_tagSize ) );

		m_fAmplitudeRate = 1.0f;
		m_tagToPos = p_tagPos;
		return this;

	} while ( 0 );
	return NULL;
}
//-------------------------------------------------------------------------
void FKCW_Action_Suck::update(float time)
{
	int i, j;
	CCPoint target = m_tagToPos;
	int nWidth = static_cast<int>(getGrid()->getGridSize().width);
	int nHeight = static_cast<int>(getGrid()->getGridSize().height);
	for( i = 0; i < ( nWidth+1 ); i++ )
	{
		for( j = 0; j < ( nHeight+1 ); j++ )
		{
			float x = static_cast<float>(i);
			float y = static_cast<float>(j);
			ccVertex3F v = vertex( CCPoint( x, y ));

			float s = ccpDistance(ccp(v.x, v.y), target);
			float distx = fabs(v.x - target.x);
			float disty = fabs(v.y - target.y);

			if (s > 0.0f) 
			{
				s = s / (10/getDuration() * 8.0f);

				float valx = distx*time/s;
				if (valx > distx) valx = distx;
				if (valx < 0) valx = 0;
				float valy = disty*time/s;
				if (valy > disty) valy = disty;
				if (valy < 0) valy = 0;

				if (v.x < target.x)
					v.x += valx;
				if (v.x >= target.x)
					v.x -= valx;

				if (v.y < target.y)
					v.y += valy;
				if (v.y >= target.y)
					v.y -= valy;
			}
			setVertex( ccp(i,j), v );
		}
	}
}
//-------------------------------------------------------------------------