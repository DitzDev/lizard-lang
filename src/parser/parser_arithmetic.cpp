#include "parser_arithmetic.h"
#include "parser.h"
#include "error_handler.h"

namespace Lizard {

ASTNodePtr ArithmeticParser::parseExpression() {
    return parseAddition();
}

ASTNodePtr ArithmeticParser::parseAddition() {
    auto expr = parseMultiplication();
    
    while (parser->check(TokenType::PLUS) || parser->check(TokenType::MINUS)) {
        Token op_token = parser->advance();
        Position op_pos = op_token.position;
        auto right = parseMultiplication();
        
        BinaryOperator op = tokenToBinaryOperator(op_token.type);
        expr = std::make_unique<BinaryExpression>(std::move(expr), op, std::move(right), op_pos);
    }
    
    return expr;
}

ASTNodePtr ArithmeticParser::parseMultiplication() {
    auto expr = parseUnary();
    
    while (parser->check(TokenType::STARS) || parser->check(TokenType::SLASH) || 
           parser->check(TokenType::INT_DIVISION) || parser->check(TokenType::PERCENT)) {
        Token op_token = parser->advance();
        Position op_pos = op_token.position;
        auto right = parseUnary();
        
        BinaryOperator op = tokenToBinaryOperator(op_token.type);
        expr = std::make_unique<BinaryExpression>(std::move(expr), op, std::move(right), op_pos);
    }
    
    return expr;
}

ASTNodePtr ArithmeticParser::parseUnary() {
    // For now, just parse primary. Later can add unary minus/plus
    return parsePrimary();
}

ASTNodePtr ArithmeticParser::parsePrimary() {
    if (parser->match(TokenType::STRING) || parser->match(TokenType::INTEGER) || 
        parser->match(TokenType::FLOAT) || parser->match(TokenType::BOOLEAN) || 
        parser->match(TokenType::NIL)) {
        return std::make_unique<Literal>(parser->previous());
    }
    
    if (parser->match(TokenType::IDENTIFIER)) {
        return std::make_unique<Identifier>(parser->previous().value, parser->previous().position);
    }
    
    // TODO: Add parentheses support later
    // if (parser->match(TokenType::LEFT_PAREN)) {
    //     auto expr = parseExpression();
    //     parser->consume(TokenType::RIGHT_PAREN, "Expected ')' after expression");
    //     return expr;
    // }
    
    ErrorHandler::reportError("Expected expression", parser->peek().position);
    return nullptr;
}

BinaryOperator ArithmeticParser::tokenToBinaryOperator(TokenType type) {
    switch (type) {
        case TokenType::PLUS:
            return BinaryOperator::ADD;
        case TokenType::MINUS:
            return BinaryOperator::SUBTRACT;
        case TokenType::STARS:
            return BinaryOperator::MULTIPLY;
        case TokenType::SLASH:
            return BinaryOperator::DIVIDE;
        case TokenType::INT_DIVISION:
            return BinaryOperator::INT_DIV;
        case TokenType::PERCENT:
            return BinaryOperator::MODULO;
        default:
            ErrorHandler::reportError("Unknown binary operator", Position());
            return BinaryOperator::ADD; // fallback
    }
}

} // namespace Lizard