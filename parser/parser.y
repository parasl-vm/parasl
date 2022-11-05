%language "c++"

%define parse.error verbose

%debug

%code requires {
    #include "ast/AssignmentNode.h"
    #include "ast/BinaryOpNode.h"
    #include "ast/CompoundStatementNode.h"
    #include "ast/DeclarationNode.h"
    #include "ast/ExpressionNode.h"
    #include "ast/ForStatementNode.h"
    #include "ast/FunctionCallNode.h"
    #include "ast/IDNode.h"
    #include "ast/IfStatementNode.h"
    #include "ast/LayerNode.h"
    #include "ast/LiteralNode.h"
    #include "ast/ReturnStatementNode.h"
    #include "ast/StatementNode.h"
    #include "ast/TypeNode.h"
    #include "ast/WhileStatementNode.h"
    #include <iostream>

    #undef yylex
    #define yylex driver.lexer->lex

    extern int yylineno;
    extern int yylex();

    void yyerror(const char *s) {
      std::cerr << s << ", line " << yylineno << std::endl;
      exit(1);
    }
%}

%union {
    std::vector<ASTNode *>  args_t;
    AssignmentNode *        assignment_node_t;
    CompoundStatementNode * compound_stmt_node_t;
    ExpressionNode *        expression_node_t;
    IDNode *                id_node_t;
    IterationRange *        iter_range_node_t;
    LayerNode *             layer_node_t;
    LiteralNode *           literal_node_t;
    PrimitiveTypeNode *     primitive_type_t;
    StatementNode *         stmt_node_t;
    TypeNode *              type_node_t;
}

%token INT_VAL FLOAT_VAL STRING ID
%token EQ NE GE LE
%token LAYER// INPUT OUTPUT REPEAT GLUE BIND
%token IF ELSE WHILE FOR IN RETURN
%token CHAR INT FLOAT DOUBLE VECTOR

%type<args_t>               ARGS
%type<assignment_node_t>    ASSIGNMENT
%type<compound_stmt_node_t> STMTS
%type<expression_node_t>    EXPR EXPR2 TERM VAL OPT_RET_VAL
%type<id_node_t>            ID
%type<iter_range_node_t>    ITER_RANGE
%type<layer_node_t>         LAYER_DEF PROGRAM
%type<literal_node_t>       LITERAL INT_VAL FLOAT_VAL STRING
%type<primitive_type_t>     PRIMITIVE
%type<stmt_node_t>          STMT STMT1 STMT2
%type<type_node_t>          OPT_TYPE_DECL TYPE TYPE_DECL

%left '+' '-'
%left '*' '/'

%start PROGRAM

%%

PROGRAM         : LAYER_DEF
                | STMTS
                {
                    $$ = new CompoundStatementNode();
                    $$ = new LayerNode(0, "", $$);
                }
;

LAYER_DEF       : LAYER '(' INT_VAL ',' STRING ')' '{' STMTS '}'
                {
                    $$ = new CompoundStatementNode();
                    $$ = new LayerNode($3, $5, $$);
                }
;

STMTS           : STMT
                {
                    $$.push_back($1);
                }
                | STMTS STMT
                {
                    $$ = $1;
                    $$.push_back($2);
                }
;

STMT            : STMT1
                | STMT2
;

STMT1           : ASSIGNMENT
                | RETURN OPT_RET_VAL ';'
                {
                    $$ = new ReturnStatementNode($2);
                }
                | '{' STMTS '}'
                {
                    $$ = new CompoundStatementNode();
                }
                | IF '(' EXPR ')' STMT1 ELSE STMT1
                {
                    $$ = new IfStatementNode($3, $5, $7);
                }
                | WHILE '(' EXPR ')' STMT1
                {
                    $$ = new WhileStatementNode($3, $5);
                }
                | FOR '(' ITER_RANGE ')' STMT1
                {
                    $$ = new ForStatementNode($3, $5);
                }
;

STMT2           : IF '(' EXPR ')' STMT
                {
                    $$ = new IfStatementNode($3, $5);
                }
                | IF '(' EXPR ')' STMT1 ELSE STMT2
                {
                    $$ = new IfStatementNode($3, $5, $7);
                }
                | WHILE '(' EXPR ')' STMT2
                {
                    $$ = new WhileStatementNode($3, $5);
                }
                | FOR '(' ITER_RANGE ')' STMT2
                {
                    $$ = new ForStatementNode($3, $5);
                }
