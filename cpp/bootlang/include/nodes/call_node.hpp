#pragma once

#include <vector>
#include "node.hpp"

class CallNode : public Node {
public:
    NodePtr left;
    std::vector<NodePtr> args;

    CallNode(size_t lineno, size_t col, NodePtr&& left, std::vector<NodePtr>&& args);

    std::string toCode(int indent) const override;
};