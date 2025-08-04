#pragma once
#include "token.h"
#include <string>

namespace Lizard {

// Returns the token type for a keyword, or IDENTIFIER if not a keyword
TokenType getKeywordType(const std::string& identifier);

// Checks if an identifier is a keyword
bool isKeyword(const std::string& identifier);

} // namespace Lizard