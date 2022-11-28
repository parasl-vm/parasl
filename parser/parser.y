// %language "c++"

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

    // #undef yylex
    // #define yylex driver.lexer->lex

    extern int yylineno;
    extern int yylex();
    extern FILE* yyin;

    void yyerror(const char *s);
}

%union {
    // std::vector<FunctionArgument>   args_decl_t;
    // std::vector<ASTNode *>          args_t;
    // AssignmentNode *                assignment_node_t;
    // CompoundStatementNode *         compound_stmt_node_t;
    // ExpressionNode *                expression_node_t;
    lexer::IDNode *                        id_node_t;
    // IterationRange *                iter_range_node_t;
    // LayerNode *                     layer_node_t;
    lexer::LiteralNode *                   literal_node_t;
    // PrimitiveTypeNode *             primitive_type_t;
    // StatementNode *                 stmt_node_t;
    lexer::TypeNode *                      type_node_t;
}

// %token STRING
%token EQ NE GE LE
%token LAYER REPEAT GLUE BIND
%token IF ELSE WHILE FOR IN RETURN
%token CHAR // lexer::Token::CHAR
%token INT // lexer::Token::INT
%token FLOAT // lexer::Token::FLOAT
%token DOUBLE // lexer::Token::DOUBLE
%token VECTOR // lexer::Token::VECTOR


%token INT_VAL // lexer::Token::INT_VAL
%token FLOAT_VAL
%token ID // lexer::Token::ID
%token INPUT // lexer::Token::INPUT
%token OUTPUT // lexer::Token::OUTPUT

// %type<args_decl_t>          ARGS_DECL
// %type<args_t>               ARGS
// %type<assignment_node_t>    ASSIGNMENT
// %type<compound_stmt_node_t> STMTS
// %type<expression_node_t>    EXPR EXPR2 TERM VAL // OPT_RET_VAL
%type<id_node_t>            ID
// %type<iter_range_node_t>    ITER_RANGE
// %type<layer_node_t>         PROGRAM // LAYER_DEF
%type<literal_node_t>       LITERAL INT_VAL // STRING FLOAT_VAL
// %type<primitive_type_t>     PRIMITIVE
// %type<stmt_node_t>          STMT STMT1 // STMT2
%type<type_node_t>          NON_FUNC_TYPE OPT_TYPE_DECL TYPE PRIMITIVE // OPT_NON_FUNC_TYPE

%left '+' '-'
%left '*' '/'
%left '<' '>'
%left EQ NE GE LE

%start PROGRAM

%%

PROGRAM         : /* LAYER_DEF */
                STMTS
                {
                    // $$ = new CompoundStatementNode();
                    // $$ = new LayerNode(0, "", $$);
                    std::cout << "[PARSER]: " << "PROGRAM" << std::endl;
                }
;

// LAYER_DEF       : LAYER '(' INT_VAL ',' STRING ')' '{' STMTS '}'
//                 {
//                     $$ = new CompoundStatementNode();
//                     $$ = new LayerNode($3, $5, $$);
//                 }
// ;

STMTS           : STMT
                {
                    // $$.push_back($1);
                    std::cout << "[PARSER]: " << "STMTS1" << std::endl;
                }
                | STMTS STMT
                {
                    std::cout << "[PARSER]: " << "STMTS2" << std::endl;
                    // $$ = $1;
                    // $$.push_back($2);
                }
;

STMT            : STMT1
                | STMT2
;

STMT1           : ASSIGNMENT
                | OUTPUT '(' ARGS ')' ';'
                {
                    std::cout << "[PARSER]: OUTPUT ( ARGS ) ;" << std::endl;
                }
                | RETURN OPT_RET_VAL ';'
                {
                    std::cout << "[PARSER]: RETURN OPT_RET_VAL ;" << std::endl;
                //     $$ = new ReturnStatementNode($2);
                }
                | IF '(' EXPR ')' '{' STMTS '}' ELSE '{' STMTS '}'
                {
                    std::cout << "[PARSER]: IF ( EXPR ) { STMTS } ELSE { STMTS }" << std::endl;
                //     $$ = new IfStatementNode($3, $5, $7);
                }
                | IF '(' EXPR ')' '{' STMTS '}' ELSE STMT1
                {
                    std::cout << "[PARSER]: IF ( EXPR ) { STMTS } ELSE STMT1" << std::endl;
                //     $$ = new IfStatementNode($3, $5, $7);
                }
                | IF '(' EXPR ')' STMT1 ELSE '{' STMTS '}'
                {
                    std::cout << "[PARSER]: IF ( EXPR ) STMT1 ELSE { STMTS }" << std::endl;
                //     $$ = new IfStatementNode($3, $5, $7);
                }
                | IF '(' EXPR ')' STMT1 ELSE STMT1
                {
                    std::cout << "[PARSER]: IF ( EXPR ) STMT1 ELSE STMT1" << std::endl;
                //     $$ = new IfStatementNode($3, $5, $7);
                }
                | WHILE '(' EXPR ')' '{' STMTS '}'
                {
                    std::cout << "[PARSER]: WHILE ( EXPR ) { STMTS }" << std::endl;
                //     $$ = new WhileStatementNode($3, $5);
                }
                | WHILE '(' EXPR ')' STMT1
                {
                    std::cout << "[PARSER]: WHILE ( EXPR ) STMT1" << std::endl;
                //     $$ = new WhileStatementNode($3, $5);
                }
                | FOR '(' ITER_RANGE ')' '{' STMTS '}'
                {
                    std::cout << "[PARSER]: FOR ( ITER_RANGE ) { STMTS }" << std::endl;
                //     $$ = new ForStatementNode($3, $5);
                }
                | FOR '(' ITER_RANGE ')' STMT1
                {
                    std::cout << "[PARSER]: FOR ( ITER_RANGE ) STMT1" << std::endl;
                //     $$ = new ForStatementNode($3, $5);
                }
