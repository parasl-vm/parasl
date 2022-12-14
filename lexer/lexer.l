%{
    #include <string>
    #include <iostream>
    #include <vector>
    #include <fstream>
    #include <sstream>
    #include "Token.h"

    #define TOKEN(NAME) return yytokentype::NAME

    // This will track current scanner location.
    // Action is called when length of the token is known.
    #define YY_USER_ACTION m_driver.increaseLocation(yyleng);

    /* using "token" to make the returns for the tokens shorter to type */
    // using token = yytokentype;

    int yyterminate();
%}

%option nodefault
%option c++
%option noyywrap
%option yyclass="Lexer"
%option prefix="ParaSL_"


DIGIT       [0-9]
INT_VAL     [+-]?{DIGIT}+
FLOAT_VAL   [+-]?{DIGIT}+"."{DIGIT}+([eE][+-]?{DIGIT}+)?

ID          [a-zA-Z_][a-zA-Z0-9_]*
STRING      ["]([^\n"]|(\"))*["]
WHITESPACE  [ \t\n]+

EQ          ==
NE          !=
GE          >=
LE          [<]=

DOUBLE_DOT  [.][.]

LAYER       layer
INPUT       input
OUTPUT      output
GLUE        glue
IF          if
ELSE        else
WHILE       while
FOR         for
IN          in
RETURN      return
CHAR        char
INT         int
FLOAT       float
DOUBLE      double
VECTOR      vector

%%

{WHITESPACE}        { /* eat up whitespace */ }
"//".*              { /* eat up comments */ }

[+-/*<>\[\]{}();:=,] {
    return *yytext;
}

{EQ} {
    std::cout << "[LEXER]: EQ" << std::endl;
    TOKEN(EQ);
}
{NE} {
    std::cout << "[LEXER]: NE" << std::endl;
    TOKEN(NE);
}
{GE} {
    std::cout << "[LEXER]: GE" << std::endl;
    TOKEN(GE);
}
{LE} {
    std::cout << "[LEXER]: LE" << std::endl;
    TOKEN(LE);
}

{DOUBLE_DOT} {
    std::cout << "[LEXER]: DOUBLE_DOT" << std::endl;
    TOKEN(DOUBLE_DOT);
}

{LAYER} {
    std::cout << "[LEXER]: LAYER" << std::endl;
    TOKEN(LAYER);
}

{INPUT} {
    std::cout << "[LEXER]: INPUT" << std::endl;
    TOKEN(INPUT);
}
{OUTPUT} {
    std::cout << "[LEXER]: OUTPUT" << std::endl;
    TOKEN(OUTPUT);
}
{GLUE} {
    std::cout << "[LEXER]: GLUE" << std::endl;
    TOKEN(GLUE);
}

{IF} {
    std::cout << "[LEXER]: IF" << std::endl;
    TOKEN(IF);
}
{ELSE} {
    std::cout << "[LEXER]: ELSE" << std::endl;
    TOKEN(ELSE);
}
{WHILE} {
    std::cout << "[LEXER]: WHILE" << std::endl;
    TOKEN(WHILE);
}
{FOR} {
    std::cout << "[LEXER]: FOR" << std::endl;
    TOKEN(FOR);
}
{IN} {
    std::cout << "[LEXER]: IN" << std::endl;
    TOKEN(IN);
}
{RETURN} {
    std::cout << "[LEXER]: RETURN" << std::endl;
    TOKEN(RETURN);
}

{CHAR} {
    std::cout << "[LEXER]: CHAR" << std::endl;
    TOKEN(CHAR);
}
{INT} {
    std::cout << "[LEXER]: INT" << std::endl;
    TOKEN(INT);
}
{FLOAT} {
    std::cout << "[LEXER]: FLOAT" << std::endl;
    TOKEN(FLOAT);
}
{DOUBLE} {
    std::cout << "[LEXER]: DOUBLE" << std::endl;
    TOKEN(DOUBLE);
}
{VECTOR} {
    std::cout << "[LEXER]: VECTOR" << std::endl;
    TOKEN(VECTOR);
}

{INT_VAL} {
    std::cout << "[LEXER]: INT_VAL: " << yytext << std::endl;
    int64_t value = 0;
    std::istringstream{yytext} >> value;
    auto *node = new frontend::LiteralNode(yytokentype::INT_VAL, value);
    return node;
    // yylval.literal_node_t = node;
    TOKEN(INT_VAL);
}
{FLOAT_VAL} {
    std::cout << "[LEXER]: FLOAT_VAL" << std::endl;
    double value = 0;
    std::istringstream{yytext} >> value;
    auto *node = new frontend::LiteralNode(yytokentype::FLOAT_VAL, value);
    return node;
    TOKEN(FLOAT_VAL);
}

{ID} {
    std::cout << "[LEXER]: ID: " << yytext << std::endl;
    lexer::IDNode* node = new lexer::IDNode(yytext);
    return node;
    // yylval.id_node_t = node;
    TOKEN(ID);
}

{STRING} {
    std::cout << "[LEXER]: STRING: " << yytext << std::endl;
    return std::string{yytext};
    TOKEN(STRING);
}

. {
    std::cout << "[LEXER]: Token not found: " << yytext << "\n";
}

<<EOF>>     { return yyterminate(); }

%%


int yyterminate() {
    std::cout << "Reached <EOF>" << std::endl;
    return 0;
}
