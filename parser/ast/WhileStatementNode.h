#ifndef PARASL_WHILE_STATEMENT_NODE_H_
#define PARASL_WHILE_STATEMENT_NODE_H_

#include "ExpressionNode.h"
#include "StatementNode.h"


namespace lexer {
class WhileStatementNode : public ASTNode {
public:
    WhileStatementNode(ExpressionNode *condition, StatementNode *internal)
        : ASTNode(), condition(condition), internal(internal) {}
    ~WhileStatementNode() noexcept = default;

private:
    ExpressionNode *condition = nullptr;
    StatementNode *internal = nullptr;
};
}   // namespace lexer

#endif  // PARASL_WHILE_STATEMENT_NODE_H_
