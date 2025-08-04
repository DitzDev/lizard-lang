#include "parser.h"
#include "error_handler.h"

namespace Lizard {

// Additional print statement parsing utilities
namespace PrintParser {

void validatePrintExpression(const ASTNode* expr, const Position& pos) {
    if (!expr) {
        ErrorHandler::reportError("Expected expression after 'put'", pos);
    }
    
    // Additional validation can be added here
    // For example, checking if the expression is printable
}

} // namespace PrintParser

} // namespace Lizard