#pragma once
#include "token.h"
#include "lexer_state.h"

namespace Lizard {

// Parse an identifier or keyword from the lexer state
Token parseIdentifier(LexerState& state);

} // namespace Lizard