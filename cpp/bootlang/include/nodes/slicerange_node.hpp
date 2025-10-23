#pragma once

#include <string>
#include <optional>
#include "node.hpp"

struct SliceRangeNode : public Node {
    std::optional<Node> start;
    std::optional<Node> end;
    std::optional<Node> step;

    SliceRangeNode(uint32_t lineno, uint32_t col, std::optional<Node> start, std::optional<Node> end, std::optional<Node> step);

    std::string toCode(int indent) const override;
};