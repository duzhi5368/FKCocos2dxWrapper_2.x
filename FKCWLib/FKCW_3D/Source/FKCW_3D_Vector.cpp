//--------------------------------------------------------------------
#include "../Include/FKCW_3D_Vector.h"
//--------------------------------------------------------------------
void FKCW_3D_Vector2::_InitMembers(){
	memset(&m_fArray, 0, sizeof(m_fArray));
}
//--------------------------------------------------------------------
void FKCW_3D_Vector2::init(float a0,float a1){
	m_fArray[0]=a0;
	m_fArray[1]=a1;
}
//--------------------------------------------------------------------
FKCW_3D_Vector2::FKCW_3D_Vector2(float a0,float a1){
	_InitMembers();
	init(a0,a1);
}
//--------------------------------------------------------------------
FKCW_3D_Vector2::FKCW_3D_Vector2(const float array[2]){
	_InitMembers();
	init(array);
}
//--------------------------------------------------------------------
vector<float> FKCW_3D_Vector2::convertToVector()const{
	vector<float> vec(m_fArray,m_fArray+2);
	return vec;
}
//--------------------------------------------------------------------
FKCW_3D_Vector2 FKCW_3D_Vector2::operator +(const FKCW_3D_Vector2&right)const {
	FKCW_3D_Vector2 rs(this->x()+right.x(),this->y()+right.y());
	return rs;
}
//--------------------------------------------------------------------
FKCW_3D_Vector2 FKCW_3D_Vector2::operator -(const FKCW_3D_Vector2&right)const {
	FKCW_3D_Vector2 rs(this->x()-right.x(),this->y()-right.y());
	return rs;
}
//--------------------------------------------------------------------
FKCW_3D_Vector2 FKCW_3D_Vector2::operator *(float k)const {
	FKCW_3D_Vector2 rs(this->x()*k,this->y()*k);
	return rs;
}
//--------------------------------------------------------------------
void FKCW_3D_Vector2::init(const float array[2]){
	for(int i=0;i<2;i++){
		m_fArray[i]=array[i];
	}
}
//--------------------------------------------------------------------
void FKCW_3D_Vector2::print(){
	FKLOG("FKCW_3D_Vector2: %f,%f",m_fArray[0],m_fArray[1]);
}
//--------------------------------------------------------------------
// FKCW_3D_Vector4
//--------------------------------------------------------------------
void FKCW_3D_Vector4::init(float a0,float a1,float a2,float a3){
	m_fArray[0]=a0;
	m_fArray[1]=a1;
	m_fArray[2]=a2;
	m_fArray[3]=a3;

}
//--------------------------------------------------------------------
FKCW_3D_Vector4::FKCW_3D_Vector4(float a0,float a1,float a2,float a3){
	_InitMembers();
	init(a0,a1,a2,a3);
}
//--------------------------------------------------------------------
FKCW_3D_Vector4::FKCW_3D_Vector4(const float array[4]){
	_InitMembers();
	init(array);
}
//--------------------------------------------------------------------
float FKCW_3D_Vector4::getAt(int index)const {
	assert(index>=0&&index<=3);
	return m_fArray[index];
}
//--------------------------------------------------------------------
vector<float> FKCW_3D_Vector4::convertToVector()const{
	vector<float> vec(m_fArray,m_fArray+4);
	return vec;
}
//--------------------------------------------------------------------
FKCW_3D_Vector4 FKCW_3D_Vector4::operator +(const FKCW_3D_Vector4&right)const {
	FKCW_3D_Vector4 rs(this->x()+right.x(),this->y()+right.y(),this->z()+right.z(),this->w()+right.w());
	return rs;
}
//--------------------------------------------------------------------
FKCW_3D_Vector4 FKCW_3D_Vector4::operator -(const FKCW_3D_Vector4&right)const {
	FKCW_3D_Vector4 rs(this->x()-right.x(),this->y()-right.y(),this->z()-right.z(),this->w()-right.w());
	return rs;
}
//--------------------------------------------------------------------
FKCW_3D_Vector4 FKCW_3D_Vector4::operator *(float k)const {
	FKCW_3D_Vector4 rs(this->x()*k,this->y()*k,this->z()*k,this->w()*k);
	return rs;
}
//--------------------------------------------------------------------
void FKCW_3D_Vector4::init(const float array[4]){
	for(int i=0;i<4;i++){
		m_fArray[i]=array[i];
	}
}
//--------------------------------------------------------------------
void FKCW_3D_Vector4::print(){
	FKLOG("FKCW_3D_Vector4: %f,%f,%f,%f",m_fArray[0],m_fArray[1],m_fArray[2],m_fArray[3]);
}
//--------------------------------------------------------------------