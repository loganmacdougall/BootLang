#pragma once

#include <vector>
#include "node.hpp"

class BlockNode : public Node {
public:
    std::vector<Node> stmts;

    BlockNode(uint32_t lineno, uint32_t col, std::vector<Node> stmts);

    std::string toCode(int indent) const override;
};