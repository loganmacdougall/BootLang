#pragma once

#include "node.hpp"

class FloatNode : public Node {
public:
    double value;

    FloatNode(size_t lineno, size_t col, double value);

    std::string toCode(int indent) const override;
};