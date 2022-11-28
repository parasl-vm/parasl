#ifndef PARASL_FUNCTION_CALL_NODE_H_
#define PARASL_FUNCTION_CALL_NODE_H_

#include "ExpressionNode.h"
#include <string>
// #include "../parser.tab.h"
#include <vector>


namespace lexer {
class FunctionCallNode : public ExpressionNode {
public:
    FunctionCallNode(const char *funcName, std::vector<ExpressionNode *> args)
        : ExpressionNode(), funcName{funcName}, args(args) {}
    ~FunctionCallNode() noexcept = default;

    const std::string &GetValue() const {
        return funcName;
    }

    const std::vector<ExpressionNode *> &GetArgs() const {
        return args;
    }

private:
    std::string funcName;
    std::vector<ExpressionNode *> args;
};
}   // namespace lexer

#endif  // PARASL_FUNCTION_CALL_NODE_H_
