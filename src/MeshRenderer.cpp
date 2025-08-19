#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(std::unique_ptr<Mesh> mesh, std::unique_ptr<Material> material) 
    : mesh(std::move(mesh)), material(std::move(material))
{

}       

MeshRenderer::~MeshRenderer() 
{

}

void MeshRenderer::render() 
{
    if (material && mesh) {
        material->apply();
        mesh->draw();
    }
}