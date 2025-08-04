#include "lexer_arithmetic.h"
#include "error_handler.h"

namespace Lizard {

Token parseArithmeticOperator(LexerState& state) {
    Position pos = state.getCurrentPosition();
    char c = state.advance();
    
    switch (c) {
        case '+':
            return Token(TokenType::PLUS, "+", pos);
        case '-':
            return Token(TokenType::MINUS, "-", pos);
        case '*':
            return Token(TokenType::STARS, "*", pos);
        case '%':
            return Token(TokenType::PERCENT, "%", pos);
        case '/':
            // Check for integer division '//'
            if (state.peek() == '/') {
                state.advance(); // consume second '/'
                return Token(TokenType::INT_DIVISION, "//", pos);
            }
            return Token(TokenType::SLASH, "/", pos);
        default:
            ErrorHandler::reportError("Unexpected character '" + std::string(1, c) + "'", pos);
            return Token(TokenType::EOF_TOKEN, "", pos);
    }
}

} // namespace Lizard