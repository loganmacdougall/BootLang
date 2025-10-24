#pragma once

#include "node.hpp"
#include "token.hpp"

class BinaryOpNode : public Node {
public:
    NodePtr left;
    NodePtr right;
    TokenType op;

    BinaryOpNode(uint32_t lineno, uint32_t col, NodePtr&& left, NodePtr&& right, TokenType op);

    std::string toCode(int indent) const override;
};