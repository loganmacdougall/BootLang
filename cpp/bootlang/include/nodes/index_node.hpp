#pragma once

#include "node.hpp"

class IndexNode : public Node {
public:
    NodePtr left;
    NodePtr right;

    IndexNode(size_t lineno, size_t col, NodePtr&& left, NodePtr&& right);

    std::string toCode(int indent) const override;
};