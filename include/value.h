#pragma once
#include <string>
#include <variant>

namespace Lizard {

enum class ValueType {
    STRING,
    INTEGER,
    FLOAT,
    BOOLEAN,
    NIL
};

class Value {
public:
    std::variant<std::string, int, double, bool, std::nullptr_t> data;
    
    Value();
    Value(const std::string& str);
    Value(int i);
    Value(double f);
    Value(bool b);
    Value(std::nullptr_t);
    
    ValueType getType() const;
    std::string toString() const;
    
    template<typename T>
    T get() const {
        return std::get<T>(data);
    }
    
    bool isString() const { return getType() == ValueType::STRING; }
    bool isInteger() const { return getType() == ValueType::INTEGER; }
    bool isFloat() const { return getType() == ValueType::FLOAT; }
    bool isBoolean() const { return getType() == ValueType::BOOLEAN; }
    bool isNil() const { return getType() == ValueType::NIL; }
    
    template<typename T>
    const T& get() const {
        return std::get<T>(data);
    }
};

} // namespace Lizard