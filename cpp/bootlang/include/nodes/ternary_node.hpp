#pragma once

#include "node.hpp"

class TernaryNode : public Node {
public:
    NodePtr left;
    NodePtr right;
    NodePtr cond;

    TernaryNode(uint32_t lineno, uint32_t col, NodePtr&& left, NodePtr&& right, NodePtr&& cond);

    std::string toCode(int indent) const override;
};