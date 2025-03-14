#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include "LuauVM/LuauVM.h"
#include "LuauVM/UserdataTest.h"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "Rendering/Shader.h"
#include "LuauVM/Vec2.h"

class Game {
public:
	Game();
	~Game();
	void Run();
private:
	static void FrameBufferResize(GLFWwindow* window, int width, int height);
	GLFWwindow* window;
	Shader* defaultShader;
	LuauVM* vm;
	int screenWidth;
	int screenHeight;
	// we use only 1 of these, since we are only gonna be messing around with quads.
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
};