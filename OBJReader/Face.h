#pragma once
#include <vector>

class Face
{
	std::vector<int> verts;
	std::vector<int> norms;
	std::vector<int> texts;
public:
	Face();
	~Face();
	std::vector<int>* getVertex();
	std::vector<int>* getNormals();
	std::vector<int>* getTexts();
	void setVertex(int aux);
	void setNormals(int aux);
	void setTexts(int aux);
};

