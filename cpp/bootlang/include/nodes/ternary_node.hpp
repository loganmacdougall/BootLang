#pragma once

#include "node.hpp"

class TernaryNode : public Node {
public:
    NodePtr left;
    NodePtr right;
    NodePtr cond;

    TernaryNode(size_t lineno, size_t col, NodePtr&& left, NodePtr&& right, NodePtr&& cond);

    std::string toCode(int indent) const override;
};