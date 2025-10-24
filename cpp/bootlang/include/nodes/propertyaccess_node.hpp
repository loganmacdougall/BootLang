#pragma once

#include <string>
#include "node.hpp"

class PropertyAccessNode : public Node {
public:
    NodePtr left;
    std::string property;

    PropertyAccessNode(uint32_t lineno, uint32_t col, NodePtr&& left, std::string property);

    std::string toCode(int indent) const override;
};