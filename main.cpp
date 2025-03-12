#include <iostream>
#include <fstream>
#include <sstream>
#include "LuauVM/LuauVM.h"
#include "LuauVM/UserdataTest.h"
#include "GLFW/glfw3.h"

int main()
{
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(800, 600, "ahhh", nullptr, nullptr);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}

	glfwTerminate();

	std::ifstream file("res/main.luau");

	std::stringstream buffer;

	buffer << file.rdbuf();
	std::string str = buffer.str();

	LuauVM vm;

	UserdataTest::Register(vm);

	vm.DoString(str);
}
