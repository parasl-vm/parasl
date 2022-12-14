#ifndef PARASL_RETURN_STATEMENT_NODE_H_
#define PARASL_RETURN_STATEMENT_NODE_H_

#include "ExpressionNode.h"
#include "StatementNode.h"


namespace frontend::parser {
class ReturnStatementNode : public StatementNode {
public:
    ReturnStatementNode(ExpressionNode *returnValue = nullptr)
        : StatementNode(), returnValue(returnValue) {}
    ~ReturnStatementNode() noexcept override = default;

    ExpressionNode *GetReturnValue() {
        return returnValue;
    }
    const ExpressionNode *GetReturnValue() const {
        return returnValue;
    }

private:
    ExpressionNode *returnValue;
};
}   // namespace frontend::parser

#endif  // PARASL_RETURN_STATEMENT_NODE_H_
