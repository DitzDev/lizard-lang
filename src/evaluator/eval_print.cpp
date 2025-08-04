#include "evaluator.h"
#include "error_handler.h"

namespace Lizard {

// Additional print evaluation utilities
namespace PrintEvaluator {

std::string formatValue(const Value& value, bool pretty_print = false) {
    switch (value.getType()) {
        case ValueType::STRING:
            return pretty_print ? "\"" + value.toString() + "\"" : value.toString();
        case ValueType::INTEGER:
        case ValueType::FLOAT:
        case ValueType::BOOLEAN:
        case ValueType::NIL:
            return value.toString();
    }
    return "nil";
}

void validatePrintableValue(const Value& value, const Position& pos) {
    // All values in Lizard are printable, but we can add specific validation here
    // For example, checking for special cases or formatting requirements
    // TODO: Add specific validation
    (void)value; // Suppress unused parameter warning
    (void)pos;   // Suppress unused parameter warning
}

} // namespace PrintEvaluator

} // namespace Lizard