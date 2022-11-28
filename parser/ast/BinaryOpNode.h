#ifndef PARASL_BIN_OP_NODE_H_
#define PARASL_BIN_OP_NODE_H_

#include "ExpressionNode.h"
// #include "../parser.tab.h"


namespace lexer {
class BinaryOpNode : public ExpressionNode {
public:
    BinaryOpNode(int opType, ExpressionNode *lhs, ExpressionNode *rhs)
        : ExpressionNode(), lhs(lhs), rhs(rhs) {}
    ~BinaryOpNode() noexcept = default;

private:
    // TODO(dslynko): may make opType template argument
    int opType;
    ExpressionNode *lhs = nullptr;
    ExpressionNode *rhs = nullptr;
};
}   // namespace lexer

#endif  // PARASL_BIN_OP_NODE_H_
