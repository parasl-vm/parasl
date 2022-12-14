%skeleton "lalr1.cc"
%require "3.2"
%language "c++"

%debug

%define parse.assert
%define parse.error verbose
%define api.namespace {frontend::parser}
%define api.parser.class {Parser}
// %define api.token.constructor
%define api.value.type variant

%parse-param {Lexer &scanner}
%parse-param {Driver &driver}

%code requires {
    #include "ast/ArrayInitNode.h"
    #include "ast/AssignmentNode.h"
    #include "ast/CompoundStatementNode.h"
    #include "ast/DeclarationNode.h"
    #include "ast/ExpressionNode.h"
    #include "ast/ForStatementNode.h"
    #include "ast/FunctionCallNode.h"
    #include "ast/FunctionDefNode.h"
    #include "ast/IDNode.h"
    #include "ast/IfStatementNode.h"
    #include "ast/LayerNode.h"
    #include "ast/LiteralNode.h"
    #include "ast/ReturnStatementNode.h"
    #include "ast/StatementNode.h"
    #include "ast/TypeNodes.h"
    #include "ast/WhileStatementNode.h"
    #include "driver.h"
    #include <iostream>
    #include "lexer.h"
    #include "parser.hpp"

    static frontend::parser::Parser::symbol_type yylex(frontend::lexer::Lexer &scanner,
                                                       frontend::parser::Driver &driver) {
        return scanner.getNextToken();
    }
}

// terminals

%token EQ NE GE LE
%token DOUBLE_DOT
%token LAYER
%token IF ELSE WHILE FOR IN RETURN

// types
%token CHAR INT FLOAT DOUBLE VECTOR

%token <frontend::parser::IDNode>   ID

// values
%token <std::string>                STRING
%token <int64_t>                    INT_VAL
%token <double>                     FLOAT_VAL

// special functions
%token GLUE INPUT

// nonterminals
%nterm <std::vector<LayerNode *>>                   program
%nterm <frontend::parser::LayerNode *>              layer_def
%nterm <frontend::parser::CompoundStatementNode *>  stmts expanded_scope
%nterm <frontend::parser::StatementNode *>          stmt stmt_closed stmt_opened
%nterm <frontend::parser::AssignmentNode *>         assignment
%nterm <frontend::parser::IterationRange *>         iter_range

%nterm <frontend::parser::ExpressionNode *>         expr expr2 term val opt_ret_val unnamed_extended_expr extended_expr
%nterm <frontend::parser::BinaryOpNode *>           cond_expr

%nterm <std::vector<ExpressionNode *>>              args opt_args
%nterm <frontend::parser::FunctionDefNode *>        function_def_scoped function_def
%nterm <frontend::parser::GlueCallNode *>           glue_call named_glue_call
%nterm <frontend::parser::InputCallNode *>          single_input_call seq_input_call input_call typed_input_call

%nterm <frontend::parser::LiteralNode *>            literal
%nterm <frontend::parser::PrimitiveTypeNode *>      primitive
%nterm <frontend::parser::TypeNode *>               non_func_type var_type arg_type opt_arg_type
%nterm <frontend::parser::SequenceTypeNode *>       sequence_type primitive_seq_type
%nterm <frontend::parser::StructTypeNode *>         struct_type
%nterm <frontend::parser::FunctionTypeNode *>       func_type func_type_as_arg
%nterm <frontend::parser::ArrayInitializerNode *>   array_init
%nterm <frontend::parser::ObjectAccessNode *>       obj_access composite_access

%nterm <std::vector<frontend::parser::FunctionArgument>>                            args_decl opt_args_decl
%nterm <std::pair<frontend::parser::ExpressionNode *, frontend::parser::IDNode *>>  single_glue_arg
%nterm <frontend::parser::NamedGlueArgs>                                            spec_glue_args
%nterm <std::pair<frontend::parser::ExpressionNode *, frontend::parser::TypeNode *>>    assignment_reminder typed_reminder untyped_reminder

// operators rules

%left '+' '-'
%left '*' '/'
%left '<' '>'
%left EQ NE GE LE

%start program

%locations

%%

