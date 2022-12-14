#ifndef PARASL_EXPRESSION_NODE_H_
#define PARASL_EXPRESSION_NODE_H_

#include "StatementNode.h"
// #include "../parser.tab.h"


namespace frontend::parser {
class ExpressionNode : public StatementNode {
public:
    ~ExpressionNode() noexcept override = default;
};

class BinaryOpNode : public ExpressionNode {
public:
    BinaryOpNode(int opType, ExpressionNode *lhs, ExpressionNode *rhs)
        : ExpressionNode(), lhs(lhs), rhs(rhs) {}
    ~BinaryOpNode() noexcept override = default;

private:
    // TODO(dslynko): may make opType template argument
    int opType;
    ExpressionNode *lhs = nullptr;
    ExpressionNode *rhs = nullptr;
};
}   // namespace frontend::parser

#endif  // PARASL_EXPRESSION_NODE_H_
