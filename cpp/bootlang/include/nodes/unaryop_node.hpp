#pragma once

#include "node.hpp"
#include "token.hpp"

struct UnaryOpNode : public Node {
    Node right;
    TokenType op;

    UnaryOpNode(uint32_t lineno, uint32_t col, Node right, TokenType op);

    std::string toCode(int indent) const override;
};