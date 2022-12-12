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

%token EQ NE GE LE
%token DOUBLE_DOT
%token LAYER REPEAT GLUE BIND
%token IF ELSE WHILE FOR IN RETURN
%token CHAR // lexer::Token::CHAR
%token INT // lexer::Token::INT
%token FLOAT // lexer::Token::FLOAT
%token DOUBLE // lexer::Token::DOUBLE
%token VECTOR // lexer::Token::VECTOR


%token STRING
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
// %type<id_node_t>            ID
// %type<iter_range_node_t>    ITER_RANGE
// %type<layer_node_t>         PROGRAM // LAYER_DEF
// %type<literal_node_t>       LITERAL INT_VAL // STRING FLOAT_VAL
// %type<primitive_type_t>     PRIMITIVE
// %type<stmt_node_t>          STMT STMT_CLOSED // STMT_OPENED
// %type<type_node_t>          NON_FUNC_TYPE PRIMITIVE // OPT_NON_FUNC_TYPE

%left '+' '-'
%left '*' '/'
%left '<' '>'
%left EQ NE GE LE

%start PROGRAM

%%

PROGRAM         : LAYERS
                | STMTS
                {
                    // $$ = new CompoundStatementNode();
                    // $$ = new LayerNode(0, "", $$);
                    std::cout << "[PARSER]: PROGRAM -> STMTS" << std::endl;
                }
;

LAYERS          : LAYERS LAYER_DEF
                {
                    std::cout << "[PARSER]: LAYERS -> LAYERS LAYER_DEF" << std::endl;
                }
                | LAYER_DEF
                {
                    std::cout << "[PARSER]: LAYERS -> LAYER_DEF" << std::endl;
                }
;

LAYER_DEF       : LAYER '(' INT_VAL ',' STRING ')' '{' STMTS '}'
                {
                    std::cout << "[PARSER]: LAYER ( INT_VAL , STRING ) { STMTS }" << std::endl;
//                     $$ = new CompoundStatementNode();
//                     $$ = new LayerNode($3, $5, $$);
                }
;

/* statements syntax */

STMTS           : STMT
                {
                    // $$.push_back($1);
                    std::cout << "[PARSER]: STMTS1" << std::endl;
                }
                | STMTS STMT
                {
                    std::cout << "[PARSER]: STMTS2" << std::endl;
                    // $$ = $1;
                    // $$.push_back($2);
                }
;

STMT            : STMT_CLOSED
                | STMT_OPENED
;

STMT_CLOSED     : EXPR ';'
                {
                    std::cout << "[PARSER]: STMT_CLOSED -> EXPR ;" << std::endl;
                }
                | RETURN OPT_RET_VAL ';'
                {
                    std::cout << "[PARSER]: STMT_CLOSED -> RETURN OPT_RET_VAL  ;" << std::endl;
                }
                | ASSIGNMENT
                | OUTPUT '(' ARGS ')' ';'
                {
                    std::cout << "[PARSER]: OUTPUT ( ARGS ) ;" << std::endl;
                }
                | IF '(' EXPR ')' '{' STMTS '}' ELSE '{' STMTS '}'
                {
                    std::cout << "[PARSER]: IF ( EXPR ) { STMTS } ELSE { STMTS }" << std::endl;
                //     $$ = new IfStatementNode($3, $5, $7);
                }
                | IF '(' EXPR ')' '{' STMTS '}' ELSE STMT_CLOSED
                {
                    std::cout << "[PARSER]: IF ( EXPR ) { STMTS } ELSE STMT_CLOSED" << std::endl;
                //     $$ = new IfStatementNode($3, $5, $7);
                }
                | IF '(' EXPR ')' STMT_CLOSED ELSE '{' STMTS '}'
                {
                    std::cout << "[PARSER]: IF ( EXPR ) STMT_CLOSED ELSE { STMTS }" << std::endl;
                //     $$ = new IfStatementNode($3, $5, $7);
                }
                | IF '(' EXPR ')' STMT_CLOSED ELSE STMT_CLOSED
                {
                    std::cout << "[PARSER]: IF ( EXPR ) STMT_CLOSED ELSE STMT_CLOSED" << std::endl;
                //     $$ = new IfStatementNode($3, $5, $7);
                }
                | WHILE '(' EXPR ')' '{' STMTS '}'
                {
                    std::cout << "[PARSER]: WHILE ( EXPR ) { STMTS }" << std::endl;
                //     $$ = new WhileStatementNode($3, $5);
                }
                | WHILE '(' EXPR ')' STMT_CLOSED
                {
                    std::cout << "[PARSER]: WHILE ( EXPR ) STMT_CLOSED" << std::endl;
                //     $$ = new WhileStatementNode($3, $5);
                }
                | FOR '(' ITER_RANGE ')' '{' STMTS '}'
                {
                    std::cout << "[PARSER]: FOR ( ITER_RANGE ) { STMTS }" << std::endl;
                //     $$ = new ForStatementNode($3, $5);
                }
                | FOR '(' ITER_RANGE ')' STMT_CLOSED
                {
                    std::cout << "[PARSER]: FOR ( ITER_RANGE ) STMT_CLOSED" << std::endl;
                //     $$ = new ForStatementNode($3, $5);
                }
