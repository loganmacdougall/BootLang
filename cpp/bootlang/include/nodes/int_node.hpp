#pragma once

#include "node.hpp"

class IntNode : public Node {
public:
    long value;

    IntNode(uint32_t lineno, uint32_t col, long value);

    std::string toCode(int indent) const override;
};