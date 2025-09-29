// Example custom plugin for Focus Nexus
// This demonstrates how to create a custom plugin using the Focus Nexus Plugin API
//
// Compile with:
// Linux: g++ -shared -fPIC -o my_plugin.so my_plugin.cpp -I../../src
// Windows: g++ -shared -o my_plugin.dll my_plugin.cpp -I../../src

#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <sstream>
#include <iomanip>
#include <ctime>

// Include the Focus Nexus plugin API
// Note: In a real implementation, this would include the actual Value class
// For this example, we'll create a simplified version

// Simplified Value class for demonstration
class Value {
private:
    enum Type { NIL, BOOL, NUMBER, STRING } type;
    union {
        bool boolValue;
        double numberValue;
    };
    std::string stringValue;

public:
    Value() : type(NIL) {}
    Value(bool b) : type(BOOL), boolValue(b) {}
    Value(double d) : type(NUMBER), numberValue(d) {}
    Value(const std::string& s) : type(STRING), stringValue(s) {}
    Value(const char* s) : type(STRING), stringValue(s) {}
    
    bool isNumber() const { return type == NUMBER; }
    bool isString() const { return type == STRING; }
    bool isBool() const { return type == BOOL; }
    bool isNil() const { return type == NIL; }
    
    double asNumber() const { return numberValue; }
    std::string asString() const { return stringValue; }
    bool asBool() const { return boolValue; }
    
    std::string toString() const {
        switch (type) {
            case NIL: return "nil";
            case BOOL: return boolValue ? "true" : "false";
            case NUMBER: return std::to_string(numberValue);
            case STRING: return stringValue;
        }
        return "unknown";
    }
};

// Plugin state
static std::mt19937 rng(std::time(nullptr));
static std::vector<std::string> plugin_log;

// Plugin initialization function
void plugin_init() {
    plugin_log.push_back("Plugin initialized at " + std::to_string(std::time(nullptr)));
    rng.seed(std::time(nullptr));
}

// Plugin cleanup function
void plugin_cleanup() {
    plugin_log.push_back("Plugin cleaned up at " + std::to_string(std::time(nullptr)));
    plugin_log.clear();
}

// Plugin information function
const char* plugin_info() {
    return "Focus Nexus Custom Plugin v1.0 - Mathematical and utility functions";
}

// Mathematical functions
Value calculate_distance(const std::vector<Value>& args) {
    if (args.size() != 4) {
        throw std::runtime_error("calculate_distance requires 4 arguments: x1, y1, x2, y2");
    }
    
    if (!args[0].isNumber() || !args[1].isNumber() || 
        !args[2].isNumber() || !args[3].isNumber()) {
        throw std::runtime_error("All arguments must be numbers");
    }
    
    double x1 = args[0].asNumber();
    double y1 = args[1].asNumber();
    double x2 = args[2].asNumber();
    double y2 = args[3].asNumber();
    
    double distance = std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
    
    plugin_log.push_back("Calculated distance: " + std::to_string(distance));
    return Value(distance);
}

Value calculate_area_circle(const std::vector<Value>& args) {
    if (args.size() != 1) {
        throw std::runtime_error("calculate_area_circle requires 1 argument: radius");
    }
    
    if (!args[0].isNumber()) {
        throw std::runtime_error("Radius must be a number");
    }
    
    double radius = args[0].asNumber();
    if (radius < 0) {
        throw std::runtime_error("Radius cannot be negative");
    }
    
    double area = M_PI * radius * radius;
    plugin_log.push_back("Calculated circle area: " + std::to_string(area));
    return Value(area);
}

Value calculate_area_rectangle(const std::vector<Value>& args) {
    if (args.size() != 2) {
        throw std::runtime_error("calculate_area_rectangle requires 2 arguments: width, height");
    }
    
    if (!args[0].isNumber() || !args[1].isNumber()) {
        throw std::runtime_error("Width and height must be numbers");
    }
    
    double width = args[0].asNumber();
    double height = args[1].asNumber();
    
    if (width < 0 || height < 0) {
        throw std::runtime_error("Width and height cannot be negative");
    }
    
    double area = width * height;
    plugin_log.push_back("Calculated rectangle area: " + std::to_string(area));
    return Value(area);
}

Value calculate_hypotenuse(const std::vector<Value>& args) {
    if (args.size() != 2) {
        throw std::runtime_error("calculate_hypotenuse requires 2 arguments: a, b");
    }
    
    if (!args[0].isNumber() || !args[1].isNumber()) {
        throw std::runtime_error("Both arguments must be numbers");
    }
    
    double a = args[0].asNumber();
    double b = args[1].asNumber();
    
    double hypotenuse = std::sqrt(a * a + b * b);
    plugin_log.push_back("Calculated hypotenuse: " + std::to_string(hypotenuse));
    return Value(hypotenuse);
}

