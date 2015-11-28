//-------------------------------------------------------------------------
#include "../Include/FKCW_3D_Material.h"
//-------------------------------------------------------------------------
FKCW_3D_Material::FKCW_3D_Material()
{
	m_shininess=2.0f;
	m_ambient.init(0.2f, 0.2f, 0.2f, 1.0f);
	m_diffuse.init(0.8f, 0.8f, 0.8f, 1.0f);
	m_specular.init(0.0f, 0.0f, 0.0f, 1.0f);
}
//-------------------------------------------------------------------------
FKCW_3D_Material::~FKCW_3D_Material()
{

}
//-------------------------------------------------------------------------