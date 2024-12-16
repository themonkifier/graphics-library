#ifndef RESOURCE_MANAGER_HH
#define RESOURCE_MANAGER_HH

#include <string>

#include <glad/glad.h>

#include "texture.h"
#include "shader.h"

namespace ResourceManager
{
    /* resource storage */
    extern std::unordered_map<std::string, Texture> Textures; // available textures
    extern std::unordered_map<std::string, Shader>  Shaders;  // available shaders

    /** @brief loads (and generates) a shader program from file loading vertex,
      *  fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
      */
    Shader& LoadShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath, std::string name);
    Shader& GetShader(std::string name);
    Texture& LoadTexture(const char* texturePath, bool alpha, std::string name);
    Texture& GetTexture(std::string name);
    void Clear();

    Texture _loadTextureFromFile(const char* texturePath, bool alpha);
}


#endif
