#include "Grid.h"
#include <iostream>
Grid::Grid(const Vector3Int& dims) 
    : dimensions(dims), voxels(dims.x * dims.y * dims.z, 0)
{ 
    initGrid(dims); 
}

void Grid::initGrid(const Vector3Int& dims) 
    
{
   this->generateVoxels();
}

std::vector<int> Grid::getData() 
{ 
    return voxels; 
}

int Grid::getVoxel(Vector3Int location)
{
    if (location.x < 0 || location.x >= dimensions.x ||
        location.y < 0 || location.y >= dimensions.y ||
        location.z < 0 || location.z >= dimensions.z) {
        throw std::out_of_range("Voxel location out of bounds");
    }
    return voxels[location.x + dimensions.x * (location.y + dimensions.y * location.z)];
}
        
void Grid::setVoxel(Vector3Int location, int value)
{
    if (location.x < 0 || location.x >= dimensions.x ||
        location.y < 0 || location.y >= dimensions.y ||
        location.z < 0 || location.z >= dimensions.z) {
        throw std::out_of_range("Voxel location out of bounds");
    }
    voxels[location.x + dimensions.x * (location.y + dimensions.y * location.z)] = value;
}

// Example voxel generation (random fill)
void Grid::generateVoxels() {
    Color colors[] = { Colors::Red, Colors::Blue, Colors::Green };

    std::cout << voxels.size() << std::endl;

    for (int y = 0; y < dimensions.y; ++y)
        for (int x = 0; x < dimensions.x; ++x)
            for (int z = 0; z < dimensions.z; ++z) {
                bool solid = (rand() / (float)RAND_MAX) > 0.3f;
                bool bottomSolid = y - 1 < 0 || getVoxel(Vector3Int(x, y-1, z)) != 0;
                Color c = colors[rand() % 3];

                int voxel = (solid && bottomSolid) ? colorToInt(c) : 0;
                setVoxel(Vector3Int(x,y,z), voxel);
            }
}

// Color packing/unpacking
int Grid::colorToInt(const Color& color) 
{
    uint8_t A = static_cast<uint8_t>(std::floor(color.a * 255));
    uint8_t B = static_cast<uint8_t>(std::floor(color.b * 255));
    uint8_t G = static_cast<uint8_t>(std::floor(color.g * 255));
    uint8_t R = static_cast<uint8_t>(std::floor(color.r * 255));
    int value = B;
    value |= G << 8;
    value |= R << 16;
    value |= A << 24;
    return value;
}

Color Grid::intToColor(int c) 
{
    float red   = ((c >> 16) & 255) / 255.0f;
    float blue  = ((c >> 8) & 255) / 255.0f;
    float green = (c & 255) / 255.0f;
    float alpha = ((c >> 24) & 255) / 255.0f;
    return Color(red, blue, green, alpha);
}