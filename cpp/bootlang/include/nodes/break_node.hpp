#pragma once

#include "node.hpp"

class BreakNode : public Node {
public:
    BreakNode(size_t lineno, size_t col);

    std::string toCode(int indent) const override;
};
