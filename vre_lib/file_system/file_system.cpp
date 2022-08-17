#include "file_system.hpp"
#include "../core/vre_core.hpp"

namespace vre::Filesystem
{
    void readFile(const std::string& filename, std::string* buffer) {

        auto filepath = validateFilePath(filename);

        std::ifstream fh(filepath, std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
        const size_t sz = fh.tellg();
        if (sz <= 0) {
            err(std::string("readFile: File size is 0: ") + filepath);
            return;
        }
        fh.seekg(0, std::ios::beg);
        // Initalizes a std::string with length sz, filled with null
        *buffer = std::string(sz, '\0');
        if (fh.is_open())
            fh.read(buffer->data(), sz);
        fh.close();
    }

    std::string validateFilePath(std::string inputFilePath)
    {
        std::ifstream fh(inputFilePath, std::ios::in | std::ios::out | std::ios::binary |
            std::ios::ate);
        if (fh.is_open() && fh.good())
        {
            fh.close();
            return inputFilePath;
        }

        auto config = VreCore::GetInstance()->GetConfig();
        std::string fullPath = config->mPathToResources + inputFilePath;
        fh.open(fullPath, std::ios::in | std::ios::out | std::ios::binary |
            std::ios::ate);

        if (fh.is_open() && fh.good())
        {
            fh.close();
            return fullPath;
        }

        err(std::string("Couldn't open either of paths 1: ") + fullPath + std::string(" \n 2: ") + inputFilePath);
        return "";
    }
}