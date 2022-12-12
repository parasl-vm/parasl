#ifndef PARASL_COMPOUND_STATEMENT_NODE_H_
#define PARASL_COMPOUND_STATEMENT_NODE_H_

#include "StatementNode.h"
#include <vector>


namespace lexer {
class CompoundStatementNode : public StatementNode {
public:
    ~CompoundStatementNode() noexcept = default;

    void AddStatement(StatementNode *stmt) {
        statements.push_back(stmt);
    }

private:
    std::vector<StatementNode *> statements;
};
}   // namespace lexer

#endif  // PARASL_COMPOUND_STATEMENT_NODE_H_
