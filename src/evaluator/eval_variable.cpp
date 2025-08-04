#include "evaluator.h"
#include "error_handler.h"

namespace Lizard {

// Additional variable evaluation utilities
namespace VariableEvaluator {

void validateVariableAccess(const std::string& name, const Variable* var, const Position& pos) {
    if (!var) {
        ErrorHandler::reportError("Undefined variable '" + name + "'", pos);
    }
    
    if (!var->is_initialized) {
        ErrorHandler::reportErrorWithNote(
            "Variable '" + name + "' is used before being initialized.",
            pos,
            "Variable declared here.",
            var->declaration_position
        );
    }
}

void validateVariableAssignment(const std::string& name, const Variable* var, const Position& pos) {
    if (!var) {
        ErrorHandler::reportError("Undefined variable '" + name + "'", pos);
    }
    
    if (var->is_constant && var->is_initialized) {
        ErrorHandler::reportErrorWithNote(
            "A variable whose contents are fixed, the value cannot be changed.",
            pos,
            "Fixed variables are declared here.",
            var->declaration_position
        );
    }
}

bool isCompatibleType(const Value& value, ValueType expected_type) {
    return value.getType() == expected_type || expected_type == ValueType::NIL;
}

} // namespace VariableEvaluator

} // namespace Lizard