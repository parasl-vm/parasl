#ifndef PARASL_LITERAL_NODE_H_
#define PARASL_LITERAL_NODE_H_

#include "ExpressionNode.h"
#include <sstream>
#include <string>
#include "../../lexer/Token.h"
// #include "../parser.tab.h"


namespace frontend::parser {
template <typename T>
concept Arithmetic = std::is_arithmetic<T>::value;

template <Arithmetic T>
class LiteralNode : public ExpressionNode {
public:
    LiteralNode(yytokentype type, T &&val) : ExpressionNode(), type{type}, value{val} {
        auto numType = static_cast<int>(type);
        ASSERT(numType >= static_cast<int>(yytokentype::STRING)
               && numType < static_cast<int>(yytokentype::INVALID_TOKEN));
    }
    ~LiteralNode() noexcept override = default;

    yytokentype GetTokenType() const {
        return type;
    }
    T GetValue() const {
        return value;
    }

private:
    yytokentype type;
    T value;
};
}   // namespace frontend::parser

#endif  // PARASL_LITERAL_NODE_H_
