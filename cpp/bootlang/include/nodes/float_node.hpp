#pragma once

#include "node.hpp"

struct FloatNode : public Node {
    double value;

    FloatNode(uint32_t lineno, uint32_t col, double value);

    std::string toCode(int indent) const override;
};