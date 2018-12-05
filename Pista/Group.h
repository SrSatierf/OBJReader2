#pragma once
#include <glm\glm.hpp>
#include <string>
#include <vector>
#include "Face.h"

#include <GL\glew.h>
class Group
{
	

public:
	Group();
	~Group();
	std::string name;
	std::string material;
	std::vector<Face> faces;
	GLuint vbo;
	GLuint vao;
};

