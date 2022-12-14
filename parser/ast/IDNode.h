#ifndef PARASL_ID_NODE_H_
#define PARASL_ID_NODE_H_

#include "ExpressionNode.h"
#include <string>
#include "TypeNodes.h"


namespace frontend::parser {
class IDNode : public ExpressionNode {
public:
    IDNode(const char *value, TypeNode *type = nullptr)
        : ExpressionNode(), value{value}, valueType{type} {}
    ~IDNode() noexcept override = default;

    const char *GetValue() const {
        return value;
    }
    void SetValue(const char *newValue) {
        value = newValue;
    }

    TypeNode *GetType() {
        return valueType;
    }
    const TypeNode *GetType() const {
        return valueType;
    }
    void SetType(TypeNode *type) {
        valueType = type;
    }

protected:
    const char *value;
    TypeNode *valueType;
};
}   // namespace frontend::parser

#endif  // PARASL_ID_NODE_H_
