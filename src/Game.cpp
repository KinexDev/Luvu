#include "../include/Game.h"

Game& Game::Instance()
{
	static Game instance;
	return instance;
}

void Game::Close()
{
	glfwSetWindowShouldClose(window, true);
}

Game::Game()
{
	if (!glfwInit())
		throw std::runtime_error("Could not initalise GLFW");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, 0);

	window = glfwCreateWindow(600, 600, "Untitled", nullptr, nullptr);

	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		throw std::runtime_error("Could not initalise Window!");
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		throw std::runtime_error("Could not initalise GLAD!");
	}

	glViewport(0, 0, 600, 600);

	glfwSetFramebufferSizeCallback(window, &FrameBufferResize);

	const char* vertexShaderSource =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout(location = 1) in vec2 aTexCoord;\n"
		"uniform mat4 model; \n"
		"uniform mat4 projection; \n"
		"out vec2 texCoord; \n"
		"void main()\n"
		"{\n"
		"   gl_Position = projection * model * vec4(aPos, 1.0);\n"
		"	texCoord = aTexCoord; \n"
		"}\0";

	const char* fragmentShaderSource =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec2 texCoord;\n"
		"uniform bool hasTexture;\n"
		"uniform sampler2D tex;\n"
		"uniform vec4 color;\n"
		"void main()\n"
		"{\n"
		"	if (hasTexture)\n"
		"	{\n"
		"		FragColor = texture(tex, texCoord) * color;"
		"	}\n"
		"	else \n"
		"	{\n"
		"		FragColor = color;\n"
		"	}\n"
		"}\0";

	defaultShader = new Shader(vertexShaderSource, fragmentShaderSource);

	// we only use 1 mesh, a quad.
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,   1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,   1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindVertexArray(0);

	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	defaultShader->Use();
	defaultShader->SetUniform("tex", 0);
}

void Game::Run()
{
	std::ifstream file("res/main.luau");

	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string str = buffer.str();

	vm = new LuauVM();
	vm->PushGlobalFunction("quit", [](lua_State* L) -> int {
		Game::Instance().Close();
		return 0;
		});
	LuaTexture::Register(vm->L);
	LuaVec2::Register(vm->L);
	LuaColor::Register(vm->L);
	Graphics::Register(vm->L);
	LuaInput::Register(vm->L);

	vm->DoString(str);

	lastFrame = (float)glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = (float)glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		if (vm->CheckFunction("_update"))
		{
			lua_getglobal(vm->L, "_update");
			lua_pushnumber(vm->L, deltaTime);
			vm->Execute(1);
		}

		glfwGetWindowSize(window, &screenWidth, &screenHeight);
		float ratio = (float)screenWidth / (float)screenHeight;
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		//glm_ortho(-ratio, ratio, -1, 1, -1, 1, p);
		//glm_ortho(screenWidth, screenHeight, -1, 1, -1, 1, p);
		glm_ortho(0.0f, (float)screenWidth, (float)screenHeight, 0.0f, -1.0f, 1.0f, p);
		if (vm->CheckFunction("_draw"))
		{
			lua_getglobal(vm->L, "_draw");
			lua_pushnumber(vm->L, deltaTime);
			vm->Execute(1);
		}

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
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();
}