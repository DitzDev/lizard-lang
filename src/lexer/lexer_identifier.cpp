#include "lexer_identifier.h"
#include "lexer_keywords.h"
#include <cctype>

namespace Lizard {

Token parseIdentifier(LexerState& state) {
    Position start_pos = state.getCurrentPosition();
    std::string value;
    
    while (!state.isAtEnd() && (std::isalnum(state.peek()) || state.peek() == '_')) {
        value += state.advance();
    }
    
    TokenType type = getKeywordType(value);
    
    return Token(type, value, start_pos);
}

} // namespace Lizard