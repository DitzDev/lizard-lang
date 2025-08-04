#pragma once
#include "token.h"
#include <memory>
#include <vector>

namespace Lizard {

struct ASTNode;
using ASTNodePtr = std::unique_ptr<ASTNode>;

enum class ASTNodeType {
    PROGRAM,
    VARIABLE_DECLARATION,
    VARIABLE_ASSIGNMENT,
    PRINT_STATEMENT,
    LITERAL,
    IDENTIFIER,
    BINARY_EXPRESSION
};

enum class BinaryOperator {
    ADD,      // +
    SUBTRACT, // -
    MULTIPLY, // *
    DIVIDE,   // /
    INT_DIV,  // //
    MODULO    // %
};

struct ASTNode {
    ASTNodeType type;
    Position position;
    
    ASTNode(ASTNodeType t, const Position& pos) : type(t), position(pos) {}
    virtual ~ASTNode() = default;
};

struct Program : public ASTNode {
    std::vector<ASTNodePtr> statements;
    
    Program(const Position& pos) : ASTNode(ASTNodeType::PROGRAM, pos) {}
};

struct VariableDeclaration : public ASTNode {
    std::string name;
    ASTNodePtr value;
    bool is_constant;
    Position name_position;
    
    VariableDeclaration(const std::string& n, ASTNodePtr v, bool is_const, 
                       const Position& pos, const Position& name_pos)
        : ASTNode(ASTNodeType::VARIABLE_DECLARATION, pos), 
          name(n), value(std::move(v)), is_constant(is_const), name_position(name_pos) {}
};

struct VariableAssignment : public ASTNode {
    std::string name;
    ASTNodePtr value;
    Position name_position;
    
    VariableAssignment(const std::string& n, ASTNodePtr v, 
                      const Position& pos, const Position& name_pos)
        : ASTNode(ASTNodeType::VARIABLE_ASSIGNMENT, pos), 
          name(n), value(std::move(v)), name_position(name_pos) {}
};

struct PrintStatement : public ASTNode {
    ASTNodePtr expression;
    
    PrintStatement(ASTNodePtr expr, const Position& pos)
        : ASTNode(ASTNodeType::PRINT_STATEMENT, pos), expression(std::move(expr)) {}
};

struct Literal : public ASTNode {
    Token token;
    
    Literal(const Token& t) : ASTNode(ASTNodeType::LITERAL, t.position), token(t) {}
};

struct Identifier : public ASTNode {
    std::string name;
    
    Identifier(const std::string& n, const Position& pos)
        : ASTNode(ASTNodeType::IDENTIFIER, pos), name(n) {}
};

struct BinaryExpression : public ASTNode {
    ASTNodePtr left;
    BinaryOperator operator_;
    ASTNodePtr right;
    
    BinaryExpression(ASTNodePtr l, BinaryOperator op, ASTNodePtr r, const Position& pos)
        : ASTNode(ASTNodeType::BINARY_EXPRESSION, pos), 
          left(std::move(l)), operator_(op), right(std::move(r)) {}
};

} // namespace Lizard