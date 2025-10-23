#pragma once

#include "node.hpp"

struct VarNode : public Node {
    std::string name;

    VarNode(uint32_t lineno, uint32_t col, std::string name);

    std::string toCode(int indent) const override;
};