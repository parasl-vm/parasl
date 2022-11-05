#ifndef PARASL_TYPE_NODE_H_
#define PARASL_TYPE_NODE_H_

#include "ASTNode.h"


namespace lexer {
class TypeNode : public ASTNode {
public:
    ~TypeNode() noexcept = default;
};

enum class PrimitiveType {
    CHAR = 0,
    INT,
    FLOAT,
    DOUBLE
};

class PrimitiveTypeNode : public TypeNode {
public:
    // TODO(dslynko): deduce default type size
    PrimitiveTypeNode(PrimitiveType type, int16_t typeSize = -1)
        : TypeNode(), type(type), typeSize(typeSize) {}

private:
    PrimitiveType type;
    int16_t typeSize;
};

enum class AggregateType {
    ARRAY = 0,
    VECTOR,
};

class SequenceTypeNode : public TypeNode {
public:
    SequenceTypeNode(AggregateType seqType, TypeNode *aggregatedType, int size)
        : seqType(seqType), aggregatedType(aggregatedType), size(size) {}

private:
    AggregateType seqType;
    TypeNode *aggregatedType = nullptr;
    int size = -1;
};
}   // namespace lexer

#endif  // PARASL_TYPE_NODE_H_
