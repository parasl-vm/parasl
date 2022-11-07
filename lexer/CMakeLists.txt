set(SOURCES Lexer.cpp)

add_library(lexer STATIC ${SOURCES})

target_sources(lexer PUBLIC Token.h)

FLEX_TARGET(Lexer lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.cc)
