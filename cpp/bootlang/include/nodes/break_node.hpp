#pragma once

#include "node.hpp"

struct BreakNode : public Node {
    BreakNode(uint32_t lineno, uint32_t col);

    std::string toCode(int indent) const override;
};
