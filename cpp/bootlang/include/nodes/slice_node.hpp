#pragma once

#include <optional>
#include "node.hpp"

class SliceNode : public Node {
public:
    NodePtr left;
    std::optional<NodePtr> start;
    std::optional<NodePtr> end;
    std::optional<NodePtr> step;

    SliceNode(size_t lineno, size_t col, NodePtr&& left, std::optional<NodePtr>&& start, std::optional<NodePtr>&& end, std::optional<NodePtr>&& step);

    std::string toCode(int indent) const override;
};
