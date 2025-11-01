#pragma once

#include <vector>
#include "node.hpp"
#include "nodes/block_node.hpp"

class WhileNode : public Node {
public:
    NodePtr cond;
    BlockNodePtr block;

    WhileNode(size_t lineno, size_t col, NodePtr&& cond, BlockNodePtr&& block);

    std::string toCode(int indent) const override;
};