program         : layers
                | stmts
                {
                    std::cout << "[PARSER]: program -> stmts" << std::endl;
                    driver.addLayer(new frontend::parser::LayerNode(0, "", $1));
                }
;

layers          : layer_def
                {
                    std::cout << "[PARSER]: layers -> layer_def" << std::endl;
                }
                | layers layer_def
                {
                    std::cout << "[PARSER]: layers -> layers layer_def" << std::endl;
                }
;

layer_def       : LAYER '(' INT_VAL ',' STRING ')' '{' stmts '}'
                {
                    std::cout << "[PARSER]: LAYER ( INT_VAL , STRING ) { stmts }" << std::endl;
                    $$ = new frontend::parser::CompoundStatementNode();
                    driver.addLayer(new frontend::parser::LayerNode($3, $5, $$));
                }
;

/* statements syntax */

stmts           : stmt
                {
                    std::cout << "[PARSER]: STMTS1" << std::endl;
                    $$.push_back($stmt);
                }
                | stmts stmt
                {
                    std::cout << "[PARSER]: STMTS2" << std::endl;
                    $$ = $1;
                    $$.push_back($stmt);
                }
;

stmt            : stmt_closed
                | stmt_opened
;

stmt_closed     : expr ';'
                {
                    std::cout << "[PARSER]: stmt_closed -> expr ;" << std::endl;
                    $$ = $1;
                }
                | RETURN opt_ret_val ';'
                {
                    std::cout << "[PARSER]: stmt_closed -> RETURN opt_ret_val  ;" << std::endl;
                    $$ = new frontend::parser::ReturnStatementNode($2);
                }
                | assignment
                | IF '(' expr ')' '{' stmts '}' ELSE '{' stmts '}'
                {
                    std::cout << "[PARSER]: IF ( expr ) { stmts } ELSE { stmts }" << std::endl;
                    $$ = new frontend::parser::IfStatementNode($3, $6, $10);
                }
                | IF '(' expr ')' '{' stmts '}' ELSE stmt_closed
                {
                    std::cout << "[PARSER]: IF ( expr ) { stmts } ELSE stmt_closed" << std::endl;
                    $$ = new frontend::parser::IfStatementNode($3, $6, $9);
                }
                | IF '(' expr ')' stmt_closed ELSE '{' stmts '}'
                {
                    std::cout << "[PARSER]: IF ( expr ) stmt_closed ELSE { stmts }" << std::endl;
                    $$ = new frontend::parser::IfStatementNode($3, $5, $8);
                }
                | IF '(' expr ')' stmt_closed ELSE stmt_closed
                {
                    std::cout << "[PARSER]: IF ( expr ) stmt_closed ELSE stmt_closed" << std::endl;
                    $$ = new frontend::parser::IfStatementNode($3, $5, $7);
                }
                | WHILE '(' expr ')' '{' stmts '}'
                {
                    std::cout << "[PARSER]: WHILE ( expr ) { stmts }" << std::endl;
                    $$ = new frontend::parser::WhileStatementNode($3, $6);
                }
                | WHILE '(' expr ')' stmt_closed
                {
                    std::cout << "[PARSER]: WHILE ( expr ) stmt_closed" << std::endl;
                    $$ = new frontend::parser::WhileStatementNode($3, $5);
                }
                | FOR '(' iter_range ')' '{' stmts '}'
                {
                    std::cout << "[PARSER]: FOR ( iter_range ) { stmts }" << std::endl;
                    $$ = new frontend::parser::ForStatementNode($3, $6);
                }
                | FOR '(' iter_range ')' stmt_closed
                {
                    std::cout << "[PARSER]: FOR ( iter_range ) stmt_closed" << std::endl;
                    $$ = new frontend::parser::ForStatementNode($3, $5);
                }
;

opt_ret_val     : %empty
                {
                    std::cout << "[PARSER]: opt_ret_val -> empty" << std::endl;
                }
                | expr
;

