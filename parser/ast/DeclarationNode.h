#ifndef PARASL_DECLARATION_NODE_H_
#define PARASL_DECLARATION_NODE_H_

#include "ExpressionNode.h"
// #include "../parser.tab.h"


namespace frontend::parser {
class DeclarationNode : public ExpressionNode {
public:
    ~DeclarationNode() noexcept override = default;
};

class FunctionDeclNode : public DeclarationNode {
};

class VariableDeclNode : public DeclarationNode {
};
}   // namespace frontend::parser

#endif  // PARASL_DECLARATION_NODE_H_
