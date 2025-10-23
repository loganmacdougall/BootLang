#pragma once

#include <optional>
#include "node.hpp"

struct ReturnNode : public Node {
    std::optional<Node> value;

    ReturnNode(uint32_t lineno, uint32_t col, std::optional<Node> value);

    std::string toCode(int indent) const override;
};