assignment              : ID assignment_reminder ';'
                        {
                            std::cout << "[PARSER]: assignment -> D assignment_reminder ;" << std::endl;
                            $$ = new frontend::parser::AssignmentNode($1, $2.first, $2.second);
                        }
                        | obj_access '=' untyped_reminder ';'
                        {
                            std::cout << "[PARSER]: assignment -> obj_access = untyped_reminder" << std::endl;
                            $$ = new frontend::parser::AssignmentNode($1, $3.first, nullptr);
                        }
;

assignment_reminder     : ':' typed_reminder
                        {
                            std::cout << "[PARSER]: assignment_reminder -> : typed_reminder" << std::endl;
                            $$ = $2;
                        }
                        | '=' untyped_reminder
                        {
                            std::cout << "[PARSER]: assignment_reminder -> = untyped_reminder" << std::endl;
                            $$ = $2;
                        }
;

typed_reminder          : var_type '=' unnamed_extended_expr
                        {
                            std::cout << "[PARSER]: var_type = unnamed_extended_expr" << std::endl;
                            $$ = {$3, $1};
                        }
                        | func_type '=' function_def
                        {
                            std::cout << "[PARSER]: func_type = function_def" << std::endl;
                            $$ = {$3, $1};
                        }
;

untyped_reminder        : unnamed_extended_expr
                        {
                            std::cout << "[PARSER]: unnamed_extended_expr" << std::endl;
                            $$ = {$1, nullptr};
                        }
                        | function_def_scoped
                        {
                            std::cout << "[PARSER]: function_def_scoped" << std::endl;
                            $$ = {$1, nullptr};
                        }
                        | typed_input_call
                        {
                            std::cout << "[PARSER]: typed_input_call" << std::endl;
                            $$ = {$1, nullptr};
                        }
                        | named_glue_call
                        {
                            std::cout << "[PARSER]: named_glue_call" << std::endl;
                            $$ = {$1, nullptr};
                        }
;

stmt_opened     : IF '(' expr ')' expanded_scope
                {
                    std::cout << "[PARSER]: IF ( expr ) expanded_scope" << std::endl;
                    $$ = new frontend::parser::IfStatementNode($3, $5);
                }
                | IF '(' expr ')' stmt_closed ELSE stmt_opened
                {
                    std::cout << "[PARSER]: IF ( expr ) stmt_closed ELSE stmt_opened" << std::endl;
                    $$ = new frontend::parser::IfStatementNode($3, $5, $7);
                }
                | WHILE '(' expr ')' stmt_opened
                {
                    std::cout << "[PARSER]: WHILE ( expr ) stmt_opened" << std::endl;
                    $$ = new frontend::parser::WhileStatementNode($3, $5);
                }
                | FOR '(' iter_range ')' stmt_opened
                {
                    std::cout << "[PARSER]: FOR ( iter_range ) stmt_opened" << std::endl;
                    $$ = new frontend::parser::ForStatementNode($3, $5);
                }
;

expanded_scope  : stmt
                {
                    $$ = new frontend::parser::CompoundStatementNode($1);
                }
                | '{' stmts '}'
                {
                    std::cout << "[PARSER]: { stmts }" << std::endl;
                    $$ = $2;
                }
;

iter_range      : ID IN ID
                {
                    std::cout << "[PARSER]: ID IN ID" << std::endl;
                    $$ = new frontend::parser::SequenceIterationRange($1, $3);
                }
                | ID IN INT_VAL ':' INT_VAL
                {
                    std::cout << "[PARSER]: ID IN INT_VAL : INT_VAL" << std::endl;
                    $$ = new frontend::parser::NumericIterationRange($1, $3, $5);
                }
                | ID IN INT_VAL ':' INT_VAL ':' INT_VAL
                {
                    std::cout << "[PARSER]: ID IN INT_VAL : INT_VAL : INT_VAL" << std::endl;
                    $$ = new frontend::parser::NumericIterationRange($1, $3, $5, $7);
                }
;

/* types syntax */

var_type        : non_func_type
                | struct_type
;

non_func_type   : primitive
                | INT '(' INT_VAL ')'
                {
                    std::cout << "[PARSER] INT ( INT_VAL )" << std::endl;
                    $$ = new frontend::parser::PrimitiveTypeNode(PrimitiveType::INT, $3);
                }
                | sequence_type
