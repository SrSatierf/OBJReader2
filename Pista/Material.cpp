#include "Material.h"



Material::Material()
{
	kd = glm::vec3(1.0f, 1.0f, 1.0f);
	ka = glm::vec3(1.0f, 1.0f, 1.0f);
	ks = glm::vec3(1.0f, 1.0f, 1.0f);
	ns = 1.0f;
	ni = 1.0f;
}


Material::~Material()
{
}
