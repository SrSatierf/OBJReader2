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

	glCullFace(GL_BACK);

	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);

	return EXIT_SUCCESS;
}

int System::SystemSetup()
{
	//Define o path dos shaders a serem usados
	coreShader = Shader("Shaders/Core/core.vert", "Shaders/Core/core.frag"); 
	coreShader.Use();

	return EXIT_SUCCESS;
}

void System::Run()
{

	coreShader.Use();
	
	//Mesh que recebe o obj e o mtl vindos do OBJReader e do MTLReader
	//Mesh* mesh = OBJReader::read("models/mesa01.obj");
	//Mesh* mesh = OBJReader::read("models/cenaPaintball.obj");
	Mesh* mesh = OBJReader::read("models/curve.obj");
	Mesh* car = OBJReader::read("models/trout.obj");

	//Faz o binding do VAO e do VBO
	mesh->loadTextures(&coreShader, "texture1", "models/");
	mesh->binding();

	//Carrega a textura no shader
	car->loadTextures(&coreShader, "texture1", "models/");
	car->binding();

	Renderer::ImportCurve("models/curve.crv");

	//Define a camera
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 cameraDir = glm::vec3(0.0f, 0.0f, 1.0f);
	float cameraAngle = 0.0f;

	glm::mat4 view;
	view = glm::lookAt(cameraPos,
		cameraPos + cameraDir,
		up);

	glm::mat4 proj = glm::perspective(glm::radians(60.0f), (float)((float)screenWidth / (float)screenHeight), 0.01f, 100.0f);

	//define translacao do objeto
	glm::mat4 trans = glm::mat4(1.0f);

	glm::mat4 troutTrans = glm::mat4(1.0f);
	glm::mat4 troutScale = glm::mat4(1.0f);
	glm::mat4 troutRot = glm::mat4(1.0f);
	glm::mat4 finalTruta = glm::mat4(1.0f);
	troutScale = glm::scale(troutScale, glm::vec3(0.025f, 0.025f, 0.025f));
	float inc = 0.001f;
	float carT = 0.0f;
	float carT2 = inc*2;
	float carAngle = 0.0f;
	float rotBias = 90.0f;


	GLuint modelLoc = glGetUniformLocation(coreShader.program, "model");
	GLuint viewLoc = glGetUniformLocation(coreShader.program, "view");
	GLuint projLoc = glGetUniformLocation(coreShader.program, "projection");
	GLuint camLoc = glGetUniformLocation(coreShader.program, "viewPos");


	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();
		 //Ajusta a view conforme as mudancas necessarias de acordo com as teclas pressionadas (pra simular a camera andando)
#pragma region Input Handling

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			cameraPos.z += 0.01f;
			view = glm::lookAt(cameraPos,
				cameraPos + cameraDir,
				up);
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			cameraPos.z -= 0.01f;
			view = glm::lookAt(cameraPos,
				cameraPos + cameraDir,
				up);
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			cameraPos.x -= 0.01f;
			view = glm::lookAt(cameraPos,
				cameraPos + cameraDir,
				up);
		}

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			cameraPos.x += 0.01f;
			view = glm::lookAt(cameraPos,
				cameraPos + cameraDir,
				up);
		}

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			cameraPos.y += 0.01f;
			view = glm::lookAt(cameraPos,
				cameraPos + cameraDir,
				up);
		}

		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			cameraPos.y -= 0.01f;
			view = glm::lookAt(cameraPos,
				cameraPos + cameraDir,
				up);
		}

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			cameraAngle -= 0.009f;
			cameraDir.x = cos(cameraAngle);
			cameraDir.z = sin(cameraAngle);
			view = glm::lookAt(cameraPos,
				cameraPos + cameraDir,
				up);
		}

		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
			cameraAngle += 0.009f;
			cameraDir.x = cos(cameraAngle);
			cameraDir.z = sin(cameraAngle);
			view = glm::lookAt(cameraPos,
				cameraPos + cameraDir,
				up);
		}


#pragma endregion
		//Limpa a tela pra desenhar o objeto na nova posicao
		glClearColor(0.42f, 0.62f, 0.85f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		coreShader.Use();

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &trans[0][0]);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, &proj[0][0]);
		glUniform3fv(camLoc, 1, &cameraPos[0]);

		//Metodo que desenha o objeto na tela
		Renderer::Render(coreShader, *mesh);

		carT += inc;
		if (carT > 1.0f) {
			carT -= 1.0f;
		}

		carT2 += inc;
		if (carT2 > 1.0f) {
			carT2 -= 1.0f;
		}

		glm::vec3 curPos = Renderer::GetPositionFromCurve(carT);
		glm::vec3 nextPos = Renderer::GetPositionFromCurve(carT2);
		glm::vec3 temp = nextPos - curPos;
		carAngle = atan2f(temp.x, temp.z) + glm::radians(rotBias);

		troutTrans = glm::translate(glm::mat4(1), curPos);
		troutRot = glm::rotate(glm::mat4(1), carAngle, glm::vec3(0.0f, 1.0f, 0.0f));

		finalTruta = troutTrans * troutRot * troutScale;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &finalTruta[0][0]);
		Renderer::Render(coreShader, *car);


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
