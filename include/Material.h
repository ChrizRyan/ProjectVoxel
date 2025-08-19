#pragma once

#include "Shader.h"
#include <vector>


struct Texture {
    unsigned int id;
    std::string type;
};

class Material
{
    public:
        Material(std::unique_ptr<Shader> shader);
        ~Material();

        void apply();

        void addTexture(Texture texture) {
            textures.push_back(texture);
        }

        void setBuffer(const std::vector<int>& data);
        void SetVector(const std::string& name, const glm::vec3& value);
        void SetVector(const std::string& name, const glm::vec4& value);

        Shader* getShader() const { return shader.get(); }

    private:

        void applyTextures();

        std::unique_ptr<Shader> shader;
        std::vector<Texture> textures;
};