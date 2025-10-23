#pragma once

#include "node.hpp"
#include "token.hpp"

struct AssignNode : public Node {
    Node left;
    Node right;
    TokenType op;

    AssignNode(uint32_t lineno, uint32_t col, Node left, Node right, TokenType op);

    std::string toCode(int indent) const override;
};