;

STMT2           : IF '(' EXPR ')' OPT_SCOPE
                {
                    std::cout << "[PARSER]: IF ( EXPR ) OPT_SCOPE" << std::endl;
//                     $$ = new IfStatementNode($3, $5);
                }
                | IF '(' EXPR ')' STMT1 ELSE STMT2
                {
                    std::cout << "[PARSER]: IF ( EXPR ) STMT1 ELSE STMT2" << std::endl;
//                     $$ = new IfStatementNode($3, $5, $7);
                }
                | WHILE '(' EXPR ')' STMT2
                {
                    std::cout << "[PARSER]: WHILE ( EXPR ) STMT2" << std::endl;
//                     $$ = new WhileStatementNode($3, $5);
                }
                | FOR '(' ITER_RANGE ')' STMT2
                {
                    std::cout << "[PARSER]: FOR ( ITER_RANGE ) STMT2" << std::endl;
//                     $$ = new ForStatementNode($3, $5);
                }
;

OPT_SCOPE       : STMT
                | '{' STMTS '}'
                {
                    std::cout << "[PARSER]: { STMTS }" << std::endl;
                }
;

OPT_RET_VAL     : %empty
                {
                    std::cout << "[PARSER]: empty RETURN;" << std::endl;
                //     $$ = new ExpressionNode(nullptr);
                }
                | EXPR
;

ITER_RANGE      : ID IN ID
                {
                    std::cout << "[PARSER]: ID IN ID" << std::endl;
                //     $$ = new SequenceIterationRange($1, $3);
                }
                | ID IN INT_VAL ':' INT_VAL
                {
                    std::cout << "[PARSER]: ID IN INT_VAL : INT_VAL" << std::endl;
                //     $$ = new NumericIterationRange($1, $3, $5);
                }
                | ID IN INT_VAL ':' INT_VAL ':' INT_VAL
                {
                    std::cout << "[PARSER]: ID IN INT_VAL : INT_VAL : INT_VAL" << std::endl;
                //     $$ = new NumericIterationRange($1, $3, $5, $7);
                }
;

PRIMITIVE       : CHAR
                {
                    // $$ = new PrimitiveTypeNode(PrimitiveType::CHAR, {});
                }
                | INT
                {
                    // $$ = new PrimitiveTypeNode(PrimitiveType::INT, {});
                    std::cout << "[PARSER]: " << "PRIMITIVE INT" << std::endl;
                }
                | FLOAT
                {
                    // $$ = new PrimitiveTypeNode(PrimitiveType::FLOAT, {});
                }
                | DOUBLE
                {
                    // $$ = new PrimitiveTypeNode(PrimitiveType::DOUBLE, {});
                }
;

TYPE            : NON_FUNC_TYPE
                // | '(' ARGS_DECL ')' OPT_NON_FUNC_TYPE
                // {
                //     $$ = new FunctionTypeNode($4);
                // }
;

// ARGS_DECL       : ID OPT_NON_FUNC_TYPE
//                 {
//                     $$.AddArgument($2, $1);
//                 }
//                 | ARGS_DECL ',' ID OPT_NON_FUNC_TYPE
//                 {
//                     $$ = $1;
//                     $$.AddArgument($4, $3);
//                 }
// ;

// OPT_NON_FUNC_TYPE   : %empty
//                     {
//                         $$ = new TypeNode(nullptr);
//                     }
//                     | ':' NON_FUNC_TYPE
//                     {
//                         $$ = $2;
//                     }
// ;

