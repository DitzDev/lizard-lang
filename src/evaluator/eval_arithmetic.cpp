#include "eval_arithmetic.h"
#include "error_handler.h"
#include <cmath>

namespace Lizard {

Value ArithmeticEvaluator::evaluateBinaryExpression(const BinaryExpression& node, 
                                                   const Value& left, const Value& right) {
    switch (node.operator_) {
        case BinaryOperator::ADD:
            return add(left, right, node.position);
        case BinaryOperator::SUBTRACT:
            return subtract(left, right, node.position);
        case BinaryOperator::MULTIPLY:
            return multiply(left, right, node.position);
        case BinaryOperator::DIVIDE:
            return divide(left, right, node.position);
        case BinaryOperator::INT_DIV:
            return integerDivide(left, right, node.position);
        case BinaryOperator::MODULO:
            return modulo(left, right, node.position);
        default:
            ErrorHandler::reportError("Unknown binary operator", node.position);
            return Value(nullptr);
    }
}

Value ArithmeticEvaluator::add(const Value& left, const Value& right, const Position& pos) {
    // String concatenation
    if (left.getType() == ValueType::STRING || right.getType() == ValueType::STRING) {
        return Value(left.toString() + right.toString());
    }
    
    // Numeric addition
    if (!isNumeric(left) || !isNumeric(right)) {
        ErrorHandler::reportError("Cannot add " + getTypeName(left) + " and " + getTypeName(right), pos);
    }
    
    // If either is float, result is float
    if (left.getType() == ValueType::FLOAT || right.getType() == ValueType::FLOAT) {
        return Value(toDouble(left) + toDouble(right));
    }
    
    // Both integers
    return Value(toInt(left) + toInt(right));
}

Value ArithmeticEvaluator::subtract(const Value& left, const Value& right, const Position& pos) {
    if (!isNumeric(left) || !isNumeric(right)) {
        ErrorHandler::reportError("Cannot subtract " + getTypeName(right) + " from " + getTypeName(left), pos);
    }
    
    // If either is float, result is float
    if (left.getType() == ValueType::FLOAT || right.getType() == ValueType::FLOAT) {
        return Value(toDouble(left) - toDouble(right));
    }
    
    // Both integers
    return Value(toInt(left) - toInt(right));
}

Value ArithmeticEvaluator::multiply(const Value& left, const Value& right, const Position& pos) {
    if (!isNumeric(left) || !isNumeric(right)) {
        ErrorHandler::reportError("Cannot multiply " + getTypeName(left) + " and " + getTypeName(right), pos);
    }
    
    // If either is float, result is float
    if (left.getType() == ValueType::FLOAT || right.getType() == ValueType::FLOAT) {
        return Value(toDouble(left) * toDouble(right));
    }
    
    // Both integers
    return Value(toInt(left) * toInt(right));
}

Value ArithmeticEvaluator::divide(const Value& left, const Value& right, const Position& pos) {
    if (!isNumeric(left) || !isNumeric(right)) {
        ErrorHandler::reportError("Cannot divide " + getTypeName(left) + " by " + getTypeName(right), pos);
    }
    
    double right_val = toDouble(right);
    if (right_val == 0.0) {
        ErrorHandler::reportError("Division by zero", pos);
    }
    
    // Division always returns float
    return Value(toDouble(left) / right_val);
}

Value ArithmeticEvaluator::integerDivide(const Value& left, const Value& right, const Position& pos) {
    if (!isNumeric(left) || !isNumeric(right)) {
        ErrorHandler::reportError("Cannot perform integer division on " + getTypeName(left) + " and " + getTypeName(right), pos);
    }
    
    int right_val = toInt(right);
    if (right_val == 0) {
        ErrorHandler::reportError("Division by zero", pos);
    }
    
    // Integer division always returns integer
    return Value(toInt(left) / right_val);
}

Value ArithmeticEvaluator::modulo(const Value& left, const Value& right, const Position& pos) {
    if (!isNumeric(left) || !isNumeric(right)) {
        ErrorHandler::reportError("Cannot perform modulo on " + getTypeName(left) + " and " + getTypeName(right), pos);
    }
    
    int right_val = toInt(right);
    if (right_val == 0) {
        ErrorHandler::reportError("Modulo by zero", pos);
    }
    
    // Modulo always returns integer
    return Value(toInt(left) % right_val);
}

bool ArithmeticEvaluator::isNumeric(const Value& value) {
    ValueType type = value.getType();
    return type == ValueType::INTEGER || type == ValueType::FLOAT;
}

double ArithmeticEvaluator::toDouble(const Value& value) {
    switch (value.getType()) {
        case ValueType::INTEGER:
            return static_cast<double>(std::get<int>(value.data));
        case ValueType::FLOAT:
            return std::get<double>(value.data);
        default:
            return 0.0; // Should never reach here if isNumeric check is done
    }
}

int ArithmeticEvaluator::toInt(const Value& value) {
    switch (value.getType()) {
        case ValueType::INTEGER:
            return std::get<int>(value.data);
        case ValueType::FLOAT:
            return static_cast<int>(std::get<double>(value.data));
        default:
            return 0; // Should never reach here if isNumeric check is done
    }
}

std::string ArithmeticEvaluator::getTypeName(const Value& value) {
    switch (value.getType()) {
        case ValueType::STRING:
            return "string";
        case ValueType::INTEGER:
            return "integer";
        case ValueType::FLOAT:
            return "float";
        case ValueType::BOOLEAN:
            return "boolean";
        case ValueType::NIL:
            return "nil";
        default:
            return "unknown";
    }
}
}