#include "lexer_keywords.h"
#include <unordered_map>

namespace Lizard {

static std::unordered_map<std::string, TokenType> keywords = {
    {"put", TokenType::PUT},       {"var", TokenType::VAR},
    {"fix", TokenType::FIX},       {"true", TokenType::BOOLEAN},
    {"false", TokenType::BOOLEAN}, {"nil", TokenType::NIL}};

TokenType getKeywordType(const std::string &identifier) {
  auto it = keywords.find(identifier);
  if (it != keywords.end()) {
    return it->second;
  }
  return TokenType::IDENTIFIER;
}

bool isKeyword(const std::string &identifier) {
  return keywords.find(identifier) != keywords.end();
}

} // namespace Lizard
