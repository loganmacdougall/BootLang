#pragma once

#include <vector>
#include "node.hpp"
#include "nodes/block_node.hpp"

struct WhileNode : public Node {
    Node cond;
    BlockNode block;

    WhileNode(uint32_t lineno, uint32_t col, Node cond, BlockNode block);

    std::string toCode(int indent) const override;
};