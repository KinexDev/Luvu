#include <iostream>
#include <fstream>
#include <sstream>
#include "LuauVM/LuauVM.h"
#include "LuauVM/UserdataTest.h"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "Rendering/Shader.h"

float vertices[] = {
	 0.5f,  0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f
};

unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3
};

int width;
int height;

unsigned int VBO;
unsigned int VAO;
unsigned int EBO;

void FrameBufferResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	std::ifstream file("res/main.luau");

	std::stringstream buffer;

	buffer << file.rdbuf();
	std::string str = buffer.str();

	LuauVM vm;

	UserdataTest::Register(vm);

	vm.DoString(str);
	return 1;

	if (!glfwInit())
		return 1;

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);

	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, FrameBufferResize);

	Shader shader;

	glViewport(0, 0, 800, 600);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	while (!glfwWindowShouldClose(window))
	{
		mat4 p, m;
		ProcessInput(window);
		glfwGetWindowSize(window, &width, &height);
		float ratio = (float)width / (float)height;
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glm_mat4_identity(p);
		glm_ortho(-ratio, ratio, -1, 1, -1, 1, p);

		glm_mat4_identity(m);
		glm_scale(m, vec3 { 1.0f, 1.0f, 1.0f });

		shader.Use();
		shader.SetUniform("model", m);
		shader.SetUniform("projection", p);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}
