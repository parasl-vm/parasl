  /* %option c++ */
%option yylineno
%option noyywrap

%{
    #include <string>
    #include <iostream>
    #include <vector>
    #include <fstream>
    #include <cstdlib>

    // #include "Token.h"

    #include "../parser/parser.tab.h"

    #define TOKEN(NAME) return yytokentype::NAME

    // void yyerror(const char *s);

    std::vector<int> tokens; // not needed
    // yyFlexLexer* my_lexer;

    // union DataType
    // {
    //     int64_t int_value;
    //     std::string string_value;
    // };

    // DataType yylval;
%}

/* TODO(dslynko): no leading zeros */
DIGIT       [0-9]
INT_VAL     [+-]?{DIGIT}+
FLOAT_VAL   [+-]?{DIGIT}+"."{DIGIT}+([eE][+-]?{DIGIT}+)?

ID          [a-z_][a-z0-9_]*
/* STRING      ["]([^\n"](\")?)+["] */
WHITESPACE  [ \t\n]+

EQ          ==
NE          !=
GE          >=
LE          [<]=

LAYER       layer
INPUT       input
OUTPUT      output
REPEAT      repeat
GLUE        glue
BIND        bind
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

{WHITESPACE}        /* eat up whitespace */
  /* [/][/]["].*\n        eat up comments? */

[+-/*<>\[\]{}();:=,]    { return *yytext; }

  /* [(]                       { tokens.push_back(TOKEN(LEFT_ROUND_PARENTHESIS)); } */
  /* [)]                       { tokens.push_back(TOKEN(RIGHT_ROUND_PARENTHESIS)); } */
  /* [;]                       { tokens.push_back(TOKEN(SEMICOLON)); } */
  /* [:]                       { tokens.push_back(TOKEN(COLON)); } */
  /* [=]                       { tokens.push_back(TOKEN(ASSIGN)); } */
  /* [,]                       { tokens.push_back(TOKEN(COMMA)); } */

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
{REPEAT} {
    std::cout << "[LEXER]: REPEAT" << std::endl;
    TOKEN(REPEAT);
}
{GLUE} {
    std::cout << "[LEXER]: GLUE" << std::endl;
    TOKEN(GLUE);
}
{BIND} {
    std::cout << "[LEXER]: BIND" << std::endl;
    TOKEN(BIND);
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
    lexer::LiteralNode* node = new lexer::LiteralNode(yytokentype::INT_VAL, yytext);
    // yylval.literal_node_t = node;
    TOKEN(INT_VAL);
}
{FLOAT_VAL} {
    std::cout << "[LEXER]: FLOAT_VAL" << std::endl;
    lexer::LiteralNode* node = new lexer::LiteralNode(yytokentype::FLOAT_VAL, yytext);
    TOKEN(FLOAT_VAL);
}
{ID} {
    std::cout << "[LEXER]: ID: " << yytext << std::endl;
    lexer::IDNode* node = new lexer::IDNode(yytext);
    // yylval.id_node_t = node;
    TOKEN(ID);
}
  /* {STRING}                { TOKEN(STRING); } */

. {
    std::cout << "[LEXER]: Token not found " << *yytext << "\n";
}

%%

// void yyerror(const char *s)
// {
//     std::cout << s << "\n";
// }

// int main(int argc, char **argv)
// {
//     std::ifstream input(argv[1]);

//     my_lexer = new yyFlexLexer(input, std::cout);
//     while(my_lexer->yylex() != 0);

//     for (auto item: tokens) {
//         std::cout << item << " ";
//     }
//     std::cout << "\n";

//     delete my_lexer;
//     return 0;
// }
