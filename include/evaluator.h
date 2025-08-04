#pragma once
#include "ast.h"
#include "value.h"
#include "environment.h"

namespace Lizard {

class Evaluator {
private:
    Environment environment;
    
public:
    Evaluator();
    
    void evaluate(const Program& program);
    
private:
    void executeStatement(const ASTNode& node);
    void executeVariableDeclaration(const VariableDeclaration& node);
    void executeVariableAssignment(const VariableAssignment& node);
    void executePrintStatement(const PrintStatement& node);
    
    Value evaluateExpression(const ASTNode& node);
    Value evaluateLiteral(const Literal& node);
    Value evaluateIdentifier(const Identifier& node);
    Value evaluateBinaryExpression(const BinaryExpression& node);
};

} // namespace Lizard