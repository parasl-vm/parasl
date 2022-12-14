#ifndef PARASL_FUNCTION_CALL_NODE_H_
#define PARASL_FUNCTION_CALL_NODE_H_

#include "ExpressionNode.h"
#include "ObjectAccessNode.h"
#include <string>
#include "TypeNodes.h"
// #include "../parser.tab.h"
#include <vector>


namespace frontend::parser {
class FunctionCallNode : public ExpressionNode {
public:
    FunctionCallNode(const char *funcName, std::vector<ExpressionNode *> &&args)
        : ExpressionNode(), funcName{funcName}, args{std::move(args)} {}
    ~FunctionCallNode() noexcept override = default;

    const char *GetValue() const {
        return funcName;
    }

    const std::vector<ExpressionNode *> &GetArgs() const {
        return args;
    }

protected:
    const char *funcName;
    std::vector<ExpressionNode *> args;
};

class InputCallNode : public FunctionCallNode {
public:
    // TODO(dslynko): set correct ExpressionNode arg
    InputCallNode(TypeNode *type)
        : FunctionCallNode{FUNCTION_NAME, {}}, retType{type} {}

    void SetReturnType(TypeNode *type) {
        retType = type;
    }

public:
    static constexpr const char *FUNCTION_NAME = "input";

private:
    TypeNode *retType;
};

class SingleInputCallNode : public InputCallNode {
public:
    // TODO(dslynko): set correct ExpressionNode arg
    SingleInputCallNode(int64_t arg, TypeNode *type = nullptr)
        : InputCallNode{type}, arg{arg} {}

private:
    int64_t arg;
};

class SequencedInputCallNode : public InputCallNode {
public:
    SequencedInputCallNode(int64_t lower, int64_t upper, TypeNode *type = nullptr)
        : InputCallNode{type}, lowerBound{lower}, upperBound{upper} {}

private:
    // TODO(dslynko): consider replacing with some generalized struct representing a range
    int64_t lowerBound;
    int64_t upperBound;
};

using NamedGlueArgs = std::pair<std::vector<ExpressionNode *>, std::vector<IDNode *>>;

class GlueCallNode : public FunctionCallNode {
public:
    GlueCallNode(std::vector<ExpressionNode *> &&args)
        : FunctionCallNode{FUNCTION_NAME, std::move(args)} {}
    GlueCallNode(NamedGlueArgs &&namedArgs)
        : FunctionCallNode{FUNCTION_NAME, std::move(namedArgs.first)},
          altNames{std::move(namedArgs.second)}
    {
        ASSERT(args.size() == names.size());
    }

public:
    static constexpr const char *FUNCTION_NAME = "glue";

private:
    std::vector<IDNode *> altNames;
};

class MethodCallNode : public FunctionCallNode {
public:
    MethodCallNode(AttributeAccessNode *method, std::vector<ExpressionNode *> &&args)
        : FunctionCallNode(method->GetAttrName(), std::move(args)), objMethod{method} {}
    ~MethodCallNode() noexcept override = default;

    AttributeAccessNode *ObjectMethod() {
        return objMethod;
    }

private:
    AttributeAccessNode *objMethod;
};
}   // namespace frontend::parser

#endif  // PARASL_FUNCTION_CALL_NODE_H_
