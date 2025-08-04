#include "lexer_string.h"
#include "error_handler.h"
#include <cctype>

namespace Lizard {

Token parseString(LexerState& state) {
    Position start_pos = state.getCurrentPosition();
    state.advance(); // consume opening quote
    
    std::string value;
    while (!state.isAtEnd() && state.peek() != '"') {
        if (state.peek() == '\n') {
            ErrorHandler::reportError("Unterminated string", start_pos);
        }
        if (state.peek() == '\\') {
            state.advance();
            if (state.isAtEnd()) {
                ErrorHandler::reportError("Unterminated string", start_pos);
            }
            char escape_char = state.peek();
            switch (escape_char) {
                case 'n': value += '\n'; break;
                case 't': value += '\t'; break;
                case 'r': value += '\r'; break;
                case '\\': value += '\\'; break;
                case '"': value += '"'; break;
                case '0': case '1': case '2': case '3':
                case '4': case '5': case '6': case '7': {
                    int octal_value = 0;
                    int digit_count = 0;

                    while (digit_count < 3 && !state.isAtEnd() && 
                           state.peek() >= '0' && state.peek() <= '7') {
                        octal_value = octal_value * 8 + (state.peek() - '0');
                        state.advance();
                        digit_count++;
                    }
                    
                    if (octal_value > 255) {
                        ErrorHandler::reportError("Octal escape sequence out of range", state.getCurrentPosition());
                    }
                    
                    value += static_cast<char>(octal_value);
                    continue; // Don't advance again, we already did in the loop
                }
                case 'x': {
                    state.advance(); // consume 'x'
                    if (state.isAtEnd()) {
                        ErrorHandler::reportError("Invalid hexadecimal escape sequence", state.getCurrentPosition());
                    }
                    
                    int hex_value = 0;
                    int digit_count = 0;
                    
                    while (digit_count < 2 && !state.isAtEnd()) {
                        char hex_char = state.peek();
                        if (hex_char >= '0' && hex_char <= '9') {
                            hex_value = hex_value * 16 + (hex_char - '0');
                        } else if (hex_char >= 'a' && hex_char <= 'f') {
                            hex_value = hex_value * 16 + (hex_char - 'a' + 10);
                        } else if (hex_char >= 'A' && hex_char <= 'F') {
                            hex_value = hex_value * 16 + (hex_char - 'A' + 10);
                        } else {
                            break;
                        }
                        state.advance();
                        digit_count++;
                    }
                    
                    if (digit_count == 0) {
                        ErrorHandler::reportError("Invalid hexadecimal escape sequence", state.getCurrentPosition());
                    }
                    
                    value += static_cast<char>(hex_value);
                    continue; // Don't advance again
                }
                default:
                    ErrorHandler::reportError("Invalid escape sequence", state.getCurrentPosition());
            }
        } else {
            value += state.peek();
        }
        state.advance();
    }
    
    if (state.isAtEnd()) {
        ErrorHandler::reportError("Unterminated string", start_pos);
    }
    
    state.advance();
    return Token(TokenType::STRING, value, start_pos);
}

Token parseMultilineString(LexerState& state) {
    Position start_pos = state.getCurrentPosition();
    
    state.advance();
    state.advance();
    state.advance();

    std::string value;
    
    while (!state.isAtEnd()) {
        if (state.peek() == '"' && 
            state.peekNext() == '"' && 
            state.peekNext(2) == '"') {
            state.advance();
            state.advance();
            state.advance();
            return Token(TokenType::STRING, value, start_pos);
        }

        if (state.peek() == '\\') {
            state.advance();
            if (state.isAtEnd()) {
                ErrorHandler::reportError("Unterminated multiline string", start_pos);
            }
            char escape_char = state.peek();
            switch (escape_char) {
                case 'n': value += '\n'; break;
                case 't': value += '\t'; break;
                case 'r': value += '\r'; break;
                case '\\': value += '\\'; break;
                case '"': value += '"'; break;
                case '0': case '1': case '2': case '3':
                case '4': case '5': case '6': case '7': {
                    int octal_value = 0;
                    int digit_count = 0;

                    while (digit_count < 3 && !state.isAtEnd() && 
                           state.peek() >= '0' && state.peek() <= '7') {
                        octal_value = octal_value * 8 + (state.peek() - '0');
                        state.advance();
                        digit_count++;
                    }
                    
                    if (octal_value > 255) {
                        ErrorHandler::reportError("Octal escape sequence out of range", state.getCurrentPosition());
                    }
                    
                    value += static_cast<char>(octal_value);
                    continue;
                }
                case 'x': {
                    state.advance(); // consume 'x'
                    if (state.isAtEnd()) {
                        ErrorHandler::reportError("Invalid hexadecimal escape sequence", state.getCurrentPosition());
                    }
                    
                    int hex_value = 0;
                    int digit_count = 0;
                    
                    while (digit_count < 2 && !state.isAtEnd()) {
                        char hex_char = state.peek();
                        if (hex_char >= '0' && hex_char <= '9') {
                            hex_value = hex_value * 16 + (hex_char - '0');
                        } else if (hex_char >= 'a' && hex_char <= 'f') {
                            hex_value = hex_value * 16 + (hex_char - 'a' + 10);
                        } else if (hex_char >= 'A' && hex_char <= 'F') {
                            hex_value = hex_value * 16 + (hex_char - 'A' + 10);
                        } else {
                            break;
                        }
                        state.advance();
                        digit_count++;
                    }
                    
                    if (digit_count == 0) {
                        ErrorHandler::reportError("Invalid hexadecimal escape sequence", state.getCurrentPosition());
                    }
                    
                    value += static_cast<char>(hex_value);
                    continue;
                }
                default:
                    ErrorHandler::reportError("Invalid escape sequence", state.getCurrentPosition());
            }
        } else {
            value += state.peek();
        }
        state.advance();
    }
    
    ErrorHandler::reportError("Unterminated multiline string", start_pos);
    return Token(TokenType::STRING, value, start_pos);
}

} // namespace Lizard