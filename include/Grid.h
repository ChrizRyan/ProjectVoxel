#pragma once
#include "Color.h"
#include <vector>
#include <array>
#include <cmath>
#include <algorithm>
#include <cstdint>
#include <stdexcept>

struct Vector3Int {
    int x, y, z;
    Vector3Int(int x=0, int y=0, int z=0) : x(x), y(y), z(z) {}
    Vector3Int operator+(const Vector3Int& o) const { return {x+o.x, y+o.y, z+o.z}; }
    Vector3Int operator-(const Vector3Int& o) const { return {x-o.x, y-o.y, z-o.z}; }
    bool operator==(const Vector3Int& o) const { return x==o.x && y==o.y && z==o.z; }
    //static const Vector3Int zero;
    //static const Vector3Int one;
};
//const Vector3Int Vector3Int::zero(0,0,0);
//const Vector3Int Vector3Int::one(1,1,1);


struct VoxelHitInfo {
    bool hit = false;
    Vector3Int coords;
    int voxel = 0;
    std::array<float,3> hitLocation = {0,0,0};
};

class Grid 
{
    public:
        
        Grid(const Vector3Int& dims);

        std::vector<int> Grid::getData(); 
        void generateVoxels();

        Vector3Int GetDimensions() const { return dimensions; }

    private:
    
        Vector3Int dimensions;
        std::vector<int> voxels;

        void initGrid(const Vector3Int& dims);

        int getVoxel(Vector3Int location);
        void setVoxel(Vector3Int location, int value);

        int colorToInt(const Color& color);
        Color intToColor(int c);
};