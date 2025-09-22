#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "interpreter.hpp"
#include "error/error_handler.hpp"
#include "utils/file_utils.hpp"
#include <iostream>
#include <string>

void runFile(const std::string& path) {
    try {
        std::string source = FileUtils::readFile(path);
        
        Lexer lexer(source);
        auto tokens = lexer.scanTokens();
        
        if (ErrorHandler::getHadError()) return;
        
        Parser parser(std::move(tokens));
        auto statements = parser.parse();
        
        if (ErrorHandler::getHadError()) return;
        
        Interpreter interpreter;
        interpreter.interpret(statements);
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void runPrompt() {
    std::cout << "Focus Nexus Interactive Interpreter v1.0" << std::endl;
    std::cout << "Type 'exit' to quit" << std::endl;
    
    Interpreter interpreter;
    std::string line;
    
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, line);
        
        if (line == "exit" || line == "quit") {
            break;
        }
        
        if (line.empty()) {
            continue;
        }
        
        // Add newline for proper parsing
        line += "\n";
        
        try {
            Lexer lexer(line);
            auto tokens = lexer.scanTokens();
            
            if (ErrorHandler::getHadError()) {
                ErrorHandler::reset();
                continue;
            }
            
            Parser parser(std::move(tokens));
            auto statements = parser.parse();
            
            if (ErrorHandler::getHadError()) {
                ErrorHandler::reset();
                continue;
            }
            
            interpreter.interpret(statements);
            
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
        
        ErrorHandler::reset();
    }
}

int main(int argc, char* argv[]) {
    if (argc > 2) {
        std::cout << "Usage: focusNexus [script]" << std::endl;
        return 64;
    } else if (argc == 2) {
        runFile(argv[1]);
        if (ErrorHandler::getHadError()) return 65;
        if (ErrorHandler::getHadRuntimeError()) return 70;
    } else {
        runPrompt();
    }
    
    return 0;
}