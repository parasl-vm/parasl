#ifndef PARASL_FOR_STATEMENT_NODE_H_
#define PARASL_FOR_STATEMENT_NODE_H_

#include "CompoundStatementNode.h"
#include "ExpressionNode.h"
#include "IDNode.h"
#include "StatementNode.h"


namespace frontend::parser {
class IterationRange : public StatementNode {
public:
    IterationRange(IDNode *variable)
        : StatementNode(), variable(variable) {}
    ~IterationRange() noexcept override = default;

    IDNode *GetIterationVariable() {
        return variable;
    }
    const IDNode *GetIterationVariable() const {
        return variable;
    }

private:
    IDNode *variable = nullptr;
};

class SequenceIterationRange : public IterationRange {
public:
    SequenceIterationRange(IDNode *variable, IDNode *sequence)
        : IterationRange(variable), sequence(sequence) {}

private:
    IDNode *sequence = nullptr;
};

class NumericIterationRange : public IterationRange {
public:
    NumericIterationRange(IDNode *variable, int start, int end, int step = 1)
        : IterationRange(variable), start(start), end(end), step(step) {}

private:
    int start = 0;
    int end = 0;
    int step = 0;
};

class ForStatementNode : public ExpressionNode {
public:
    ForStatementNode(IterationRange *iterRange, CompoundStatementNode *loopBody)
        : ExpressionNode(), iterRange(iterRange), body(loopBody) {}
    ForStatementNode(IterationRange *iterRange, StatementNode *stmt)
        : ExpressionNode(), iterRange(iterRange), body(new CompoundStatementNode(stmt)) {}
    ~ForStatementNode() noexcept override = default;

private:
    IterationRange *iterRange = nullptr;
    CompoundStatementNode *body = nullptr;
};
}   // namespace frontend::parser

#endif  // PARASL_FOR_STATEMENT_NODE_H_
