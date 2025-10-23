#pragma once

#include <vector>
#include "node.hpp"

struct BlockNode : public Node {
    std::vector<Node> stmts;

    BlockNode(uint32_t lineno, uint32_t col, std::vector<Node> stmts);

    std::string toCode(int indent) const override;
};