;

primitive       : CHAR
                {
                    std::cout << "[PARSER]: primitive CHAR" << std::endl;
                    $$ = new frontend::parser::PrimitiveTypeNode(PrimitiveType::CHAR, {});
                }
                | INT
                {
                    $$ = new frontend::parser::PrimitiveTypeNode(PrimitiveType::INT, {});
                    std::cout << "[PARSER]: primitive INT" << std::endl;
                }
                | FLOAT
                {
                    $$ = new frontend::parser::PrimitiveTypeNode(PrimitiveType::FLOAT, {});
                    std::cout << "[PARSER]: primitive FLOAT" << std::endl;
                }
                | DOUBLE
                {
                    $$ = new frontend::parser::PrimitiveTypeNode(PrimitiveType::DOUBLE, {});
                    std::cout << "[PARSER]: primitive DOUBLE" << std::endl;
                }
;

// TODO(dslynko): allow arrays of functions (substitute 'var_type' with 'arg_type')
sequence_type   : var_type '[' INT_VAL ']'
                {
                    std::cout << "[PARSER] var_type [ INT_VAL ]" << std::endl;
                    $$ = new frontend::parser::SequenceTypeNode(AggregateType::ARRAY, $1, $3);
                }
                | VECTOR '<' primitive ',' INT_VAL '>'
                {
                    std::cout << "[PARSER] VECTOR < primitive , INT_VAL >" << std::endl;
                    $$ = new frontend::parser::SequenceTypeNode(AggregateType::VECTOR, $3, $5);
                }
;

struct_type     : '{' args_decl '}'
                {
                    std::cout << "[PARSER] struct_type -> { args_decl }" << std::endl;
                    $$ = new frontend::parser::StructTypeNode($2);
                }
;

args_decl       : ID opt_arg_type
                {
                    std::cout << "[PARSER]: args_decl -> ID opt_arg_type" << std::endl;
                    $$.AddArgument($2, $1);
                }
                | args_decl ',' ID opt_arg_type
                {
                    std::cout << "[PARSER]: args_decl -> args_decl , ID opt_arg_type" << std::endl;
                    $$ = $1;
                    $$.AddArgument($4, $3);
                }
;

opt_args_decl   : %empty
                {
                    // auto-generate an empty vector
                }
                | args_decl
;

func_type       : '(' opt_args_decl ')' opt_arg_type
                {
                    std::cout << "[PARSER]: func_type -> ( opt_args_decl ) opt_arg_type" << std::endl;
                    $$ = new frontend::parser::FunctionTypeNode($4);
                }
;

arg_type        : var_type
                | func_type_as_arg
;

func_type_as_arg    : '(' ')'
                    {
                        std::cout << "[PARSER] func_type_as_arg -> ( )" << std::endl;
                        $$ = new frontend::parser::FunctionTypeNode(nullptr);
                    }
;

opt_arg_type    : %empty
                {
                    std::cout << "[PARSER]: opt_arg_type -> empty" << std::endl;
                    $$ = nullptr;
                }
                | ':' arg_type
                {
                    std::cout << "[PARSER]: opt_arg_type -> : arg_type" << std::endl;
                    $$ = $2;
                }
;

/* functions definition syntax */

function_def        : function_def_scoped
                    | expr
                    {
                        std::cout << "[PARSER]: function_def -> expr" << std::endl;
                        $$ = new frontend::parser::FunctionDefNode($1);
                    }
;

function_def_scoped : '{' stmts '}'
                    {
                        std::cout << "[PARSER]: function_def_scoped -> { stmt }" << std::endl;
                        $$ = new frontend::parser::FunctionDefNode($2);
                    }
;

/* expressions syntax */

// we extend expr with syntax constructs which are impossible to be used in arithmetic and logic expressions
extended_expr           : unnamed_extended_expr
                        | named_glue_call
;

unnamed_extended_expr   : expr
                        | array_init
                        | glue_call
;

