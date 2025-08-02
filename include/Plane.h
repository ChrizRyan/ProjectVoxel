#pragma once

#include "Mesh.h"

class Plane: public Mesh 
{

    public:
        Plane() 
        {
            this->vertices = {
                {glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
                {glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
                {glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec2(0.0f, 1.0f)},
                {glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)}
            };
            
            this->indices = {
                0, 1, 2,
                2, 1, 3
            };
            // this->textures = textures;

            init();
        };  

};