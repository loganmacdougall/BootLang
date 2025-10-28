#pragma once

#include <vector>
#include "node.hpp"
#include "nodes/block_node.hpp"

class ForNode : public Node {
public:
    NodePtr target;
    NodePtr iterable;
    BlockNodePtr block;

    ForNode(uint32_t lineno, uint32_t col, NodePtr&& target, NodePtr&& iterable, BlockNodePtr&& block);

    std::string toCode(int indent) const override;
};