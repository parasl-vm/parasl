#ifndef PARASL_LAYER_NODE_H_
#define PARASL_LAYER_NODE_H_

#include "ASTNode.h"
#include "CompoundStatementNode.h"


namespace lexer {
class LayerNode : public ASTNode {
public:
    LayerNode(size_t layerId, std::string layerName, CompoundStatementNode *stmts)
        : ASTNode(), layerId(layerId), layerName(layerName), stmts(stmts) {}
    ~LayerNode() noexcept = default;

    void AddStatement(StatementNode *stmt) {
        stmts->AddStatement(stmt);
    }

private:
    size_t layerId;
    std::string layerName;
    CompoundStatementNode *stmts;
};
}   // namespace lexer

#endif  // PARASL_LAYER_NODE_H_
