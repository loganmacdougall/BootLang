#pragma once

#include "node.hpp"

struct ContinueNode : public Node {
    ContinueNode(uint32_t lineno, uint32_t col);

    std::string toCode(int indent) const override;
};
