#include "System.h"
#include "Renderer.h"

System::System()
{
}


System::~System()
{
}

int System::GLFWInit()
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(WIDTH, HEIGHT, "OBJ Reader", nullptr, nullptr);

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	if (window == nullptr) {
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cout << "Failed no init GLEW." << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, screenWidth, screenHeight);

	return EXIT_SUCCESS;

}

int System::OpenGLSetup()
{

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_CULL_FACE);

	/*glCullFace(GL_BACK);

	glFrontFace(GL_CCW);*/

	glEnable(GL_DEPTH_TEST);

	return EXIT_SUCCESS;
}

int System::SystemSetup()
{
	//Define o path dos shaders a serem usados
	coreShader = Shader("Shaders/Core/core.vert", "Shaders/Core/core.frag");
	coreShader.Use();

	Renderer::Init();

	return EXIT_SUCCESS;
}

bool pressed = false;
bool enterPressed = false;
double xpos, ypos;
int closestIdx = 0;
void System::Run()
{

	coreShader.Use();

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

#pragma region Mouse Clicks

		glfwGetCursorPos(window, &xpos, &ypos);
		xpos = (2 * xpos / WIDTH) - 1;
		ypos = ((2 * ypos / HEIGHT) - 1)*-1;
		closestIdx = Renderer::findClosestPoint(glm::vec3(xpos, ypos, 0));

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			if (!pressed) {
				pressed = true;
				//std::cout << xpos << ";" << ypos << std::endl;
				if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) != GLFW_PRESS) {
					Renderer::addPoint(glm::vec3(xpos, ypos, 0));
				}
				else {
					Renderer::addPoint(glm::vec3(Renderer::points[0].pos.x, Renderer::points[0].pos.y, 0));
				}
			}
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
			pressed = false;
		}

		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			Renderer::changePointHeight(closestIdx, +0.01f);
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			Renderer::changePointHeight(closestIdx, -0.01f);
		}

		if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
			if (!enterPressed) {
				enterPressed = true;
				OBJReader::write("curve.obj", Renderer::curveMesh);
				Renderer::ExportCurve("curve.crv");
			}
		}

		if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE) {
			enterPressed = false;
		}

#pragma endregion


		//Limpa a tela pra desenhar o objeto na nova posicao
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		coreShader.Use();

		//Metodo que desenha a linha na tela
		Renderer::Render(coreShader);

		glfwSwapBuffers(window);
	}


}
void System::read() {

}

void System::drawScene()
{
}



void System::Finish()
{
	coreShader.Delete();

	glfwTerminate();
}
