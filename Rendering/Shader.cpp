#include "Shader.h"

Shader::Shader(const char* vertexShaderSource, const char* fragmentShaderSource)
{
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Use()
{
    glUseProgram(shaderProgram);
}

void Shader::SetUniform(const char* name, const mat4& mat)
{
    int matLoc = glGetUniformLocation(shaderProgram, name);
    glUniformMatrix4fv(matLoc, 1, GL_FALSE, (const GLfloat*)mat);
}

void Shader::SetUniform(const char* name, const vec4& mat)
{
    int matLoc = glGetUniformLocation(shaderProgram, name);
    glUniform4f(matLoc, mat[0], mat[1], mat[2], mat[3]);
}

void Shader::SetUniform(const char* name, const int& mat)
{
    int matLoc = glGetUniformLocation(shaderProgram, name);
    glUniform1i(matLoc, mat);
}

Shader::~Shader()
{
    glDeleteProgram(shaderProgram);
}