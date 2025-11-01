#pragma once

#include "node.hpp"
#include "token.hpp"

class AssignNode : public Node {
public:
    NodePtr left;
    NodePtr right;
    Token::Type op;

    AssignNode(size_t lineno, size_t col, NodePtr&& left, NodePtr&& right, Token::Type op);

    std::string toCode(int indent) const override;
};