;

OPT_RET_VAL     : %empty
                {
                    std::cout << "[PARSER]: OPT_RET_VAL -> empty" << std::endl;
                //     $$ = new ExpressionNode(nullptr);
                }
                | EXPR
;

ASSIGNMENT              : ID ASSIGNMENT_REMINDER ';'
;

ASSIGNMENT_REMINDER     : ':' TYPED_REMINDER
                        {
                            std::cout << "[PARSER]: : TYPED_REMINDER" << std::endl;
                        }
                        | '=' UNTYPED_REMINDER
                        {
                            std::cout << "[PARSER]: = UNTYPED_REMINDER" << std::endl;
                        }
                        | OBJ_ACCESSORS '=' UNTYPED_REMINDER
                        {
                            std::cout << "[PARSER]: OBJ_ACCESSORS = UNTYPED_REMINDER" << std::endl;
                        }
;

TYPED_REMINDER          : VAR_TYPE '=' UNNAMED_EXTENDED_EXPR
                        {
                            std::cout << "[PARSER]: VAR_TYPE = UNNAMED_EXTENDED_EXPR" << std::endl;
                        }
                        | FUNC_TYPE '=' FUNCTION_DEF
                        {
                            std::cout << "[PARSER]: FUNC_TYPE = FUNCTION_DEF" << std::endl;
                        }
;

UNTYPED_REMINDER        : UNNAMED_EXTENDED_EXPR
                        {
                            std::cout << "[PARSER]: UNNAMED_EXTENDED_EXPR" << std::endl;
                        }
                        | FUNCTION_DEF_SCOPED
                        {
                            std::cout << "[PARSER]: FUNCTION_DEF_SCOPED" << std::endl;
                        }
                        | TYPED_INPUT_CALL
                        {
                            std::cout << "[PARSER]: TYPED_INPUT_CALL" << std::endl;
                        }
                        | NAMED_GLUE_CALL
                        {
                            std::cout << "[PARSER]: NAMED_GLUE_CALL" << std::endl;
                        }
;

STMT_OPENED     : IF '(' EXPR ')' OPT_SCOPE
                {
                    std::cout << "[PARSER]: IF ( EXPR ) OPT_SCOPE" << std::endl;
//                     $$ = new IfStatementNode($3, $5);
                }
                | IF '(' EXPR ')' STMT_CLOSED ELSE STMT_OPENED
                {
                    std::cout << "[PARSER]: IF ( EXPR ) STMT_CLOSED ELSE STMT_OPENED" << std::endl;
//                     $$ = new IfStatementNode($3, $5, $7);
                }
                | WHILE '(' EXPR ')' STMT_OPENED
                {
                    std::cout << "[PARSER]: WHILE ( EXPR ) STMT_OPENED" << std::endl;
//                     $$ = new WhileStatementNode($3, $5);
                }
                | FOR '(' ITER_RANGE ')' STMT_OPENED
                {
                    std::cout << "[PARSER]: FOR ( ITER_RANGE ) STMT_OPENED" << std::endl;
//                     $$ = new ForStatementNode($3, $5);
                }
;

OPT_SCOPE       : STMT
                | '{' STMTS '}'
                {
                    std::cout << "[PARSER]: { STMTS }" << std::endl;
                }
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

/* types syntax */

VAR_TYPE        : NON_FUNC_TYPE
                | STRUCT_TYPE
;

NON_FUNC_TYPE   : PRIMITIVE
                | INT '(' INT_VAL ')'
                {
                    std::cout << "[PARSER] INT ( INT_VAL )" << std::endl;
                //     $$ = new PrimitiveTypeNode(PrimitiveType::INT, $3);
                }
                | SEQUENCE_TYPE
;

PRIMITIVE       : CHAR
                {
                    // $$ = new PrimitiveTypeNode(PrimitiveType::CHAR, {});
                    std::cout << "[PARSER]: PRIMITIVE CHAR" << std::endl;
                }
                | INT
                {
                    // $$ = new PrimitiveTypeNode(PrimitiveType::INT, {});
                    std::cout << "[PARSER]: PRIMITIVE INT" << std::endl;
                }
                | FLOAT
                {
                    // $$ = new PrimitiveTypeNode(PrimitiveType::FLOAT, {});
                    std::cout << "[PARSER]: PRIMITIVE FLOAT" << std::endl;
                }
                | DOUBLE
                {
                    // $$ = new PrimitiveTypeNode(PrimitiveType::DOUBLE, {});
                    std::cout << "[PARSER]: PRIMITIVE DOUBLE" << std::endl;
                }
