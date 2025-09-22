#pragma once
#include <string>
#include <vector>

class FileUtils {
public:
    static std::string readFile(const std::string& path);
    static bool writeFile(const std::string& path, const std::string& content);
    static bool fileExists(const std::string& path);
    static std::vector<std::string> readLines(const std::string& path);
    static bool writeLines(const std::string& path, const std::vector<std::string>& lines);
    static std::string getFileExtension(const std::string& path);
    static std::string getFileName(const std::string& path);
    static std::string getDirectory(const std::string& path);
};