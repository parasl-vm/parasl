#ifndef PARASL_LEXER_H_
#define PARASL_LEXER_H_

/**
 * Generated Flex class name is yyFlexLexer by default. If we want to use more flex-generated
 * classes we should name them differently. See scanner.l prefix option.
 * 
 * Unfortunately the implementation relies on this trick with redefining class name
 * with a preprocessor macro. See GNU Flex manual, "Generating C++ Scanners" section
 */
#if ! defined(yyFlexLexerOnce)
#undef yyFlexLexer
#define yyFlexLexer ParaSL_Lexer // the trick with prefix; no namespace here :(
#include <FlexLexer.h>
#endif  // yyFlexLexerOnce

#include "../parser/parser.hpp"

// Scanner method signature is defined by this macro. Original yylex() returns int.
// Sinice Bison 3 uses symbol_type, we must change returned type. We also rename it
// to something sane, since you cannot overload return type.
#undef YY_DECL
#define YY_DECL frontend::parser::Parser::symbol_type frontend::lexer::Lexer::getNextToken()

namespace frontend::parser {
    class Driver;
}

namespace frontend::lexer {
class Lexer : public yyFlexLexer {
public:
    Lexer(parser::parser::Driver &dr) : driver{dr} {}
    ~Lexer() noexcept override = default;

    parser::Parser::symbol_type getNextToken();

private:
    Driver &driver;
};
}   // namespace frontend::lexer

#endif  // PARASL_LEXER_H_
