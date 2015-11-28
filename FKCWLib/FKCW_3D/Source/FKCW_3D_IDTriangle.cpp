//-------------------------------------------------------------------------
#include "../Include/FKCW_3D_IDTriangle.h"
//-------------------------------------------------------------------------
FKCW_3D_IDTriangle::FKCW_3D_IDTriangle()
{
	_InitMembers();
}
//-------------------------------------------------------------------------
FKCW_3D_IDTriangle::FKCW_3D_IDTriangle(int ID0,int ID1,int ID2)
{
	_InitMembers();
	init(ID0, ID1, ID2);
}
//-------------------------------------------------------------------------
void FKCW_3D_IDTriangle::init(int ID0,int ID1,int ID2)
{
	m_array[0]=ID0;
	m_array[1]=ID1;
	m_array[2]=ID2;
}
//-------------------------------------------------------------------------
short FKCW_3D_IDTriangle::vID(int i)const 
{
	assert(i>=0&&i<=3);
	return m_array[i];
}
//-------------------------------------------------------------------------
void FKCW_3D_IDTriangle::setVID(int i,short vID)
{
	assert(i>=0&&i<=3);
	m_array[i]=vID;
}
//-------------------------------------------------------------------------
void FKCW_3D_IDTriangle::_InitMembers()
{
	memset(&m_array, 0, sizeof(m_array));
}
//-------------------------------------------------------------------------