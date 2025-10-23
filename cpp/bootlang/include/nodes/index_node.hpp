#pragma once

#include "node.hpp"

struct IndexNode : public Node {
    Node left;
    Node right;

    IndexNode(uint32_t lineno, uint32_t col, Node left, Node right);

    std::string toCode(int indent) const override;
};