;

OPT_RET_VAL     : %empty
                {
                    $$ = new ExpressionNode(nullptr);
                }
                | EXPR
;

ITER_RANGE      : ID IN ID
                {
                    $$ = new SequenceIterationRange($1, $3);
                }
                | ID IN INT_VAL ':' INT_VAL
                {
                    $$ = new NumericIterationRange($1, $3, $5);
                }
                | ID IN INT_VAL ':' INT_VAL ':' INT_VAL
                {
                    $$ = new NumericIterationRange($1, $3, $5, $7);
                }
;

PRIMITIVE       : CHAR
                {
                    $$ = new PrimitiveTypeNode(PrimitiveType::CHAR);
                }
                | INT
                {
                    $$ = new PrimitiveTypeNode(PrimitiveType::INT);
                }
                | FLOAT
                {
                    $$ = new PrimitiveTypeNode(PrimitiveType::FLOAT);
                }
                | DOUBLE
                {
                    $$ = new PrimitiveTypeNode(PrimitiveType::DOUBLE);
                }
;

TYPE            : PRIMITIVE
                | INT '(' INT_VAL ')'
                {
                    $$ = new PrimitiveTypeNode(PrimitiveType::INT, $3);
                }
                | TYPE '[' INT_VAL ']'
                {
                    $$ = new SequenceTypeNode(AggregateType::ARRAY, $1, $3);
                }
                | VECTOR '<' TYPE ',' INT_VAL '>'
                {
                    $$ = new SequenceTypeNode(AggregateType::VECTOR, $3, $5);
                }
;

TYPE_DECL       : ':' TYPE
                {
                    $$ = $2;
                }
;

OPT_TYPE_DECL   : %empty
                {
                    $$ = new TypeNode(nullptr);
                }
                | TYPE_DECL
;

ASSIGNMENT      : ID OPT_TYPE_DECL '=' EXPR ';'
                {
                    $$ = new AssignmentNode($1, $4);
                }
;

EXPR            : EXPR2
                | EXPR EQ EXPR2
                {
                    $$ = new BinOpNode(Token::EQ, $1, $3);
                }
                | EXPR NE EXPR2
                {
                    $$ = new BinOpNode(Token::NE, $1, $3);
                }
                | EXPR GE EXPR2
                {
                    $$ = new BinOpNode(Token::GE, $1, $3);
                }
                | EXPR LE EXPR2
                {
                    $$ = new BinOpNode(Token::LE, $1, $3);
                }
                | EXPR '>' EXPR2
                {
                    $$ = new BinOpNode(Token::GT, $1, $3);
                }
                | EXPR '<' EXPR2
                {
                    $$ = new BinOpNode(Token::LT, $1, $3);
                }
;

EXPR2           : TERM
                | TERM '+' TERM
                {
                    $$ = new BinOpNode(Token::PLUS, $1, $3);
                }
                | TERM '-' TERM
                {
                    $$ = new BinOpNode(Token::MINUS, $1, $3);
                }
;

TERM            : VAL
                | TERM '*' TERM
                {
                    $$ = new BinOpNode(Token::MUL, $1, $3);
                }
                | TERM '/' TERM
                {
                    $$ = new BinOpNode(Token::DIV, $1, $3);
                }
;

LITERAL         : INT_VAL
                {
                    $$ = new LiteralNode(Token::INT_VAL, $1);
                }
                | FLOAT_VAL
                {
                    $$ = new LiteralNode(Token::FLOAT_VAL, $1);
                }
                | STRING
                {
                    $$ = new LiteralNode(Token::STRING, $1);
                }
;

VAL             : LITERAL
                | '(' EXPR ')'
                {
                    $$ = new ExpressionNode($2);
                }
                | ID
                {
                    $$ = new IDNode($1);
                }
                | ID '(' ARGS ')'
                {
                    $$ = new FunctionCallNode($1, $3);
                }
;

ARGS            : EXPR
                {
                    // $$.clear();
                    $$.push_back($1);
                }
                | ARGS ',' EXPR
                {
                    $$ = $1;
                    $$.push_back($3);
                }
;

%%
