#ifndef PARASL_DECLARATION_NODE_H_
#define PARASL_DECLARATION_NODE_H_

#include "ExpressionNode.h"
// #include "../parser.tab.h"


namespace lexer {
class DeclarationNode : public ExpressionNode {
public:
    ~DeclarationNode() noexcept = default;
};

class FunctionDeclNode : public DeclarationNode {
};

class VariableDeclNode : public DeclarationNode {
};
}   // namespace lexer

#endif  // PARASL_DECLARATION_NODE_H_
