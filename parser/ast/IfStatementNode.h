#ifndef PARASL_IF_STATEMENT_NODE_H_
#define PARASL_IF_STATEMENT_NODE_H_

#include "ExpressionNode.h"
#include "StatementNode.h"


namespace lexer {
class IfStatementNode : public StatementNode {
public:
    IfStatementNode(ExpressionNode *condition, StatementNode *trueBranch,
                    StatementNode *falseBranch = nullptr)
        : StatementNode(),
          condition(condition),
          trueBranch(trueBranch),
          falseBranch(falseBranch) {}
    ~IfStatementNode() noexcept = default;

private:
    ExpressionNode *condition = nullptr;
    StatementNode *trueBranch = nullptr;
    StatementNode *falseBranch = nullptr;
};
}   // namespace lexer

#endif  // PARASL_IF_STATEMENT_NODE_H_
