#ifndef PARASL_IF_STATEMENT_NODE_H_
#define PARASL_IF_STATEMENT_NODE_H_

#include "ExpressionNode.h"
#include "StatementNode.h"


namespace frontend::parser {
class IfStatementNode : public StatementNode {
public:
    IfStatementNode(ExpressionNode *condition,
                    CompoundStatementNode *trueBranch,
                    CompoundStatementNode *falseBranchStmt = nullptr)
        : StatementNode(),
          condition(condition),
          trueBranch(trueBranch),
          falseBranch(falseBranchStmt) {}
    IfStatementNode(ExpressionNode *condition,
                    StatementNode *trueBranch,
                    CompoundStatementNode *falseBranchStmt = nullptr)
        : IfStatementNode(condition, new CompoundStatementNode(trueBranch), falseBranchStmt) {}
    IfStatementNode(ExpressionNode *condition,
                    StatementNode *trueBranch,
                    StatementNode *falseBranchStmt = nullptr)
        : IfStatementNode(condition, new CompoundStatementNode(trueBranch), nullptr)
    {
        if (falseBranchStmt) {
            falseBranch = new CompoundStatementNode(falseBranchStmt);
        }
    }
    ~IfStatementNode() noexcept override = default;

private:
    ExpressionNode *condition = nullptr;
    CompoundStatementNode *trueBranch = nullptr;
    CompoundStatementNode *falseBranch = nullptr;
};
}   // namespace frontend::parser

#endif  // PARASL_IF_STATEMENT_NODE_H_
