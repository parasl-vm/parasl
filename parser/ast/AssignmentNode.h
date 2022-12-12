#ifndef PARASL_ASSIGNMENT_NODE_H_
#define PARASL_ASSIGNMENT_NODE_H_

#include "ExpressionNode.h"
#include "IDNode.h"
#include "StatementNode.h"


namespace lexer {
class AssignmentNode : public StatementNode {
public:
    AssignmentNode(IDNode *target, ExpressionNode *value)
        : StatementNode(), target(target), value(value) {}
    ~AssignmentNode() noexcept = default;

    ExpressionNode *GetValue() {
        return value;
    }
    const ExpressionNode *GetValue() const {
        return value;
    }

private:
    IDNode *target = nullptr;
    ExpressionNode *value = nullptr;
};
}   // namespace lexer

#endif  // PARASL_ASSIGNMENT_NODE_H_
