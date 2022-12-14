#ifndef PARASL_LAYER_NODE_H_
#define PARASL_LAYER_NODE_H_

#include "ASTNode.h"
#include "CompoundStatementNode.h"


namespace frontend::parser {
class LayerNode : public ASTNode {
public:
    LayerNode(size_t layerId, std::string layerName, CompoundStatementNode *stmts)
        : ASTNode(), layerId(layerId), layerName(layerName), stmts(stmts) {}
    ~LayerNode() noexcept override = default;

    void AddStatement(StatementNode *stmt) {
        stmts->AddStatement(stmt);
    }

private:
    size_t layerId;
    std::string layerName;
    CompoundStatementNode *stmts;
};
}   // namespace frontend::parser

#endif  // PARASL_LAYER_NODE_H_
