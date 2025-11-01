#pragma once

#include <string>
#include "node.hpp"

class StringNode : public Node {
public:
    std::string value;

    StringNode(size_t lineno, size_t col, std::string value);

    std::string toCode(int indent) const override;
};