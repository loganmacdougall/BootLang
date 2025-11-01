#pragma once

#include "node.hpp"

class BoolNode : public Node {
public:
    bool value;

    BoolNode(size_t lineno, size_t col, bool value);

    std::string toCode(int indent) const override;
};