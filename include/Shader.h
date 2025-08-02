#pragma once

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

class Shader
{
    public:
        Shader(const std::string&  vertexFilePath, const std::string& fragFilePath);
        ~Shader();

        void use();
        void updateUniform3fv(const std::string& name, const glm::vec3& value);
        void updateUniform4fv(const std::string& name, const glm::vec4& value);

        void updateUniform4f(const std::string& name, float redValue, float greenValue, float blueValue, float alphaValue);

    private:
        void init(const std::string& vertexFilePath, const std::string& fragFilePath);
        unsigned int createShader(GLenum shaderType, const std::string& filepath);
        const std::string loadSource(const std::string& filepath);

        unsigned int mShaderProgramId;
};