#include "Renderer.h"
#include <string>
#include <iomanip>

GLuint Renderer::vao;
GLuint Renderer::vbo;
GLuint Renderer::curveVao;
GLuint Renderer::curveVbo;
GLuint Renderer::intVao;
GLuint Renderer::intVbo;
GLuint Renderer::extVao;
GLuint Renderer::extVbo;
std::vector<Point> Renderer::points;
std::vector<Point> Renderer::curvePoints;
std::vector<Point> Renderer::intCurve;
std::vector<Point> Renderer::extCurve;

Mesh* Renderer::curveMesh = nullptr;
Shader Renderer::meshShader;

Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

void Renderer::spline(float steps) {
	float x, y, z;
	float inc = 1.0f / steps;
	curvePoints.clear();
	if (points.size() >= 4) {
		for (size_t i = 0; i < points.size() - 3; i++)
		{
			for (float t = 0; t <= 1; t += inc)
			{
				x = (((-1 * pow(t, 3) + 3 * pow(t, 2) - 3 * t + 1)*points[i].pos.x
					+ (3 * pow(t, 3) - 6 * pow(t, 2) + 0 * t + 4)*points[i + 1].pos.x
					+ (-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1)*points[i + 2].pos.x
					+ (1 * pow(t, 3) + 0 * pow(t, 2) + 0 * t + 0)*points[i + 3].pos.x) / 6);

				y = (((-1 * pow(t, 3) + 3 * pow(t, 2) - 3 * t + 1)*points[i].pos.y
					+ (3 * pow(t, 3) - 6 * pow(t, 2) + 0 * t + 4)*points[i + 1].pos.y
					+ (-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1)*points[i + 2].pos.y
					+ (1 * pow(t, 3) + 0 * pow(t, 2) + 0 * t + 0)*points[i + 3].pos.y) / 6);

				z = (((-1 * pow(t, 3) + 3 * pow(t, 2) - 3 * t + 1)*points[i].pos.z
					+ (3 * pow(t, 3) - 6 * pow(t, 2) + 0 * t + 4)*points[i + 1].pos.z
					+ (-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1)*points[i + 2].pos.z
					+ (1 * pow(t, 3) + 0 * pow(t, 2) + 0 * t + 0)*points[i + 3].pos.z) / 6);
				curvePoints.push_back(Point(glm::vec3(x, y, z), glm::vec3(1, 0, 0)));
			}
		}

		/*int i = 0;
		for (float t = 0; t <= 1; t += inc)
		{
			x = (((-1 * pow(t, 3) + 3 * pow(t, 2) - 3 * t + 1)*points[points.size() - 3].pos.x
				+ (3 * pow(t, 3) - 6 * pow(t, 2) + 0 * t + 4)*points[points.size() - 2].pos.x
				+ (-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1)*points[points.size() - 1].pos.x
				+ (1 * pow(t, 3) + 0 * pow(t, 2) + 0 * t + 0)*points[0].pos.x) / 6);

			y = (((-1 * pow(t, 3) + 3 * pow(t, 2) - 3 * t + 1)*points[points.size() - 3].pos.y
				+ (3 * pow(t, 3) - 6 * pow(t, 2) + 0 * t + 4)*points[points.size() - 2].pos.y
				+ (-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1)*points[points.size() - 1].pos.y
				+ (1 * pow(t, 3) + 0 * pow(t, 2) + 0 * t + 0)*points[0].pos.y) / 6);
			curvePoints.push_back(Point(glm::vec3(x, y, 0), glm::vec3(1, 0, 0)));
		}*/

	}

	Bind();
}

