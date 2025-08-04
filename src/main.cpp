#include "lexer.h"
#include "parser.h"
#include "evaluator.h"
#include "error_handler.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace Lizard;

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "'" << std::endl;
        exit(1);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::vector<std::string> splitLines(const std::string& content) {
    std::vector<std::string> lines;
    std::stringstream ss(content);
    std::string line;
    
    while (std::getline(ss, line)) {
        lines.push_back(line);
    }
    
    return lines;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: lizard <file.lz>" << std::endl;
        return 1;
    }
    
    std::string filename = argv[1];

    if (filename.length() < 3 || filename.substr(filename.length() - 3) != ".lz") {
        std::cerr << "Error: Lizard files must have .lz extension" << std::endl;
        return 1;
    }
    
    try {
        std::string source = readFile(filename);
        std::vector<std::string> source_lines = splitLines(source);
        
        ErrorHandler::setSourceFile(filename, source_lines);
        
        Lexer lexer(source, filename);
        std::vector<Token> tokens = lexer.tokenize();

        Parser parser(tokens);
        auto program = parser.parse();

        Evaluator evaluator;
        evaluator.evaluate(*program);
        
    } catch (const LizardError& e) {
        std::cerr << e.formatError() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Internal error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}