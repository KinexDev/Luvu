#pragma once

#include "stb_image.h"
#include "glad/glad.h"
#include "iostream"

class Texture {
public:
    const char* filePath;
    Texture(const char* file);
    void Bind();
    ~Texture();
    unsigned int textureId;
};