/* array_init construction may be encountered in the following cases:
  - Array/vector initialization/assignment
  - Nested array/vector initialization/assignment, e.g.: int[2][2] = {{1, 2}, {3, 4}};
  - Function argument: array/vector will be initialized in-place
  - Return value if returned type of the function was specified
*/
array_init      : '{' args '}'
                {
                    std::cout << "[PARSER]: { args }" << std::endl;
                    $$ = new frontend::parser::ArrayInitializerNode(std::move($2));
                }
;

// TODO(dslynko): consider supporting chained assignment

expr            : expr2
                | cond_expr
                /* | ID '=' expr
                {
                    std::cout << "[PARSER]: ID = expr" << std::endl;
                } */
;

cond_expr       : expr EQ expr
                {
                    std::cout << "[PARSER]: EQ" << std::endl;
                    $$ = new frontend::parser::BinOpNode(Token::EQ, $1, $3);
                }
                | expr NE expr
                {
                    std::cout << "[PARSER]: NE" << std::endl;
                    $$ = new frontend::parser::BinOpNode(Token::NE, $1, $3);
                }
                | expr GE expr
                {
                    std::cout << "[PARSER]: GE" << std::endl;
                    $$ = new frontend::parser::BinOpNode(Token::GE, $1, $3);
                }
                | expr '>' expr
                {
                    std::cout << "[PARSER]: GT" << std::endl;
                    $$ = new frontend::parser::BinOpNode(Token::GT, $1, $3);
                }
                | expr LE expr
                {
                    std::cout << "[PARSER]: LE" << std::endl;
                    $$ = new frontend::parser::BinOpNode(Token::LE, $1, $3);
                }
                | expr '<' expr
                {
                    std::cout << "[PARSER]: LT" << std::endl;
                    $$ = new frontend::parser::BinOpNode(Token:LT, $1, $3);
                }
;

expr2           : term
                | term '+' expr2
                {
                    std::cout << "[PARSER]: " << "term + term" << std::endl;
                    $$ = new frontend::parser::BinOpNode(Token::PLUS, $1, $3);
                }
                | term '-' expr2
                {
                    std::cout << "[PARSER]: " << "term - term" << std::endl;
                    $$ = new frontend::parser::BinOpNode(Token::MINUS, $1, $3);
                }
;

term            : val
                | '(' expr ')'
                {
                    std::cout << "[PARSER]: " << "term -> ( expr )" << std::endl;
                    $$ = $2;
                }
                | term '*' term
                {
                    std::cout << "[PARSER]: " << "term * term" << std::endl;
                    $$ = new frontend::parser::BinOpNode(Token::MUL, $1, $3);
                }
                | term '/' term
                {
                    std::cout << "[PARSER]: " << "term / term" << std::endl;
                    $$ = new frontend::parser::BinOpNode(Token::DIV, $1, $3);
                }
;

val             : literal
                {
                    std::cout << "[PARSER]: val -> literal" << std::endl;
                }
                | ID
                {
                    std::cout << "[PARSER]: val -> ID" << std::endl;
                }
                | ID '(' opt_args ')'
                {
                    std::cout << "[PARSER]: val -> ID ( opt_args )" << std::endl;
                    $$ = new frontend::parser::FunctionCallNode($1, $3);
                }
                | input_call
                {
                    std::cout << "[PARSER]: val -> input_call" << std::endl;
                }
                | obj_access
                {
                    std::cout << "[PARSER]: val -> obj_access" << std::endl;
                }
                | obj_access '(' opt_args ')'
                {
                    std::cout << "[PARSER]: val -> obj_access ( opt_args )" << std::endl;
                    $$ = new frontend::parser::MethodCallNode($1, std::move($3));
                }
;

literal         : INT_VAL
                {
                    std::cout << "[PARSER]: INT_VAL" << std::endl;
                    // $$ = new frontend::parser::LiteralNode(Token::INT_VAL, $1);
                }
                | FLOAT_VAL
                {
                    std::cout << "[PARSER]: FLOAT_VAL" << std::endl;
                    // $$ = new frontend::parser::LiteralNode(Token::FLOAT_VAL, $1);
                }
;

opt_args        : %empty
                {
                    // auto-generate an empty vector
                }
                | args
;

