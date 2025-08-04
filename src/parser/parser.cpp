#include "parser.h"
#include "error_handler.h"

namespace Lizard {

Parser::Parser(const std::vector<Token>& tokens) 
    : tokens(tokens), current(0), arithmetic_parser(this) {}

std::unique_ptr<Program> Parser::parse() {
    auto program = std::make_unique<Program>(Position());
    
    while (!isAtEnd()) {
        // Skip newlines
        if (match(TokenType::NEWLINE)) {
            continue;
        }
        
        try {
            auto stmt = statement();
            if (stmt) {
                program->statements.push_back(std::move(stmt));
            }
        } catch (const LizardError&) {
            synchronize();
        }
    }
    
    return program;
}

bool Parser::isAtEnd() const {
    return peek().type == TokenType::EOF_TOKEN;
}

Token Parser::peek() const {
    return tokens[current];
}

Token Parser::previous() const {
    return tokens[current - 1];
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

void Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) {
        advance();
        return;
    }
    
    ErrorHandler::reportError(message, peek().position);
}

void Parser::synchronize() {
    advance();
    
    while (!isAtEnd()) {
        if (previous().type == TokenType::NEWLINE) return;
        
        switch (peek().type) {
            case TokenType::VAR:
            case TokenType::FIX:
            case TokenType::PUT:
                return;
            default:
                break;
        }
        
        advance();
    }
}

ASTNodePtr Parser::statement() {
    if (match(TokenType::VAR) || match(TokenType::FIX)) {
        return variableDeclaration();
    }
    
    if (match(TokenType::PUT)) {
        return printStatement();
    }
    
    if (check(TokenType::IDENTIFIER)) {
        // Look ahead to see if this is an assignment
        size_t saved_current = current;
        advance(); // consume identifier
        if (match(TokenType::ASSIGN)) {
            current = saved_current; // restore position
            return variableAssignment();
        }
        current = saved_current; // restore position
    }
    
    ErrorHandler::reportError("Expected statement", peek().position);
    return nullptr;
}

ASTNodePtr Parser::variableDeclaration() {
    bool is_constant = previous().type == TokenType::FIX;
    Position decl_pos = previous().position;
    
    if (!check(TokenType::IDENTIFIER)) {
        ErrorHandler::reportError("Expected variable name", peek().position);
    }
    
    Token name_token = advance();
    
    ASTNodePtr value = nullptr;
    if (match(TokenType::ASSIGN)) {
        value = expression();
    }
    
    return std::make_unique<VariableDeclaration>(
        name_token.value, std::move(value), is_constant, decl_pos, name_token.position
    );
}

ASTNodePtr Parser::variableAssignment() {
    Token name_token = advance();
    Position assign_pos = peek().position;
    
    consume(TokenType::ASSIGN, "Expected '=' after variable name");
    
    auto value = expression();
    
    return std::make_unique<VariableAssignment>(
        name_token.value, std::move(value), assign_pos, name_token.position
    );
}

ASTNodePtr Parser::printStatement() {
    Position print_pos = previous().position;
    auto expr = expression();
    
    return std::make_unique<PrintStatement>(std::move(expr), print_pos);
}

ASTNodePtr Parser::expression() {
    return arithmetic_parser.parseExpression();
}

ASTNodePtr Parser::primary() {
    return arithmetic_parser.parsePrimary();
}

} // namespace Lizard