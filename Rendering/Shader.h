#pragma once
#include "glad/glad.h"
#include "iostream"
#include "cglm/cglm.h"

class Shader
{
public:
	Shader();
	~Shader();
	void Use();
	void SetUniform(const char* name, const mat4& mat);
private:
	unsigned int shaderProgram;
};