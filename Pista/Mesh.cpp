#include "Mesh.h"

#include <iostream>

Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}

std::vector<glm::vec3>* Mesh::getVertex()
{
	return &vertex;
}

std::vector<glm::vec3>* Mesh::getNormals()
{
	return &normals;
}

std::vector<glm::vec2>* Mesh::getMappings()
{
	return &mappings;
}

std::vector<Group*>* Mesh::getGroups()
{
	return &groups;
}

void Mesh::setVertex(glm::vec3 args)
{
	vertex.push_back(args);
}

void Mesh::setNormals(glm::vec3 args)
{
	normals.push_back(args);
}

void Mesh::setMappings(glm::vec2 args)
{
	mappings.push_back(args);
}

void Mesh::setGroups(Group* arg)
{
	groups.push_back(arg);
}

void Mesh::binding()
{
	for (Group* group : groups) {
		std::vector<GLfloat> vertices;

		if (group->faces.size() <= 0) {
			continue;
		}
		bool haveTexture = group->faces[0].getTexts()->size() > 0;
		bool haveNorm = group->faces[0].getNormals()->size() > 0;

		for (Face face : group->faces) {
			std::vector<int> currentVertex = *face.getVertex();
			std::vector<int> currentNorm = *face.getNormals();
			std::vector<int> currentText = *face.getTexts();
			for (int i = 0; i < currentVertex.size(); i++) {
				glm::vec3 tempVertex = vertex[currentVertex[i] - 1];
				vertices.push_back(tempVertex.x);
				vertices.push_back(tempVertex.y);
				vertices.push_back(tempVertex.z);
				if (haveTexture) {
					glm::vec2 tempText = mappings[currentText[i] - 1];
					vertices.push_back(tempText.s);
					vertices.push_back(tempText.t);
				}
				if (haveNorm) {
					tempVertex = normals[currentNorm[i] - 1];
					vertices.push_back(tempVertex.x);
					vertices.push_back(tempVertex.y);
					vertices.push_back(tempVertex.z);
				}
			}
		}

		glGenVertexArrays(1, &group->vao);
		glGenBuffers(1, &group->vbo);

		glBindVertexArray(group->vao);

		//Define o VBO
		glBindBuffer(GL_ARRAY_BUFFER, group->vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

		float stride = 3 + haveTexture * 2 + haveNorm * 3;

		//Define o VAO dos vertices
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid *)0);
		glEnableVertexAttribArray(0);

		//Se tiver textura, define o VAO da textura
		if (haveTexture) {

			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);
		}

		//Define o VAO da normal se tiver
		if (haveNorm) {
			//Usado pra modificar o stride dependendo se existe textura 
			int bytesFromBegining = 3 + haveTexture * 2;
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid *)(bytesFromBegining * sizeof(GLfloat)));
			glEnableVertexAttribArray(2);
		}

		glBindVertexArray(0);

	}
}

void Mesh::loadTextures(Shader* shader, const char * uniformName, const char* path)
{
	//Identifica o material, carregando todas as texturas, com o proprio nome da textura no map_Kd 
	shader->Use();
	for (std::map<std::string, Material>::iterator it = materials.begin(); it != materials.end(); ++it) {
		std::string temp = (*it).second.map_kd.c_str();
		if (temp != "") {
			std::string texturePath = path + temp;
			shader->LoadTexture(texturePath.c_str(), uniformName, (*it).second.map_kd);
		}
	}
}

