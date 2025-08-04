#pragma once
#include "value.h"
#include "token.h"
#include <unordered_map>
#include <string>

namespace Lizard {

struct Variable {
    Value value;
    bool is_constant;
    bool is_initialized;
    Position declaration_position;
    
    Variable(const Value& v, bool is_const, bool init, const Position& pos)
        : value(v), is_constant(is_const), is_initialized(init), declaration_position(pos) {}
};

class Environment {
public:
    void define(const std::string& name, const Value& value, bool is_constant, const Position& pos);
    void assign(const std::string& name, const Value& value, const Position& assign_pos);
    Value get(const std::string& name, const Position& access_pos);
    bool exists(const std::string& name) const;
    Variable* getVariable(const std::string& name);

private:
    std::unordered_map<std::string, Variable> variables;
};

} // namespace Lizard