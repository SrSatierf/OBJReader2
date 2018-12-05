#include "OBJReader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "MTLReader.h"

OBJReader::OBJReader()
{
}



OBJReader::~OBJReader()
{
}

Mesh * OBJReader::read(std::string path)
{
	Mesh* mesh = new Mesh;
	std::ifstream arq(path);
	Group* currentGroup = nullptr;
	std::string currentMaterial;

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
		if (temp == "v") {
			float x, y, z;
			sline >> x >> y >> z;
			mesh->setVertex(glm::vec3(x, y, z));

		}
		else if (temp == "vt") {
			float s, t;
			sline >> s >> t;
			mesh->setMappings(glm::vec2(s, t));
		}
		else if (temp == "vn") {
			float x, y, z;
			sline >> x >> y >> z;
			mesh->setNormals(glm::vec3(x, y, z));
		}
		else if (temp == "f") {
			Face face;
			Face newFace;
			int fourVertsCounter = 0;
			while (!sline.eof()) {
				fourVertsCounter++;
				std::string token;
				sline >> token;
				std::stringstream stoken;
				stoken << token;
				std::string auxV;
				std::string auxT;
				std::string auxN;

				std::getline(stoken, auxV, '/');
				std::getline(stoken, auxT, '/');
				std::getline(stoken, auxN, '/');

				if (fourVertsCounter == 4) {
					newFace.setVertex(face.getVertex()->at(2));
					newFace.setVertex(atoi(auxV.c_str()));
					newFace.setVertex(face.getVertex()->at(0));

					if (face.getTexts()->size() > 0) {
						newFace.setTexts(face.getTexts()->at(2));
						newFace.setTexts(atoi(auxT.c_str()));
						newFace.setTexts(face.getTexts()->at(0));
					}
					if (face.getNormals()->size() > 0) {
						newFace.setNormals(face.getNormals()->at(2));
						newFace.setNormals(atoi(auxN.c_str()));
						newFace.setNormals(face.getNormals()->at(0));
					}
				}
				else {
					face.setVertex(atoi(auxV.c_str()));

					if (auxT != "") {
						face.setTexts(atoi(auxT.c_str()));
					}

					if (auxN != "") {
						face.setNormals(atoi(auxN.c_str()));
					}
				}
			}
			if (currentGroup == nullptr) {
				Group* group = new Group();
				group->name = "default";
				currentGroup = group;
				mesh->setGroups(currentGroup);
			}
			currentGroup->faces.push_back(face);
			if (fourVertsCounter == 4) {
				currentGroup->faces.push_back(newFace);
			}
		}
		else if (temp == "g") {
			std::string name;
			std::string tempName;
			while (!sline.eof()) {
				sline >> tempName;
				name += tempName;
			}

			bool foundGroup = false;
			for (Group* g : *mesh->getGroups()) {
				if (name == g->name) {
					currentGroup = g;
					foundGroup = true;
					break;
				}
			}
			if (!foundGroup) {
				Group* group = new Group();
				group->name = name;
				currentGroup = group;
				currentGroup->material = currentMaterial;
				mesh->getGroups()->push_back(currentGroup);
			}
		}
		else if (temp == "mtllib") {
			std::string materialPath;
			sline >> materialPath;
			std::stringstream sPath;
			sPath << path;
			std::string auxPath;
			std::getline(sPath, auxPath, '/');
			mesh->materials = MTLReader::read(auxPath + "/" + materialPath);
		}
		else if (temp == "usemtl") {
			std::string materialName;
			sline >> materialName;
			if (currentGroup == nullptr) {
				Group* group = new Group();
				group->name = "default";
				currentGroup = group;
				mesh->setGroups(currentGroup);
			}
			currentGroup->material = materialName;
			currentMaterial = materialName;
		}
	}

	arq.close();
	return mesh;
}
