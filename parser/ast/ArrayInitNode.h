#ifndef PARASL_ARRAY_INIT_NODE_H_
#define PARASL_ARRAY_INIT_NODE_H_

#include "ExpressionNode.h"
#include <vector>
// #include "../parser.tab.h"


namespace frontend::parser {
class ArrayInitializerNode : public ExpressionNode {
public:
    ArrayInitializerNode(std::vector<ExpressionNode *> vals)
        : ExpressionNode(), values(vals) {}
    ~ArrayInitializerNode() noexcept override = default;

    // void AddValue(ExpressionNode *value) {
    //     ASSERT(value);
    //     values.push_back(value);
    // }

private:
    std::vector<ExpressionNode *> values;
};
}   // namespace frontend::parser

#endif  // PARASL_ARRAY_INIT_NODE_H_
