#pragma once
#include "ast.h"
#include "token.h"
#include "parser_arithmetic.h"
#include <vector>
#include <memory>

namespace Lizard {

class Parser {
    friend class ArithmeticParser;
    
private:
    std::vector<Token> tokens;
    size_t current;
    ArithmeticParser arithmetic_parser;
    
public:
    Parser(const std::vector<Token>& tokens);
    
    std::unique_ptr<Program> parse();
    
    // Token navigation methods (made public for ArithmeticParser)
    bool isAtEnd() const;
    Token peek() const;
    Token previous() const;
    Token advance();
    bool check(TokenType type) const;
    bool match(TokenType type);
    void consume(TokenType type, const std::string& message);
    
private:
    void synchronize();
    
    // Statement parsing
    ASTNodePtr statement();
    ASTNodePtr variableDeclaration();
    ASTNodePtr variableAssignment();
    ASTNodePtr printStatement();
    
    // Expression parsing (now delegated to ArithmeticParser)
    ASTNodePtr expression();
    ASTNodePtr primary();
};

} // namespace Lizard