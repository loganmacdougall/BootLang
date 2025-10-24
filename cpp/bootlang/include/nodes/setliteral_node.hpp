#pragma once

#include <vector>
#include "node.hpp"

class SetLiteralNode : public Node {
public:
    std::vector<Node> elems;

    SetLiteralNode(uint32_t lineno, uint32_t col, std::vector<Node> elems);

    std::string toCode(int indent) const override;
};