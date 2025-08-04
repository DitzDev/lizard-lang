#pragma once
#include "ast.h"
#include "value.h"

namespace Lizard {

class ArithmeticEvaluator {
public:
    static Value evaluateBinaryExpression(const BinaryExpression& node, 
                                        const Value& left, const Value& right);
    
private:
    static Value add(const Value& left, const Value& right, const Position& pos);
    static Value subtract(const Value& left, const Value& right, const Position& pos);
    static Value multiply(const Value& left, const Value& right, const Position& pos);
    static Value divide(const Value& left, const Value& right, const Position& pos);
    static Value integerDivide(const Value& left, const Value& right, const Position& pos);
    static Value modulo(const Value& left, const Value& right, const Position& pos);
    
    static bool isNumeric(const Value& value);
    static double toDouble(const Value& value);
    static int toInt(const Value& value);
    static std::string getTypeName(const Value& value);
};

} // namespace Lizard