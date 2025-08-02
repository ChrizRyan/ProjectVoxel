#include "../include/Shader.h"

#include "../include/FileLoader.h"

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string& vertexFilePath, const std::string&  fragFilePath) 
{
    init(vertexFilePath, fragFilePath);
}   

Shader::~Shader() {
    // Destructor implementation
    glDeleteProgram(mShaderProgramId);
}

void Shader::use() 
{
    glUseProgram(mShaderProgramId);
}

void Shader::updateUniform3fv(const std::string& name, const glm::vec3& value)
{
    int location = glGetUniformLocation(mShaderProgramId, name.c_str());
    if (location != -1) {
        glUniform3fv(location, 1, glm::value_ptr(value));
    } else {
        std::cerr << "Warning: Uniform '" << name << "' not found in shader program." << std::endl;
    }
}

void Shader::updateUniform4f(const std::string& name, float redValue, float greenValue, float blueValue, float alphaValue)
{
    int location = glGetUniformLocation(mShaderProgramId, name.c_str());
    if (location != -1) {
        glUniform4f(location, redValue, greenValue, blueValue, alphaValue);
    } else {
        std::cerr << "Warning: Uniform '" << name << "' not found in shader program." << std::endl;
    }
}

void Shader::updateUniform4fv(const std::string& name, const glm::vec4& value)
{
    int location = glGetUniformLocation(mShaderProgramId, name.c_str());
    if (location != -1) {
        glUniform4fv(location, 1, glm::value_ptr(value));
    } else {
        std::cerr << "Warning: Uniform '" << name << "' not found in shader program." << std::endl;
    }
}

void Shader::init(const std::string& vertexFilePath, const std::string& fragFilePath) 
{
    unsigned int vertexShader = createShader(GL_VERTEX_SHADER, vertexFilePath);
    unsigned int fragmentShader = createShader(GL_FRAGMENT_SHADER, fragFilePath);

    mShaderProgramId = glCreateProgram();
    glAttachShader(mShaderProgramId, vertexShader);
    glAttachShader(mShaderProgramId, fragmentShader);
    glLinkProgram(mShaderProgramId);

    // Delete shaders after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

unsigned int Shader::createShader(GLenum shaderType, const std::string& filepath) {
    // Initialize shader program
    const std::string shaderSource = loadSource(filepath);
    const char* source = shaderSource.c_str();

    unsigned int shader = glCreateShader(shaderType);

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::filepath::COMPILATION_FAILED\n" << infoLog << std::endl << shaderSource << std::endl;
    }

    return shader;
}

const std::string Shader::loadSource(const std::string& filepath)  
{
    std::unique_ptr<FileLoader> fileLoader = std::make_unique<FileLoader>();

    std::string filecontets = fileLoader->loadFile(filepath);
    return filecontets;
}