#pragma once

#include "Mesh.h"
#include "Material.h"

class MeshRenderer
{
    public:
        MeshRenderer(std::unique_ptr<Mesh> mesh, std::unique_ptr<Material> material);
        ~MeshRenderer();

        void render();

        Material* getMaterial() const { return material.get(); }
        Mesh* getMesh() const { return mesh.get(); }

    private:
        std::unique_ptr<Mesh> mesh;
        std::unique_ptr<Material> material;
};