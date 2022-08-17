#pragma once
#include "../common.hpp"

namespace vre::Filesystem
{
    /// @brief Read the file located by the filename and put its content into the string passed by the parameter buffer.
    /// @param filename - The path to the file.
    /// @param buffer - The string to fill with the content.
    void readFile(const std::string& filename, std::string* buffer);

    /// @brief Returns the path to resource folder in the current project.
    /// The function must be defined by the project itself, usually by cmake.
    std::string getResourcePath();

    /// @brief Checks if the input file path exists either with the absolut project path or
    /// the path relative to the executable. If the file exists the validated path is returned.
    /// Otherwise the function terminates the program with an error.
    /// @param inputFilePath - The file to validate.
    /// @return A valid file path.
    std::string validateFilePath(std::string inputFilePath);
}