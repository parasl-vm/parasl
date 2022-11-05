#ifndef PARASL_LITERAL_NODE_H_
#define PARASL_LITERAL_NODE_H_

#include "ExpressionNode.h"
#include <string>
#include "Token.h"


namespace lexer {
class LiteralNode : public ExpressionNode {
public:
    LiteralNode(Token type, const char *value)
        : ExpressionNode(), type(type), value{value}
    {
        auto numType = static_cast<int>(type);
        ASSERT(numType >= static_cast<int>(Token::STRING)
            && numType < static_cast<int>(Token::INVALID_TOKEN));
    }
    ~LiteralNode() noexcept = default;

    Token GetTokenType() const {
        return type;
    }
    const std::string &GetValue() const {
        return value;
    }

private:
    Token type = Token::INVALID_TOKEN;
    std::string value;
};
}   // namespace lexer

#endif  // PARASL_LITERAL_NODE_H_