// Random number functions
Value random_number(const std::vector<Value>& args) {
    double min = 0.0, max = 1.0;
    
    if (args.size() >= 1 && args[0].isNumber()) {
        min = args[0].asNumber();
    }
    if (args.size() >= 2 && args[1].isNumber()) {
        max = args[1].asNumber();
    }
    
    if (min > max) {
        std::swap(min, max);
    }
    
    std::uniform_real_distribution<double> dist(min, max);
    double result = dist(rng);
    
    plugin_log.push_back("Generated random number: " + std::to_string(result));
    return Value(result);
}

Value random_integer(const std::vector<Value>& args) {
    int min = 0, max = 100;
    
    if (args.size() >= 1 && args[0].isNumber()) {
        min = static_cast<int>(args[0].asNumber());
    }
    if (args.size() >= 2 && args[1].isNumber()) {
        max = static_cast<int>(args[1].asNumber());
    }
    
    if (min > max) {
        std::swap(min, max);
    }
    
    std::uniform_int_distribution<int> dist(min, max);
    int result = dist(rng);
    
    plugin_log.push_back("Generated random integer: " + std::to_string(result));
    return Value(static_cast<double>(result));
}

Value shuffle_seed(const std::vector<Value>& args) {
    if (args.size() != 1 || !args[0].isNumber()) {
        throw std::runtime_error("shuffle_seed requires 1 numeric argument: seed");
    }
    
    unsigned int seed = static_cast<unsigned int>(args[0].asNumber());
    rng.seed(seed);
    
    plugin_log.push_back("Set random seed to: " + std::to_string(seed));
    return Value(true);
}

// String manipulation functions
Value capitalize_words(const std::vector<Value>& args) {
    if (args.size() != 1 || !args[0].isString()) {
        throw std::runtime_error("capitalize_words requires one string argument");
    }
    
    std::string input = args[0].asString();
    std::string result;
    bool capitalize_next = true;
    
    for (char c : input) {
        if (std::isspace(c)) {
            capitalize_next = true;
            result += c;
        } else if (capitalize_next) {
            result += std::toupper(c);
            capitalize_next = false;
        } else {
            result += std::tolower(c);
        }
    }
    
    plugin_log.push_back("Capitalized words: " + result);
    return Value(result);
}

Value reverse_string(const std::vector<Value>& args) {
    if (args.size() != 1 || !args[0].isString()) {
        throw std::runtime_error("reverse_string requires one string argument");
    }
    
    std::string input = args[0].asString();
    std::string result = input;
    std::reverse(result.begin(), result.end());
    
    plugin_log.push_back("Reversed string: " + result);
    return Value(result);
}

Value count_characters(const std::vector<Value>& args) {
    if (args.size() != 1 || !args[0].isString()) {
        throw std::runtime_error("count_characters requires one string argument");
    }
    
    std::string input = args[0].asString();
    double count = static_cast<double>(input.length());
    
    plugin_log.push_back("Counted characters: " + std::to_string(count));
    return Value(count);
}

Value count_words(const std::vector<Value>& args) {
    if (args.size() != 1 || !args[0].isString()) {
        throw std::runtime_error("count_words requires one string argument");
    }
    
    std::string input = args[0].asString();
    std::istringstream iss(input);
    std::string word;
    int count = 0;
    
    while (iss >> word) {
        count++;
    }
    
    plugin_log.push_back("Counted words: " + std::to_string(count));
    return Value(static_cast<double>(count));
}

Value remove_spaces(const std::vector<Value>& args) {
    if (args.size() != 1 || !args[0].isString()) {
        throw std::runtime_error("remove_spaces requires one string argument");
    }
    
    std::string input = args[0].asString();
    std::string result;
    
    for (char c : input) {
        if (!std::isspace(c)) {
            result += c;
        }
    }
    
    plugin_log.push_back("Removed spaces: " + result);
    return Value(result);
}

// Utility functions
Value format_number(const std::vector<Value>& args) {
    if (args.size() < 1 || !args[0].isNumber()) {
        throw std::runtime_error("format_number requires at least one numeric argument");
    }
    
    double number = args[0].asNumber();
    int precision = 2;
    
    if (args.size() >= 2 && args[1].isNumber()) {
        precision = static_cast<int>(args[1].asNumber());
        if (precision < 0) precision = 0;
        if (precision > 10) precision = 10;
    }
    
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << number;
    std::string result = oss.str();
    
    plugin_log.push_back("Formatted number: " + result);
    return Value(result);
}

Value current_timestamp(const std::vector<Value>& args) {
    double timestamp = static_cast<double>(std::time(nullptr));
    plugin_log.push_back("Got current timestamp: " + std::to_string(timestamp));
    return Value(timestamp);
}

Value is_even(const std::vector<Value>& args) {
    if (args.size() != 1 || !args[0].isNumber()) {
        throw std::runtime_error("is_even requires one numeric argument");
    }
    
    double number = args[0].asNumber();
    int intNumber = static_cast<int>(number);
    
    // Check if it's actually an integer
    if (number != intNumber) {
        return Value(false);
    }
    
    bool result = (intNumber % 2 == 0);
    plugin_log.push_back("Checked if " + std::to_string(intNumber) + " is even: " + (result ? "true" : "false"));
    return Value(result);
}

