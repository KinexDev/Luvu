#pragma once
#include "glad/glad.h"
#include "iostream"
#include "cglm/cglm.h"

class Shader
{
public:
	Shader(const char* vertexShaderSource, const char* fragmentShaderSource);
	~Shader();
	void Use();
	void SetUniform(const char* name, const mat4& mat);
	void SetUniform(const char* name, const vec4& mat);
	void SetUniform(const char* name, const int& mat);
private:
	unsigned int shaderProgram;
};