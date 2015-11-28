//-------------------------------------------------------------------------
#include "../Include/FKCW_Pixel_BaseStruct.h"
//-------------------------------------------------------------------------
FKCW_Pixel_IJ::FKCW_Pixel_IJ()
{
	initMemebers();
}
//-------------------------------------------------------------------------
FKCW_Pixel_IJ::~FKCW_Pixel_IJ()
{

}
//-------------------------------------------------------------------------
FKCW_Pixel_IJ::FKCW_Pixel_IJ(int i,int j)
{
	initMemebers();
	m_i=i;
	m_j=j;
}
//-------------------------------------------------------------------------
void FKCW_Pixel_IJ::setI(int i)
{
	m_i=i;
}
//-------------------------------------------------------------------------
void FKCW_Pixel_IJ::setJ(int j)
{
	m_j=j;
}
//-------------------------------------------------------------------------
void FKCW_Pixel_IJ::setIJ(int i,int j)
{
	m_i=i;
	m_j=j;
}
//-------------------------------------------------------------------------
int FKCW_Pixel_IJ::getI()const
{
	return m_i;
}
//-------------------------------------------------------------------------
int FKCW_Pixel_IJ::getJ()const
{
	return m_j;
}
void FKCW_Pixel_IJ::initMemebers()
{
	m_i=m_j=0;
}
//-------------------------------------------------------------------------
FKCW_Pixel_CollisionRect::FKCW_Pixel_CollisionRect()
{

}
//-------------------------------------------------------------------------
FKCW_Pixel_CollisionRect::~FKCW_Pixel_CollisionRect()
{

}
//-------------------------------------------------------------------------
FKCW_Pixel_CollisionRect::FKCW_Pixel_CollisionRect(const CCRect&rect,const string&name)
{
	init(rect, name);
}
//-------------------------------------------------------------------------
bool FKCW_Pixel_CollisionRect::init(const CCRect&rect,const string&name)
{
	setRect(rect);
	setName(name);
	return true;
}
//-------------------------------------------------------------------------
void FKCW_Pixel_CollisionRect::setRect(const CCRect&rect)
{
	m_rect=rect;
}
//-------------------------------------------------------------------------
void FKCW_Pixel_CollisionRect::setName(const string&name)
{
	m_name=name;
}
//-------------------------------------------------------------------------
CCRect FKCW_Pixel_CollisionRect::getRect()const
{
	return m_rect;
}
//-------------------------------------------------------------------------
string FKCW_Pixel_CollisionRect::getName()const
{
	return m_name;
}
//-------------------------------------------------------------------------