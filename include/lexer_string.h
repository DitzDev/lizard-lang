#pragma once
#include "token.h"
#include "lexer_state.h"

namespace Lizard {

Token parseString(LexerState& state);
Token parseMultilineString(LexerState& state);

} // namespace Lizard