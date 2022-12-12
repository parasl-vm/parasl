#ifndef PARASL_ID_NODE_H_
#define PARASL_ID_NODE_H_

#include "ExpressionNode.h"
#include <string>


namespace lexer {
class IDNode : public ExpressionNode {
public:
    IDNode(const char *value)
        : ExpressionNode(), value{value} {}
    ~IDNode() noexcept = default;

    const std::string &GetValue() const {
        return value;
    }

private:
    std::string value;
};
}   // namespace lexer

#endif  // PARASL_ID_NODE_H_
