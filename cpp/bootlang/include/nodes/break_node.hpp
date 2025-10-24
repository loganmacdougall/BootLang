#pragma once

#include "node.hpp"

class BreakNode : public Node {
public:
    BreakNode(uint32_t lineno, uint32_t col);

    std::string toCode(int indent) const override;
};
