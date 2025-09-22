#pragma once
#include <string>
#include <vector>

class StringUtils {
public:
    static std::string trim(const std::string& str);
    static std::vector<std::string> split(const std::string& str, char delimiter);
    static std::string join(const std::vector<std::string>& strings, const std::string& delimiter);
    static bool startsWith(const std::string& str, const std::string& prefix);
    static bool endsWith(const std::string& str, const std::string& suffix);
    static std::string toLower(const std::string& str);
    static std::string toUpper(const std::string& str);
    static std::string replace(const std::string& str, const std::string& from, const std::string& to);
};