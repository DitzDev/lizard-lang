#pragma once
#include "token.h"
#include "lexer_state.h"

namespace Lizard {

Token parseArithmeticOperator(LexerState& state);

} // namespace Lizard