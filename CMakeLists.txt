cmake_minimum_required(VERSION 3.16)
project(Lizard VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

set(LIZARD_SOURCES
    src/main.cpp
    src/error/error_handler.cpp
    src/lexer/lexer.cpp
    src/lexer/lexer_identifier.cpp
    src/lexer/lexer_keywords.cpp
    src/lexer/lexer_number.cpp
    src/lexer/lexer_state.cpp
    src/lexer/lexer_string.cpp
    src/lexer/lexer_arithmetic.cpp
    src/parser/parser.cpp
    src/parser/parser_print.cpp
    src/parser/parser_variable.cpp
    src/parser/parser_arithmetic.cpp
    src/evaluator/evaluator.cpp
    src/evaluator/eval_print.cpp
    src/evaluator/eval_variable.cpp
    src/evaluator/eval_arithmetic.cpp
    src/types/environment.cpp
    src/types/value.cpp
)

add_executable(lizard ${LIZARD_SOURCES})

set_target_properties(lizard PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

install(TARGETS lizard
    RUNTIME DESTINATION bin
)