#pragma once

#include <vector>
#include "node.hpp"
#include "nodes/block_node.hpp"

class WhileNode : public Node {
public:
    Node cond;
    BlockNode block;

    WhileNode(uint32_t lineno, uint32_t col, Node cond, BlockNode block);

    std::string toCode(int indent) const override;
};