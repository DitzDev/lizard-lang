#pragma once
#include "token.h"
#include <string>

namespace Lizard {

// Shared state for lexer operations
class LexerState {
public:
    LexerState(const std::string& source, const std::string& filename);
    
    bool isAtEnd() const;
    char advance();
    char peek() const;
    char peekNext(int offset = 1) const;
    bool match(char expected);
    
    void skipWhitespace();
    void skipComment();
    
    Position getCurrentPosition() const;
    
private:
    std::string source;
    std::string filename;
    size_t current;
    int line;
    int column;
};

} // namespace Lizard