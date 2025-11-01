#pragma once

#include "node.hpp"
#include "token.hpp"

class BinaryOpNode : public Node {
public:
    NodePtr left;
    NodePtr right;
    Token::Type op;

    BinaryOpNode(size_t lineno, size_t col, NodePtr&& left, NodePtr&& right, Token::Type op);

    std::string toCode(int indent) const override;
};