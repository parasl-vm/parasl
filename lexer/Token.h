#ifndef PARASL_TOKENS_H_
#define PARASL_TOKENS_H_

namespace lexer {
enum class Token {
    EQ = 0,
    NE,
    GE,
    LE,
    GT,
    LT,
    PLUS,
    MINUS,
    MUL,
    DIV,
    ASSIGN,
    LAYER,
    INPUT,
    OUTPUT,
    REPEAT,
    GLUE,
    BIND,
    IF,
    ELSE,
    WHILE,
    FOR,
    IN,
    RETURN,
    STRING,
    CHAR,
    INT,
    FLOAT,
    DOUBLE,
    VECTOR,
    INVALID_TOKEN
};
}   // namespace lexer

#endif  // PARASL_TOKENS_H_
