#include "MTLReader.h"
#include <fstream>
#include <sstream>
#include <iostream>

MTLReader::MTLReader()
{
}


MTLReader::~MTLReader()
{
}

MaterialMapping MTLReader::read(std::string path)
{
	MaterialMapping materialMapping;
	Material* currentMaterial = new Material();
	std::ifstream arq(path);

	if (!arq.is_open()) {
		std::cout << "File could not be loaded. Wrong path!" << std::endl;
	}

	while (!arq.eof()) {
		std::string line;
		getline(arq, line);
		std::stringstream sline;
		sline << line;
		std::string temp;
		sline >> temp;
		if (temp == "Kd") {
			float x, y, z;
			sline >> x >> y >> z;
			currentMaterial->kd = (glm::vec3(x, y, z));
		}
		else if (temp == "Ka") {
			float x, y, z;
			sline >> x >> y >> z;
			currentMaterial->ka = (glm::vec3(x, y, z));
		}
		else if (temp == "Ks") {
			float x, y, z;
			sline >> x >> y >> z;
			currentMaterial->ks = (glm::vec3(x, y, z));
		}
		else if (temp == "Ni") {
			float x;
			sline >> x;
			currentMaterial->ni = (x);
		}
		else if (temp == "Ns") {
			float x;
			sline >> x;
			currentMaterial->ns = (x);
		}
		else if (temp == "map_Kd") {
			std::string x;
			sline >> x;
			currentMaterial->map_kd = (x);
		}
		else if (temp == "newmtl") {
			std::string materialName;
			sline >> materialName;
			materialMapping[materialName] = Material();
			currentMaterial = &materialMapping[materialName];
		}
	}

	arq.close();
	return materialMapping;
}
