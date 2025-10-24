#pragma once

#include "node.hpp"

class IndexNode : public Node {
public:
    NodePtr left;
    NodePtr right;

    IndexNode(uint32_t lineno, uint32_t col, NodePtr&& left, NodePtr&& right);

    std::string toCode(int indent) const override;
};