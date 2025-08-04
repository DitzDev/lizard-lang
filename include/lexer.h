#pragma once
#include "token.h"
#include "lexer_state.h"
#include <string>
#include <vector>

namespace Lizard {

class Lexer {
public:
    Lexer(const std::string& source, const std::string& filename);
    
    std::vector<Token> tokenize();
    
private:
    LexerState state;
};

} // namespace Lizard