void Renderer::CalcBezierCurve(std::vector<Point> pointsVector, int segmentsPerCurve)
{
	std::vector<Point> tempPoints = pointsVector;
	curvePoints.clear();
	std::vector<std::vector<Point>> curves;
	int numberOfCurves = 0;

	size_t pointsVecSize = tempPoints.size();
	if (pointsVecSize < 4) {
		return;
	}

	// Init each curve
	for (size_t j = 0; j < (pointsVecSize - 3); j += 3)
	{
		curves.push_back(std::vector<Point>());
		numberOfCurves++;
	}

	// Fill curves
	float step = 1.0f / (float)segmentsPerCurve;
	int curveIndex = 0;
	for (size_t j = 0; j < (pointsVecSize - 3); j += 3, curveIndex++)
	{
		// Initalizing i to first vertex if it's not the first curve
		for (size_t i = (j == 0) ? 0 : 1; i <= segmentsPerCurve; i++) {
			// Calculating here is not optiomal but avoids float error missing one iteration
			float step = (float)i / (float)segmentsPerCurve;
			glm::vec3 pos = CalcBezierPoint(step, tempPoints[j].pos, tempPoints[j + 1].pos, tempPoints[j + 2].pos, tempPoints[j + 3].pos);
			curves[curveIndex].push_back(Point(pos, glm::vec3(1, 1, pos.z)));
		}
	}

	// Creating final curve
	int tempI = 0;
	for (size_t j = 0; j < pointsVecSize - 3; j += 3)
	{
		curvePoints.insert(curvePoints.end(), curves[tempI].begin(), curves[tempI].end());
		tempI++;
	}
}

glm::vec3 Renderer::CalcBezierPoint(float t, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {

	float u = 1 - t;
	float tt = t * t;
	float uu = u * u;
	float uuu = uu * u;
	float ttt = tt * t;

	glm::vec3 p = uuu * p0; //first term
	p += 3 * uu * t * p1; //second term
	p += 3 * u * tt * p2; //third term
	p += ttt * p3; //fourth term

	return p;
}

void Renderer::Render(Shader shader)
{
	shader.Use();

	glBindVertexArray(vao);
	glDrawArrays(GL_POINTS, 0, points.size());
	glBindVertexArray(0);

	if (points.size() >= 4) {
		glBindVertexArray(curveVao);
		glDrawArrays(GL_LINE_STRIP, 0, curvePoints.size());
		glBindVertexArray(0);

		glBindVertexArray(intVao);
		glDrawArrays(GL_LINE_STRIP, 0, intCurve.size());
		glBindVertexArray(0);

		glBindVertexArray(extVao);
		glDrawArrays(GL_LINE_STRIP, 0, extCurve.size());
		glBindVertexArray(0);

		meshShader.Use();

		for (Group* group : (*curveMesh->getGroups())) {
			if (group->faces.size() > 0) {
				glBindVertexArray(group->vao);
				glDrawArrays(GL_TRIANGLES, 0, group->faces.size() * 3);
				glBindVertexArray(0);
			}
		}
	}
}

void Renderer::ExtrudeBezierCurve(float spacing)
{
	intCurve.clear();
	extCurve.clear();
	for (size_t i = 0; i < curvePoints.size() - 1; i++)
	{
		glm::vec3 intPt = CalcExtrudePoint(curvePoints[i].pos, curvePoints[i + 1].pos, true, spacing);
		glm::vec3 extPt = CalcExtrudePoint(curvePoints[i].pos, curvePoints[i + 1].pos, false, -spacing);
		intCurve.push_back(Point(intPt, glm::vec3(1, 1, intPt.z)));
		extCurve.push_back(Point(extPt, glm::vec3(1, 1, extPt.z)));
	}
}

glm::vec3 Renderer::CalcExtrudePoint(glm::vec3 pa, glm::vec3 pb, bool inside, float size)
{
	glm::vec3 vecAB = glm::normalize(pb - pa);
	float angle = atan2(vecAB.y, vecAB.x) + glm::radians(90.0f);
	float c = glm::cos(angle);
	float s = glm::sin(angle);
	return glm::vec3(pa.x + glm::cos(angle) * size, pa.y + glm::sin(angle) * size, 0.0f);
}

float Renderer::AngleBetweenVector(glm::vec3 a, glm::vec3 b)
{
	return glm::acos(glm::dot(a, b));
}

void Renderer::Init()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glGenVertexArrays(1, &curveVao);
	glGenBuffers(1, &curveVbo);

	glGenVertexArrays(1, &intVao);
	glGenBuffers(1, &intVbo);

	glGenVertexArrays(1, &extVao);
	glGenBuffers(1, &extVbo);

	meshShader = Shader("Shaders/Core/mesh.vert", "Shaders/Core/mesh.frag");
	meshShader.Use();

	Bind();
}

