#include "error_handler.h"
#include <sstream>
#include <iostream>

namespace Lizard {

std::string ErrorHandler::current_filename;
std::vector<std::string> ErrorHandler::source_lines;

LizardError::LizardError(const std::string& message, const Position& pos)
    : std::runtime_error(message), position(pos), error_message(message) {}

void LizardError::addNote(const std::string& note, const Position& note_pos) {
    notes.emplace_back(note_pos, note);
}

void LizardError::setSourceLines(const std::vector<std::string>& lines) {
    source_lines = lines;
}

std::string LizardError::formatError() const {
    std::ostringstream oss;
    
    oss << "\033[1;31m" << position.filename << " (Line " << position.line << ", Column " << position.column << "): Error: " << "\033[0m" << error_message << "\n\n";

    if (!source_lines.empty() && position.line > 0 && position.line <= (int)source_lines.size()) {
        std::string line = source_lines[position.line - 1];
        oss << position.line << " | " << line << "\n";

        std::string pointer_line = std::string(std::to_string(position.line).length() + 3, ' ');
        for (int i = 1; i < position.column; ++i) {
            pointer_line += " ";
        }
        pointer_line += "^";

        size_t start_col = position.column - 1;
        size_t end_col = start_col;
        if (start_col < line.length()) {
            while (end_col < line.length() && !std::isspace(line[end_col]) && line[end_col] != '=' && line[end_col] != '"') {
                end_col++;
            }
            for (size_t i = start_col + 1; i < end_col && i < line.length(); ++i) {
                pointer_line += "~";
            }
        }
        
        oss << pointer_line << "\n";
    }

    for (const auto& note : notes) {
        oss << "\033[34m" << "~ Note: " << "\033[0m" << note.second << "\n";
        if (note.first.line > 0 && !source_lines.empty() && note.first.line <= (int)source_lines.size()) {
            oss << "\n" << note.first.line << " | " << source_lines[note.first.line - 1] << "\n";
        }
    }
    
    return oss.str();
}

void ErrorHandler::setSourceFile(const std::string& filename, const std::vector<std::string>& lines) {
    current_filename = filename;
    source_lines = lines;
}

void ErrorHandler::reportError(const std::string& message, const Position& pos) {
    LizardError error(message, pos);
    error.setSourceLines(source_lines);
    throw error;
}

void ErrorHandler::reportErrorWithNote(const std::string& message, const Position& pos, 
                                     const std::string& note, const Position& note_pos) {
    LizardError error(message, pos);
    error.setSourceLines(source_lines);
    error.addNote(note, note_pos);
    throw error;
}

std::string ErrorHandler::highlightLine(const std::string& line, int column, int length) {
    std::string result = line;
    if (column > 0 && column <= (int)line.length()) {
        // Simple highlighting by adding markers
        result = line.substr(0, column - 1) + ">>>" + 
                line.substr(column - 1, length) + "<<<" + 
                line.substr(column - 1 + length);
    }
    return result;
}

} // namespace Lizard