args            : extended_expr
                {
                    std::cout << "[PARSER]: ARG1" << std::endl;
                    $$.push_back($1);
                }
                | args ',' extended_expr
                {
                    std::cout << "[PARSER]: ARG2" << std::endl;
                    $$ = $1;
                    $$.push_back($3);
                }
;

// arrays and structs access

obj_access          : ID composite_access
                    {
                        $$ = $2;
                        $$->SetObject($1);
                    }
                    | obj_access composite_access
                    {
                        $$ = $2;
                        $$->SetObject($1);
                    }
;

composite_access    : '.' ID
                    {
                        std::cout << "[PARSER]: composite_access -> . ID" << std::endl;
                        $$ = new frontend::parser::AttributeAccessNode(nullptr, $2);
                    }
                    | '[' expr ']'
                    {
                        std::cout << "[PARSER]: composite_access -> [ expr ]" << std::endl;
                        $$ = new frontend::parser::SequenceAccessNode(nullptr, $2);
                    }
;

/* special functions syntax */

// TODO(dslynko): specify semantic meaning of integer arguments in `output` and `input`

input_call      : single_input_call
                | seq_input_call
;

single_input_call   : INPUT '(' INT_VAL ')'
                    {
                        std::cout << "[PARSER]: single_input_call -> INPUT ( INT_VAL )" << std::endl;
                        $$ = new frontend::parser::SingleInputCallNode($3);
                    }
;

seq_input_call      : INPUT '(' INT_VAL DOUBLE_DOT INT_VAL ')'
                    {
                        std::cout << "[PARSER]: seq_input_call -> INPUT ( INT_VAL DOUBLE_DOT INT_VAL )" << std::endl;
                        $$ = new frontend::parser::SequencedInputCallNode($3, $5);
                    }
;

typed_input_call    : single_input_call ':' primitive
                    {
                        std::cout << "[PARSER]: typed_input_call -> single_input_call : primitive" << std::endl;
                        $$ = $1;
                        $$->SetReturnType($3);
                    }
                    | seq_input_call ':' primitive_seq_type
                    {
                        std::cout << "[PARSER]: typed_input_call -> seq_input_call : primitive_seq_type" << std::endl;
                        $$ = $1;
                        $$->SetReturnType($3);
                    }
;

primitive_seq_type  : primitive '[' INT_VAL ']'
                    {
                        std::cout << "[PARSER] primitive [ INT_VAL ]" << std::endl;
                        $$ = new frontend::parser::SequenceTypeNode(AggregateType::ARRAY, $1, $3);
                    }
                    | VECTOR '<' primitive ',' INT_VAL '>'
                    {
                        std::cout << "[PARSER] VECTOR < arg_type , INT_VAL >" << std::endl;
                        $$ = new frontend::parser::SequenceTypeNode(AggregateType::VECTOR, $3, $5);
                    }
;

glue_call           : GLUE '(' args ')'
                    {
                        std::cout << "[PARSER] glue_call" << std::endl;
                        $$ = new frontend::parser::GlueCallNode(std::move($3));
                    }
;

named_glue_call     : GLUE '(' spec_glue_args ')'
                    {
                        std::cout << "[PARSER] named_glue_call" << std::endl;
                        $$ = new frontend::parser::GlueCallNode(std::move($3));
                    }
;

spec_glue_args      : single_glue_arg
                    {
                        std::cout << "[PARSER] single_glue_arg" << std::endl;
                        $$.first.push_back($1.first);
                        $$.second.push_back($1.second);
                    }
                    | spec_glue_args ',' single_glue_arg
                    {
                        std::cout << "[PARSER] spec_glue_args , single_glue_arg" << std::endl;
                        $$ = $1;
                        $$.first.push_back($1.first);
                        $$.second.push_back($1.second);
                    }
;

single_glue_arg     : extended_expr ':' ID
                    {
                        std::cout << "[PARSER] single_glue_arg -> extended_expr : ID" << std::endl;
                        $$ = {$1, $3};
                    }
;

%%

void parser::Parser::error(const location_type &loc, const std::string &message) {
    std::cerr << "Error: " << message << " at " << loc << "\n";
}
