#pragma once

#include <vector>
#include "node.hpp"
#include "nodes/block_node.hpp"

class ForNode : public Node {
public:
    NodePtr target;
    NodePtr iterable;
    BlockNodePtr block;

    ForNode(size_t lineno, size_t col, NodePtr&& target, NodePtr&& iterable, BlockNodePtr&& block);

    std::string toCode(int indent) const override;
};