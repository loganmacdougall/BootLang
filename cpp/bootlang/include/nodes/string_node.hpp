#pragma once

#include <string>
#include "node.hpp"

struct StringNode : public Node {
    std::string value;

    StringNode(uint32_t lineno, uint32_t col, std::string value);

    std::string toCode(int indent) const override;
};