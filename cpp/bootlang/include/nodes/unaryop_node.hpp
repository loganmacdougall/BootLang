#pragma once

#include "node.hpp"
#include "token.hpp"

class UnaryOpNode : public Node {
public:
    NodePtr right;
    Token::Type op;

    UnaryOpNode(uint32_t lineno, uint32_t col, NodePtr&& right, Token::Type op);

    std::string toCode(int indent) const override;
};