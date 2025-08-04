#include "lexer.h"
#include "error_handler.h"
#include "lexer_string.h"
#include "lexer_number.h"
#include "lexer_identifier.h"
#include "lexer_arithmetic.h"
#include <cctype>

namespace Lizard {

Lexer::Lexer(const std::string& source, const std::string& filename)
    : state(source, filename) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    
    while (!state.isAtEnd()) {
        while (!state.isAtEnd() && (std::isspace(state.peek()) || state.peek() == '#')) {
            if (state.peek() == '#') {
                state.skipComment();
            } else if (state.peek() == '\n') {
                tokens.emplace_back(TokenType::NEWLINE, "\n", state.getCurrentPosition());
                state.advance();
            } else {
                state.skipWhitespace();
            }
        }
        
        if (state.isAtEnd()) break;
        
        char c = state.peek();
        
        if (c == '"') {
            if (state.peekNext() == '"' && state.peekNext(2) == '"') {
                tokens.push_back(parseMultilineString(state));
            } else {
                tokens.push_back(parseString(state));
            }
        } else if (std::isdigit(c)) {
            tokens.push_back(parseNumber(state));
        } else if (std::isalpha(c) || c == '_') {
            tokens.push_back(parseIdentifier(state));
        } else if (c == '=') {
            tokens.emplace_back(TokenType::ASSIGN, "=", state.getCurrentPosition());
            state.advance();
        } else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%') {
            tokens.push_back(parseArithmeticOperator(state));
        } else {
            ErrorHandler::reportError("Unexpected character '" + std::string(1, c) + "'", state.getCurrentPosition());
        }
    }
    
    tokens.emplace_back(TokenType::EOF_TOKEN, "", state.getCurrentPosition());
    return tokens;
}

} // namespace Lizard