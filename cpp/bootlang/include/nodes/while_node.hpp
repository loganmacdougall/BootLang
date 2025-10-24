#pragma once

#include <vector>
#include "node.hpp"
#include "nodes/block_node.hpp"

class WhileNode : public Node {
public:
    NodePtr cond;
    BlockNodePtr block;

    WhileNode(uint32_t lineno, uint32_t col, NodePtr&& cond, BlockNodePtr&& block);

    std::string toCode(int indent) const override;
};