NON_FUNC_TYPE   : PRIMITIVE
                | INT '(' INT_VAL ')'
                {
                    std::cout << "[PARSER] INT ( INT_VAL )" << std::endl;
                //     $$ = new PrimitiveTypeNode(PrimitiveType::INT, $3);
                }
                /* | NON_FUNC_TYPE '[' INT_VAL ']'
                {
                    std::cout << "[PARSER] NON_FUNC_TYPE [ INT_VAL ]" << std::endl;
                //     $$ = new SequenceTypeNode(AggregateType::ARRAY, $1, $3);
                }
                | VECTOR '<' NON_FUNC_TYPE ',' INT_VAL '>'
                {
                    std::cout << "[PARSER] VECTOR < NON_FUNC_TYPE , INT_VAL >" << std::endl;
                //     $$ = new SequenceTypeNode(AggregateType::VECTOR, $3, $5);
                } */
;

OPT_TYPE_DECL   : %empty
                {
                    std::cout << "[PARSER]: " << "OPT_TYPE_DECL1" << std::endl;
                    // $$ = new TypeNode(nullptr);
                }
                | ':' TYPE
                {
                    std::cout << "[PARSER]: " << "OPT_TYPE_DECL2" << std::endl;
                    // $$ = $2;
                }
;

ASSIGNMENT      : ID OPT_TYPE_DECL '=' EXPR ';'
                {
                    std::cout << "[PARSER]: " << "ASSIGNMENT" << std::endl;
                    // $$ = new AssignmentNode($1, $4);
                }
;

EXPR            : EXPR2
                | COND_EXPR
;

COND_EXPR       : EXPR EQ EXPR
                {
                    std::cout << "[PARSER]: EQ" << std::endl;
                }
                | EXPR NE EXPR
                {
                    std::cout << "[PARSER]: NE" << std::endl;
                }
                | EXPR GE EXPR
                {
                    std::cout << "[PARSER]: GE" << std::endl;
                }
                | EXPR '>' EXPR
                {
                    std::cout << "[PARSER]: GT" << std::endl;
                }
                | EXPR LE EXPR
                {
                    std::cout << "[PARSER]: LE" << std::endl;
                }
                | EXPR '<' EXPR
                {
                    std::cout << "[PARSER]: LT" << std::endl;
                }
;

EXPR2           : TERM
                | TERM '+' EXPR2
                {
                    std::cout << "[PARSER]: " << "TERM + TERM" << std::endl;
                //     $$ = new BinOpNode(Token::PLUS, $1, $3);
                }
                | TERM '-' EXPR2
                {
                    std::cout << "[PARSER]: " << "TERM - TERM" << std::endl;
                //     $$ = new BinOpNode(Token::MINUS, $1, $3);
                }
;

TERM            : VAL
                | '(' EXPR ')'
                {
                    std::cout << "[PARSER]: " << "TERM -> ( EXPR )" << std::endl;
                }
                | TERM '*' TERM
                {
                    std::cout << "[PARSER]: " << "TERM * TERM" << std::endl;
                //     $$ = new BinOpNode(Token::MUL, $1, $3);
                }
                | TERM '/' TERM
                {
                    std::cout << "[PARSER]: " << "TERM / TERM" << std::endl;
                //     $$ = new BinOpNode(Token::DIV, $1, $3);
                }
;

LITERAL         : INT_VAL
                {
                    // $$ = new LiteralNode(Token::INT_VAL, $1);
                    std::cout << "[PARSER]: " << "INT_VAL" << std::endl;
                }
                | FLOAT_VAL
                {
                    std::cout << "[PARSER]: " << "FLOAT_VAL" << std::endl;
                //     $$ = new LiteralNode(Token::FLOAT_VAL, $1);
                }
                // | STRING
                // {
                //     $$ = new LiteralNode(Token::STRING, $1);
                // }
;

VAL             : LITERAL
                {
                    // $$ = new ExpressionNode($2);
                    std::cout << "[PARSER]: " << "LITERAL" << std::endl;
                }
                | ID
                {
                    // $$ = new IDNode($1);
                    std::cout << "[PARSER]: " << "VAL1" << std::endl;
                }
                | ID '(' ARGS ')'
                {
                    // $$ = new FunctionCallNode($1, $3);
                    std::cout << "[PARSER]: " << "VAL2" << std::endl;
                }
                | INPUT '(' ARGS ')'
                {
                    std::cout << "[PARSER]: " << "VAL3" << std::endl;
                }
                | OUTPUT '(' ARGS ')'
                {
                    std::cout << "[PARSER]: " << "VAL4" << std::endl;
                }
;

ARGS            : EXPR
                {
                    // $$.clear();
                    // $$.push_back($1);
                    std::cout << "[PARSER]: " << "ARG1" << std::endl;
                }
                | ARGS ',' EXPR
                {
                    // $$ = $1;
                    // $$.push_back($3);
                    std::cout << "[PARSER]: " << "ARG2" << std::endl;
                }
;

%%

void yyerror(const char *s) {
    std::cerr << s << ", line " << yylineno << std::endl;
    exit(1);
}

main(int argc, char **argv)
{
    yyin = fopen(argv[1], "r");
    yyparse();
}
