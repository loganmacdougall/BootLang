#pragma once

#include "node.hpp"

class VarNode : public Node {
public:
    std::string name;

    VarNode(size_t lineno, size_t col, std::string name);

    std::string toCode(int indent) const override;
};