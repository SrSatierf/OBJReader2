#pragma once
#include"Mesh.h"
#include <string>
class OBJReader
{
public:
	OBJReader();
	~OBJReader();
	static Mesh* read(std::string path);
};

