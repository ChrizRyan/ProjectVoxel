#pragma once

#include <vector>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh
{
    public:
        Mesh() = default;
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
        ~Mesh();

        void draw();

    protected:
        void init();

        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;
        

    private:
        unsigned int VAO, VBO, EBO;


};