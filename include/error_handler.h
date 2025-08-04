#pragma once
#include "token.h"
#include <string>
#include <vector>
#include <stdexcept>

namespace Lizard {

class LizardError : public std::runtime_error {
public:
    Position position;
    std::string error_message;
    std::vector<std::string> source_lines;
    std::vector<std::pair<Position, std::string>> notes;

    LizardError(const std::string& message, const Position& pos);
    
    void addNote(const std::string& note, const Position& note_pos = Position());
    void setSourceLines(const std::vector<std::string>& lines);
    std::string formatError() const;
};

class ErrorHandler {
public:
    static void setSourceFile(const std::string& filename, const std::vector<std::string>& lines);
    static void reportError(const std::string& message, const Position& pos);
    static void reportErrorWithNote(const std::string& message, const Position& pos, 
                                   const std::string& note, const Position& note_pos = Position());
    static std::string highlightLine(const std::string& line, int column, int length = 1);

private:
    static std::string current_filename;
    static std::vector<std::string> source_lines;
};

} // namespace Lizard