Value is_odd(const std::vector<Value>& args) {
    if (args.size() != 1 || !args[0].isNumber()) {
        throw std::runtime_error("is_odd requires one numeric argument");
    }
    
    double number = args[0].asNumber();
    int intNumber = static_cast<int>(number);
    
    // Check if it's actually an integer
    if (number != intNumber) {
        return Value(false);
    }
    
    bool result = (intNumber % 2 != 0);
    plugin_log.push_back("Checked if " + std::to_string(intNumber) + " is odd: " + (result ? "true" : "false"));
    return Value(result);
}

Value clamp_number(const std::vector<Value>& args) {
    if (args.size() != 3 || !args[0].isNumber() || !args[1].isNumber() || !args[2].isNumber()) {
        throw std::runtime_error("clamp_number requires 3 numeric arguments: value, min, max");
    }
    
    double value = args[0].asNumber();
    double min_val = args[1].asNumber();
    double max_val = args[2].asNumber();
    
    if (min_val > max_val) {
        std::swap(min_val, max_val);
    }
    
    double result = std::max(min_val, std::min(max_val, value));
    plugin_log.push_back("Clamped " + std::to_string(value) + " to range [" + 
                        std::to_string(min_val) + ", " + std::to_string(max_val) + 
                        "] = " + std::to_string(result));
    return Value(result);
}

// Plugin management functions
Value get_plugin_log(const std::vector<Value>& args) {
    std::string log_string;
    for (size_t i = 0; i < plugin_log.size(); i++) {
        if (i > 0) log_string += "\n";
        log_string += plugin_log[i];
    }
    return Value(log_string);
}

Value clear_plugin_log(const std::vector<Value>& args) {
    size_t count = plugin_log.size();
    plugin_log.clear();
    plugin_log.push_back("Log cleared");
    return Value(static_cast<double>(count));
}

Value get_plugin_version(const std::vector<Value>& args) {
    return Value("1.0.0");
}

// Export functions using the Focus Nexus plugin API
extern "C" {
    // Plugin lifecycle functions
    void focus_nexus_plugin_init() {
        plugin_init();
    }
    
    void focus_nexus_plugin_cleanup() {
        plugin_cleanup();
    }
    
    const char* focus_nexus_plugin_info() {
        return plugin_info();
    }
    
    // Mathematical functions
    Value focus_nexus_calculate_distance(const std::vector<Value>& args) {
        return calculate_distance(args);
    }
    
    Value focus_nexus_calculate_area_circle(const std::vector<Value>& args) {
        return calculate_area_circle(args);
    }
    
    Value focus_nexus_calculate_area_rectangle(const std::vector<Value>& args) {
        return calculate_area_rectangle(args);
    }
    
    Value focus_nexus_calculate_hypotenuse(const std::vector<Value>& args) {
        return calculate_hypotenuse(args);
    }
    
    // Random number functions
    Value focus_nexus_random_number(const std::vector<Value>& args) {
        return random_number(args);
    }
    
    Value focus_nexus_random_integer(const std::vector<Value>& args) {
        return random_integer(args);
    }
    
    Value focus_nexus_shuffle_seed(const std::vector<Value>& args) {
        return shuffle_seed(args);
    }
    
    // String manipulation functions
    Value focus_nexus_capitalize_words(const std::vector<Value>& args) {
        return capitalize_words(args);
    }
    
    Value focus_nexus_reverse_string(const std::vector<Value>& args) {
        return reverse_string(args);
    }
    
    Value focus_nexus_count_characters(const std::vector<Value>& args) {
        return count_characters(args);
    }
    
    Value focus_nexus_count_words(const std::vector<Value>& args) {
        return count_words(args);
    }
    
    Value focus_nexus_remove_spaces(const std::vector<Value>& args) {
        return remove_spaces(args);
    }
    
    // Utility functions
    Value focus_nexus_format_number(const std::vector<Value>& args) {
        return format_number(args);
    }
    
    Value focus_nexus_current_timestamp(const std::vector<Value>& args) {
        return current_timestamp(args);
    }
    
    Value focus_nexus_is_even(const std::vector<Value>& args) {
        return is_even(args);
    }
    
    Value focus_nexus_is_odd(const std::vector<Value>& args) {
        return is_odd(args);
    }
    
    Value focus_nexus_clamp_number(const std::vector<Value>& args) {
        return clamp_number(args);
    }
    
    // Plugin management functions
    Value focus_nexus_get_plugin_log(const std::vector<Value>& args) {
        return get_plugin_log(args);
    }
    
    Value focus_nexus_clear_plugin_log(const std::vector<Value>& args) {
        return clear_plugin_log(args);
    }
    
    Value focus_nexus_get_plugin_version(const std::vector<Value>& args) {
        return get_plugin_version(args);
    }
}