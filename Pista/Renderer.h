#pragma once
#include "Mesh.h"
#include <vector>
#include "glm/glm.hpp"
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
	static void Render(Shader shader);
	static void Init();
	static void Bind();

	static GLuint vao, vbo;
	static GLuint curveVao, curveVbo;
	static GLuint intVao, intVbo;
	static GLuint extVao, extVbo;
	static std::vector<Point> points;
	static std::vector<Point> curvePoints, intCurve, extCurve;

	static Mesh* curveMesh;

	static float* pointsToArr(std::vector<Point> ppoints);
	static float* curvePointsToArr();
	static void addPoint(glm::vec3 pos);
	static int findClosestPoint(glm::vec3 pos);
	static void changePointHeight(int idx, float val);
	static void spline(float step);

	static void CalcBezierCurve(std::vector<Point> pointsVector, int segmentsPerCurve);
	static glm::vec3 CalcBezierPoint(float t, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);

	static void ExtrudeBezierCurve(float spacing);
	static glm::vec3 CalcExtrudePoint(glm::vec3 pa, glm::vec3 pb, bool inside, float size);
	static float AngleBetweenVector(glm::vec3 a, glm::vec3 b);

	static Mesh* GenerateMeshFromCurves();

	static Shader meshShader;

	static void ExportCurve(std::string path);
	static void ImportCurve(std::string path);
};