;

SEQUENCE_TYPE   : ARG_TYPE '[' INT_VAL ']'
                {
                    std::cout << "[PARSER] ARG_TYPE [ INT_VAL ]" << std::endl;
                //     $$ = new SequenceTypeNode(AggregateType::ARRAY, $1, $3);
                }
                | VECTOR '<' PRIMITIVE ',' INT_VAL '>'
                {
                    std::cout << "[PARSER] VECTOR < PRIMITIVE , INT_VAL >" << std::endl;
                //     $$ = new SequenceTypeNode(AggregateType::VECTOR, $3, $5);
                }
;

STRUCT_TYPE     : '{' ARGS_DECL '}'
                {
                    std::cout << "[PARSER] STRUCT_TYPE -> { ARGS_DECL }" << std::endl;
                }
;

ARGS_DECL       : ID OPT_ARG_TYPE
                {
                    std::cout << "[PARSER]: ARGS_DECL -> ID OPT_ARG_TYPE" << std::endl;
//                     $$.AddArgument($2, $1);
                }
                | ARGS_DECL ',' ID OPT_ARG_TYPE
                {
                    std::cout << "[PARSER]: ARGS_DECL -> ARGS_DECL , ID OPT_ARG_TYPE" << std::endl;
//                     $$ = $1;
//                     $$.AddArgument($4, $3);
                }
;

FUNC_TYPE       : '(' ARGS_DECL ')' OPT_ARG_TYPE
                {
                    std::cout << "[PARSER]: FUNC_TYPE -> ( ARGS_DECL ) OPT_ARG_TYPE" << std::endl;
                //     $$ = new FunctionTypeNode($4);
                }
;

ARG_TYPE        : VAR_TYPE
                | FUNC_TYPE_AS_ARG
;

FUNC_TYPE_AS_ARG    : '(' ')'
                    {
                        std::cout << "[PARSER] FUNC_TYPE_AS_ARG -> ( )" << std::endl;
                    }
;

OPT_ARG_TYPE    : %empty
                    {
                        std::cout << "[PARSER]: OPT_ARG_TYPE -> empty" << std::endl;
//                         $$ = new TypeNode(nullptr);
                    }
                    | ':' ARG_TYPE
                    {
                        std::cout << "[PARSER]: OPT_ARG_TYPE -> : ARG_TYPE" << std::endl;
//                         $$ = $2;
                    }
;

/* functions definition syntax */

FUNCTION_DEF    : FUNCTION_DEF_SCOPED
                | EXPR
                {
                    std::cout << "[PARSER]: FUNCTION_DEF -> EXPR" << std::endl;
                }
;

FUNCTION_DEF_SCOPED : '{' STMTS '}'
                    {
                        std::cout << "[PARSER]: FUNCTION_DEF_SCOPED -> { STMT }" << std::endl;
                    }
;

/* expressions syntax */

// we extend EXPR with syntax constructs which are impossible to be used in arithmetic and logic expressions
EXTENDED_EXPR   : UNNAMED_EXTENDED_EXPR
                | NAMED_GLUE_CALL
;

UNNAMED_EXTENDED_EXPR   : EXPR
                        | ARRAY_INIT
                        | GLUE_CALL
;

/* ARRAY_INIT construction may be encountered in the following cases:
  - Array/vector initialization/assignment
  - Nested array/vector initialization/assignment, e.g.: int[2][2] = {{1, 2}, {3, 4}};
  - Function argument: array/vector will be initialized in-place
  - Return value if returned type of the function was specified
*/
ARRAY_INIT      : '{' ARGS '}'
                {
                    std::cout << "[PARSER]: { ARGS }" << std::endl;
                }
;

// TODO(dslynko): consider supporting chained assignment

EXPR            : EXPR2
                | COND_EXPR
                /* | ID '=' EXPR
                {
                    std::cout << "[PARSER]: ID = EXPR" << std::endl;
                } */
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

VAL             : LITERAL
                {
                    // $$ = new ExpressionNode($2);
                    std::cout << "[PARSER]: VAL -> LITERAL" << std::endl;
                }
                | ID
                {
                    // $$ = new IDNode($1);
                    std::cout << "[PARSER]: VAL -> ID" << std::endl;
                }
                | ID '(' ARGS ')'
                {
                    // $$ = new FunctionCallNode($1, $3);
                    std::cout << "[PARSER]: VAL -> ID ( ARGS )" << std::endl;
                }
                | INPUT_CALL
                {
                    std::cout << "[PARSER]: VAL -> INPUT_CALL" << std::endl;
                }
                | ID OBJ_ACCESSORS
                {
                    std::cout << "[PARSER]: VAL -> ID OBJ_ACCESSORS" << std::endl;
                }
