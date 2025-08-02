#include "../include/FileLoader.h"
#include <fstream>
#include <iostream>
#include <sstream> 
#include <string>
#include <filesystem>

const std::string FileLoader::ASSET_FOLDER = "C:/Users/Zed/Documents/Projects/C++ Projects/Test/assets";

FileLoader::FileLoader(/* args */) {
    // Constructor implementation
}   

FileLoader::~FileLoader() {
    // Destructor implementation
}

std::string FileLoader::loadFile(const std::string& filePath)  {

    std::ifstream file(ASSET_FOLDER + filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << ASSET_FOLDER + filePath << std::endl;
        return ""; // Return an empty string or handle the error as appropriate
    }

    std::stringstream buffer;
    buffer << file.rdbuf(); // Read entire file content into stringstream
    file.close(); // Close the file stream

    return buffer.str(); // Convert stringstream content to std::string
}