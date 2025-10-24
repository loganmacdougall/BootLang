#pragma once

#include "node.hpp"
#include "token.hpp"

class AssignNode : public Node {
public:
    NodePtr left;
    NodePtr right;
    TokenType op;

    AssignNode(uint32_t lineno, uint32_t col, NodePtr&& left, NodePtr&& right, TokenType op);

    std::string toCode(int indent) const override;
};