#pragma once

#include <vector>
#include "node.hpp"

struct CallNode : public Node {
    Node left;
    std::vector<Node> args;

    CallNode(uint32_t lineno, uint32_t col, Node left, std::vector<Node> args);

    std::string toCode(int indent) const override;
};