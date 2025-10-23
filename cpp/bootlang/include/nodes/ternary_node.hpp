#pragma once

#include "node.hpp"

struct TernaryNode : public Node {
    Node left;
    Node right;
    Node cond;

    TernaryNode(uint32_t lineno, uint32_t col, Node left, Node right, Node cond);

    std::string toCode(int indent) const override;
};