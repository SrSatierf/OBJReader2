#pragma once
#include "Mesh.h"

struct Point {
	glm::vec3 pos;
	glm::vec3 color;

	Point(glm::vec3 pos, glm::vec3 color) {
		this->pos = pos;
		this->color = color;
	}
};

class Renderer
{
public:
	Renderer();
	~Renderer();
	static void Render(Shader shader, Mesh mesh);
	static void ImportCurve(std::string path);
	static std::vector<Point> points;
	static glm::vec3 GetPositionFromCurve(float t);
	static glm::vec3 CalcBezierPoint(float t, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	static float MapValues(float input, float input_start, float input_end, float output_start, float output_end);
};

