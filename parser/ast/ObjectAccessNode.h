#ifndef PARASL_OBJECT_ACCESS_NODE_H_
#define PARASL_OBJECT_ACCESS_NODE_H_

#include "IDNode.h"


namespace frontend::parser {
// TODO(dslynko): object (critical)
class ObjectAccessNode : public IDNode {
public:
    ObjectAccessNode(IDNode *obj) : IDNode{nullptr}, object{obj} {}
    ~ObjectAccessNode() noexcept override = default;

    IDNode *GetObject() {
        return object;
    }
    const IDNode *GetObject() const {
        return object;
    }
    void SetObject(IDNode *obj) {
        object = obj;
    }

protected:
    IDNode *object;
};

class AttributeAccessNode : public ObjectAccessNode {
public:
    AttributeAccessNode(IDNode *obj, const char *attrName)
        : ObjectAccessNode{obj}, attrName{attrName}
    {
        ASSERT(attrName);
    }
    ~AttributeAccessNode() noexcept override = default;

    const char *GetAttrName() const {
        return attrName;
    }

private:
    const char *attrName;
};

class SequenceAccessNode : public ObjectAccessNode {
public:
    SequenceAccessNode(IDNode *obj, ExpressionNode *index)
        : ObjectAccessNode{obj}, index{index}
    {
        ASSERT(index);
    }
    ~SequenceAccessNode() noexcept override = default;

    ExpressionNode *GetIndex() {
        return index;
    }
    const ExpressionNode *GetIndex() const {
        return index;
    }
    void SetIndex(ExpressionNode *newIndex) {
        index = newIndex;
    }

private:
    ExpressionNode *index;
};
}   // namespace frontend::parser

#endif  // PARASL_OBJECT_ACCESS_NODE_H_
