#include "lexer_number.h"
#include <cctype>

namespace Lizard {

Token parseNumber(LexerState& state) {
    Position start_pos = state.getCurrentPosition();
    std::string value;
    
    while (!state.isAtEnd() && std::isdigit(state.peek())) {
        value += state.advance();
    }
    
    bool is_float = false;
    if (!state.isAtEnd() && state.peek() == '.' && std::isdigit(state.peekNext())) {
        is_float = true;
        value += state.advance(); // consume '.'
        
        while (!state.isAtEnd() && std::isdigit(state.peek())) {
            value += state.advance();
        }
    }
    
    return Token(is_float ? TokenType::FLOAT : TokenType::INTEGER, value, start_pos);
}

} // namespace Lizard