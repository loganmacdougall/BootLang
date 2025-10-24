#pragma once

#include <vector>
#include <optional>
#include "node.hpp"
#include "nodes/block_node.hpp"

typedef std::pair<Node, BlockNode> CondBlock;

class IfNode : public Node {
public:
    CondBlock if_block;
    std::vector<CondBlock> elif_blocks;
    std::optional<BlockNode> else_block;

    IfNode(uint32_t lineno, uint32_t col, CondBlock if_block, std::vector<CondBlock> elif_blocks, std::optional<BlockNode> else_block);

    std::string toCode(int indent) const override;
};
