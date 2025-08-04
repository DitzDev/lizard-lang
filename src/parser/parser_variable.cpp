#include "parser.h"
#include "error_handler.h"
#include <set>
#include <cctype>

namespace Lizard {

// Additional variable parsing utilities and validation
namespace ParserUtils {

bool isValidVariableName(const std::string& name) {
    if (name.empty()) return false;
    
    // Must start with letter or underscore
    if (!std::isalpha(name[0]) && name[0] != '_') {
        return false;
    }
    
    // Rest can be alphanumeric or underscore
    for (size_t i = 1; i < name.length(); ++i) {
        if (!std::isalnum(name[i]) && name[i] != '_') {
            return false;
        }
    }
    
    return true;
}

bool isReservedKeyword(const std::string& name) {
    static const std::set<std::string> reserved = {
        "put", "var", "fix", "true", "false", "nil",
        "str", "int", "float", "bool"
    };
    
    return reserved.find(name) != reserved.end();
}

void validateVariableName(const std::string& name, const Position& pos) {
    if (!isValidVariableName(name)) {
        ErrorHandler::reportError("Invalid variable name '" + name + "'", pos);
    }
    
    if (isReservedKeyword(name)) {
        ErrorHandler::reportError("Cannot use reserved keyword '" + name + "' as variable name", pos);
    }
}

} // namespace ParserUtils

} // namespace Lizard