;

LITERAL         : INT_VAL
                {
                    // $$ = new LiteralNode(Token::INT_VAL, $1);
                    std::cout << "[PARSER]: INT_VAL" << std::endl;
                }
                | FLOAT_VAL
                {
                    std::cout << "[PARSER]: FLOAT_VAL" << std::endl;
                //     $$ = new LiteralNode(Token::FLOAT_VAL, $1);
                }
;

ARGS            : EXTENDED_EXPR
                {
                    // $$.clear();
                    // $$.push_back($1);
                    std::cout << "[PARSER]: ARG1" << std::endl;
                }
                | ARGS ',' EXTENDED_EXPR
                {
                    // $$ = $1;
                    // $$.push_back($3);
                    std::cout << "[PARSER]: ARG2" << std::endl;
                }
;

OBJ_ACCESSORS       : OBJ_ACCESSORS COMPOSITE_ACCESS
                    | COMPOSITE_ACCESS
;

COMPOSITE_ACCESS    : '.' ID
                    {
                        std::cout << "[PARSER]: COMPOSITE_ACCESS -> . ID" << std::endl;
                    }
                    | '[' EXPR ']'
                    {
                        std::cout << "[PARSER]: COMPOSITE_ACCESS -> [ EXPR ]" << std::endl;
                    }
;

/* special functions syntax */

// TODO(dslynko): specify semantic meaning of integer arguments in `output` and `input`

INPUT_CALL      : SINGLE_INPUT_CALL
                | SEQ_INPUT_CALL
;

SINGLE_INPUT_CALL   : INPUT '(' INT_VAL ')'
                    {
                        std::cout << "[PARSER]: SINGLE_INPUT_CALL -> INPUT ( INT_VAL )" << std::endl;
                    }
;

SEQ_INPUT_CALL  : INPUT '(' INT_VAL DOUBLE_DOT INT_VAL ')'
                {
                    std::cout << "[PARSER]: SEQ_INPUT_CALL -> INPUT ( INT_VAL DOUBLE_DOT INT_VAL )" << std::endl;
                }
;

TYPED_INPUT_CALL    : SINGLE_INPUT_CALL ':' PRIMITIVE
                    {
                        std::cout << "[PARSER]: TYPED_INPUT_CALL -> SINGLE_INPUT_CALL : PRIMITIVE" << std::endl;
                    }
                    | SEQ_INPUT_CALL ':' PRIMITIVE_SEQ_TYPE
                    {
                        std::cout << "[PARSER]: TYPED_INPUT_CALL -> SEQ_INPUT_CALL : PRIMITIVE_SEQ_TYPE" << std::endl;
                    }
;

PRIMITIVE_SEQ_TYPE  : PRIMITIVE '[' INT_VAL ']'
                    {
                        std::cout << "[PARSER] PRIMITIVE [ INT_VAL ]" << std::endl;
                    //     $$ = new SequenceTypeNode(AggregateType::ARRAY, $1, $3);
                    }
                    | VECTOR '<' PRIMITIVE ',' INT_VAL '>'
                    {
                        std::cout << "[PARSER] VECTOR < ARG_TYPE , INT_VAL >" << std::endl;
                    //     $$ = new SequenceTypeNode(AggregateType::VECTOR, $3, $5);
                    }
;

GLUE_CALL           : GLUE '(' ARGS ')'
                    {
                        std::cout << "[PARSER] GLUE_CALL" << std::endl;
                    }
;

NAMED_GLUE_CALL     : GLUE '(' SPEC_GLUE_ARGS ')'
                    {
                        std::cout << "[PARSER] NAMED_GLUE_CALL" << std::endl;
                    }
;

SPEC_GLUE_ARGS      : SINGLE_GLUE_ARG
                    {
                        std::cout << "[PARSER] SINGLE_GLUE_ARG" << std::endl;
                    }
                    | SPEC_GLUE_ARGS ',' SINGLE_GLUE_ARG
                    {
                        std::cout << "[PARSER] SPEC_GLUE_ARGS , SINGLE_GLUE_ARG" << std::endl;
                    }
;

SINGLE_GLUE_ARG     : EXTENDED_EXPR ':' ID
                    {
                        std::cout << "[PARSER] SINGLE_GLUE_ARG -> EXTENDED_EXPR : ID" << std::endl;
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
