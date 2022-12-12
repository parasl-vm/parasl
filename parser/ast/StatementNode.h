#ifndef PARASL_STATEMENT_NODE_H_
#define PARASL_STATEMENT_NODE_H_

#include "ASTNode.h"


namespace lexer {
class StatementNode : public ASTNode {
public:
    ~StatementNode() noexcept = default;
};
}   // namespace lexer

#endif  // PARASL_STATEMENT_NODE_H_
