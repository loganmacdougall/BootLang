#pragma once

#include <string>
#include "node.hpp"

class PropertyAccessNode : public Node {
public:
    NodePtr left;
    std::string property;

    PropertyAccessNode(size_t lineno, size_t col, NodePtr&& left, std::string property);

    std::string toCode(int indent) const override;
};