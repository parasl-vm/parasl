#ifndef PARASL_COMPOUND_STATEMENT_NODE_H_
#define PARASL_COMPOUND_STATEMENT_NODE_H_

#include "StatementNode.h"
#include <vector>


namespace frontend::parser {
class CompoundStatementNode : public StatementNode {
public:
    CompoundStatementNode(StatementNode *stmt) : statements{stmt} {}
    ~CompoundStatementNode() noexcept override = default;

    void AddStatement(StatementNode *stmt) {
        statements.push_back(stmt);
    }

private:
    std::vector<StatementNode *> statements;
};
}   // namespace frontend::parser

#endif  // PARASL_COMPOUND_STATEMENT_NODE_H_
