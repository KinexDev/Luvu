#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include "LuauVM.h"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "Shader.h"
#include "LuaVec2.h"
#include "LuaTexture.h"
#include "Graphics.h"
#include "LuaColor.h"
#include "LuaInput.h"

class Game {
public:
	Game(const Game&) = delete;
	void operator=(const Game&) = delete;
	static Game& Instance();
	void Close();
	Game();
	~Game();
	void Run();
	GLFWwindow* window;
	Shader* defaultShader;
	int screenWidth, screenHeight;
	unsigned int VBO, VAO, EBO;
	mat4 p;
private:
	static void FrameBufferResize(GLFWwindow* window, int width, int height);
	LuauVM* vm;
	float lastFrame;
	// we use only 1 of these, since we are only gonna be messing around with quads.
};