float* Renderer::pointsToArr(std::vector<Point> ppoints) {
	float* arrRet = new float[ppoints.size() * 6];

	for (size_t i = 0; i < ppoints.size(); i++)
	{
		arrRet[i * 6 + 0] = ppoints[i].pos.x;
		arrRet[i * 6 + 1] = ppoints[i].pos.y;
		arrRet[i * 6 + 2] = ppoints[i].pos.z;
		arrRet[i * 6 + 3] = ppoints[i].color.r;
		arrRet[i * 6 + 4] = ppoints[i].color.g;
		arrRet[i * 6 + 5] = ppoints[i].color.b;
	}

	return arrRet;
}

float* Renderer::curvePointsToArr() {
	float* arrRet = new float[curvePoints.size() * 6];

	for (size_t i = 0; i < curvePoints.size(); i++)
	{
		arrRet[i * 6 + 0] = curvePoints[i].pos.x;
		arrRet[i * 6 + 1] = curvePoints[i].pos.y;
		arrRet[i * 6 + 2] = curvePoints[i].pos.z;
		arrRet[i * 6 + 3] = curvePoints[i].color.r;
		arrRet[i * 6 + 4] = curvePoints[i].color.g;
		arrRet[i * 6 + 5] = curvePoints[i].color.b;
	}

	return arrRet;
}

void Renderer::addPoint(glm::vec3 pos)
{
	points.push_back(Point(pos, glm::vec3(1, 1, 0)));
	CalcBezierCurve(points, 30);
	Bind();
}

Mesh* Renderer::GenerateMeshFromCurves()
{
	meshShader.Use();

	Mesh* newMesh = new Mesh();

	size_t curveSize = intCurve.size();

	// Adding internal curve
	for (size_t i = 0; i < curveSize; i++)
	{
		newMesh->setVertex(glm::vec3(intCurve[i].pos));
	}

	// Adding external curve
	for (size_t i = 0; i < curveSize; i++)
	{
		newMesh->setVertex(glm::vec3(extCurve[i].pos));
	}
	
	// Calculate normals
	newMesh->setNormals(glm::vec3(0.0f, 0.0f, 1.0f));

	newMesh->setMappings(glm::vec2(0.0f, 0.0f));
	newMesh->setMappings(glm::vec2(1.0f, 0.0f));
	newMesh->setMappings(glm::vec2(0.0f, 1.0f));
	newMesh->setMappings(glm::vec2(1.0f, 1.0f));

	// Calculate UV
	Material newMaterial = Material();
	newMaterial.name = "default";
	newMesh->materials["default"] = newMaterial;

	Group* newGroup = new Group();
	newGroup->name = "defaultMesh";
	newGroup->material = "default";

	// Indexes for binding start in 1(as in an OBJ)
	for (int interPos = 1; interPos <= curveSize; interPos++)
	{
		Face newFace;
		Face newFace2;

		int tempPos = interPos;
		if ((tempPos + 1) > curveSize) {
			tempPos = 0;
		}

		newFace.setVertex(interPos);
		newFace.setVertex(interPos + curveSize);
		newFace.setVertex(tempPos + 1 + curveSize);

		newFace2.setVertex(tempPos + 1 + curveSize);
		newFace2.setVertex(tempPos + 1);
		newFace2.setVertex(interPos);

		newFace.setNormals(1);
		newFace.setNormals(1);
		newFace.setNormals(1);

		newFace2.setNormals(1);
		newFace2.setNormals(1);
		newFace2.setNormals(1);

		newFace.setTexts(1);
		newFace.setTexts(3);
		newFace.setTexts(4);

		newFace2.setTexts(4);
		newFace2.setTexts(2);
		newFace2.setTexts(1);

		newGroup->faces.push_back(newFace);
		newGroup->faces.push_back(newFace2);
	}

	newMesh->setGroups(newGroup);

	newMesh->binding();

	return newMesh;
}

