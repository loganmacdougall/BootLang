#pragma once

#include "node.hpp"

class BoolNode : public Node {
public:
    bool value;

    BoolNode(uint32_t lineno, uint32_t col, bool value);

    std::string toCode(int indent) const override;
};