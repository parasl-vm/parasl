#ifndef PARASL_TOKENS_H_
#define PARASL_TOKENS_H_

// namespace lexer {
enum yytokentype {
    EQ = 258,
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
    VECTOR, // ?
    INT_VAL,
    ID,
    COLON,
    SEMICOLON,
    COMMA,
    LEFT_ROUND_PARENTHESIS,
    RIGHT_ROUND_PARENTHESIS,
    LEFT_CURLY_PARENTHESIS,
    RIGHT_CURLY_PARENTHESIS,
    INVALID_TOKEN
};
// }   // namespace lexer

#endif  // PARASL_TOKENS_H_
