#pragma once

#include <optional>
#include "node.hpp"

class SliceNode : public Node {
public:
    Node left;
    std::optional<Node> start;
    std::optional<Node> end;
    std::optional<Node> step;

    SliceNode(uint32_t lineno, uint32_t col, Node left, std::optional<Node> start, std::optional<Node> end, std::optional<Node> step);

    std::string toCode(int indent) const override;
};
