#include "Face.h"



Face::Face()
{
}


Face::~Face()
{
}

std::vector<int>* Face::getVertex()
{
	return &verts;
}

std::vector<int>* Face::getNormals()
{
	return &norms;
}

std::vector<int>* Face::getTexts()
{
	return &texts;
}

void Face::setVertex(int aux)
{
	verts.push_back(aux);
}

void Face::setNormals(int aux)
{
	norms.push_back(aux);
}

void Face::setTexts(int aux)
{
	texts.push_back(aux);
}
