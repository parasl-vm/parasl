#ifndef PARASL_AST_NODE_H_
#define PARASL_AST_NODE_H_

#include "../../utils/macros.h"
// #include <span>


namespace lexer {
class ASTNode {
public:
    ASTNode() = default;
    NO_COPY_SEMANTIC(ASTNode);
    NO_MOVE_SEMANTIC(ASTNode);
    virtual DEFAULT_DTOR(ASTNode);

    ASTNode *GetParent() {
        return parent;
    }
    const ASTNode *GetParent() const {
        return parent;
    }

    // virtual std::span<ASTNode *> GetChildren() = 0;

private:
    ASTNode *parent = nullptr;
};
}   // namespace lexer

#endif  // PARASL_AST_NODE_H_
