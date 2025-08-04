#include "lexer_state.h"
#include <cctype>

namespace Lizard {

LexerState::LexerState(const std::string& source, const std::string& filename)
    : source(source), filename(filename), current(0), line(1), column(1) {}

bool LexerState::isAtEnd() const {
    return current >= source.length();
}

char LexerState::advance() {
    if (isAtEnd()) return '\0';
    
    char c = source[current++];
    if (c == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    return c;
}

char LexerState::peek() const {
    if (isAtEnd()) return '\0';
    return source[current];
}

char LexerState::peekNext(int offset) const {
    if (current + offset >= source.length()) {
        return '\0';
    }
    return source[current + offset];
}

bool LexerState::match(char expected) {
    if (isAtEnd()) return false;
    if (source[current] != expected) return false;
    
    current++;
    column++;
    return true;
}

void LexerState::skipWhitespace() {
    while (!isAtEnd() && std::isspace(peek()) && peek() != '\n') {
        advance();
    }
}

void LexerState::skipComment() {
    while (!isAtEnd() && peek() != '\n') {
        advance();
    }
}

Position LexerState::getCurrentPosition() const {
    return Position(filename, line, column);
}

} // namespace Lizard