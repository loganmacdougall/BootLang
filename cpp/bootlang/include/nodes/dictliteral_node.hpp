#pragma once

#include <vector>
#include "node.hpp"

struct DictLiteralNode : public Node {
    std::vector<std::pair<Node, Node>> elems;

    DictLiteralNode(uint32_t lineno, uint32_t col, std::vector<std::pair<Node, Node>> elems);

    std::string toCode(int indent) const override;
};