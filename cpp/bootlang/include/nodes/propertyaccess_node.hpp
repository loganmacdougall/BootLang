#pragma once

#include <string>
#include "node.hpp"

struct PropertyAccessNode : public Node {
    Node left;
    std::string property;

    PropertyAccessNode(uint32_t lineno, uint32_t col, Node left, std::string property);

    std::string toCode(int indent) const override;
};