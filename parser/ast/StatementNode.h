#ifndef PARASL_STATEMENT_NODE_H_
#define PARASL_STATEMENT_NODE_H_

#include "ASTNode.h"


namespace frontend::parser {
class StatementNode : public ASTNode {
public:
    ~StatementNode() noexcept override = default;
};
}   // namespace frontend::parser

#endif  // PARASL_STATEMENT_NODE_H_
