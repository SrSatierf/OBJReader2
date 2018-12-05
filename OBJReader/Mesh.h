#pragma once
#include <glm\glm.hpp>
#include <vector>
#include "Group.h"
#include "Material.h"
#include <map>
#include <GL\glew.h>
#include "Shader.h"
class Mesh
{
	std::vector<glm::vec2> mappings;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> vertex;
	std::vector<Group*> groups;

public:
	Mesh();
	~Mesh();
	std::vector<glm::vec3>* getVertex();
	std::vector<glm::vec3>* getNormals();
	std::vector<glm::vec2>* getMappings();
	std::vector<Group*>* getGroups();
	void setVertex(glm::vec3 args);
	void setNormals(glm::vec3 args);
	void setMappings(glm::vec2 args);
	void setGroups(Group* arg);
	std::map<std::string, Material> materials;
	void binding();

	void loadTextures(Shader* shader, const char* uniformName, const char* path);

	
};

