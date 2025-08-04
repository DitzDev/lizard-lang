#pragma once
#include <functional>
#include <string>

namespace Lizard {

enum class TokenType {
  // Literals
  STRING,
  INTEGER,
  FLOAT,
  BOOLEAN,
  NIL,
  IDENTIFIER,

  // Keywords
  PUT, // put
  VAR, // var
  FIX, // fix

  // Operators
  ASSIGN, // =

  // Math Operators
  PLUS,         // +
  MINUS,        // -
  SLASH,        // /
  STARS,        // *
  PERCENT,      // %
  INT_DIVISION, // //

  // Punctuation
  LEFT_PAREN,   // (
  RIGHT_PAREN,  // )
  NEWLINE,
  EOF_TOKEN
};

struct Position {
  std::string filename;
  int line;
  int column;

  Position(const std::string &file = "", int l = 1, int c = 1)
      : filename(file), line(l), column(c) {}
};

struct Token {
  TokenType type;
  std::string value;
  Position position;

  Token(TokenType t, const std::string &v, const Position &pos)
      : type(t), value(v), position(pos) {}
};

} // namespace Lizard