#include "Game.h"


Game::Game()
{
	// setting up the default shader
    const char* vertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "uniform mat4 model; \n"
        "uniform mat4 projection; \n"
        "void main()\n"
        "{\n"
        "   gl_Position = projection * model * vec4(aPos, 1.0);\n"
        "}\0";

    const char* fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";

	std::ifstream file("res/main.luau");

	std::stringstream buffer;

	buffer << file.rdbuf();
	std::string str = buffer.str();

	vm = new LuauVM();
	Vec2::Register(*vm);

	vm->DoString(str);

	if (!glfwInit())
		throw std::exception("Could not initalise GLFW");

	window = glfwCreateWindow(800, 600, "Game", nullptr, nullptr);

	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		throw std::exception("Could not initalise Window!");
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		throw std::exception("Could not initalise GLAD!");
	}

	defaultShader = new Shader(vertexShaderSource, fragmentShaderSource);

	glfwSetFramebufferSizeCallback(window, &FrameBufferResize);
	
	// we only use 1 mesh, a quad.
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
	glBindVertexArray(0);
}

void Game::Run()
{
	while (!glfwWindowShouldClose(window))
	{
		mat4 p, m;
		//ProcessInput(window);
		glfwGetWindowSize(window, &screenWidth, &screenHeight);
		float ratio = (float)screenWidth / (float)screenHeight;
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glm_mat4_identity(p);
		glm_ortho(-ratio, ratio, -1, 1, -1, 1, p);

		glm_mat4_identity(m);
		glm_scale(m, vec3{ 1.0f, 1.0f, 1.0f });

		defaultShader->Use();
		defaultShader->SetUniform("model", m);
		defaultShader->SetUniform("projection", p);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Game::FrameBufferResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

Game::~Game()
{
	delete vm;
	delete defaultShader;
	glfwTerminate();
}