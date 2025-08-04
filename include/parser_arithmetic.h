#pragma once
#include "ast.h"
#include "token.h"
#include <vector>

namespace Lizard {

class Parser; // Forward declaration

class ArithmeticParser {
private:
    Parser* parser;
    
public:
    ArithmeticParser(Parser* p) : parser(p) {}
    
    ASTNodePtr parseExpression();
    ASTNodePtr parseAddition();
    ASTNodePtr parseMultiplication();
    ASTNodePtr parseUnary();
    ASTNodePtr parsePrimary();
    
    BinaryOperator tokenToBinaryOperator(TokenType type);
};

} // namespace Lizard