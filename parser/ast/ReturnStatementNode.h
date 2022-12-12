#ifndef PARASL_RETURN_STATEMENT_NODE_H_
#define PARASL_RETURN_STATEMENT_NODE_H_

#include "ExpressionNode.h"
#include "StatementNode.h"


namespace lexer {
class ReturnStatementNode : public StatementNode {
public:
    ReturnStatementNode(ExpressionNode *returnValue = nullptr)
        : StatementNode(), returnValue(returnValue) {}
    ~ReturnStatementNode() noexcept = default;

    ExpressionNode *GetReturnValue() {
        return returnValue;
    }
    const ExpressionNode *GetReturnValue() const {
        return returnValue;
    }

private:
    ExpressionNode *returnValue;
};
}   // namespace lexer

#endif  // PARASL_RETURN_STATEMENT_NODE_H_
