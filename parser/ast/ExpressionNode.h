#ifndef PARASL_EXPRESSION_NODE_H_
#define PARASL_EXPRESSION_NODE_H_

#include "StatementNode.h"
#include "Token.h"


namespace lexer {
class ExpressionNode : public StatementNode {
public:
    ~ExpressionNode() noexcept = default;
};
}   // namespace lexer

#endif  // PARASL_EXPRESSION_NODE_H_
