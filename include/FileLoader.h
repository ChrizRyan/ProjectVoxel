#pragma once
#include <string>

class FileLoader
{
    private:
        /* data */
        static const std::string ASSET_FOLDER;
    public:
        FileLoader();
        ~FileLoader();

        std::string loadFile(const std::string& filePath);
};
