#ifndef PARASL_FUNCTION_DEF_NODE_H_
#define PARASL_FUNCTION_DEF_NODE_H_

#include "CompoundStatementNode.h"
#include "ExpressionNode.h"


namespace frontend::parser {
class FunctionDefNode : public ExpressionNode {
public:
    FunctionDefNode(CompoundStatementNode *funcBody) : body{funcBody} {}
    FunctionDefNode(StatementNode *stmt) : body{new CompoundStatementNode(stmt)} {}
    ~FunctionDefNode() noexcept override = default;

private:
    CompoundStatementNode *body;
};
}   // namespace frontend::parser

#endif  // PARASL_FUNCTION_DEF_NODE_H_
