#include "environment.h"
#include "error_handler.h"

namespace Lizard {

void Environment::define(const std::string& name, const Value& value, bool is_constant, const Position& pos) {
    if (exists(name)) {
        ErrorHandler::reportError("Variable '" + name + "' is already defined", pos);
    }
    
    variables.emplace(name, Variable(value, is_constant, true, pos));
}

void Environment::assign(const std::string& name, const Value& value, const Position& assign_pos) {
    auto it = variables.find(name);
    if (it == variables.end()) {
        ErrorHandler::reportError("Undefined variable '" + name + "'", assign_pos);
    }
    
    Variable& var = it->second;
    
    if (var.is_constant && var.is_initialized) {
        ErrorHandler::reportErrorWithNote(
            "A variable whose contents are fixed, the value cannot be changed.",
            assign_pos,
            "Fixed variables are declared here.",
            var.declaration_position
        );
    }
    
    var.value = value;
    var.is_initialized = true;
}

Value Environment::get(const std::string& name, const Position& access_pos) {
    auto it = variables.find(name);
    if (it == variables.end()) {
        ErrorHandler::reportError("Undefined variable '" + name + "'", access_pos);
    }
    
    Variable& var = it->second;
    if (!var.is_initialized) {
        ErrorHandler::reportErrorWithNote(
            "Variable '" + name + "' is used before being initialized.",
            access_pos,
            "Variable declared here.",
            var.declaration_position
        );
    }
    
    return var.value;
}

bool Environment::exists(const std::string& name) const {
    return variables.find(name) != variables.end();
}

Variable* Environment::getVariable(const std::string& name) {
    auto it = variables.find(name);
    if (it == variables.end()) {
        return nullptr;
    }
    return &it->second;
}

} // namespace Lizard