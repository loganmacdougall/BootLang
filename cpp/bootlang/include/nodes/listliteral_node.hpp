#pragma once

#include <vector>
#include "node.hpp"

struct ListLiteralNode : public Node {
    std::vector<Node> elems;

    ListLiteralNode(uint32_t lineno, uint32_t col, std::vector<Node> elems);

    std::string toCode(int indent) const override;
};