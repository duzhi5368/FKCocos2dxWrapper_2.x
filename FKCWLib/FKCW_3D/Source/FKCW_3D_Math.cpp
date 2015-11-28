//--------------------------------------------------------------------
#include "../Include/FKCW_3D_Math.h"
//--------------------------------------------------------------------
FKCW_3D_Matrix4 unitMat(){
	FKCW_3D_Matrix4 mat(1,0,0,0,//col 1
		0,1,0,0,
		0,0,1,0,
		0,0,0,1);
	return mat;
}
//--------------------------------------------------------------------
FKCW_3D_Matrix4 zeroMat(){
	FKCW_3D_Matrix4 mat(0,0,0,0,//col 1
		0,0,0,0,
		0,0,0,0,
		0,0,0,0);
	return mat;
}
//--------------------------------------------------------------------
bool isEqual(const FKCW_3D_Matrix4&mat1,const FKCW_3D_Matrix4&mat2,float eps){
	for(int i=0;i<16;i++){
		float d=fabsf(mat1.getAt(i)-mat2.getAt(i));
		if(d>eps)return false;
	}
	return true;
}
//--------------------------------------------------------------------
bool isEqual(const FKCW_3D_Vector4&v1,const FKCW_3D_Vector4&v2,float eps){
	for(int i=0;i<4;i++){
		float d=fabsf(v1.getAt(i)-v2.getAt(i));
		if(d>eps)return false;
	}
	return true;
}
//--------------------------------------------------------------------
bool isUnitMat(const FKCW_3D_Matrix4&mat){
	return isEqual(mat, unitMat());
}
//--------------------------------------------------------------------
FKCW_3D_Vector4 normalize(const FKCW_3D_Vector4&v){
	assert(v.w()==0);
	float r2=v.x()*v.x()+v.y()*v.y()+v.z()*v.z();
	if(r2==0){
		return FKCW_3D_Vector4(0,0,0,0);
	}
	float r=sqrtf(r2);
	FKCW_3D_Vector4 rs(v.x()/r,v.y()/r,v.z()/r,0);
	return rs;
}
//--------------------------------------------------------------------
float dot(const FKCW_3D_Vector4&v1,const FKCW_3D_Vector4&v2){
	return v1.x()*v2.x()+v1.y()*v2.y()+v1.z()*v2.z();
}
//--------------------------------------------------------------------
FKCW_3D_Vector4 cross(const FKCW_3D_Vector4&v1,const FKCW_3D_Vector4&v2)
{
	assert(v1.w()==0);
	assert(v2.w()==0);
	FKCW_3D_Vector4 rs(v1.y()*v2.z()-v1.z()*v2.y(),
		v1.z()*v2.x()-v1.x()*v2.z(),
		v1.x()*v2.y()-v1.y()*v2.x(),
		0);//cross product result must be a vector, so the fourth component set to zero
	return rs;
}
//--------------------------------------------------------------------
FKCW_3D_Matrix4 transpose(const FKCW_3D_Matrix4&mat){

	const float* m=mat.getArray();
	float rs[16]={
		m[0],m[4],m[8],m[12],//col 1
		m[1],m[5],m[9],m[13],//col 2
		m[2],m[6],m[10],m[14],//col 3
		m[3],m[7],m[11],m[15]//col 4
	};
	return FKCW_3D_Matrix4(rs);
}
//--------------------------------------------------------------------
FKCW_3D_Matrix4 inverse(const FKCW_3D_Matrix4&mat)
{
	float m[16];
	for(int i=0;i<16;i++)m[i]=mat.getAt(i);

	float invOut[16];

	float inv[16], det;
	int i;

	inv[0] = m[5]  * m[10] * m[15] -
		m[5]  * m[11] * m[14] -
		m[9]  * m[6]  * m[15] +
		m[9]  * m[7]  * m[14] +
		m[13] * m[6]  * m[11] -
		m[13] * m[7]  * m[10];

	inv[4] = -m[4]  * m[10] * m[15] +
		m[4]  * m[11] * m[14] +
		m[8]  * m[6]  * m[15] -
		m[8]  * m[7]  * m[14] -
		m[12] * m[6]  * m[11] +
		m[12] * m[7]  * m[10];

	inv[8] = m[4]  * m[9] * m[15] -
		m[4]  * m[11] * m[13] -
		m[8]  * m[5] * m[15] +
		m[8]  * m[7] * m[13] +
		m[12] * m[5] * m[11] -
		m[12] * m[7] * m[9];

	inv[12] = -m[4]  * m[9] * m[14] +
		m[4]  * m[10] * m[13] +
		m[8]  * m[5] * m[14] -
		m[8]  * m[6] * m[13] -
		m[12] * m[5] * m[10] +
		m[12] * m[6] * m[9];

	inv[1] = -m[1]  * m[10] * m[15] +
		m[1]  * m[11] * m[14] +
		m[9]  * m[2] * m[15] -
		m[9]  * m[3] * m[14] -
		m[13] * m[2] * m[11] +
		m[13] * m[3] * m[10];

	inv[5] = m[0]  * m[10] * m[15] -
		m[0]  * m[11] * m[14] -
		m[8]  * m[2] * m[15] +
		m[8]  * m[3] * m[14] +
		m[12] * m[2] * m[11] -
		m[12] * m[3] * m[10];

	inv[9] = -m[0]  * m[9] * m[15] +
		m[0]  * m[11] * m[13] +
		m[8]  * m[1] * m[15] -
		m[8]  * m[3] * m[13] -
		m[12] * m[1] * m[11] +
		m[12] * m[3] * m[9];

	inv[13] = m[0]  * m[9] * m[14] -
		m[0]  * m[10] * m[13] -
		m[8]  * m[1] * m[14] +
		m[8]  * m[2] * m[13] +
		m[12] * m[1] * m[10] -
		m[12] * m[2] * m[9];

	inv[2] = m[1]  * m[6] * m[15] -
		m[1]  * m[7] * m[14] -
		m[5]  * m[2] * m[15] +
		m[5]  * m[3] * m[14] +
		m[13] * m[2] * m[7] -
		m[13] * m[3] * m[6];

	inv[6] = -m[0]  * m[6] * m[15] +
		m[0]  * m[7] * m[14] +
		m[4]  * m[2] * m[15] -
		m[4]  * m[3] * m[14] -
		m[12] * m[2] * m[7] +
		m[12] * m[3] * m[6];

	inv[10] = m[0]  * m[5] * m[15] -
		m[0]  * m[7] * m[13] -
		m[4]  * m[1] * m[15] +
		m[4]  * m[3] * m[13] +
		m[12] * m[1] * m[7] -
		m[12] * m[3] * m[5];

	inv[14] = -m[0]  * m[5] * m[14] +
		m[0]  * m[6] * m[13] +
		m[4]  * m[1] * m[14] -
		m[4]  * m[2] * m[13] -
		m[12] * m[1] * m[6] +
		m[12] * m[2] * m[5];

	inv[3] = -m[1] * m[6] * m[11] +
		m[1] * m[7] * m[10] +
		m[5] * m[2] * m[11] -
		m[5] * m[3] * m[10] -
		m[9] * m[2] * m[7] +
		m[9] * m[3] * m[6];

	inv[7] = m[0] * m[6] * m[11] -
		m[0] * m[7] * m[10] -
		m[4] * m[2] * m[11] +
		m[4] * m[3] * m[10] +
		m[8] * m[2] * m[7] -
		m[8] * m[3] * m[6];

	inv[11] = -m[0] * m[5] * m[11] +
		m[0] * m[7] * m[9] +
		m[4] * m[1] * m[11] -
		m[4] * m[3] * m[9] -
		m[8] * m[1] * m[7] +
		m[8] * m[3] * m[5];

	inv[15] = m[0] * m[5] * m[10] -
		m[0] * m[6] * m[9] -
		m[4] * m[1] * m[10] +
		m[4] * m[2] * m[9] +
		m[8] * m[1] * m[6] -
		m[8] * m[2] * m[5];

	det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

	if (det == 0)
		assert(false);

	det = 1.0f / det;

	for (i = 0; i < 16; i++)
		invOut[i] = inv[i] * det;

	return FKCW_3D_Matrix4(invOut);
}
//--------------------------------------------------------------------
FKCW_3D_Vector4 toV4(const FKCW_3D_Vector2&v2,float z,float w){
	return FKCW_3D_Vector4(v2.x(), v2.y(), z, w);
}
//--------------------------------------------------------------------
FKCW_3D_Vector2 toV2(const FKCW_3D_Vector4&v4){
	return FKCW_3D_Vector2(v4.x(), v4.y());
}
//--------------------------------------------------------------------
float getLength2(const FKCW_3D_Vector4&v){
	assert(v.w()==0);
	return v.x()*v.x()+v.y()*v.y()+v.z()*v.z();
}
//--------------------------------------------------------------------
float getLength(const FKCW_3D_Vector4&v){
	assert(v.w()==0);
	return sqrtf(v.x()*v.x()+v.y()*v.y()+v.z()*v.z());
}
//--------------------------------------------------------------------