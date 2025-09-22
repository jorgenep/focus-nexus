#include "file_utils.hpp"
#include <fstream>
#include <sstream>
#include <filesystem>

std::string FileUtils::readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + path);
    }
    
    std::ostringstream oss;
    oss << file.rdbuf();
    return oss.str();
}

bool FileUtils::writeFile(const std::string& path, const std::string& content) {
    std::ofstream file(path);
    if (!file.is_open()) {
        return false;
    }
    
    file << content;
    return true;
}

bool FileUtils::fileExists(const std::string& path) {
    return std::filesystem::exists(path);
}

std::vector<std::string> FileUtils::readLines(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + path);
    }
    
    std::vector<std::string> lines;
    std::string line;
    
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    
    return lines;
}

bool FileUtils::writeLines(const std::string& path, const std::vector<std::string>& lines) {
    std::ofstream file(path);
    if (!file.is_open()) {
        return false;
    }
    
    for (const auto& line : lines) {
        file << line << std::endl;
    }
    
    return true;
}

std::string FileUtils::getFileExtension(const std::string& path) {
    size_t pos = path.find_last_of('.');
    if (pos == std::string::npos) return "";
    return path.substr(pos + 1);
}

std::string FileUtils::getFileName(const std::string& path) {
    size_t pos = path.find_last_of("/\\");
    if (pos == std::string::npos) return path;
    return path.substr(pos + 1);
}

std::string FileUtils::getDirectory(const std::string& path) {
    size_t pos = path.find_last_of("/\\");
    if (pos == std::string::npos) return ".";
    return path.substr(0, pos);
}