void Renderer::ExportCurve(std::string path)
{
	std::ofstream file;
	file.exceptions(std::ofstream::badbit);

	try {
		file.open(path, std::ios::out);

		if (!file.is_open()) {
			throw std::runtime_error("ERROR::GIZMOS::PATH ERROR");
		}

		file << "# Curve File \n";

		size_t ptsSize = points.size();
		for (size_t i = 0; i < ptsSize; i++)
		{
			file << i << " " << std::fixed << std::setprecision(3) << points[i].pos.x << " " << points[i].pos.z << " " << points[i].pos.y << '\n';
		}

	}
	catch (const std::ifstream::failure& e) {

		if (!file.eof()) {
			std::cout << "ERROR::GIZMOS::FILE NOT SUCCESUFULLY WRITTEN" << std::endl;
			file.close();
		}

	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		file.close();
	}

	file.close();
}

void Renderer::ImportCurve(std::string path)
{
	points.clear();
	std::ifstream file;
	file.exceptions(std::ifstream::badbit);

	try {
		file.open(path, std::ios::out);

		if (!file.is_open()) {
			throw std::runtime_error("ERROR::GIZMOS::PATH ERROR");
		}

		std::string line, temp;
		std::stringstream sstream;

		while (!file.eof()) {

			// Clearing for safety
			sstream.str(std::string());
			sstream.clear();

			line = temp = "";

			std::getline(file, line);

			sstream << line;
			sstream >> temp;

			switch (temp[0]) {
			case '#':
				break;
			default:
			{
				float x, y, z;
				sstream >> x >> y >> z;
				points.push_back(Point(glm::vec3(x, y, z), glm::vec3(1,1,z)));
			}
			break;
			}

		}

	}
	catch (const std::ifstream::failure& e) {

		if (!file.eof()) {
			std::cout << "ERROR::GIZMOS::FILE NOT SUCCESUFULLY WRITTEN" << std::endl;
			file.close();
		}

	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		file.close();
	}

	file.close();
}


int Renderer::findClosestPoint(glm::vec3 pos)
{
	int maxIdx = INT32_MIN;
	float minDst = INT32_MAX;

	for (size_t i = 0; i < points.size(); i++)
	{
		float dst = glm::distance(pos, points[i].pos);
		if (dst < minDst) {
			minDst = dst;
			maxIdx = i;
		}
	}

	return maxIdx;
}

void Renderer::changePointHeight(int idx, float val)
{
	points[idx].color.b += val;
	if (points[idx].color.b > 1.0f) {
		points[idx].color.b = 1.0f;
	}

	if (points[idx].color.b < 0.0f) {
		points[idx].color.b = 0.0f;
	}

	Bind();
}

void Renderer::Bind()
{
	glBindVertexArray(vao);

	float* arr = pointsToArr(points);

	//Define o VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Point)*points.size(), &arr[0], GL_STATIC_DRAW);

	//Define o VAO dos vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Se tiver textura, define o VAO da textura

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	//Curve
	if (points.size() >= 4) {


		glBindVertexArray(curveVao);

		float* carr = curvePointsToArr();

		//Define o VBO
		glBindBuffer(GL_ARRAY_BUFFER, curveVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Point)*curvePoints.size(), &carr[0], GL_STATIC_DRAW);

		//Define o VAO dos vertices
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
		glEnableVertexAttribArray(0);

		//Se tiver textura, define o VAO da textura

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);

		ExtrudeBezierCurve(0.1f);
		//INTERNAL
		float* intarr = pointsToArr(intCurve);

		glBindVertexArray(intVao);
		glBindBuffer(GL_ARRAY_BUFFER, intVbo);

		glBufferData(GL_ARRAY_BUFFER, sizeof(Point)*intCurve.size(), &intarr[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);

		//EXTERNAL
		float* extarr = pointsToArr(extCurve);

		glBindVertexArray(extVao);
		glBindBuffer(GL_ARRAY_BUFFER, extVbo);

		glBufferData(GL_ARRAY_BUFFER, sizeof(Point)*extCurve.size(), &extarr[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);

		curveMesh = GenerateMeshFromCurves();
	}
}
