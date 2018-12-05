#include "Renderer.h"
#include <string>
#include <iomanip>


std::vector<Point> Renderer::points;
Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

void Renderer::Render(Shader shader, Mesh mesh)
{
	shader.Use();
	glm::vec3 lightColorVec= { 1,1,1 };
	glm::vec3 lightPosVec = { 1,1,1 };
	GLuint kdLoc = glGetUniformLocation(shader.program, "kd");
	GLuint ksLoc = glGetUniformLocation(shader.program, "ks");
	GLuint kaLoc = glGetUniformLocation(shader.program, "ka");
	GLuint nsLoc = glGetUniformLocation(shader.program, "ns");
	GLuint niLoc = glGetUniformLocation(shader.program, "ni");
	GLuint texLoc = glGetUniformLocation(shader.program, "texture1");
	GLuint lightColorLoc = glGetUniformLocation(shader.program, "lightColor");
	GLuint lightPosLoc = glGetUniformLocation(shader.program, "lightPos");


	for (Group* group : *mesh.getGroups()) {
		if (group->faces.size() > 0) {
			Material material = mesh.materials[group->material];
			std::string materialName = material.map_kd;
			if (materialName != "") {
				shader.UseTexture(materialName);

				/*if (materialName == "muro02.jpg") {
					glActiveTexture(GL_TEXTURE0 + 3);
					glBindTexture(GL_TEXTURE_2D, 4);
				}*/

			}
			glUniform3fv(kdLoc, 1, &material.kd[0]);
			glUniform3fv(ksLoc, 1, &material.ks[0]);
			glUniform3fv(kaLoc, 1, &material.ka[0]);
			glUniform1fv(nsLoc, 1, &material.ns);
			glUniform1fv(niLoc, 1, &material.ni);
			glUniform3fv(lightColorLoc, 1, &lightColorVec[0]);
			glUniform3fv(lightPosLoc, 1, &lightPosVec[0]);

			glBindVertexArray(group->vao);
			glDrawArrays(GL_TRIANGLES, 0, group->faces.size() * 3);
			glBindVertexArray(0);
		}
	}
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
				points.push_back(Point(glm::vec3(x, y, z), glm::vec3(1, 1, z)));
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

glm::vec3 Renderer::GetPositionFromCurve(float t)
{
	int numberOfCurves = 0;

	size_t pointsVecSize = points.size();
	for (size_t j = 0; j < (pointsVecSize - 3); j += 3)
	{
		numberOfCurves++;
	}

	int totalSegments = numberOfCurves * 30;
	float segmentStep = 1.0f / ((float)totalSegments);

	float stepTotal = t * numberOfCurves;
	size_t curveNumber = ceilf(stepTotal);
	if ((int)(t * 10) == 0) {
		curveNumber = 1;
	}
	// What's the min and max value for mapping?
	float temp = MapValues(stepTotal, curveNumber - 1, curveNumber, 0.0f, 1.0f);

	size_t finalCurveNumber = (curveNumber * 3) - 3;
	return CalcBezierPoint(temp, points[finalCurveNumber].pos, points[finalCurveNumber + 1].pos, points[finalCurveNumber + 2].pos,points[finalCurveNumber + 3].pos);
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

float Renderer::MapValues(float input, float input_start, float input_end, float output_start, float output_end) {
	return (output_start + ((output_end - output_start) / (input_end - input_start)) * (input - input_start));
}