%option c++
%option yylineno
%option noyywrap

%{
    #include <string>
    #include "Token.h"

    #define TOKEN(NAME) return lexer::Token::NAME

    void yyerror(const char *s);
%}

DIGIT       [0-9]
INT_VAL     [+-]{DIGIT}+
FLOAT_VAL   {DIGIT}+("."{DIGIT}+)?([eE][+-]?{DIGIT}+)?

ID          [a-zA-Z][a-zA-Z0-9_]*
STRING      ["][^\n"]+["]
WHITESPACE  [ \t]

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

{WHITESPACE}
[/][/]["].*\n

[+-/*=<>;:,()\[\]{}]    { return *yytext; }

{EQ}                    { TOKEN(EQ); }
{NE}                    { TOKEN(NE); }
{GE}                    { TOKEN(GE); }
{LE}                    { TOKEN(LE); }

{LAYER}                 { TOKEN(LAYER); }
{INPUT}                 { TOKEN(INPUT); }
{OUTPUT}                { TOKEN(OUTPUT); }
{REPEAT}                { TOKEN(REPEAT); }
{GLUE}                  { TOKEN(GLUE); }
{BIND}                  { TOKEN(BIND); }
{IF}                    { TOKEN(IF); }
{ELSE}                  { TOKEN(ELSE); }
{WHILE}                 { TOKEN(WHILE); }
{FOR}                   { TOKEN(FOR); }
{IN}                    { TOKEN(IN); }
{RETURN}                { TOKEN(RETURN); }
{CHAR}                  { TOKEN(CHAR); }
{INT}                   { TOKEN(INT); }
{FLOAT}                 { TOKEN(FLOAT); }
{DOUBLE}                { TOKEN(DOUBLE); }
{VECTOR}                { TOKEN(VECTOR); }

{INT_VAL}               { TOKEN(INT_VAL); }
{FLOAT_VAL}             { TOKEN(FLOAT_VAL); }

{ID}                    { TOKEN(ID); }
{STRING}                { TOKEN(STRING); }

.                       { yyerror(""); }

%%
