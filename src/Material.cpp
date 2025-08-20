#include "Material.h"

Material::Material(std::unique_ptr<Shader> shader) 
    : shader(std::move(shader)) 
{

}   

Material::~Material() 
{

}

void Material::apply() 
{
    applyTextures();

    if (shader) {
        shader->use();
    }
}

void Material::setBuffer(const std::vector<int>& data)  
{
    unsigned int uniformBlockIndex = glGetUniformBlockIndex(shader->getId(), "VoxelBuffer");

    GLuint ssbo;
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, data.size() * sizeof(int), data.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
}

void Material::applyTextures() 
{
    // for(unsigned int i = 0; i < textures.size(); i++)
    // {
    //     glActiveTexture(GL_TEXTURE0 + i);
    //     glBindTexture(GL_TEXTURE_2D, textures[i].id);
    // }
    // glActiveTexture(GL_TEXTURE0);
}