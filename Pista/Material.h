#pragma once
#include <glm\glm.hpp>
#include <string>
class Material
{
public:
	Material();
	~Material();

	glm::vec3 kd;
	glm::vec3 ka;
	glm::vec3 ks;
	float ni;
	float ns;
	std::string map_kd;

	std::string name;
};

