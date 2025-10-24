#pragma once

#include "node.hpp"

class FloatNode : public Node {
public:
    double value;

    FloatNode(uint32_t lineno, uint32_t col, double value);

    std::string toCode(int indent) const override;
};