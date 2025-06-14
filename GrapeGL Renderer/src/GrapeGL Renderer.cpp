#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Model.h"
#include "Shader.h"
#include "Common.h"

#include <imGUI/imgui.h>
#include <imGUI/imgui_impl_glfw.h>
#include <imGUI/imgui_impl_opengl3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Epic!", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);


	Shader::LoadAll();
	Log(Shader::GetLoadedShader(0)->GetName());
	Model model = Model("box.blend");



	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;


	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	bool wireframe = false;

	float fov = 45.0f;
	float scale = 0.01f;
	glm::mat4 projectionMat = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
	glm::mat4 orthoMat = glm::ortho(-(int)SCR_WIDTH * scale, SCR_WIDTH * scale, -(int)SCR_HEIGHT * scale, SCR_HEIGHT * scale);

	bool debugMenuOpen = true;

	glm::vec3 camPosition = glm::vec3(0.0f);
	glm::vec3 camRotation = glm::vec3(0.0f);
	float speed = 1.0f;
	float previousTime = glfwGetTime();
	double preX, preY = 0;
	glfwGetCursorPos(window, &preX, &preY);

	while (!glfwWindowShouldClose(window))
	{
		float deltaTime = glfwGetTime() - previousTime;
		previousTime = glfwGetTime();

		glm::mat4 modelMat = glm::mat4(1.0f);

		glm::mat4 viewMat = glm::mat4(1.0f);

		if (wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT, GL_FILL);

		processInput(window);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			ImGui::Begin("Debug", &debugMenuOpen);
			ImGui::Checkbox("Wireframe", &wireframe);
			ImGui::End();
		}

		double newX, newY = 0;
		glfwGetCursorPos(window, &newX, &newY);
		double deltaX, deltaY = 0;
		deltaX = newX - preX;
		deltaY = newY - preY;

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
		{
			glfwSetCursorPos(window, SCR_WIDTH / 2, SCR_HEIGHT / 2);
			camRotation.y -= deltaX * 0.01f;
			camRotation.x -= deltaY * 0.01f;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwGetCursorPos(window, &preX, &preY);
		glm::mat4 camMat = glm::rotate(glm::mat4(1.0f), camRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		camMat = glm::rotate(camMat, camRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));

		glm::quat quat = glm::quat_cast(camMat);
		glm::vec3 up = quat * glm::vec3(0, 1, 0);
		glm::vec3 front = quat * glm::vec3(0, 0, -1);
		glm::vec3 right = quat * glm::vec3(1, 0, 0);

		if (glfwGetKey(window, GLFW_KEY_W))
			camPosition += front * speed * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_S))
			camPosition -= front * speed * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_A))
			camPosition -= right * speed * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_D))
			camPosition += right * speed * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_SPACE))
			camPosition += up * speed * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_C))
			camPosition -= up * speed * deltaTime;


		glm::mat4 viewmat = glm::lookAtRH(camPosition, camPosition + front, up);


		glm::mat4 mvp = projectionMat * modelMat * viewmat;

		Shader* shader = Shader::GetLoadedShader(1);
		shader->setMat4("mvp", mvp);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		Shader::GetLoadedShader(1)->Activate();
		model.Bind();
		glDrawElements(GL_TRIANGLES, model.Meshes[0].Triangles.size(), GL_UNSIGNED_INT, 0);



		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();


	glfwTerminate();
	return 0;
}


void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
	glViewport(0, 0, width, height);
}