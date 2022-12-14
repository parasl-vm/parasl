#ifndef PARASL_TYPE_NODES_H_
#define PARASL_TYPE_NODES_H_

#include "ASTNode.h"
#include <optional>
#include <vector>


namespace frontend::parser {
class IDNode;

class TypeNode : public ASTNode {
public:
    ~TypeNode() noexcept override = default;
};

enum class PrimitiveType {
    CHAR = 0,
    INT,
    FLOAT,
    DOUBLE
};

static int16_t getDefaultPrimitiveSize(PrimitiveType type) {
    switch (type) {
    case PrimitiveType::CHAR:
        return sizeof(int8_t);
    case PrimitiveType::INT:
        return sizeof(int32_t);
    case PrimitiveType::FLOAT:
        return sizeof(float);
    case PrimitiveType::DOUBLE:
        return sizeof(double);
    }
    UNREACHABLE("");
}

class PrimitiveTypeNode : public TypeNode {
public:
    PrimitiveTypeNode(PrimitiveType type, std::optional<int16_t> typeSz)
        : TypeNode(), type(type)
    {
        if (typeSz.has_value()) {
            ASSERT(type == PrimitiveType::INT);
            typeSize = typeSz.value();
            ASSERT(typeSize > 0);
        } else {
            typeSize = getDefaultPrimitiveSize(type);
        }
    }

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

struct FunctionArgument {
    TypeNode *type;
    // TODO(dslynko): consider replacing IDNode with std::string, as it only represents the name
    IDNode *arg;
};

class FunctionTypeNode : public TypeNode {
public:
    FunctionTypeNode(TypeNode *returnType)
        : returnType(returnType) {}

    void AddArgument(TypeNode *type, IDNode *arg) {
        args.emplace_back(type, arg);
    }

private:
    TypeNode *returnType = nullptr;
    std::vector<FunctionArgument> args;
};

class StructTypeNode : public TypeNode {
public:
    StructTypeNode(std::vector<FunctionArgument> &&attrs) : attributes{std::move(attrs)} {}

    void AddArgument(TypeNode *type, IDNode *arg) {
        attributes.emplace_back(type, arg);
    }

private:
    std::vector<FunctionArgument> attributes;
};
}   // namespace frontend::parser

#endif  // PARASL_TYPE_NODES_H_
