#include "evaluator.h"
#include "eval_arithmetic.h"
#include "error_handler.h"
#include <iostream>

namespace Lizard {

Evaluator::Evaluator() {}

void Evaluator::evaluate(const Program& program) {
    for (const auto& stmt : program.statements) {
        executeStatement(*stmt);
    }
}

void Evaluator::executeStatement(const ASTNode& node) {
    switch (node.type) {
        case ASTNodeType::VARIABLE_DECLARATION:
            executeVariableDeclaration(static_cast<const VariableDeclaration&>(node));
            break;
        case ASTNodeType::VARIABLE_ASSIGNMENT:
            executeVariableAssignment(static_cast<const VariableAssignment&>(node));
            break;
        case ASTNodeType::PRINT_STATEMENT:
            executePrintStatement(static_cast<const PrintStatement&>(node));
            break;
        default:
            ErrorHandler::reportError("Unknown statement type", node.position);
    }
}

void Evaluator::executeVariableDeclaration(const VariableDeclaration& node) {
    Value value(nullptr); // Default to nil
    
    if (node.value) {
        value = evaluateExpression(*node.value);
        environment.define(node.name, value, node.is_constant, node.position);
    } else {
        // Late initialization - store uninitialized variable
        Variable uninit_var(value, node.is_constant, false, node.position);
        if (environment.exists(node.name)) {
            ErrorHandler::reportError("Variable '" + node.name + "' is already defined", node.position);
        }
        environment.define(node.name, value, node.is_constant, node.position);
        // Mark as uninitialized
        Variable* var = environment.getVariable(node.name);
        if (var) {
            var->is_initialized = false;
        }
    }
}

void Evaluator::executeVariableAssignment(const VariableAssignment& node) {
    Value value = evaluateExpression(*node.value);
    environment.assign(node.name, value, node.position);
}

void Evaluator::executePrintStatement(const PrintStatement& node) {
    Value value = evaluateExpression(*node.expression);
    std::cout << value.toString() << std::endl;
}

Value Evaluator::evaluateExpression(const ASTNode& node) {
    switch (node.type) {
        case ASTNodeType::LITERAL:
            return evaluateLiteral(static_cast<const Literal&>(node));
        case ASTNodeType::IDENTIFIER:
            return evaluateIdentifier(static_cast<const Identifier&>(node));
        case ASTNodeType::BINARY_EXPRESSION:
            return evaluateBinaryExpression(static_cast<const BinaryExpression&>(node));
        default:
            ErrorHandler::reportError("Unknown expression type", node.position);
    }
    return Value(nullptr);
}

Value Evaluator::evaluateLiteral(const Literal& node) {
    switch (node.token.type) {
        case TokenType::STRING:
            return Value(node.token.value);
        case TokenType::INTEGER:
            return Value(std::stoi(node.token.value));
        case TokenType::FLOAT:
            return Value(std::stod(node.token.value));
        case TokenType::BOOLEAN:
            return Value(node.token.value == "true");
        case TokenType::NIL:
            return Value(nullptr);
        default:
            ErrorHandler::reportError("Unknown literal type", node.position);
    }
    return Value(nullptr);
}

Value Evaluator::evaluateIdentifier(const Identifier& node) {
    return environment.get(node.name, node.position);
}

Value Evaluator::evaluateBinaryExpression(const BinaryExpression& node) {
    Value left = evaluateExpression(*node.left);
    Value right = evaluateExpression(*node.right);
    
    return ArithmeticEvaluator::evaluateBinaryExpression(node, left, right);
}

} // namespace Lizard