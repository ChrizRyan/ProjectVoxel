#pragma once
#include <vector>
#include <string>
#include <fstream>

class VoxelLoader {
public:
    // Loads a 3D voxel grid from a binary file.
    // Returns true on success, false on failure.
    static bool load(const std::string& fileName, std::vector<std::vector<std::vector<int>>>& voxels) {
        std::ifstream in(fileName, std::ios::binary);
        if (!in) {
            voxels.clear();
            return false;
        }

        int x, y, z;
        in.read(reinterpret_cast<char*>(&x), sizeof(int));
        in.read(reinterpret_cast<char*>(&y), sizeof(int));
        in.read(reinterpret_cast<char*>(&z), sizeof(int));
        if (!in) {
            voxels.clear();
            return false;
        }

        voxels.resize(x, std::vector<std::vector<int>>(y, std::vector<int>(z)));
        for (int i = 0; i < x; ++i)
            for (int j = 0; j < y; ++j)
                in.read(reinterpret_cast<char*>(voxels[i][j].data()), sizeof(int) * z);

        return in.good();
    }

    // Saves a 3D voxel grid to a binary file.
    // Returns true on success, false on failure.
    static bool save(const std::string& fileName, const std::vector<std::vector<std::vector<int>>>& voxels) {
        std::ofstream out(fileName, std::ios::binary);
        if (!out) return false;

        int x = static_cast<int>(voxels.size());
        int y = x > 0 ? static_cast<int>(voxels[0].size()) : 0;
        int z = (x > 0 && y > 0) ? static_cast<int>(voxels[0][0].size()) : 0;

        out.write(reinterpret_cast<const char*>(&x), sizeof(int));
        out.write(reinterpret_cast<const char*>(&y), sizeof(int));
        out.write(reinterpret_cast<const char*>(&z), sizeof(int));

        for (int i = 0; i < x; ++i)
            for (int j = 0; j < y; ++j)
                out.write(reinterpret_cast<const char*>(voxels[i][j].data()), sizeof(int) * z);

        return out.good();
    }
};