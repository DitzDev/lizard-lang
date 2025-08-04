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
    if (parser->match(TokenType::MINUS) || parser->match(TokenType::PLUS)) {
        Token op_token = parser->previous();
        Position op_pos = op_token.position;
        auto expr = parseUnary();
        
        // For unary minus, create a binary expression: 0 - expr
        // For unary plus, just return the expression as-is
        if (op_token.type == TokenType::MINUS) {
            auto zero = std::make_unique<Literal>(Token(TokenType::INTEGER, "0", op_pos));
            BinaryOperator op = BinaryOperator::SUBTRACT;
            return std::make_unique<BinaryExpression>(std::move(zero), op, std::move(expr), op_pos);
        } else {
            return expr; // Unary plus does nothing
        }
    }
    
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
    
    // Handle parentheses for grouping
    if (parser->match(TokenType::LEFT_PAREN)) {
        Position paren_pos = parser->previous().position;
        auto expr = parseExpression();
        
        if (!parser->match(TokenType::RIGHT_PAREN)) {
            ErrorHandler::reportError("Expected ')' after expression", parser->peek().position);
            return nullptr;
        }
        
        return expr; // Return the grouped expression
    }
    
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