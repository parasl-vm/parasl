#ifndef PARASL_ASSIGNMENT_NODE_H_
#define PARASL_ASSIGNMENT_NODE_H_

#include "ExpressionNode.h"
#include "IDNode.h"
#include "StatementNode.h"
#include "TypeNodes.h"


namespace frontend::parser {
class AssignmentNode : public StatementNode {
public:
    AssignmentNode(IDNode *target, ExpressionNode *value, TypeNode *type)
        : StatementNode(), target{target}, value{value}, rvalueType{type} {}
    ~AssignmentNode() noexcept override = default;

    IDNode *GetTarget() {
        return target;
    }
    const IDNode *GetTarget() const {
        return target;
    }
    void SetTarget(IDNode *newTarget) {
        target = newTarget;
    }

    ExpressionNode *GetValue() {
        return value;
    }
    const ExpressionNode *GetValue() const {
        return value;
    }
    void SetValue(ExpressionNode *newValue) {
        value = newValue;
    }

    TypeNode *GetValueType() {
        return rvalueType;
    }
    const TypeNode *GetValueType() const {
        return rvalueType;
    }
    void SetValueType(TypeNode *type) {
        rvalueType = type;
    }

private:
    IDNode *target;
    ExpressionNode *value;
    TypeNode *rvalueType;
};
}   // namespace frontend::parser

#endif  // PARASL_ASSIGNMENT_NODE_H_
