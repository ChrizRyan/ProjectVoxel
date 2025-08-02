#pragma once
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
    static const Vector3Int zero;
    static const Vector3Int one;
};
const Vector3Int Vector3Int::zero(0,0,0);
const Vector3Int Vector3Int::one(1,1,1);

struct Color {
    float r, g, b, a;
    Color(float r=0, float g=0, float b=0, float a=1) : r(r), g(g), b(b), a(a) {}
    static const Color red, green, blue, black;
};
const Color Color::red(1,0,0,1);
const Color Color::green(0,1,0,1);
const Color Color::blue(0,0,1,1);
const Color Color::black(0,0,0,1);

struct VoxelHitInfo {
    bool hit = false;
    Vector3Int coords;
    int voxel = 0;
    std::array<float,3> hitLocation = {0,0,0};
};

class Grid {
public:
    Vector3Int dimensions;
    std::vector<std::vector<std::vector<int>>> voxels;

    Grid() {}
    Grid(const Vector3Int& dims) { InitGrid(dims); }

    void InitGrid(const Vector3Int& dims) {
        dimensions = dims;
        voxels.assign(dimensions.x, std::vector<std::vector<int>>(dimensions.y, std::vector<int>(dimensions.z, 0)));
    }

    void InitGrid(const Vector3Int& dims, const std::vector<std::vector<std::vector<int>>>& v) {
        dimensions = dims;
        voxels = v;
    }

    std::vector<std::vector<std::vector<int>>>& GetData() { return voxels; }

    void DeleteVoxel(const Vector3Int& coords) {
        if (IsInsideGrid(coords))
            voxels[coords.x][coords.y][coords.z] = 0;
    }

    void DeleteVoxels(const Vector3Int& coords, const Vector3Int& brushSize) {
        for (int y = 0; y < brushSize.y; ++y)
            for (int x = 0; x < brushSize.x; ++x)
                for (int z = 0; z < brushSize.z; ++z)
                    DeleteVoxel(coords + Vector3Int(x, y, z));
    }

    void AddVoxel(const Vector3Int& coords, const Color& color) {
        if (IsInsideGrid(coords))
            voxels[coords.x][coords.y][coords.z] = colorToInt(color);
    }

    void AddVoxels(const Vector3Int& coords, const Vector3Int& brushSize, const Color& color) {
        for (int y = 0; y < brushSize.y; ++y)
            for (int x = 0; x < brushSize.x; ++x)
                for (int z = 0; z < brushSize.z; ++z)
                    AddVoxel(coords + Vector3Int(x, y, z), color);
    }

    Color GetVoxelColor(const VoxelHitInfo& info) const {
        if (info.hit)
            return intToColor(info.voxel);
        return Color::black;
    }

    bool IsInsideGrid(const Vector3Int& coords) const {
        return coords.x >= 0 && coords.x < dimensions.x &&
               coords.y >= 0 && coords.y < dimensions.y &&
               coords.z >= 0 && coords.z < dimensions.z;
    }

    // Color packing/unpacking
    static int colorToInt(const Color& color) {
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
    static Color intToColor(int c) {
        float red   = ((c >> 16) & 255) / 255.0f;
        float blue  = ((c >> 8) & 255) / 255.0f;
        float green = (c & 255) / 255.0f;
        float alpha = ((c >> 24) & 255) / 255.0f;
        return Color(red, blue, green, alpha);
    }

    // Example voxel generation (random fill)
    void GenerateVoxels() {
        Color colors[] = { Color::red, Color::blue, Color::green };
        for (int y = 0; y < dimensions.y; ++y)
            for (int x = 0; x < dimensions.x; ++x)
                for (int z = 0; z < dimensions.z; ++z) {
                    bool solid = (rand() / (float)RAND_MAX) > 0.3f;
                    bool bottomSolid = y - 1 < 0 || voxels[x][y - 1][z] != 0;
                    Color c = colors[rand() % 3];
                    voxels[x][y][z] = (solid && bottomSolid) ? colorToInt(c) : 0;
                }
    }
};