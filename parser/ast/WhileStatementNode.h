#ifndef PARASL_WHILE_STATEMENT_NODE_H_
#define PARASL_WHILE_STATEMENT_NODE_H_

#include "CompoundStatementNode.h"
#include "ExpressionNode.h"
#include "StatementNode.h"


namespace frontend::parser {
class WhileStatementNode : public ASTNode {
public:
    WhileStatementNode(ExpressionNode *condition, CompoundStatementNode *loopBody)
        : ASTNode(), condition(condition), body(loopBody) {}
    WhileStatementNode(ExpressionNode *condition, StatementNode *stmt)
        : ASTNode(), condition(condition), body(new CompoundStatementNode(stmt)) {}
    ~WhileStatementNode() noexcept override = default;

private:
    ExpressionNode *condition = nullptr;
    CompoundStatementNode *body = nullptr;
};
}   // namespace frontend::parser

#endif  // PARASL_WHILE_STATEMENT_NODE_H_
