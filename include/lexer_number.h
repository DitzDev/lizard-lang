#pragma once
#include "token.h"
#include "lexer_state.h"

namespace Lizard {

// Parse a number (integer or float) from the lexer state
Token parseNumber(LexerState& state);

} // namespace Lizard