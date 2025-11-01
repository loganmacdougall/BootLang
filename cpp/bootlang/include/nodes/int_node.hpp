#pragma once

#include "node.hpp"

class IntNode : public Node {
public:
    long value;

    IntNode(size_t lineno, size_t col, long value);

    std::string toCode(int indent) const override;
};