#include "value.h"
#include <sstream>

namespace Lizard {

Value::Value() : data(nullptr) {}

Value::Value(const std::string& str) : data(str) {}

Value::Value(int i) : data(i) {}

Value::Value(double f) : data(f) {}

Value::Value(bool b) : data(b) {}

Value::Value(std::nullptr_t) : data(nullptr) {}

ValueType Value::getType() const {
    if (std::holds_alternative<std::string>(data)) {
        return ValueType::STRING;
    } else if (std::holds_alternative<int>(data)) {
        return ValueType::INTEGER;
    } else if (std::holds_alternative<double>(data)) {
        return ValueType::FLOAT;
    } else if (std::holds_alternative<bool>(data)) {
        return ValueType::BOOLEAN;
    } else {
        return ValueType::NIL;
    }
}

std::string Value::toString() const {
    switch (getType()) {
        case ValueType::STRING:
            return std::get<std::string>(data);
        case ValueType::INTEGER:
            return std::to_string(std::get<int>(data));
        case ValueType::FLOAT: {
            std::ostringstream oss;
            oss << std::get<double>(data);
            return oss.str();
        }
        case ValueType::BOOLEAN:
            return std::get<bool>(data) ? "true" : "false";
        case ValueType::NIL:
            return "nil";
    }
    return "nil";
}

} // namespace Lizard