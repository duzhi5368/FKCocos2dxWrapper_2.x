//-------------------------------------------------------------------------
#include "../Include/FKCW_3D_Range.h"
//-------------------------------------------------------------------------
bool FKCW_3D_Range::init(float xmin,float xmax,float ymin,float ymax,float zmin,float zmax)
{
	assert(xmax>=xmin);
	assert(ymax>=ymin);
	assert(zmax>=zmin);
	m_xmin=xmin;
	m_xmax=xmax;
	m_ymin=ymin;
	m_ymax=ymax;
	m_zmin=zmin;
	m_zmax=zmax;
	return true;
}
//-------------------------------------------------------------------------
FKCW_3D_Range::FKCW_3D_Range()
{
	_InitMembers();
}
//-------------------------------------------------------------------------
FKCW_3D_Range::FKCW_3D_Range(float xmin,float xmax,float ymin,float ymax,float zmin,float zmax)
{
	_InitMembers();
	init(xmin, xmax, ymin, ymax, zmin, zmax);
}
//-------------------------------------------------------------------------
FKCW_3D_Range::~FKCW_3D_Range()
{

}
//-------------------------------------------------------------------------
void FKCW_3D_Range::_InitMembers()
{
	m_xmin=m_xmax=0;
	m_ymin=m_ymax=0;
	m_zmin=m_zmax=0;
}
//-------------------------------------------------------------------------